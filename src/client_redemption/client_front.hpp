/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Product name: redemption, a FLOSS RDP proxy
 * Copyright (C) Wallix 2015
 *
 * free RDP client main program
 *
 */

#include "utils/log.hpp"
#include "utils/png.hpp"
#include "utils/difftimeval.hpp"
#include "utils/file.hpp"
#include "utils/timebase.hpp"
#include "utils/select.hpp"
#include "core/events.hpp"
#include "core/front_api.hpp"
#include "core/RDP/RDPDrawable.hpp"
#include "core/channel_list.hpp"
#include "cxx/cxx.hpp"

class ClientFront : public FrontAPI
{
    bool verbose;
    ScreenInfo& screen_info;
    CHANNELS::ChannelDefArray   cl;

public:
    ClientFront(ScreenInfo& screen_info, bool verbose)
    : verbose(verbose)
    , screen_info(screen_info)
    {}

    void session_update(timeval /*now*/, LogId /*id*/, KVLogList /*kv_list*/) override {}
    void possible_active_window_change() override {}

    bool can_be_start_capture(bool /*force_capture*/) override
    {
        return false;
    }

    bool must_be_stop_capture() override
    {
        return false;
    }

    bool is_capture_in_progress() const override
    {
        return false;
    }

    ResizeResult server_resize(ScreenInfo screen_server) override
    {
        this->screen_info = screen_server;
        LOG_IF(this->verbose, LOG_INFO,
            "ClientFront::server_resize(width=%u, height=%u, bpp=%d",
            screen_server.width, screen_server.height, screen_server.bpp);
        return ResizeResult::instant_done;
    }

    const CHANNELS::ChannelDefArray & get_channel_list() const override { return cl; }

    void send_to_channel( const CHANNELS::ChannelDef & /*channel*/, bytes_view /*chunk_data*/
                        , std::size_t /*total_length*/, int /*flags*/) override {
        LOG_IF(this->verbose, LOG_INFO, "ClientFront::send_to_channel");
    }

    void update_pointer_position(uint16_t /*unused*/, uint16_t /*unused*/) override {}
};

#include "mod/mod_api.hpp"

inline int run_connection_test(
    char const * type,
    EventContainer & events,
    mod_api& mod)
{
    int       timeout_counter = 0;
    int const timeout_counter_max = 3;
    std::chrono::milliseconds const timeout = 5s;

    for (;;) {
        LOG(LOG_INFO, "run_connection_test");

        timeval now = tvtime();
        int max = 0;
        fd_set   rfds;
        io_fd_zero(rfds);

        timeval ultimatum =  now + timeout;

        events.get_fds([&rfds,&max](int fd){ io_fd_set(fd, rfds); max = std::max(max, fd);});
        events.get_fds_timeouts([&ultimatum](timeval tv){ultimatum = std::min(tv,ultimatum);});
        if (ultimatum < now){
            ultimatum = now;
        }
        std::chrono::microseconds difftime = ultimatum - now;
        timeval timeoutastv = timeval{difftime.count()/1000000u,difftime.count()%1000000};

        int num = select(max + 1, &rfds, nullptr, nullptr, &timeoutastv);

        if (num < 0) {
            if (errno != EINTR) {
                LOG(LOG_INFO, "%s CLIENT :: errno = %d", type, errno);
                return 1;
            }
            continue;
        }

        timeval now_after_select = tvtime();
        events.execute_events(now_after_select, [](int /*fd*/){ return false; }, false);
        if (num) {
            events.execute_events(now_after_select, [&rfds](int fd){ return io_fd_isset(fd, rfds); }, false);
            if (mod.is_up_and_running()) {
                LOG(LOG_INFO, "%s CLIENT :: Done", type);
                return 0;
            }
            continue;
        }
        ++timeout_counter;
        LOG(LOG_INFO, "%s CLIENT :: Timeout (%d/%d)", type, timeout_counter, timeout_counter_max);
        if (timeout_counter == timeout_counter_max) {
            return 2;
        }
    }
}

// return 0 : do screenshot, don't do screenshot an error occurred
inline int wait_for_screenshot(
    char const* type,
        EventContainer & events,
        std::chrono::milliseconds inactivity_time,
        std::chrono::milliseconds max_time)
{
    auto const time_start = ustime();

    for (;;) {
        auto const elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            ustime() - time_start);

        if (elapsed >= max_time) {
            return 0;
        }

        std::chrono::milliseconds timeout = std::min(max_time - elapsed, inactivity_time);

        timeval now = tvtime();
        unsigned max = 0;
        fd_set   rfds;
        io_fd_zero(rfds);

        timeval ultimatum =  now + timeout;

        events.get_fds([&rfds,&max](int fd){ io_fd_set(fd, rfds); max = std::max(max, unsigned(fd));});
        events.get_fds_timeouts([&ultimatum](timeval tv){ultimatum = std::min(tv,ultimatum);});
        if (ultimatum < now){
            ultimatum = now;
        }
        std::chrono::microseconds difftime = ultimatum - now;
        timeval timeoutastv = timeval{difftime.count()/1000000u,difftime.count()%1000000};

        int num = select(max + 1, &rfds, nullptr, nullptr, &timeoutastv);

        if (num < 0) {
            if (errno != EINTR) {
                LOG(LOG_INFO, "%s CLIENT :: errno = %d", type, errno);
                return 1;
            }
        }

        timeval now_after_select = tvtime();
        events.execute_events(now_after_select, [](int /*fd*/){ return false; }, false);
        if (num > 0) {
            events.execute_events(now_after_select, [&rfds](int fd){ return io_fd_isset(fd, rfds); }, true);
            LOG(LOG_INFO, "%s CLIENT :: draw_event", type);
        }
        events.execute_events(now_after_select, [](int /*fd*/){ return false; }, false);
        if (timeout == 0ms) {
            return 0;
        }
    }
}

inline int run_test_client(
    char const* type,
        EventContainer & events,
        mod_api& mod, std::chrono::milliseconds inactivity_time, std::chrono::milliseconds max_time,
    std::string const& screen_output)
{
    try {
        if (int err = run_connection_test(type, events, mod)) {
            return err;
        }

        if (screen_output.empty()) {
            return 0;
        }

        File f(screen_output, "w");
        if (!f) {
            LOG(LOG_ERR, "%s CLIENT :: %s: %s", type, screen_output, strerror(errno));
            return ERR_RECORDER_FAILED_TO_OPEN_TARGET_FILE;
        }

        Dimension dim = mod.get_dim();
        RDPDrawable gd(dim.w, dim.h);

        if (int err = wait_for_screenshot(type, events, inactivity_time, max_time)) {
            return err;
        }

        dump_png24(f.get(), gd, true);

        return 0;
    }
    catch (Error const & e) {
        LOG(LOG_ERR, "%s CLIENT :: Exception raised = %s !", type, e.errmsg());
        return e.id;
    }
}
