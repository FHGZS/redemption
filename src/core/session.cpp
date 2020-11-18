/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2010-2012
   Author(s): Christophe Grosjean, Javier Caverni, Raphael Zhou, Meng Tan
*/

#include "acl/end_session_warning.hpp"

#include "acl/module_manager/mod_factory.hpp"
#include "core/RDP/tpdu_buffer.hpp"
#include "core/session.hpp"

#include "acl/session_inactivity.hpp"
#include "acl/acl_serializer.hpp"
#include "acl/sesman.hpp"
#include "acl/mod_pack.hpp"
#include "acl/session_logfile.hpp"

#include "capture/capture.hpp"
#include "configs/config.hpp"
#include "utils/timebase.hpp"
#include "front/front.hpp"
#include "mod/mod_api.hpp"
#include "mod/rdp/rdp_api.hpp"
#include "transport/socket_transport.hpp"
#include "transport/ws_transport.hpp"
#include "utils/genfstat.hpp"
#include "utils/invalid_socket.hpp"
#include "utils/netutils.hpp"
#include "utils/select.hpp"
#include "utils/log_siem.hpp"
#include "utils/load_theme.hpp"
#include "utils/difftimeval.hpp"
#include "utils/redirection_info.hpp"
#include "utils/verbose_flags.hpp"
#include "utils/local_err_msg.hpp"

#include <cassert>
#include <cerrno>
#include <cstring>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <unistd.h>

#include "acl/mod_wrapper.hpp"
#include "utils/genrandom.hpp"

namespace
{

enum class SessionVerbose : uint32_t
{
    Event   = 0x02,
    Acl     = 0x04,
    Trace   = 0x08,
};

bool operator & (SessionVerbose a, SessionVerbose b)
{
    return bool(uint32_t(a) & uint32_t(b));
}

struct VerboseSession
{
    static bool has_verbose_event(Inifile const& ini)
    {
        return debug(ini) & SessionVerbose::Event;
    }

    static bool has_verbose_acl(Inifile const& ini)
    {
        return debug(ini) & SessionVerbose::Acl;
    }

    static bool has_verbose_trace(Inifile const& ini)
    {
        return debug(ini) & SessionVerbose::Trace;
    }

private:
    static SessionVerbose debug(Inifile const& ini)
    {
        return safe_cast<SessionVerbose>(ini.get<cfg::debug::session>());
    }
};

class Session
{
    class KeepAlive
    {
        // Keep alive Variables
        int  grace_delay;
        long timeout = 0;
        long renew_time = 0;
        bool wait_answer = false;   // true when we are waiting for a positive response
                                    // false when positive response has been received and
                                    // timers have been set to new timers.
        bool connected = false;

    public:
        KeepAlive(std::chrono::seconds grace_delay_)
        : grace_delay(grace_delay_.count())
        {
        }

        bool is_started() const
        {
            return this->connected;
        }

        void start(time_t now)
        {
            this->connected = true;
            this->timeout    = now + 2 * this->grace_delay;
            this->renew_time = now + this->grace_delay;
        }

        void stop()
        {
            this->connected = false;
        }

        bool check(time_t now, Inifile & ini)
        {
            if (this->connected) {
                if (now > this->timeout) {
                    LOG(LOG_INFO, "auth::keep_alive_or_inactivity : connection closed by manager (timeout)");
                    return true;
                }

                // Keepalive received positive response
                if (this->wait_answer
                    && !ini.is_asked<cfg::context::keepalive>()
                    && ini.get<cfg::context::keepalive>()) {
                    LOG_IF(VerboseSession::has_verbose_event(ini),
                        LOG_INFO, "auth::keep_alive ACL incoming event");
                    this->timeout    = now + 2*this->grace_delay;
                    this->renew_time = now + this->grace_delay;
                    this->wait_answer = false;
                }

                // Keep alive asking for an answer from ACL
                if (!this->wait_answer
                    && (now > this->renew_time)) {

                    this->wait_answer = true;

                    ini.ask<cfg::context::keepalive>();
                }
            }
            return false;
        }
    };

    struct Select
    {
        unsigned max = 0;
        fd_set rfds;
        fd_set wfds;
        timeval timeout;
        bool want_write = false;

        Select(timeval timeout)
        : timeout{timeout}
        {
            io_fd_zero(this->rfds);
            io_fd_zero(this->wfds);
        }

        int select(timeval now)
        {
            timeval timeoutastv = {0,0};
            const timeval & ultimatum = this->timeout;
            const timeval & starttime = now;
            if (ultimatum > starttime) {
                timeoutastv = to_timeval(
                      std::chrono::seconds(ultimatum.tv_sec)
                    - std::chrono::seconds(starttime.tv_sec)
                    + std::chrono::microseconds(ultimatum.tv_usec)
                    - std::chrono::microseconds(starttime.tv_usec));
            }

            return ::select(
                this->max + 1, &this->rfds,
                this->want_write ? &this->wfds : nullptr,
                nullptr, &timeoutastv);
        }

        void set_timeout(timeval next_timeout){ this->timeout = next_timeout; }
        timeval get_timeout() const { return this->timeout; }
        bool is_set_for_writing(int fd) const { bool res = io_fd_isset(fd, this->wfds); return res; }
        bool is_set_for_reading(int fd) const { bool res = io_fd_isset(fd, this->rfds); return res; }
        void set_read_sck(int sck) { this->max = prepare_fds(sck, this->max, this->rfds); }
        void set_write_sck(int sck) {
            if (!this->want_write) {
                this->want_write = true;
                io_fd_zero(this->wfds);
            }
            this->max = prepare_fds(sck, this->max, this->wfds);
        }
    };

    ModuleIndex last_state = MODULE_UNKNOWN;
    Inifile & ini;

    static const time_t select_timeout_tv_sec = 3;

    bool remote_answer = false; // false initialy, set to true once response is
                                // received from acl and asked_remote_answer is
                                // set to false

private:
    enum class EndSessionResult
    {
        close_box,
        retry,
        redirection,
    };

    EndSessionResult end_session_exception(Error const& e, Inifile & ini, const ModWrapper & mod_wrapper)
    {
        if (e.id == ERR_RAIL_LOGON_FAILED_OR_WARNING){
            ini.set_acl<cfg::context::session_probe_launch_error_message>(local_err_msg(e, language(ini)));
        }

        if ((e.id == ERR_SESSION_PROBE_LAUNCH)
        ||  (e.id == ERR_SESSION_PROBE_ASBL_FSVC_UNAVAILABLE)
        ||  (e.id == ERR_SESSION_PROBE_ASBL_MAYBE_SOMETHING_BLOCKS)
        ||  (e.id == ERR_SESSION_PROBE_ASBL_UNKNOWN_REASON)
        ||  (e.id == ERR_SESSION_PROBE_CBBL_FSVC_UNAVAILABLE)
        ||  (e.id == ERR_SESSION_PROBE_CBBL_CBVC_UNAVAILABLE)
        ||  (e.id == ERR_SESSION_PROBE_CBBL_DRIVE_NOT_READY_YET)
        ||  (e.id == ERR_SESSION_PROBE_CBBL_MAYBE_SOMETHING_BLOCKS)
        ||  (e.id == ERR_SESSION_PROBE_CBBL_LAUNCH_CYCLE_INTERRUPTED)
        ||  (e.id == ERR_SESSION_PROBE_CBBL_UNKNOWN_REASON_REFER_TO_SYSLOG)
        ||  (e.id == ERR_SESSION_PROBE_RP_LAUNCH_REFER_TO_SYSLOG)
        ) {
            if (ini.get<cfg::mod_rdp::session_probe_on_launch_failure>() ==
                    SessionProbeOnLaunchFailure::retry_without_session_probe)
            {
                LOG(LOG_INFO, "Retry connection without session probe");
                ini.set<cfg::mod_rdp::enable_session_probe>(false);
                return EndSessionResult::retry;
            }
            this->ini.set<cfg::context::auth_error_message>(local_err_msg(e, language(ini)));
            return EndSessionResult::close_box;
        }

        if (e.id == ERR_SESSION_PROBE_DISCONNECTION_RECONNECTION) {
            LOG(LOG_INFO, "Retry Session Probe Disconnection Reconnection");
            return EndSessionResult::close_box;
        }

        if (e.id == ERR_AUTOMATIC_RECONNECTION_REQUIRED) {
            LOG(LOG_INFO, "Retry Automatic Reconnection Required");
            ini.set<cfg::context::perform_automatic_reconnection>(true);
            return EndSessionResult::retry;
        }

        if (e.id == ERR_RAIL_NOT_ENABLED) {
            LOG(LOG_INFO, "Retry without native remoteapp capability");
            ini.set<cfg::mod_rdp::use_native_remoteapp_capability>(false);
            return EndSessionResult::retry;
        }

        if (e.id == ERR_RDP_SERVER_REDIR){
            if (ini.get<cfg::mod_rdp::server_redirection_support>()) {
                LOG(LOG_INFO, "Server redirection");
                return EndSessionResult::redirection;
            }
            else {
                LOG(LOG_ERR, "Start Session Failed: forbidden redirection = %s", e.errmsg());
                this->ini.set<cfg::context::auth_error_message>(local_err_msg(e, language(ini)));
                return EndSessionResult::close_box;
            }
        }

        if (e.id == ERR_SESSION_CLOSE_ENDDATE_REACHED){
            LOG(LOG_INFO, "Close because disconnection time reached");
            this->ini.set<cfg::context::auth_error_message>(TR(trkeys::session_out_time, language(this->ini)));
            return EndSessionResult::close_box;
        }

        if (e.id == ERR_MCS_APPID_IS_MCS_DPUM){
            LOG(LOG_INFO, "Remote Session Closed by User");
            this->ini.set<cfg::context::auth_error_message>(TR(trkeys::end_connection, language(this->ini)));
            return EndSessionResult::close_box;
        }

        if (e.id == ERR_SESSION_CLOSE_REJECTED_BY_ACL_MESSAGE){
            // Close by rejeted message received
            this->ini.set<cfg::context::auth_error_message>(this->ini.get<cfg::context::rejected>());
            LOG(LOG_INFO, "Close because rejected message was received : %s", this->ini.get<cfg::context::rejected>());
            this->ini.set<cfg::context::rejected>("");
            return EndSessionResult::close_box;
        }

        if (e.id == ERR_SESSION_CLOSE_ACL_KEEPALIVE_MISSED) {
            LOG(LOG_INFO, "Close because of missed ACL keepalive");
            this->ini.set<cfg::context::auth_error_message>(TR(trkeys::miss_keepalive, language(this->ini)));
            return EndSessionResult::close_box;
        }

        if (e.id == ERR_SESSION_CLOSE_USER_INACTIVITY) {
            LOG(LOG_INFO, "Close because of user Inactivity");
            this->ini.set<cfg::context::auth_error_message>(TR(trkeys::close_inactivity, language(this->ini)));
            return EndSessionResult::close_box;
        }

        if (e.id == ERR_SESSION_CLOSE_MODULE_NEXT) {
            LOG(LOG_INFO, "Acl confirmed user close");
            return EndSessionResult::close_box;
        }

        if ((e.id == ERR_TRANSPORT_WRITE_FAILED || e.id == ERR_TRANSPORT_NO_MORE_DATA)
         && mod_wrapper.get_mod_transport()
         && static_cast<uintptr_t>(mod_wrapper.get_mod_transport()->sck) == e.data
         && ini.get<cfg::mod_rdp::auto_reconnection_on_losing_target_link>()
         && mod_wrapper.get_mod()->is_auto_reconnectable()
         && !mod_wrapper.get_mod()->server_error_encountered()
        ) {
            LOG(LOG_INFO, "Session::end_session_exception: target link exception. %s",
                ERR_TRANSPORT_WRITE_FAILED == e.id
                    ? "ERR_TRANSPORT_WRITE_FAILED"
                    : "ERR_TRANSPORT_NO_MORE_DATA");
            ini.set<cfg::context::perform_automatic_reconnection>(true);
            return EndSessionResult::retry;
        }

        LOG(LOG_INFO,
            "ModTrans=<%p> Sock=%d AutoReconnection=%s AutoReconnectable=%s ErrorEncountered=%s",
            mod_wrapper.get_mod_transport(),
            (mod_wrapper.get_mod_transport() ? mod_wrapper.get_mod_transport()->sck : -1),
            (ini.get<cfg::mod_rdp::auto_reconnection_on_losing_target_link>() ? "Yes" : "No"),
            (mod_wrapper.get_mod()->is_auto_reconnectable() ? "Yes" : "No"),
            (mod_wrapper.get_mod()->server_error_encountered() ? "Yes" : "No")
            );

        this->ini.set<cfg::context::auth_error_message>(local_err_msg(e, language(ini)));
        return EndSessionResult::close_box;
    }

private:
    static void wabam_settings(Inifile & ini, Front & front)
    {
        if (ini.get<cfg::client::force_bitmap_cache_v2_with_am>()
         && ini.get<cfg::context::is_wabam>()
        ) {
            front.force_using_cache_bitmap_r2();
        }
    }

    static void rt_display(Inifile& ini, ModWrapper& mod_wrapper, Front& front)
    {
        const Capture::RTDisplayResult rt_status =
            front.set_rt_display(ini.get<cfg::video::rt_display>());

        if (ini.get<cfg::client::enable_osd_4_eyes>()
            && rt_status == Capture::RTDisplayResult::Enabled)
        {
            zstring_view msg = TR(trkeys::enable_rt_display, language(ini));

            mod_wrapper.display_osd_message(msg.to_sv());
        }
    }


    void new_mod(ModuleIndex next_state, ModWrapper & mod_wrapper, ModFactory & mod_factory, Front & front)
    {
        if (mod_wrapper.current_mod != MODULE_INTERNAL_TRANSITION){
            this->last_state = mod_wrapper.current_mod;
            LOG_IF(VerboseSession::has_verbose_trace(ini),
                LOG_INFO, "new_mod::changed state Current Mod is %s Previous %s next %s",
                get_module_name(mod_wrapper.current_mod),
                get_module_name(this->last_state),
                get_module_name(next_state)
                );
        }

        if (mod_wrapper.current_mod != next_state) {
            if ((mod_wrapper.current_mod == MODULE_RDP) ||
                (mod_wrapper.current_mod == MODULE_VNC)) {
                front.must_be_stop_capture();
            }
        }

        switch (next_state) {
            case MODULE_RDP:
            case MODULE_VNC:
                this->target_connection_start_time = tvtime();
                break;
            default:
                this->target_connection_start_time = {};
        }

        auto mod_pack = mod_factory.create_mod(next_state);
        mod_wrapper.set_mod(next_state, mod_pack);
    }

    bool next_backend_module(SessionLogFile & log_file, Inifile& ini,
                             ModFactory & mod_factory, ModWrapper & mod_wrapper,
                             Front & front,
                             Sesman & sesman,
                             ClientExecute & rail_client_execute)
    {
        auto next_state = get_module_id(ini.get<cfg::context::module>());

        switch (next_state){
        case MODULE_TRANSITORY: // NO MODULE CHANGE INFO YET, ASK MORE FROM ACL
        {
            // In case of transitory we are still expecting spontaneous data
            this->remote_answer = true;
            auto next_state = MODULE_INTERNAL_TRANSITION;
            this->new_mod(next_state, mod_wrapper, mod_factory, front);
        } // case next_state == MODULE_TRANSITORY  in switch (next_state)
        break;
        case MODULE_RDP:
        {
            if (mod_wrapper.is_connected()) {
                if (ini.get<cfg::context::auth_error_message>().empty()) {
                    ini.set<cfg::context::auth_error_message>(TR(trkeys::end_connection, language(ini)));
                }
                throw Error(ERR_SESSION_CLOSE_MODULE_NEXT);
            }
            rail_client_execute.enable_remote_program(front.get_client_info().remote_program);
            log_proxy::set_user(this->ini.get<cfg::globals::auth_user>().c_str());

            try {
                if (mod_wrapper.current_mod != next_state) {
                    log_file.open_session_log();
                    sesman.set_connect_target();
                }
                this->new_mod(next_state, mod_wrapper, mod_factory, front);
                if (ini.get<cfg::globals::bogus_refresh_rect>()
                && ini.get<cfg::globals::allow_using_multiple_monitors>()
                && (front.get_client_info().cs_monitor.monitorCount > 1)
                ) {
                    mod_wrapper.get_mod()->rdp_suppress_display_updates();
                    mod_wrapper.get_mod()->rdp_allow_display_updates(0, 0,
                        front.get_client_info().screen_info.width, front.get_client_info().screen_info.height);
                }
                mod_wrapper.get_mod()->rdp_input_invalidate(
                        Rect(0, 0, front.get_client_info().screen_info.width, front.get_client_info().screen_info.height));
                ini.set<cfg::context::auth_error_message>("");
            }
            catch (...) {
                sesman.log6(LogId::SESSION_CREATION_FAILED, {});
                front.must_be_stop_capture();
                throw;
            }
        } // case next_state == MODULE_RDP  in switch (next_state)
        break;
        case MODULE_VNC:
        {
            if (mod_wrapper.is_connected()) {
                if (ini.get<cfg::context::auth_error_message>().empty()) {
                    ini.set<cfg::context::auth_error_message>(TR(trkeys::end_connection, language(ini)));
                }
                throw Error(ERR_SESSION_CLOSE_MODULE_NEXT);
            }

            rail_client_execute.enable_remote_program(front.get_client_info().remote_program);
            log_proxy::set_user(ini.get<cfg::globals::auth_user>().c_str());

            try {
                if (mod_wrapper.current_mod != next_state) {
                    log_file.open_session_log();
                    sesman.set_connect_target();
                }
                this->new_mod(next_state, mod_wrapper, mod_factory, front);

                ini.set<cfg::context::auth_error_message>("");
            }
            catch (...) {
                sesman.log6(LogId::SESSION_CREATION_FAILED, {});
                throw;
            }

        } // case next_state == MODULE_VNC  in switch (next_state)
        break;
        case MODULE_INTERNAL:
        {
            next_state = get_internal_module_id_from_target(ini.get<cfg::context::target_host>());
            if (next_state != last_state){
                rail_client_execute.enable_remote_program(front.get_client_info().remote_program);
                log_proxy::set_user(this->ini.get<cfg::globals::auth_user>().c_str());
                this->new_mod(next_state, mod_wrapper, mod_factory, front);
            }
        } // case next_state == MODULE_INTERNAL  in switch (next_state)
        break;

        case MODULE_UNKNOWN:
        case MODULE_INTERNAL_CLOSE:
        case MODULE_INTERNAL_CLOSE_BACK:
            throw Error(ERR_SESSION_CLOSE_MODULE_NEXT);

        case MODULE_INTERNAL_LOGIN:
                log_proxy::set_user("");
                rail_client_execute.enable_remote_program(front.get_client_info().remote_program);
                this->new_mod(next_state, mod_wrapper, mod_factory, front);
            break;
        case MODULE_INTERNAL_WAIT_INFO:
                log_proxy::set_user("");
                rail_client_execute.enable_remote_program(front.get_client_info().remote_program);
                this->new_mod(next_state, mod_wrapper, mod_factory, front);
            break;
        case MODULE_INTERNAL_DIALOG_DISPLAY_MESSAGE:
                log_proxy::set_user("");
                rail_client_execute.enable_remote_program(front.get_client_info().remote_program);
                this->new_mod(next_state, mod_wrapper, mod_factory, front);
            break;
        case MODULE_INTERNAL_DIALOG_VALID_MESSAGE:
                log_proxy::set_user("");
                rail_client_execute.enable_remote_program(front.get_client_info().remote_program);
                this->new_mod(next_state, mod_wrapper, mod_factory, front);
            break;
        case MODULE_INTERNAL_DIALOG_CHALLENGE:
                log_proxy::set_user("");
                rail_client_execute.enable_remote_program(front.get_client_info().remote_program);
                this->new_mod(next_state, mod_wrapper, mod_factory, front);
            break;
        default:
            log_proxy::set_user(this->ini.get<cfg::globals::auth_user>().c_str());
            this->new_mod(next_state, mod_wrapper, mod_factory, front);
            break;
        } // switch (next_state)

        return true;
    }

    // This function takes care of outgoing data waiting in buffers
    // happening because system write buffer is full and immediate send failed
    // hopefully it should be a rare case

    bool perform_delayed_writes(TimeBase & time_base, SocketTransport & front_trans, SocketTransport * pmod_trans, bool & run_session)
    {
        bool front_has_waiting_data_to_write = front_trans.has_data_to_write();
        bool mod_has_waiting_data_to_write   = pmod_trans && pmod_trans->has_data_to_write();

        if (front_has_waiting_data_to_write || mod_has_waiting_data_to_write){
            timeval now = time_base.get_current_time();
            Select ioswitch(timeval{now.tv_sec + this->select_timeout_tv_sec, now.tv_usec});
            if (front_has_waiting_data_to_write){
                ioswitch.set_write_sck(front_trans.sck);
            }
            if (mod_has_waiting_data_to_write){
                ioswitch.set_write_sck(pmod_trans->sck);
            }

            int num = ioswitch.select(now);
            if (num < 0) {
                if (errno != EINTR) {
                    // Cope with EBADF, EINVAL, ENOMEM : none of these should ever happen
                    // EBADF: means fd has been closed (by me) or as already returned an error on another call
                    // EINVAL: invalid value in timeout (my fault again)
                    // ENOMEM: no enough memory in kernel (unlikely fort 3 sockets)
                    LOG(LOG_ERR, "Proxy send wait raised error %d : %s", errno, strerror(errno));
                    run_session = false;
                    return true;
                }
            }

            if (pmod_trans && ioswitch.is_set_for_writing(pmod_trans->sck)) {
                pmod_trans->send_waiting_data();
            }

            if (front_trans.sck != INVALID_SOCKET
             && ioswitch.is_set_for_writing(front_trans.sck)
            ) {
                front_trans.send_waiting_data();
            }

            if (num > 0) {
                return true;
            }
            // if the select stopped on timeout or EINTR we will give a try to reading
        }
        return false;
    }

    void retry_rdp(ClientExecute & rail_client_execute, Front & front, ModWrapper & mod_wrapper, ModFactory & mod_factory, Sesman & sesman)
    {
        LOG(LOG_INFO, "Retry RDP");
        this->remote_answer = false;

        rail_client_execute.enable_remote_program(front.get_client_info().remote_program);
        log_proxy::set_user(this->ini.get<cfg::globals::auth_user>().c_str());

        auto next_state = MODULE_RDP;
        try {
            this->new_mod(next_state, mod_wrapper, mod_factory, front);

            if (ini.get<cfg::globals::bogus_refresh_rect>() &&
                ini.get<cfg::globals::allow_using_multiple_monitors>() &&
                (front.get_client_info().cs_monitor.monitorCount > 1)) {
                mod_wrapper.get_mod()->rdp_suppress_display_updates();
                mod_wrapper.get_mod()->rdp_allow_display_updates(0, 0,
                    front.get_client_info().screen_info.width, front.get_client_info().screen_info.height);
            }
            mod_wrapper.get_mod()->rdp_input_invalidate(
                    Rect(0, 0,
                        front.get_client_info().screen_info.width,
                        front.get_client_info().screen_info.height));
            ini.set<cfg::context::auth_error_message>("");
        }
        catch (...) {
            sesman.log6(LogId::SESSION_CREATION_FAILED, {});
            front.must_be_stop_capture();

            throw;
        }
    }

    timeval target_connection_start_time {};

    std::string acl_manager_disconnect_reason;

public:
    Session(SocketTransport&& front_trans, timeval sck_start_time, Inifile& ini)
    : ini(ini)
    {
        CryptoContext cctx;
        UdevRandom rnd;
        Fstat fstat;

        std::string disconnection_message_error;

        TimeBase time_base(tvtime());
        EventContainer events;

        const bool source_is_localhost = ini.get<cfg::globals::host>() == "127.0.0.1";

        Sesman sesman(ini, time_base);

        struct SessionFront final : Front
        {
            timeval* target_connection_start_time_ptr = nullptr;
            Inifile* ini_ptr = nullptr;

            using Front::Front;

            bool can_be_start_capture(bool force_capture) override
            {
                if (*this->target_connection_start_time_ptr != timeval{}) {
                    auto elapsed = difftimeval(tvtime(), *this->target_connection_start_time_ptr);
                    this->ini_ptr->set_acl<cfg::globals::target_connection_time>(
                        std::chrono::duration_cast<std::chrono::milliseconds>(elapsed));
                    *this->target_connection_start_time_ptr = {};
                }

                return this->Front::can_be_start_capture(force_capture);
            }
        };

        SessionFront front(time_base, events, sesman,
            front_trans, rnd, ini, cctx, ini.get<cfg::client::fast_path>()
        );
        front.ini_ptr = &ini;
        front.target_connection_start_time_ptr = &this->target_connection_start_time;
        sesman.set_front(&front);

        KeepAlive keepalive(ini.get<cfg::globals::keepalive_grace_delay>());

        SessionInactivity inactivity;

        AclSerializer acl_serial(ini);

        std::string session_type;
        SessionLogFile log_file(ini, time_base, cctx, rnd, fstat,
                        [&sesman](const Error & error){
                            if (error.errnum == ENOSPC) {
                                // error.id = ERR_TRANSPORT_WRITE_NO_ROOM;
                                sesman.report("FILESYSTEM_FULL", "100|unknown");
                            }
                        });

        std::unique_ptr<Transport> auth_trans;

        SiemLogger siem_logger;
        auto write_acl_log6_fn = [&ini,&log_file,&time_base,&session_type,&siem_logger](
            LogId id, KVLogList kv_list)
        {
            /* Log to SIEM (redirected syslog) */
            siem_logger.log_syslog_format(id, kv_list, ini, session_type);
            auto const now = time_base.get_current_time().tv_sec;
            siem_logger.log_arcsight_format(now, id, kv_list, ini, session_type);
            log_file.log6(id, kv_list);
        };

        TpduBuffer rbuf;

        try {
            Font glyphs = Font(app_path(AppPath::DefaultFontFile), ini.get<cfg::globals::spark_view_specific_glyph_width>());

            Theme theme;
            ::load_theme(theme, ini);

            RedirectionInfo redir_info;

            ClientExecute rail_client_execute(
                time_base, events, front, front, front.get_client_info().window_list_caps,
                ini.get<cfg::debug::mod_internal>() & 1);

            windowing_api* winapi = nullptr;
            ModWrapper mod_wrapper(
                front.get_palette(), front, front.keymap, front.get_client_info(), glyphs,
                rail_client_execute, winapi, this->ini, sesman);
            ModFactory mod_factory(
                mod_wrapper, time_base, sesman, events, front.get_client_info(), front, front,
                redir_info, ini, glyphs, theme, rail_client_execute, front.keymap, rnd,
                cctx);
            EndSessionWarning end_session_warning;

            const time_t start_time = time(nullptr);

            sesman.set_login_language
                (ini.get<cfg::translation::login_language>());

            bool run_session = true;
            bool is_first_looping_on_mod_selector = true;

            using namespace std::chrono_literals;

            while (run_session) {

                time_base.set_current_time(tvtime());

                SocketTransport * pmod_trans = mod_wrapper.get_mod_transport();
                if (this->perform_delayed_writes(time_base, front_trans, pmod_trans, run_session)){
                    continue;
                }

                // =============================================================
                // prepare select for listening on all read sockets
                // timeout or immediate wakeups are managed using timeout
                // =============================================================

                Select ioswitch(timeval{
                        time_base.get_current_time().tv_sec + this->select_timeout_tv_sec,
                        time_base.get_current_time().tv_usec});

                if (mod_wrapper.get_mod_transport()) {
                    int fd = mod_wrapper.get_mod_transport()->sck;
                    if (fd != INVALID_SOCKET) {
                        ioswitch.set_read_sck(fd);
                    }
                }

                if (front_trans.sck != INVALID_SOCKET) {
                    ioswitch.set_read_sck(front_trans.sck);
                }

                // gather fd from events
                events.get_fds([&ioswitch](int fd){ioswitch.set_read_sck(fd);});

                if (acl_serial.is_connected()) {
                    ioswitch.set_read_sck(auth_trans->get_sck());
                }

                timeval ultimatum = ioswitch.get_timeout();
                auto tv = events.next_timeout();
                // tv {0,0} means no timeout to trigger
                if ((tv.tv_sec != 0) || (tv.tv_usec != 0)){
                    ultimatum = std::min(tv, ultimatum);
                }

                if (front.front_must_notify_resize) {
                    ultimatum = time_base.get_current_time();
                }

                if ((mod_wrapper.get_mod_transport() && mod_wrapper.get_mod_transport()->has_tls_pending_data())) {
                    ultimatum = time_base.get_current_time();
                }

                if (front_trans.has_tls_pending_data()) {
                    ultimatum = time_base.get_current_time();
                }
                ioswitch.set_timeout(ultimatum);

                int num = ioswitch.select(time_base.get_current_time());

                if (num < 0) {
                    if (errno != EINTR) {
                        // Cope with EBADF, EINVAL, ENOMEM : none of these should ever happen
                        // EBADF: means fd has been closed (by me) or as already returned an error on another call
                        // EINVAL: invalid value in timeout (my fault again)
                        // ENOMEM: no enough memory in kernel (unlikely fort 3 sockets)
                        LOG(LOG_ERR, "Proxy data wait loop raised error %d : %s", errno, strerror(errno));
                        run_session = false;
                    }
                    continue;
                }

                time_base.set_current_time(tvtime());

                try {
                    bool const front_is_set = front_trans.has_tls_pending_data()
                        || (front_trans.sck != INVALID_SOCKET && ioswitch.is_set_for_reading(front_trans.sck));
                    if (front_is_set){
                        if (front.front_must_notify_resize) {
                            LOG(LOG_INFO, "Notify resize to front");
                            front.notify_resize(mod_wrapper.get_callback());
                        }

                        rbuf.load_data(front_trans);

                        while (rbuf.next(TpduBuffer::PDU)) // or TdpuBuffer::CredSSP in NLA
                        {
                            bytes_view tpdu = rbuf.current_pdu_buffer();
                            uint8_t current_pdu_type = rbuf.current_pdu_get_type();
                            front.incoming(tpdu, current_pdu_type, mod_wrapper.get_callback());
                        }
                    }
                }
                catch (Error const& e) {
                    if (ERR_TRANSPORT_WRITE_FAILED == e.id || ERR_TRANSPORT_NO_MORE_DATA == e.id)
                    {
                        SocketTransport* socket_transport_ptr = mod_wrapper.get_mod_transport();

                        if (   socket_transport_ptr && (static_cast<int>(e.data) == socket_transport_ptr->sck)
                            && ini.get<cfg::mod_rdp::auto_reconnection_on_losing_target_link>()
                            && mod_wrapper.get_mod()->is_auto_reconnectable()
                            && !mod_wrapper.get_mod()->server_error_encountered())
                        {
                            LOG(LOG_INFO, "Session::Session: target link exception. %s",
                                (ERR_TRANSPORT_WRITE_FAILED == e.id ? "ERR_TRANSPORT_WRITE_FAILED" : "ERR_TRANSPORT_NO_MORE_DATA"));

                            ini.set<cfg::context::perform_automatic_reconnection>(true);

                            retry_rdp(rail_client_execute, front, mod_wrapper, mod_factory, sesman);
                            continue;
                        }
                    }

                    // RemoteApp disconnection initiated by user
                    // ERR_DISCONNECT_BY_USER == e.id
                    if (
                        // Can be caused by client disconnect.
                        (e.id != ERR_X224_RECV_ID_IS_RD_TPDU)
                        // Can be caused by client disconnect.
                        && (e.id != ERR_MCS_APPID_IS_MCS_DPUM)
                        && (e.id != ERR_RDP_HANDSHAKE_TIMEOUT)
                        // Can be caused by wabwatchdog.
                        && (e.id != ERR_TRANSPORT_NO_MORE_DATA)) {
                        LOG(LOG_ERR, "Proxy data processing raised error %u : %s", e.id, e.errmsg(false));
                    }
                    front_trans.sck = INVALID_SOCKET;
                    run_session = false;
                    continue;
                } catch (...) {
                    LOG(LOG_ERR, "Proxy data processing raised an unknown error");
                    run_session = false;
                    continue;
                }

                // exchange data with sesman
                if (acl_serial.is_connected()) {
                    if (ioswitch.is_set_for_reading(auth_trans->get_sck())) {
                        try {
                            acl_serial.incoming();
                            if (ini.get<cfg::context::module>() == "RDP"
                             || ini.get<cfg::context::module>() == "VNC"
                            ) {
                                session_type = ini.get<cfg::context::module>();

                                auto const& inactivity_timeout
                                    = ini.get<cfg::globals::inactivity_timeout>();

                                auto timeout = (inactivity_timeout == inactivity_timeout.zero())
                                    ? inactivity_timeout
                                    : ini.get<cfg::globals::session_timeout>();

                                inactivity.update_inactivity_timeout(timeout);
                            }
                            this->remote_answer = true;
                        }
                        catch (...) {
                            LOG(LOG_INFO, "acl_serial.incoming() Session lost");
                            // acl connection lost
                            acl_serial.set_disconnected_by_authentifier();

                            if (acl_manager_disconnect_reason.empty()) {
                                ini.set<cfg::context::rejected>(TR(trkeys::manager_close_cnx, language(ini)));
                            }
                            else {
                                ini.set<cfg::context::rejected>(acl_manager_disconnect_reason);
                                acl_manager_disconnect_reason.clear();
                            }
                        }

                        if (!ini.get_acl_fields_changed().size()) {
                            mod_wrapper.acl_update();
                        }
                    }

                    /* On target disconnection, condition will be only executed
                       after received packet containing module=close_back
                       from Sesman, but also when user can 
                       access several targets on selector */
                    if (ModuleIndex next_state = 
                        get_module_id(ini.get<cfg::context::module>());
                        next_state == MODULE_INTERNAL_CLOSE_BACK
                        && mod_wrapper.current_mod == MODULE_INTERNAL_CLOSE)
                    {
                        new_mod(next_state, mod_wrapper, mod_factory, front);
                        mod_wrapper.get_mod()->set_mod_signal(BACK_EVENT_NONE);
                    }

                    // propagate changes made in sesman structure to actual acl changes
                    sesman.flush_acl_report(
                        [&ini](zstring_view reason, zstring_view message)
                        {
                            ini.ask<cfg::context::keepalive>();
                            char report[1024];
                            snprintf(report, sizeof(report), "%s:%s:%s", reason.c_str(),
                                ini.get<cfg::globals::target_device>().c_str(), message.c_str());
                            ini.set_acl<cfg::context::reporting>(report);
                        });
                    sesman.flush_acl_log6(write_acl_log6_fn);
                    sesman.flush_acl(VerboseSession::has_verbose_acl(ini));

                    if (acl_serial.send_acl_data()) {
                        this->remote_answer = false;
                    }

                    sesman.flush_acl_disconnect_target([&log_file]() {
                        log_file.close_session_log();
                    });
                }
                else if (mod_wrapper.current_mod != MODULE_INTERNAL_CLOSE && mod_wrapper.current_mod != MODULE_INTERNAL_CLOSE_BACK) {
                    if (acl_serial.is_after_connexion()) {
                        this->ini.set<cfg::context::auth_error_message>("Authentifier closed connexion");
                        mod_wrapper.disconnect();
                        run_session = false;
                        LOG(LOG_INFO, "Session Closed by ACL : %s",
                            (acl_serial.is_disconnected_by_authentifier())
                                ? "closed by authentifier"
                                : "closed by proxy");
                        if (ini.get<cfg::globals::enable_close_box>()) {
                            auto next_state = MODULE_INTERNAL_CLOSE;
                            this->new_mod(next_state, mod_wrapper, mod_factory, front);
                            run_session = true;
                        }
                        continue;
                    }
                    else if (front.is_up_and_running()) {
                        if (acl_serial.is_before_connexion()) {
                            unique_fd client_sck = addr_connect_non_blocking(
                                ini.get<cfg::globals::authfile>().c_str(),
                                (ini.get<cfg::globals::host>() == "127.0.0.1"));

                            if (!client_sck.is_open()) {
                                LOG(LOG_ERR, "Failed to connect to authentifier (%s)",
                                    ini.get<cfg::globals::authfile>().c_str());
                                acl_serial.set_failed_auth_trans();

                                this->ini.set<cfg::context::auth_error_message>("No authentifier available");
                                run_session = false;
                                LOG(LOG_INFO, "Start of acl failed : no authentifier available");
                                if (ini.get<cfg::globals::enable_close_box>()) {
                                    auto next_state = MODULE_INTERNAL_CLOSE;
                                    this->new_mod(next_state, mod_wrapper, mod_factory, front);
                                    run_session = true;
                                }
                            }
                            else {
                                auth_trans = std::make_unique<SocketTransport>(
                                    "Authentifier", std::move(client_sck),
                                    ini.get<cfg::globals::authfile>().c_str(), 0,
                                    std::chrono::seconds(1), SocketTransport::Verbose::none);

                                acl_serial.set_auth_trans(auth_trans.get());

                                auto elapsed = difftimeval(tvtime(), sck_start_time);
                                this->ini.set_acl<cfg::globals::front_connection_time>(
                                    std::chrono::duration_cast<std::chrono::milliseconds>(
                                        elapsed));
                            }

                            continue;
                        }
                        else {
                            LOG_IF(VerboseSession::has_verbose_acl(ini),
                                LOG_ERR, "can't flush acl: not connected yet");
                        }
                    }
                }

                try {
                    events.execute_events(time_base.get_current_time(),
                        [&ioswitch](int fd){ return ioswitch.is_set_for_reading(fd); },
                        VerboseSession::has_verbose_event(ini));

                    if (front.is_up_and_running()) {
                        const uint32_t enddate = this->ini.get<cfg::context::end_date_cnx>();
                        if (enddate != 0
                        && (static_cast<uint32_t>(time_base.get_current_time().tv_sec) > enddate)) {
                            throw Error(ERR_SESSION_CLOSE_ENDDATE_REACHED);
                        }
                        if (!this->ini.get<cfg::context::rejected>().empty()) {
                            throw Error(ERR_SESSION_CLOSE_REJECTED_BY_ACL_MESSAGE);
                        }
                        if (keepalive.check(time_base.get_current_time().tv_sec, this->ini)) {
                            throw Error(ERR_SESSION_CLOSE_ACL_KEEPALIVE_MISSED);
                        }
                        if (mod_wrapper.current_mod != MODULE_INTERNAL_CLOSE_BACK
                            && !inactivity.activity(time_base.get_current_time().tv_sec,
                                                     front.has_user_activity)
                        ) {
                            throw Error(ERR_SESSION_CLOSE_USER_INACTIVITY);
                        }

                        // new value incoming from authentifier
                        if (ini.check_from_acl()) {
                           this->wabam_settings(ini, front);
                           this->rt_display(ini, mod_wrapper, front);
                        }

                        if (ini.get<cfg::globals::enable_osd>()) {
                            const uint32_t enddate = ini.get<cfg::context::end_date_cnx>();
                            if (enddate && mod_wrapper.is_up_and_running()) {
                                std::string message = end_session_warning.update_osd_state(
                                    language(ini), start_time,
                                    static_cast<time_t>(enddate),
                                    time_base.get_current_time().tv_sec);
                                mod_wrapper.display_osd_message(message);
                            }
                        }

                        if (acl_serial.is_connected()
                         && !keepalive.is_started()
                         && mod_wrapper.is_connected())
                        {
                            if (ini.get<cfg::globals::inactivity_timeout>().count() != 0)
                            {
                                inactivity.update_inactivity_timeout
                                    (ini.get<cfg::globals::inactivity_timeout>());
                            }
                            keepalive.start(time_base.get_current_time().tv_sec);
                        }

                        if (mod_wrapper.get_mod_signal() == BACK_EVENT_STOP){
                            LOG(LOG_INFO, "Module asked Front Disconnection");
                            run_session = false;
                            continue;
                        }

                        // BACK FROM EXTERNAL MODULE (RDP, VNC)
                        if (mod_wrapper.get_mod_signal() == BACK_EVENT_NEXT
                         && mod_wrapper.is_connected()
                        ) {
                            LOG(LOG_INFO, "Exited from target connection");
                            mod_wrapper.disconnect();
                            auto next_state = MODULE_INTERNAL_CLOSE_BACK;
                            if (acl_serial.is_connected()){
                                for (auto field : this->ini.get_acl_fields_changed()) {
                                        zstring_view key = field.get_acl_name();

                                        LOG_IF(VerboseSession::has_verbose_trace(ini),
                                               LOG_INFO, "field to send: %s", key.c_str());
                                }
                                keepalive.stop();
                                sesman.set_disconnect_target();
                                this->remote_answer = false;
                                acl_serial.send_acl_data();
                            }
                            else {
                                next_state = MODULE_INTERNAL_CLOSE;
                            }
                            if (ini.get<cfg::globals::enable_close_box>()) {
                                this->new_mod(next_state, mod_wrapper, mod_factory, front);
                                mod_wrapper.get_mod()->set_mod_signal(BACK_EVENT_NONE);
                                continue;
                            }
                            LOG(LOG_INFO, "Close Box disabled : ending session");
                            run_session = false;
                            continue;
                        }

                        LOG_IF(VerboseSession::has_verbose_trace(ini),
                            LOG_INFO, " Current Mod is %s Previous %s Acl_mod %s",
                            get_module_name(mod_wrapper.current_mod),
                            get_module_name(this->last_state),
                            ini.get<cfg::context::module>()
                            );

                        if (mod_wrapper.get_mod_signal() == BACK_EVENT_STOP){
                            throw Error(ERR_UNEXPECTED);
                        }

                        if (mod_wrapper.get_mod_signal() == BACK_EVENT_NEXT){
                            rail_client_execute.enable_remote_program(
                                front.get_client_info().remote_program);
                            log_proxy::set_user(this->ini.get<cfg::globals::auth_user>().c_str());
                            auto next_state = MODULE_INTERNAL_TRANSITION;
                            this->new_mod(next_state, mod_wrapper, mod_factory, front);
                        }

                        // There are modified fields to send to sesman
                        if (acl_serial.is_connected() && this->remote_answer){
                            this->remote_answer = false;

                            auto next_state = get_module_id(ini.get<cfg::context::module>());

                            if ((mod_wrapper.current_mod == MODULE_INTERNAL_TRANSITION)
                             != (next_state == MODULE_TRANSITORY)
                            ) {
                                run_session = this->next_backend_module(
                                    log_file, ini, mod_factory, mod_wrapper,
                                    front, sesman, rail_client_execute);
                            }
                        }

                        if (mod_wrapper.is_connected()
                         && mod_wrapper.current_mod == MODULE_RDP
                        ) {
                            auto mod = mod_wrapper.get_mod();
                            // AuthCHANNEL CHECK
                            // if an answer has been received, send it to
                            // rdp serveur via mod (should be rdp module)
                            auto & auth_channel_answer = ini.get<cfg::context::auth_channel_answer>();
                            if (ini.get<cfg::mod_rdp::auth_channel>()[0]
                             // Get sesman answer to AUTHCHANNEL_TARGET
                             && !auth_channel_answer.empty()
                            ) {
                                // If set, transmit to auth_channel
                                mod->send_auth_channel_data(auth_channel_answer.c_str());
                                // Erase the context variable
                                ini.set<cfg::context::auth_channel_answer>("");
                            }

                            // CheckoutCHANNEL CHECK
                            // if an answer has been received, send it to
                            // rdp serveur via mod (should be rdp module)
                            auto & pm_response = ini.get<cfg::context::pm_response>();
                            if (ini.get<cfg::mod_rdp::checkout_channel>()[0]
                             // Get sesman answer to AUTHCHANNEL_TARGET
                             && !pm_response.empty()
                            ) {
                                // If set, transmit to auth_channel channel
                                mod->send_checkout_channel_data(pm_response.c_str());
                                    // Erase the context variable
                                ini.set<cfg::context::pm_response>("");
                            }

                            auto & rd_shadow_type = ini.get<cfg::context::rd_shadow_type>();
                            if (!rd_shadow_type.empty()) {
                                auto & rd_shadow_userdata = ini.get<cfg::context::rd_shadow_userdata>();
                                LOG(LOG_INFO, "got rd_shadow_type calling create_shadow_session()");
                                mod->create_shadow_session(
                                    rd_shadow_userdata.c_str(),
                                    rd_shadow_type.c_str());
                                ini.set<cfg::context::rd_shadow_type>("");
                            }

                            if (!ini.get<cfg::context::disconnect_reason>().empty()) {
                                acl_manager_disconnect_reason = ini.get<cfg::context::disconnect_reason>();
                                ini.set<cfg::context::disconnect_reason>("");
                                ini.set_acl<cfg::context::disconnect_reason_ack>(true);
                            }
                            else if (!ini.get<cfg::context::auth_command>().empty()) {
                                if (!::strcasecmp(this->ini.get<cfg::context::auth_command>().c_str(), "rail_exec")) {
                                    const uint16_t flags                = ini.get<cfg::context::auth_command_rail_exec_flags>();
                                    const char*    original_exe_or_file = ini.get<cfg::context::auth_command_rail_exec_original_exe_or_file>().c_str();
                                    const char*    exe_or_file          = ini.get<cfg::context::auth_command_rail_exec_exe_or_file>().c_str();
                                    const char*    working_dir          = ini.get<cfg::context::auth_command_rail_exec_working_dir>().c_str();
                                    const char*    arguments            = ini.get<cfg::context::auth_command_rail_exec_arguments>().c_str();
                                    const uint16_t exec_result          = ini.get<cfg::context::auth_command_rail_exec_exec_result>();
                                    const char*    account              = ini.get<cfg::context::auth_command_rail_exec_account>().c_str();
                                    const char*    password             = ini.get<cfg::context::auth_command_rail_exec_password>().c_str();

                                    rdp_api* rdpapi = mod_wrapper.get_rdp_api();

                                    if (!exec_result) {
                                        //LOG(LOG_INFO,
                                        //    "RailExec: "
                                        //        "original_exe_or_file=\"%s\" "
                                        //        "exe_or_file=\"%s\" "
                                        //        "working_dir=\"%s\" "
                                        //        "arguments=\"%s\" "
                                        //        "flags=%u",
                                        //    original_exe_or_file, exe_or_file, working_dir, arguments, flags);

                                        if (rdpapi) {
                                            rdpapi->auth_rail_exec(flags, original_exe_or_file, exe_or_file, working_dir, arguments, account, password);
                                        }
                                    }
                                    else {
                                        //LOG(LOG_INFO,
                                        //    "RailExec: "
                                        //        "exec_result=%u "
                                        //        "original_exe_or_file=\"%s\" "
                                        //        "flags=%u",
                                        //    exec_result, original_exe_or_file, flags);

                                        if (rdpapi) {
                                            rdpapi->auth_rail_exec_cancel(flags, original_exe_or_file, exec_result);
                                        }
                                    }
                                }

                                ini.set<cfg::context::auth_command>("");
                            }
                        }

                        if (mod_wrapper.current_mod == MODULE_INTERNAL_SELECTOR)
                        {
                            inactivity.start_timer(ini.get<cfg::globals::session_timeout>(),
                                                   time_base.get_current_time().tv_sec);
                            if (is_first_looping_on_mod_selector)
                            {
                                switch (this->ini.get<cfg::translation::language>()) {
                                    case Language::en:
                                        this->ini.set_acl<cfg::translation::login_language>(
                                            LoginLanguage::EN);
                                        break;
                                    case Language::fr:
                                        this->ini.set_acl<cfg::translation::login_language>(
                                            LoginLanguage::FR);
                                        break;
                                }
                                is_first_looping_on_mod_selector = false;
                            }
                        }
                        else if (mod_wrapper.current_mod == MODULE_INTERNAL_LOGIN)
                        {
                            inactivity.stop_timer();
                        }
                    }
                } catch (Error const& e) {
                    run_session = false;

                    if (!front.is_up_and_running()) {
                        sesman.flush_acl_log6(write_acl_log6_fn);
                        sesman.flush_acl_disconnect_target([&log_file]()
                        {
                            log_file.close_session_log();
                        });
                        continue;
                    }

                    switch (end_session_exception(e, ini, mod_wrapper)){
                    case EndSessionResult::close_box:
                    {
                        keepalive.stop();
                        sesman.set_disconnect_target();
                        mod_wrapper.disconnect();
                        if (ini.get<cfg::globals::enable_close_box>())
                        {
                            rail_client_execute
                                .enable_remote_program(front.get_client_info()
                                                       .remote_program);
                            new_mod(MODULE_INTERNAL_CLOSE,
                                    mod_wrapper,
                                    mod_factory,
                                    front);
                            mod_wrapper
                                .get_mod()->set_mod_signal(BACK_EVENT_NONE);
                            run_session = true;
                            inactivity.stop_timer();
                        }
                        else
                        {
                            LOG(LOG_INFO, "Close Box disabled : ending session");
                        }
                    }
                    break;
                    case EndSessionResult::redirection:
                    {
                        // SET new target in ini
                        const char * host = char_ptr_cast(redir_info.host);
                        const char * password = char_ptr_cast(redir_info.password);
                        const char * username = char_ptr_cast(redir_info.username);
                        const char * change_user = "";
                        if (redir_info.dont_store_username && username[0] != 0) {
                            LOG(LOG_INFO, "SrvRedir: Change target username to '%s'", username);
                            ini.set_acl<cfg::globals::target_user>(username);
                            change_user = username;
                        }
                        if (password[0] != 0) {
                            LOG(LOG_INFO, "SrvRedir: Change target password");
                            ini.set_acl<cfg::context::target_password>(password);
                        }
                        LOG(LOG_INFO, "SrvRedir: Change target host to '%s'", host);
                        ini.set_acl<cfg::context::target_host>(host);
                        auto message = str_concat(change_user, '@', host);
                        sesman.report("SERVER_REDIRECTION", message.c_str());
                    }
                    REDEMPTION_CXX_FALLTHROUGH;

                    // TODO: should we put some counter to avoid retrying indefinitely?
                    case EndSessionResult::retry:
                        retry_rdp(rail_client_execute, front, mod_wrapper, mod_factory, sesman);
                        run_session = true;
                    break;
                    }
                }
            } // loop

            mod_wrapper.disconnect();
            front.disconnect();
        }
        catch (Error const& e) {
            // silent message for localhost for watchdog
            if (!source_is_localhost || e.id != ERR_TRANSPORT_WRITE_FAILED) {
                LOG(LOG_INFO, "Session Init exception %s", e.errmsg());
            }
        }
        catch (const std::exception & e) {
            LOG(LOG_ERR, "Session exception %s!", e.what());
        }
        catch(...) {
            LOG(LOG_ERR, "Session unexpected exception");
        }

        // silent message for localhost for watchdog
        if (!source_is_localhost) {
            if (!ini.is_asked<cfg::globals::host>()) {
                LOG(LOG_INFO, "Client Session Disconnected");
            }
            log_proxy::disconnection(disconnection_message_error.c_str());
        }

        front.must_be_stop_capture();

        // flush buffered messages
        try {
            sesman.flush_acl_log6(write_acl_log6_fn);
        }
        catch (...) {
            LOG(LOG_ERR, "flush_acl_log6()");
        }
    }

    Session(Session const &) = delete;

    ~Session()
    {
        // Suppress Session file from disk (original name with PID or renamed with session_id)
        auto const& session_id = this->ini.get<cfg::context::session_id>();
        if (!session_id.empty()) {
            char new_session_file[256];
            snprintf( new_session_file, sizeof(new_session_file), "%s/session_%s.pid"
                    , app_path(AppPath::LockDir).c_str(), session_id.c_str());
            unlink(new_session_file);
        }
        else {
            int child_pid = getpid();
            char old_session_file[256];
            sprintf(old_session_file, "%s/session_%d.pid", app_path(AppPath::LockDir).c_str(), child_pid);
            unlink(old_session_file);
        }
    }
};

template<class SocketType, class... Args>
void session_start_sck(
    char const* name, unique_fd&& sck,
    timeval sck_start_time, Inifile& ini,
    Args&&... args)
{
    auto const watchdog_verbosity = (ini.get<cfg::globals::host>() == "127.0.0.1")
        ? SocketTransport::Verbose::watchdog
        : SocketTransport::Verbose();
    auto const sck_verbosity = safe_cast<SocketTransport::Verbose>(
        ini.get<cfg::debug::sck_front>());

    Session session(
        SocketType(
            name, std::move(sck), "", 0, ini.get<cfg::client::recv_timeout>(),
            static_cast<Args&&>(args)..., sck_verbosity | watchdog_verbosity
        ),
        sck_start_time, ini
    );
}

} // anonymous namespace

void session_start_tls(unique_fd sck, timeval sck_start_time, Inifile& ini)
{
    session_start_sck<SocketTransport>("RDP Client", std::move(sck), sck_start_time, ini);
}

void session_start_ws(unique_fd sck, timeval sck_start_time, Inifile& ini)
{
    session_start_sck<WsTransport>("RDP Ws Client", std::move(sck), sck_start_time, ini,
        WsTransport::UseTls::No, WsTransport::TlsOptions());
}

void session_start_wss(unique_fd sck, timeval sck_start_time, Inifile& ini)
{
    session_start_sck<WsTransport>("RDP Wss Client", std::move(sck), sck_start_time, ini,
        WsTransport::UseTls::Yes, WsTransport::TlsOptions{
            ini.get<cfg::globals::certificate_password>(),
            ini.get<cfg::client::ssl_cipher_list>(),
            ini.get<cfg::client::tls_min_level>(),
            ini.get<cfg::client::tls_max_level>(),
            ini.get<cfg::client::show_common_cipher_list>(),
        });
}
