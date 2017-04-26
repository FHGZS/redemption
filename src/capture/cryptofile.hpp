/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Product name: redemption, a FLOSS RDP proxy
 *   Copyright (C) Wallix 2010-2017
 *   Author(s): Christophe Grosjean, Raphael Zhou, Jonathan Poelen, Meng Tan
 */

#pragma once

#include <string.h>
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <array>
#include <snappy-c.h>

#include "utils/log.hpp"
#include "utils/sugar/bytes_t.hpp"
#include "utils/genrandom.hpp"
#include "openssl_crypto.hpp"
#include "system/ssl_sha256.hpp"

using MD_HASH = SslSha256;

enum crypto_file_state {
    CF_READY = 0, // Crypto File Reading
    CF_EOF = 1,   // Crypto File reached EOF
    CF_INIT = 2,  // Crypto File nor yet initialised
};

// "MFCW"
constexpr uint32_t WABCRYPTOFILE_MAGIC = 0x4D464357;
constexpr uint32_t WABCRYPTOFILE_EOF_MAGIC = 0x5743464D;
constexpr uint32_t WABCRYPTOFILE_VERSION = 0x00000001;

enum {
    DERIVATOR_LENGTH = 8
};


constexpr std::size_t CRYPTO_BUFFER_SIZE = 4096 * 4;

extern "C" {
    typedef int get_hmac_key_prototype(char * buffer);
    typedef int get_trace_key_prototype(char * base, int len, char * buffer, unsigned oldscheme);
}


constexpr std::size_t CRYPTO_KEY_LENGTH = MD_HASH::DIGEST_LENGTH;
constexpr std::size_t HMAC_KEY_LENGTH = CRYPTO_KEY_LENGTH;


class CryptoContext
{
    unsigned char master_key[CRYPTO_KEY_LENGTH] {};
    unsigned char hmac_key[HMAC_KEY_LENGTH] {};

    get_hmac_key_prototype * get_hmac_key_cb = nullptr;
    get_trace_key_prototype * get_trace_key_cb = nullptr;

    bool master_key_loaded = false;
    bool hmac_key_loaded = false;


public:
    bool old_encryption_scheme = false;


public:
    auto get_hmac_key() -> unsigned char const (&)[HMAC_KEY_LENGTH]
    {
        if (!this->hmac_key_loaded){
            if (!this->get_hmac_key_cb) {
                LOG(LOG_ERR, "CryptoContext: undefined hmac_key callback");
                throw Error(ERR_WRM_INVALID_INIT_CRYPT);
            }
            // if we have a callback ask key
            this->get_hmac_key_cb(reinterpret_cast<char*>(this->hmac_key));
            this->hmac_key_loaded = true;
        }
        return this->hmac_key;
    }

    const unsigned char * get_master_key() const
    {
        assert(this->master_key_loaded);
        return this->master_key;
    }

    void get_derived_key(uint8_t (& trace_key)[CRYPTO_KEY_LENGTH], const uint8_t * derivator, size_t derivator_len)
    {
        if (this->old_encryption_scheme){
            if (this->get_trace_key_cb != nullptr){
                // if we have a callback ask key
                uint8_t tmp[MD_HASH::DIGEST_LENGTH];
                this->get_trace_key_cb(
                      reinterpret_cast<char*>(const_cast<uint8_t*>(derivator))
                    , static_cast<int>(derivator_len)
                    , reinterpret_cast<char*>(tmp)
                    , this->old_encryption_scheme?1:0
                    );
                memcpy(trace_key, tmp, HMAC_KEY_LENGTH);
                return;
            }
        }

        if (!this->master_key_loaded){
            if (this->get_trace_key_cb == nullptr) {
                LOG(LOG_ERR, "CryptoContext: undefined trace_key callback");
                throw Error(ERR_WRM_INVALID_INIT_CRYPT);
            }

            // if we have a callback ask key
            this->get_trace_key_cb(
                reinterpret_cast<char*>(const_cast<uint8_t*>(derivator))
              , static_cast<int>(derivator_len)
              , reinterpret_cast<char*>(this->master_key)
              , this->old_encryption_scheme?1:0
            );
            this->master_key_loaded = true;
        }

        uint8_t tmp[MD_HASH::DIGEST_LENGTH];
        {
            MD_HASH sha256;
            sha256.update(derivator, derivator_len);
            sha256.final(tmp);
        }
        {
            MD_HASH sha256;
            sha256.update(tmp, DERIVATOR_LENGTH);
            sha256.update(this->master_key, CRYPTO_KEY_LENGTH);
            sha256.final(tmp);
        }
        memcpy(trace_key, tmp, HMAC_KEY_LENGTH);
    }

    CryptoContext() = default;

    size_t unbase64(char *buffer, size_t bufsiz, const char *txt)
    {
        const unsigned char _base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        unsigned int bits = 0;
        int nbits = 0;
        char base64tbl[256];
        size_t nbytes = 0;

        memset(base64tbl, -1, sizeof base64tbl);

        for (unsigned i = 0; _base64chars[i]; i++) {
            base64tbl[_base64chars[i]] = i;
        }

        base64tbl[int('.')] = 62;
        base64tbl[int('-')] = 62;
        base64tbl[int('_')] = 63;

        while (*txt) {
            char const v = base64tbl[static_cast<unsigned char>(*txt)];
            if (v >= 0) {
                bits <<= 6;
                bits += v;
                nbits += 6;
                if (nbits >= 8) {
                    if (nbytes < bufsiz)
                        *buffer++ = (bits >> (nbits - 8));
                    nbytes++;
                    nbits -= 8;
                }
            }
            txt++;
        }

        return nbytes;
    }

    class key_data : private const_bytes_array
    {
        static constexpr std::size_t key_length = 32;

        static_assert(sizeof(master_key) == key_length, "");
        static_assert(sizeof(hmac_key) == key_length, "");

        friend class CryptoContext;

    public:
        template<class T>
        key_data(T const & bytes32) noexcept
        : const_bytes_array(bytes32)
        {
            assert(this->size() == key_length);
        }

        template<class T, std::size_t array_length>
        key_data(std::array<T, array_length> const & data) noexcept
        : const_bytes_array(data.data(), data.size())
        {
            static_assert(array_length == key_length, "");
        }

        template<class T, std::size_t array_length>
        key_data(T const (& data)[array_length]) noexcept
        : const_bytes_array(data, array_length)
        {
            static_assert(array_length == key_length, "");
        }
    };

    void set_master_key(key_data key) noexcept
    {
        memcpy(this->master_key, key.data(), sizeof(this->master_key));
        this->master_key_loaded = true;
    }

    void set_hmac_key(key_data key) noexcept
    {
        memcpy(this->hmac_key, key.data(), sizeof(this->hmac_key));
        this->hmac_key_loaded = true;
    }

    void set_get_hmac_key_cb(get_hmac_key_prototype * get_hmac_key_cb)
    {
        this->get_hmac_key_cb = get_hmac_key_cb;
    }

    void set_get_trace_key_cb(get_trace_key_prototype * get_trace_key_cb)
    {
        this->get_trace_key_cb = get_trace_key_cb;
    }
};

struct ocrypto
{
    struct Result {
        const_bytes_array buf;
        std::size_t consumed;
    };

private:
    EVP_CIPHER_CTX ectx;                    // [en|de]cryption context
    SslHMAC_Sha256_Delayed hm;              // full hash context
    SslHMAC_Sha256_Delayed hm4k;             // quick hash context
    uint32_t       pos;                     // current position in buf
    uint32_t       raw_size;                // the unciphered/uncompressed file size
    uint32_t       file_size;               // the current file size
    uint8_t header_buf[40];
    uint8_t result_buffer[32768] = {};
    char           buf[CRYPTO_BUFFER_SIZE]; //

    CryptoContext & cctx;
    Random & rnd;
    bool encryption;
    bool checksum;

    /* Encrypt src_buf into dst_buf. Update dst_sz with encrypted output size
     * Return 0 on success, negative value on error
     */
    void xaes_encrypt(const unsigned char *src_buf, uint32_t src_sz, unsigned char *dst_buf, uint32_t *dst_sz)
    {
        int safe_size = *dst_sz;
        int remaining_size = 0;

        /* allows reusing of ectx for multiple encryption cycles */
        if (EVP_EncryptInit_ex(&this->ectx, nullptr, nullptr, nullptr, nullptr) != 1){
            throw Error(ERR_SSL_CALL_FAILED);
        }
        if (EVP_EncryptUpdate(&this->ectx, dst_buf, &safe_size, src_buf, src_sz) != 1) {
            throw Error(ERR_SSL_CALL_FAILED);
        }
        if (EVP_EncryptFinal_ex(&this->ectx, dst_buf + safe_size, &remaining_size) != 1){
            throw Error(ERR_SSL_CALL_FAILED);
        }
        *dst_sz = safe_size + remaining_size;
    }

    /* Flush procedure (compression, encryption)
     * Return 0 on success, negatif on error
     */
    void flush(uint8_t * buffer, size_t buflen, size_t & towrite)
    {
        // No data to flush
        if (!this->pos) {
            return;
        }
        // Compress
        // TODO: check this
        char compressed_buf[65536];
        size_t compressed_buf_sz = ::snappy_max_compressed_length(this->pos);
        snappy_status status = snappy_compress(this->buf, this->pos, compressed_buf, &compressed_buf_sz);

        switch (status)
        {
            case SNAPPY_OK:
                break;
            case SNAPPY_INVALID_INPUT:
                throw Error(ERR_CRYPTO_SNAPPY_COMPRESSION_INVALID_INPUT);
            case SNAPPY_BUFFER_TOO_SMALL:
                throw Error(ERR_CRYPTO_SNAPPY_BUFFER_TOO_SMALL);
        }

        // Encrypt
        unsigned char ciphered_buf[4 + 65536];
        uint32_t ciphered_buf_sz = compressed_buf_sz + AES_BLOCK_SIZE;
        this->xaes_encrypt(reinterpret_cast<unsigned char*>(compressed_buf),
                           compressed_buf_sz,
                           ciphered_buf + 4, &ciphered_buf_sz);

        ciphered_buf[0] = ciphered_buf_sz & 0xFF;
        ciphered_buf[1] = (ciphered_buf_sz >> 8) & 0xFF;
        ciphered_buf[2] = (ciphered_buf_sz >> 16) & 0xFF;
        ciphered_buf[3] = (ciphered_buf_sz >> 24) & 0xFF;

        ciphered_buf_sz += 4;
        if (ciphered_buf_sz > buflen){
            throw Error(ERR_CRYPTO_BUFFER_TOO_SMALL);
        }
        ::memcpy(buffer, ciphered_buf, ciphered_buf_sz);
        towrite += ciphered_buf_sz;

        this->update_hmac(&ciphered_buf[0], ciphered_buf_sz);

        // Reset buffer
        this->pos = 0;
    }

    void update_hmac(uint8_t const * buf, size_t len)
    {
        if (this->checksum){
            this->hm.update(buf, len);
            if (this->file_size < 4096) {
                size_t remaining_size = 4096 - this->file_size;
                this->hm4k.update(buf, std::min(remaining_size, len));
            }
            this->file_size += len;
        }
    }

public:
    ocrypto(bool encryption, bool checksum, CryptoContext & cctx, Random & rnd)
        : cctx(cctx)
        , rnd(rnd)
        , encryption(encryption)
        , checksum(checksum)
    {
    }

    ~ocrypto() = default;

    Result open(const uint8_t * derivator, size_t derivator_len)
    {
        this->file_size = 0;
        this->pos = 0;
        if (this->checksum) {
            this->hm.init(this->cctx.get_hmac_key(), CRYPTO_KEY_LENGTH);
            this->hm4k.init(this->cctx.get_hmac_key(), CRYPTO_KEY_LENGTH);
        }

        if (this->encryption) {
            unsigned char trace_key[CRYPTO_KEY_LENGTH]; // derived key for cipher
            this->cctx.get_derived_key(trace_key, derivator, derivator_len);
            unsigned char iv[32];
            this->rnd.random(iv, 32);

            ::memset(this->buf, 0, sizeof(this->buf));
            ::memset(&this->ectx, 0, sizeof(this->ectx));
            this->pos = 0;
            this->raw_size = 0;

            const EVP_CIPHER * cipher  = ::EVP_aes_256_cbc();
            const unsigned int salt[]  = { 12345, 54321 };    // suspicious, to check...
            const int          nrounds = 5;
            unsigned char      key[32];
            const int i = ::EVP_BytesToKey(cipher, ::EVP_sha1(), reinterpret_cast<const unsigned char *>(salt),
                                           trace_key, CRYPTO_KEY_LENGTH, nrounds, key, nullptr);
            if (i != 32) {
                throw Error(ERR_SSL_CALL_FAILED);
            }

            ::EVP_CIPHER_CTX_init(&this->ectx);
            if (::EVP_EncryptInit_ex(&this->ectx, cipher, nullptr, key, iv) != 1) {
                throw Error(ERR_SSL_CALL_FAILED);
            }

            // update context with previously written data
            this->header_buf[0] = 'W';
            this->header_buf[1] = 'C';
            this->header_buf[2] = 'F';
            this->header_buf[3] = 'M';
            this->header_buf[4] = WABCRYPTOFILE_VERSION & 0xFF;
            this->header_buf[5] = (WABCRYPTOFILE_VERSION >> 8) & 0xFF;
            this->header_buf[6] = (WABCRYPTOFILE_VERSION >> 16) & 0xFF;
            this->header_buf[7] = (WABCRYPTOFILE_VERSION >> 24) & 0xFF;
            ::memcpy(this->header_buf + 8, iv, 32);
            this->update_hmac(&this->header_buf[0], 40);
            return Result{{this->header_buf, 40u}, 0u};
        }
        else {
            return Result{{this->header_buf, 0u}, 0u};
        }
    }

    ocrypto::Result close(uint8_t (&qhash)[MD_HASH::DIGEST_LENGTH], uint8_t (&fhash)[MD_HASH::DIGEST_LENGTH])
    {
        size_t towrite = 0;
        if (this->encryption) {
            size_t buflen = sizeof(this->result_buffer);
            this->flush(this->result_buffer, buflen, towrite);

            unsigned char tmp_buf[8] = {
                'M','F','C','W',
                uint8_t(this->raw_size & 0xFF),
                uint8_t((this->raw_size >> 8) & 0xFF),
                uint8_t((this->raw_size >> 16) & 0xFF),
                uint8_t((this->raw_size >> 24) & 0xFF),
            };

            if (towrite + 8 > buflen){
                throw Error(ERR_CRYPTO_BUFFER_TOO_SMALL);
            }
            ::memcpy(this->result_buffer + towrite, tmp_buf, 8);
            towrite += 8;

            this->update_hmac(tmp_buf, 8);
        }

        if (this->checksum) {
            this->hm.final(fhash);
            this->hm4k.final(qhash);

        }
        return Result{{this->result_buffer, towrite}, 0u};

    }

    ocrypto::Result write(const uint8_t * data, size_t len)
    {
        if (!this->encryption) {
            this->update_hmac(data, len);
            return Result{{data, len}, len};
        }

        size_t buflen = sizeof(this->result_buffer);
        size_t towrite = 0;
        unsigned int remaining_size = len;
        while (remaining_size > 0) {
            // Check how much we can append into buffer
            unsigned int available_size = std::min(unsigned(CRYPTO_BUFFER_SIZE - this->pos), remaining_size);
            // Append and update pos pointer
            ::memcpy(this->buf + this->pos, data + (len - remaining_size), available_size);
            this->pos += available_size;
            // If buffer is full, flush it to disk
            if (this->pos == CRYPTO_BUFFER_SIZE) {
                size_t tmp_towrite = 0;
                this->flush(this->result_buffer + towrite, buflen - towrite, tmp_towrite);
                towrite += tmp_towrite;
            }
            remaining_size -= available_size;
        }
        // Update raw size counter
        this->raw_size += len;
        return {{this->result_buffer, towrite}, len};
    }
};
