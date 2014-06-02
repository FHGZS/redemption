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
   Copyright (C) Wallix 2012
   Author(s): Christophe Grosjean

   Transport layer abstraction
*/

#ifndef REDEMPTION_TRANSPORT_OUTFILENAMETRANSPORT_HPP
#define REDEMPTION_TRANSPORT_OUTFILENAMETRANSPORT_HPP

#include "transport.hpp"
#include "rio/rio_impl.h"
#include "rio/rio_outsequence.h"
#include "rio/rio.h"
#include "rio/cryptofile.hpp"
#include "outfiletransport.hpp"
#include "sequence_generator.hpp"
#include "fdbub.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace detail
{
    class OutFilenameCreator
    {
        FilenameGenerator filegen;
        char        current_filename[1024];
        unsigned    count;
        int         fd;

    public:
        OutFilenameCreator(SQ_FORMAT format,
                           const char * const prefix,
                           const char * const filename,
                           const char * const extension,
                           const int groupid)
        : filegen(format, prefix, filename, extension, groupid)
        , count(0)
        , fd(-1)
        {
            this->current_filename[0] = 0;
        }

        const FilenameGenerator & seqgen() const
        {
            return this->filegen;
        }

        const char * get_filename() const
        {
            return this->filegen.get(this->count);
        }

        const char * get_path() const
        {
            return this->filegen.path;
        }

        int get_fd() const
        {
            return this->fd;
        }

        void open_if_not_open(int err_id)
        {
            if (!this->is_open()) {
                snprintf(this->current_filename, sizeof(this->current_filename), "%sred-XXXXXX.tmp", this->filegen.path);
                TODO("add rights information to constructor");
                this->fd = ::mkostemps(this->current_filename, 4, O_WRONLY | O_CREAT);
                if (this->fd < 0) {
                    throw Error(err_id, errno);
                }
                if (chmod( this->current_filename
                         , (this->filegen.groupid ? (S_IRUSR | S_IRGRP) : S_IRUSR)) == -1) {
                    LOG( LOG_ERR, "can't set file %s mod to %s : %s [%u]"
                    , this->current_filename, strerror(errno), errno
                    , (this->filegen.groupid ? "u+r, g+r" : "u+r"));
                }
                this->filegen.set_last_filename(this->count, this->current_filename);
            }
        }

        bool is_open() const
        {
            return -1 != this->fd;
        }

        void rename_tmp()
        {
            if (this->is_open()) {
                int res = close(this->fd);
                this->fd = -1;
                if (res < 0) {
                    LOG(LOG_ERR, "closing file failed erro=%u : %s\n", errno, strerror(errno));
                }
                this->filegen.set_last_filename(0, 0);

                // LOG(LOG_INFO, "\"%s\" -> \"%s\".", this->current_filename, this->rename_to);
                const char * filename = this->get_filename();
                res = rename(this->current_filename, filename);
                if (res < 0) {
                    LOG( LOG_ERR, "renaming file \"%s\" -> \"%s\" failed erro=%u : %s\n"
                    , this->current_filename, filename, errno, strerror(errno));
                }

                this->current_filename[0] = 0;
            }
        }

        void next()
        {
            this->rename_tmp();
            ++this->count;
        }

        void send(const char * const buffer, size_t len, auth_api * authentifier) throw (Error)
        {
            this->open_if_not_open(ERR_TRANSPORT_WRITE_FAILED);
            ssize_t res = io::posix::write_all(this->fd, buffer, len);
            if (res < 0){
                if (errno == ENOSPC) {
                    char message[1024];
                    snprintf(message, sizeof(message), "100|%s", this->filegen.path);
                    authentifier->report("FILESYSTEM_FULL", message);
                }
                LOG(LOG_INFO, "Write to transport failed (F): code=%d", errno);
                throw Error(ERR_TRANSPORT_WRITE_FAILED, errno);
            }
        }

        void seek(int64_t offset, int whence) throw (Error)
        {
            if (::lseek(this->fd, offset, whence) < 0) {
                throw Error(ERR_TRANSPORT_SEEK_FAILED, errno);
            }
        }
    };
}


class OutFilenameTransport
: public Transport
{
    detail::OutFilenameCreator filename_creator;

public:
    OutFilenameTransport(SQ_FORMAT format,
                         const char * const prefix,
                         const char * const filename,
                         const char * const extension,
                         const int groupid,
                         auth_api * authentifier = NULL,
                         unsigned verbose = 0)
    : filename_creator(format, prefix, filename, extension, groupid)
    {
        if (authentifier) {
            this->set_authentifier(authentifier);
        }
    }

    virtual ~OutFilenameTransport()
    {
        this->filename_creator.next();
    }

    const FilenameGenerator * seqgen() const
    {
        return &this->filename_creator.seqgen();
    }

    using Transport::send;
    virtual void send(const char * const buffer, size_t len) throw (Error) {
        this->filename_creator.send(buffer, len, this->authentifier);
    }

    using Transport::recv;
    virtual void recv(char**, size_t) throw (Error)
    {
        LOG(LOG_ERR, "OutFilenameTransport used for recv");
        throw Error(ERR_TRANSPORT_OUTPUT_ONLY_USED_FOR_SEND, 0);
    }

    virtual void seek(int64_t offset, int whence) throw (Error)
    {
        this->filename_creator.seek(offset, whence);
    }

    virtual bool next()
    {
        this->filename_creator.next();
        return Transport::next();
    }

    virtual void disconnect()
    {
        this->filename_creator.rename_tmp();
        this->status = false;
    }
};



/*****************************
* CryptoOutFilenameTransport *
*****************************/

// class CryptoOutFilenameTransport
// : public Transport
// {
//     CryptoContext * crypto_ctx;
//     crypto_file cf_struct;
//
//
//
//     int fd;
//
//     SQ_FORMAT   format;
//     char        path[1024];
//     char        filename[1024];
//     char        extension[12];
//     char        tempnam[2048];
//     unsigned    pid;
//     unsigned    count;
//     int         groupid;
//
//
// public:
//     SQ   seq;
//     RIO  rio;
//     char path2[512];
//
//     CryptoOutFilenameTransport(
//             CryptoContext * crypto_ctx,
//             SQ_FORMAT format,
//             const char * const prefix,
//             const char * const filename,
//             const char * const extension,
//             const int groupid,
//             auth_api * authentifier = NULL,
//             unsigned verbose = 0)
//     {
//         if (authentifier) {
//             this->set_authentifier(authentifier);
//         }
//
//         this->count      = 0;
//         this->format     = format;
//         this->pid        = getpid();
//         this->crypto_ctx = crypto_ctx;
//
//         const size_t len = strlen(filename);
//
//         if (len > sizeof(this->path) - 1){
//             LOG(LOG_ERR, "Sequence outfilename initialisation failed (%u)", RIO_ERROR_STRING_PATH_TOO_LONG);
//             throw Error(ERR_TRANSPORT);
//         }
//         strcpy(this->path, filename);
//         if (len > sizeof(this->filename) - 1){
//             LOG(LOG_ERR, "Sequence outfilename initialisation failed (%u)", RIO_ERROR_STRING_FILENAME_TOO_LONG);
//             throw Error(ERR_TRANSPORT);
//         }
//         strcpy(this->filename, filename);
//
//         if (strlen(extension) > sizeof(this->extension) - 1){
//             LOG(LOG_ERR, "Sequence outfilename initialisation failed (%u)", RIO_ERROR_STRING_EXTENSION_TOO_LONG);
//             throw Error(ERR_TRANSPORT);
//         }
//         strcpy(this->extension, extension);
//         this->groupid = groupid;
//
//         size_t max_path_length = sizeof(path2) - 1;
//         strncpy(this->path2, prefix, max_path_length);
//         this->path2[max_path_length] = 0;
//     }
//
//     ~CryptoOutFilenameTransport()
//     {
//         rio_clear(&this->rio);
//         sq_clear(&this->seq);
//     }
//
//     using Transport::send;
//     virtual void send(const char * const buffer, size_t len) throw (Error) {
//         ssize_t res = rio_send(&this->rio, buffer, len);
//         if (res < 0){
//             if (errno == ENOSPC) {
//                 char message[1024];
//                 snprintf(message, sizeof(message), "100|%s", this->path2);
//                 this->authentifier->report("FILESYSTEM_FULL", message);
//             }
//
//             LOG(LOG_INFO, "Write to transport failed (CF): code=%d", errno);
//             throw Error(ERR_TRANSPORT_WRITE_FAILED, errno);
//         }
//     }
//
//     using Transport::recv;
//     virtual void recv(char**, size_t) throw (Error)
//     {
//         LOG(LOG_INFO, "OutFilenameTransport used for recv");
//         throw Error(ERR_TRANSPORT_OUTPUT_ONLY_USED_FOR_SEND, 0);
//     }
//
//     virtual void seek(int64_t offset, int whence) throw (Error) {
//         throw Error(ERR_TRANSPORT_SEEK_NOT_AVAILABLE, errno);
//     }
//
//     virtual bool next()
//     {
//         sq_next(&this->seq);
//         return Transport::next();
//     }
// };

#endif
