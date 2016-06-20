/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2010
   Author(s): Christophe Grosjean, Javier Caverni, Meng Tan
   Based on xrdp Copyright (C) Jay Sorg 2004-2010

   Synchronisation objects

*/


#pragma once

#include "utils/difftimeval.hpp"
#include "utils/sugar/noncopyable.hpp"
#include "utils/invalid_socket.hpp"

enum BackEvent_t {
    BACK_EVENT_NONE = 0,
    BACK_EVENT_NEXT,
    BACK_EVENT_STOP = 4,
    BACK_EVENT_REFRESH,

    BACK_EVENT_RETRY_CURRENT
};


class wait_obj : noncopyable
{
public:
    bool        set_state;
    BackEvent_t signal;
    timeval     trigger_time;
    bool        object_and_time;
    bool        waked_up_by_time;

    wait_obj()
    : set_state(false)
    , signal(BACK_EVENT_NONE)
    , object_and_time(false)
    , waked_up_by_time(false)
    {
        this->trigger_time = tvtime();
    }

    void reset()
    {
        this->set_state = false;
    }

    // Idle time in microsecond
    void set(uint64_t idle_usec = 0)
    {
        this->set_state = true;
        struct timeval now = tvtime();

        // uint64_t sum_usec = (now.tv_usec + idle_usec);
        // this->trigger_time.tv_sec = (sum_usec / 1000000) + now.tv_sec;
        // this->trigger_time.tv_usec = sum_usec % 1000000;
        this->trigger_time = addusectimeval(idle_usec, now);
    }

    // Idle time in microsecond
    void update(uint64_t idle_usec)
    {
        if (!idle_usec) {
            return;
        }
        if (this->set_state) {
            timeval now = tvtime();
            timeval new_trigger = addusectimeval(idle_usec, now);
            if (lessthantimeval(new_trigger, this->trigger_time)) {
                this->trigger_time = new_trigger;
            }
        }
        else {
            this->set(idle_usec);
        }
    }

// NOTE: old-style-cast is ignored because of FD_xxx macros using it behind the hood
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
    void add_to_fd_set(int fd, fd_set & rfds, unsigned & max, timeval & timeout) const
    {
        if (fd > INVALID_SOCKET) {
            FD_SET(fd, &rfds);
            max = (static_cast<unsigned>(fd) > max) ? fd : max;
        }
        if ((fd <= INVALID_SOCKET || this->object_and_time) && this->set_state) {
            struct timeval now;
            now = tvtime();
            timeval remain = how_long_to_wait(this->trigger_time, now);
            if (lessthantimeval(remain, timeout)) {
                timeout = remain;
            }
        }
    }

    bool is_set(int fd, fd_set & rfds)
    {
        this->waked_up_by_time = false;

        if (fd > INVALID_SOCKET) {
            bool res = FD_ISSET(fd, &rfds);

            if (res || !this->object_and_time) {
                return res;
            }
        }

        if (this->set_state) {
            if (tvtime() >= this->trigger_time) {
                this->waked_up_by_time = true;
                return true;
            }
        }

        return false;
    }
#pragma GCC diagnostic pop
};

