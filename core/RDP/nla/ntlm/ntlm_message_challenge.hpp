/*
    This program is free software; you can redistribute it and/or modify it
     under the terms of the GNU General Public License as published by the
     Free Software Foundation; either version 2 of the License, or (at your
     option) any later version.

    This program is distributed in the hope that it will be useful, but
     WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
     Public License for more details.

    You should have received a copy of the GNU General Public License along
     with this program; if not, write to the Free Software Foundation, Inc.,
     675 Mass Ave, Cambridge, MA 02139, USA.

    Product name: redemption, a FLOSS RDP proxy
    Copyright (C) Wallix 2013
    Author(s): Christophe Grosjean, Raphael Zhou, Meng Tan
*/

#ifndef _REDEMPTION_CORE_RDP_NLA_NTLM_NTLMMESSAGECHALLENGE_HPP_
#define _REDEMPTION_CORE_RDP_NLA_NTLM_NTLMMESSAGECHALLENGE_HPP_

#include "RDP/nla/ntlm/ntlm_message.hpp"
#include "RDP/nla/ntlm/ntlm_avpair.hpp"

// [MS-NLMP]

// 2.2.1.2   CHALLENGE_MESSAGE
// ======================================================================
// The CHALLENGE_MESSAGE defines an NTLM challenge message that is sent from
//  the server to the client. The CHALLENGE_MESSAGE is used by the server to
//  challenge the client to prove its identity.
//  For connection-oriented requests, the CHALLENGE_MESSAGE generated by the
//  server is in response to the NEGOTIATE_MESSAGE (section 2.2.1.1) from the client.

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                           Signature                           |
// +---------------+---------------+---------------+---------------+
// |                              ...                              |
// +---------------+---------------+---------------+---------------+
// |                          MessageType                          |
// +---------------+---------------+---------------+---------------+
// |                        TargetNameFields                       |
// +---------------+---------------+---------------+---------------+
// |                              ...                              |
// +---------------+---------------+---------------+---------------+
// |                         NegotiateFlags                        |
// +---------------+---------------+---------------+---------------+
// |                         ServerChallenge                       |
// +---------------+---------------+---------------+---------------+
// |                              ...                              |
// +---------------+---------------+---------------+---------------+
// |                           Reserved                            |
// +---------------+---------------+---------------+---------------+
// |                              ...                              |
// +---------------+---------------+---------------+---------------+
// |                        TargetInfoFields                       |
// +---------------+---------------+---------------+---------------+
// |                              ...                              |
// +---------------+---------------+---------------+---------------+
// |                            Version                            |
// +---------------+---------------+---------------+---------------+
// |                              ...                              |
// +---------------+---------------+---------------+---------------+
// |                       Payload (Variable)                      |
// +---------------+---------------+---------------+---------------+
// |                              ...                              |
// +---------------+---------------+---------------+---------------+

// Signature (8 bytes):  An 8-byte character array that MUST contain the
//  ASCII string ('N', 'T', 'L', 'M', 'S', 'S', 'P', '\0').

// MessageType (4 bytes):  A 32-bit unsigned integer that indicates the message
//  type. This field MUST be set to 0x00000002.

// TargetNameFields (8 bytes):  If the NTLMSSP_REQUEST_TARGET flag is not set in
//  NegotiateFlags, indicating that no TargetName is required:
//  - TargetNameLen and TargetNameMaxLen SHOULD be set to zero on transmission.
//  - TargetNameBufferOffset field SHOULD be set to the offset from the beginning of the
//     CHALLENGE_MESSAGE to where the TargetName would be in Payload if it were present.
//  - TargetNameLen, TargetNameMaxLen, and TargetNameBufferOffset MUST be ignored
//     on receipt.
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |        TargetNameLen          |       TargetNameMaxLen        |
// +---------------+---------------+---------------+---------------+
// |                     TargetNameBufferOffset                    |
// +---------------+---------------+---------------+---------------+
//    TargetNameLen (2 bytes):  A 16-bit unsigned integer that defines the size,
//     in bytes, of TargetName in Payload.
//    TargetNameMaxLen (2 bytes):  A 16-bit unsigned integer that SHOULD be set
//     to the value of TargetNameLen and MUST be ignored on receipt.
//    TargetNameBufferOffset (4 bytes):  A 32-bit unsigned integer that defines
//     the offset, in bytes, from the beginning of the CHALLENGE_MESSAGE to
//     TargetName in Payload.
//     If TargetName is a Unicode string, the values of TargetNameBufferOffset and
//     TargetNameLen MUST be multiples of 2.

// NegotiateFlags (4 bytes):  A NEGOTIATE structure that contains a set of bit flags,
//  as defined in section 2.2.2.5. The server sets flags to indicate options it supports or,
//  if thre has been a NEGOTIATE_MESSAGE (section 2.2.1.1), the choices it has made from
//   the options offered by the client.

// ServerChallenge (8 bytes):  A 64-bit value that contains the NTLM challenge.
//  The challenge is a 64-bit nonce. The processing of the ServerChallenge is specified
//  in sections 3.1.5 and 3.2.5.
// Reserved (8 bytes):  An 8-byte array whose elements MUST be zero when sent and MUST be
//  ignored on receipt.

// TargetInfoFields (8 bytes):  If the NTLMSSP_NEGOTIATE_TARGET_INFO flag of
//  NegotiateFlags is clear, indicating that no TargetInfo is required:
//  - TargetInfoLen and TargetInfoMaxLen SHOULD be set to zero on transmission.
//  - TargetInfoBufferOffset field SHOULD be set to the offset from the beginning of the
//    CHALLENGE_MESSAGE to where the TargetInfo would be in Payload if it were present.
//  - TargetInfoLen, TargetInfoMaxLen, and TargetInfoBufferOffset MUST be ignored on receipt.
//  Otherwise, these fields are defined as:
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |        TargetInfoLen          |       TargetInfoMaxLen        |
// +---------------+---------------+---------------+---------------+
// |                     TargetInfoBufferOffset                    |
// +---------------+---------------+---------------+---------------+
//    TargetInfoLen (2 bytes):  A 16-bit unsigned integer that defines the size,
//     in bytes, of TargetInfo in Payload.
//    TargetInfoMaxLen (2 bytes):  A 16-bit unsigned integer that SHOULD be set
//     to the value of TargetInfoLen and MUST be ignored on receipt.
//    TargetInfoBufferOffset (4 bytes):  A 32-bit unsigned integer that defines
//     the offset, in bytes, from the beginning of the CHALLENGE_MESSAGE to
//     TargetInfo in Payload.

// Version (8 bytes):  A VERSION structure (as defined in section 2.2.2.10) that
//  is present only when the NTLMSSP_NEGOTIATE_VERSION flag is set in the
//  NegotiateFlags field. This structure is used for debugging purposes only.
//  In normal (non-debugging) protocol messages, it is ignored and does not affect
//  the NTLM message processing.


// Payload (variable):  A byte-array that contains the data referred to by the
//   TargetNameBufferOffset and TargetInfoBufferOffset message fields. Payload data
//   can be present in any order within the Payload field, with variable-length padding
//   before or after the data. The data that can be present in the Payload field of
//   this message, in no particular order, are:
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// | | | | | | | | | | |1| | | | | | | | | |2| | | | | | | | | |3| |
// |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                      TargetName (Variable)                    |
// +---------------+---------------+---------------+---------------+
// |                              ...                              |
// +---------------+---------------+---------------+---------------+
// |                      TargetInfo (Variable)                    |
// +---------------+---------------+---------------+---------------+
// |                              ...                              |
// +---------------+---------------+---------------+---------------+
//    TargetName (variable):  If TargetNameLen does not equal 0x0000, TargetName
//     MUST be a byte-array that contains the name of the server authentication realm,
//     and MUST be expressed in the negotiated character set. A server that is a
//     member of a domain returns the domain of which it is a member, and a server
//     that is not a member of a domain returns the server name.
//    TargetInfo (variable):  If TargetInfoLen does not equal 0x0000, TargetInfo
//     MUST be a byte array that contains a sequence of AV_PAIR structures.
//     The AV_PAIR structure is defined in section 2.2.2.1. The length of each
//     AV_PAIR is determined by its AvLen field (plus 4 bytes).

//     Note  An AV_PAIR structure can start on any byte alignment and the sequence of
//      AV_PAIRs has no padding between structures.
//     The sequence MUST be terminated by an AV_PAIR structure with an AvId field of
//      MsvAvEOL. The total length of the TargetInfo byte array is the sum of the lengths,
//      in bytes, of the AV_PAIR structures it contains.
//     Note  If a TargetInfo AV_PAIR Value is textual, it MUST be encoded in Unicode
//      irrespective of what character set was negotiated (section 2.2.2.1).


struct NTLMChallengeMessage : public NTLMMessage {

    NtlmField TargetName;          /* 8 Bytes */
    NtlmNegotiateFlags negoFlags;  /* 4 Bytes */
    uint8_t serverChallenge[8];    /* 8 Bytes */
    // uint64_t serverChallenge;
    /* 8 Bytes reserved */
    NtlmField TargetInfo;          /* 8 Bytes */
    NtlmVersion version;           /* 8 Bytes */
    uint32_t PayloadOffset;

    NtlmAvPairList AvPairList;


    NTLMChallengeMessage()
        : NTLMMessage(NtlmChallenge)
        , PayloadOffset(12+8+4+8+8+8+8)
    {
    }

    virtual ~NTLMChallengeMessage() {}

    void emit(Stream & stream) {
        this->TargetInfo.Buffer.reset();
        this->AvPairList.emit(this->TargetInfo.Buffer);

        uint32_t currentOffset = this->PayloadOffset;
        NTLMMessage::emit(stream);
        currentOffset += this->TargetName.emit(stream, currentOffset);
        this->negoFlags.emit(stream);
        stream.out_copy_bytes(this->serverChallenge, 8);
        stream.out_clear_bytes(8);
        currentOffset += this->TargetInfo.emit(stream, currentOffset);
        this->version.emit(stream);

        // PAYLOAD
        this->TargetName.write_payload(stream);
        this->TargetInfo.write_payload(stream);
        stream.mark_end();
    }

    void recv(Stream & stream) {
        uint8_t * pBegin = stream.p;
        bool res;
        res = NTLMMessage::recv(stream);
        if (!res) {
            LOG(LOG_ERR, "INVALID MSG RECEIVED type: %u", this->msgType);
        }
        this->TargetName.recv(stream);
        this->negoFlags.recv(stream);
        stream.in_copy_bytes(this->serverChallenge, 8);
        // this->serverChallenge = stream.in_uint64_le();
        stream.in_skip_bytes(8);
        this->TargetInfo.recv(stream);
        this->version.recv(stream);

        // PAYLOAD
        this->TargetName.read_payload(stream, pBegin);
        this->TargetInfo.read_payload(stream, pBegin);

        this->AvPairList.recv(this->TargetInfo.Buffer);

    }

    void avpair_decode() {
        this->TargetInfo.Buffer.reset();
        this->AvPairList.emit(this->TargetInfo.Buffer);
    }

};



#endif


//     SECURITY_STATUS ntlm_read_ChallengeMessage(PSecBuffer buffer)
//     {
// 	wStream* s;
// 	int length;
// 	PBYTE StartOffset;
// 	PBYTE PayloadOffset;
// 	NTLM_AV_PAIR* AvTimestamp;
// 	NTLM_CHALLENGE_MESSAGE* message;

// 	ntlm_generate_client_challenge(context);

// 	message = &context->CHALLENGE_MESSAGE;
// 	ZeroMemory(message, sizeof(NTLM_CHALLENGE_MESSAGE));

// 	s = Stream_New(buffer->pvBuffer, buffer->cbBuffer);

// 	StartOffset = Stream_Pointer(s);

// 	ntlm_read_message_header(s, (NTLM_MESSAGE_HEADER*) message);

// 	if (!ntlm_validate_message_header(s, (NTLM_MESSAGE_HEADER*) message, MESSAGE_TYPE_CHALLENGE))
//             {
// 		Stream_Free(s, FALSE);
// 		return SEC_E_INVALID_TOKEN;
//             }

// 	/* TargetNameFields (8 bytes) */
// 	ntlm_read_message_fields(s, &(message->TargetName));

// 	Stream_Read_UINT32(s, message->NegotiateFlags); /* NegotiateFlags (4 bytes) */
// 	context->NegotiateFlags = message->NegotiateFlags;

// 	Stream_Read(s, message->ServerChallenge, 8); /* ServerChallenge (8 bytes) */
// 	CopyMemory(context->ServerChallenge, message->ServerChallenge, 8);

// 	Stream_Read(s, message->Reserved, 8); /* Reserved (8 bytes), should be ignored */

// 	/* TargetInfoFields (8 bytes) */
// 	ntlm_read_message_fields(s, &(message->TargetInfo));

// 	if (context->NegotiateFlags & NTLMSSP_NEGOTIATE_VERSION)
//             ntlm_read_version_info(s, &(message->Version)); /* Version (8 bytes) */

// 	/* Payload (variable) */
// 	PayloadOffset = Stream_Pointer(s);

// 	if (message->TargetName.Len > 0)
//             ntlm_read_message_fields_buffer(s, &(message->TargetName));

// 	if (message->TargetInfo.Len > 0)
//             {
// 		ntlm_read_message_fields_buffer(s, &(message->TargetInfo));

// 		context->ChallengeTargetInfo.pvBuffer = message->TargetInfo.Buffer;
// 		context->ChallengeTargetInfo.cbBuffer = message->TargetInfo.Len;

// 		AvTimestamp = ntlm_av_pair_get((NTLM_AV_PAIR*) message->TargetInfo.Buffer, MsvAvTimestamp);

// 		if (AvTimestamp != NULL)
//                     {
// 			if (context->NTLMv2)
//                             context->UseMIC = TRUE;

// 			CopyMemory(context->ChallengeTimestamp, ntlm_av_pair_get_value_pointer(AvTimestamp), 8);
//                     }
//             }

// 	length = (PayloadOffset - StartOffset) + message->TargetName.Len + message->TargetInfo.Len;

// 	sspi_SecBufferAlloc(&context->ChallengeMessage, length);
// 	CopyMemory(context->ChallengeMessage.pvBuffer, StartOffset, length);

// #ifdef WITH_DEBUG_NTLM
// 	fprintf(stderr, "CHALLENGE_MESSAGE (length = %d)\n", length);
// 	winpr_HexDump(context->ChallengeMessage.pvBuffer, context->ChallengeMessage.cbBuffer);
// 	fprintf(stderr, "\n");

// 	ntlm_print_negotiate_flags(context->NegotiateFlags);

// 	if (context->NegotiateFlags & NTLMSSP_NEGOTIATE_VERSION)
//             ntlm_print_version_info(&(message->Version));

// 	ntlm_print_message_fields(&(message->TargetName), "TargetName");
// 	ntlm_print_message_fields(&(message->TargetInfo), "TargetInfo");

// 	if (context->ChallengeTargetInfo.cbBuffer > 0)
//             {
// 		fprintf(stderr, "ChallengeTargetInfo (%d):\n", (int) context->ChallengeTargetInfo.cbBuffer);
// 		ntlm_print_av_pair_list(context->ChallengeTargetInfo.pvBuffer);
//             }
// #endif
// 	/* AV_PAIRs */

// 	if (context->NTLMv2)
//             {
// 		ntlm_construct_authenticate_target_info(context);
// 		sspi_SecBufferFree(&context->ChallengeTargetInfo);
// 		context->ChallengeTargetInfo.pvBuffer = context->AuthenticateTargetInfo.pvBuffer;
// 		context->ChallengeTargetInfo.cbBuffer = context->AuthenticateTargetInfo.cbBuffer;
//             }

// 	/* Timestamp */
// 	ntlm_generate_timestamp(context);

// 	/* LmChallengeResponse */
// 	ntlm_compute_lm_v2_response(context);

// 	/* NtChallengeResponse */
// 	ntlm_compute_ntlm_v2_response(context);

// 	/* KeyExchangeKey */
// 	ntlm_generate_key_exchange_key(context);

// 	/* RandomSessionKey */
// 	ntlm_generate_random_session_key(context);

// 	/* ExportedSessionKey */
// 	ntlm_generate_exported_session_key(context);

// 	/* EncryptedRandomSessionKey */
// 	ntlm_encrypt_random_session_key(context);

// 	/* Generate signing keys */
// 	ntlm_generate_client_signing_key(context);
// 	ntlm_generate_server_signing_key(context);

// 	/* Generate sealing keys */
// 	ntlm_generate_client_sealing_key(context);
// 	ntlm_generate_server_sealing_key(context);

// 	/* Initialize RC4 seal state using client sealing key */
// 	ntlm_init_rc4_seal_states(context);

// #ifdef WITH_DEBUG_NTLM
// 	fprintf(stderr, "ClientChallenge\n");
// 	winpr_HexDump(context->ClientChallenge, 8);
// 	fprintf(stderr, "\n");

// 	fprintf(stderr, "ServerChallenge\n");
// 	winpr_HexDump(context->ServerChallenge, 8);
// 	fprintf(stderr, "\n");

// 	fprintf(stderr, "SessionBaseKey\n");
// 	winpr_HexDump(context->SessionBaseKey, 16);
// 	fprintf(stderr, "\n");

// 	fprintf(stderr, "KeyExchangeKey\n");
// 	winpr_HexDump(context->KeyExchangeKey, 16);
// 	fprintf(stderr, "\n");

// 	fprintf(stderr, "ExportedSessionKey\n");
// 	winpr_HexDump(context->ExportedSessionKey, 16);
// 	fprintf(stderr, "\n");

// 	fprintf(stderr, "RandomSessionKey\n");
// 	winpr_HexDump(context->RandomSessionKey, 16);
// 	fprintf(stderr, "\n");

// 	fprintf(stderr, "ClientSigningKey\n");
// 	winpr_HexDump(context->ClientSigningKey, 16);
// 	fprintf(stderr, "\n");

// 	fprintf(stderr, "ClientSealingKey\n");
// 	winpr_HexDump(context->ClientSealingKey, 16);
// 	fprintf(stderr, "\n");

// 	fprintf(stderr, "ServerSigningKey\n");
// 	winpr_HexDump(context->ServerSigningKey, 16);
// 	fprintf(stderr, "\n");

// 	fprintf(stderr, "ServerSealingKey\n");
// 	winpr_HexDump(context->ServerSealingKey, 16);
// 	fprintf(stderr, "\n");

// 	fprintf(stderr, "Timestamp\n");
// 	winpr_HexDump(context->Timestamp, 8);
// 	fprintf(stderr, "\n");
// #endif

// 	context->state = NTLM_STATE_AUTHENTICATE;

// 	ntlm_free_message_fields_buffer(&(message->TargetName));

// 	Stream_Free(s, FALSE);

// 	return SEC_I_CONTINUE_NEEDED;
//     }








//     SECURITY_STATUS ntlm_write_ChallengeMessage(PSecBuffer buffer)
//     {
// 	wStream* s;
// 	int length;
// 	UINT32 PayloadOffset;
// 	NTLM_CHALLENGE_MESSAGE* message;

// 	message = &context->CHALLENGE_MESSAGE;
// 	ZeroMemory(message, sizeof(NTLM_CHALLENGE_MESSAGE));

// 	s = Stream_New(buffer->pvBuffer, buffer->cbBuffer);

// 	/* Version */
// 	ntlm_get_version_info(&(message->Version));

// 	/* Server Challenge */
// 	ntlm_generate_server_challenge(context);

// 	/* Timestamp */
// 	ntlm_generate_timestamp(context);

// 	/* TargetInfo */
// 	ntlm_construct_challenge_target_info(context);

// 	/* ServerChallenge */
// 	CopyMemory(message->ServerChallenge, context->ServerChallenge, 8);

// 	message->NegotiateFlags = context->NegotiateFlags;

// 	ntlm_populate_message_header((NTLM_MESSAGE_HEADER*) message, MESSAGE_TYPE_CHALLENGE);

// 	/* Message Header (12 bytes) */
// 	ntlm_write_message_header(s, (NTLM_MESSAGE_HEADER*) message);

// 	if (message->NegotiateFlags & NTLMSSP_REQUEST_TARGET)
//             {
// 		message->TargetName.Len = (UINT16) context->TargetName.cbBuffer;
// 		message->TargetName.Buffer = context->TargetName.pvBuffer;
//             }

// 	message->NegotiateFlags |= NTLMSSP_NEGOTIATE_TARGET_INFO;

// 	if (message->NegotiateFlags & NTLMSSP_NEGOTIATE_TARGET_INFO)
//             {
// 		message->TargetInfo.Len = (UINT16) context->ChallengeTargetInfo.cbBuffer;
// 		message->TargetInfo.Buffer = context->ChallengeTargetInfo.pvBuffer;
//             }

// 	PayloadOffset = 48;

// 	if (message->NegotiateFlags & NTLMSSP_NEGOTIATE_VERSION)
//             PayloadOffset += 8;

// 	message->TargetName.BufferOffset = PayloadOffset;
// 	message->TargetInfo.BufferOffset = message->TargetName.BufferOffset + message->TargetName.Len;

// 	/* TargetNameFields (8 bytes) */
// 	ntlm_write_message_fields(s, &(message->TargetName));

// 	Stream_Write_UINT32(s, message->NegotiateFlags); /* NegotiateFlags (4 bytes) */

// 	Stream_Write(s, message->ServerChallenge, 8); /* ServerChallenge (8 bytes) */
// 	Stream_Write(s, message->Reserved, 8); /* Reserved (8 bytes), should be ignored */

// 	/* TargetInfoFields (8 bytes) */
// 	ntlm_write_message_fields(s, &(message->TargetInfo));

// 	if (message->NegotiateFlags & NTLMSSP_NEGOTIATE_VERSION)
//             ntlm_write_version_info(s, &(message->Version)); /* Version (8 bytes) */

// 	/* Payload (variable) */

// 	if (message->NegotiateFlags & NTLMSSP_REQUEST_TARGET)
//             ntlm_write_message_fields_buffer(s, &(message->TargetName));

// 	if (message->NegotiateFlags & NTLMSSP_NEGOTIATE_TARGET_INFO)
//             ntlm_write_message_fields_buffer(s, &(message->TargetInfo));

// 	length = Stream_GetPosition(s);
// 	buffer->cbBuffer = length;

// 	sspi_SecBufferAlloc(&context->ChallengeMessage, length);
// 	CopyMemory(context->ChallengeMessage.pvBuffer, Stream_Buffer(s), length);

// #ifdef WITH_DEBUG_NTLM
// 	fprintf(stderr, "CHALLENGE_MESSAGE (length = %d)\n", length);
// 	winpr_HexDump(context->ChallengeMessage.pvBuffer, context->ChallengeMessage.cbBuffer);
// 	fprintf(stderr, "\n");

// 	ntlm_print_negotiate_flags(message->NegotiateFlags);

// 	if (message->NegotiateFlags & NTLMSSP_NEGOTIATE_VERSION)
//             ntlm_print_version_info(&(message->Version));

// 	ntlm_print_message_fields(&(message->TargetName), "TargetName");
// 	ntlm_print_message_fields(&(message->TargetInfo), "TargetInfo");
// #endif

// 	context->state = NTLM_STATE_AUTHENTICATE;

// 	Stream_Free(s, FALSE);

// 	return SEC_I_CONTINUE_NEEDED;
//     }
