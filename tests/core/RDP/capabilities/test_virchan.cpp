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
   Copyright (C) Wallix 2010
   Author(s): Christophe Grosjean

   Unit test to RDP VirtualChannel object
   Using lib boost functions for testing
*/
#define RED_TEST_MODULE CapabilityVirtualChannelEmit

#include "system/redemption_unit_tests.hpp"


#include "core/RDP/capabilities/virchan.hpp"

RED_AUTO_TEST_CASE(TestCapabilityVirtualChannelEmit)
{
    VirtualChannelCaps virtualchannel_caps;
    virtualchannel_caps.flags = VCCAPS_COMPR_CS_8K;
    virtualchannel_caps.VCChunkSize = 56897;

    RED_CHECK_EQUAL(virtualchannel_caps.capabilityType, static_cast<uint16_t>(CAPSTYPE_VIRTUALCHANNEL));
    RED_CHECK_EQUAL(virtualchannel_caps.len, static_cast<uint16_t>(CAPLEN_VIRTUALCHANNEL));
    RED_CHECK_EQUAL(virtualchannel_caps.flags, static_cast<uint16_t>(2));
    RED_CHECK_EQUAL(virtualchannel_caps.VCChunkSize, static_cast<uint16_t>(56897));

    StaticOutStream<1024> out_stream;
    virtualchannel_caps.emit(out_stream);

    InStream stream(out_stream.get_data(), out_stream.get_offset());

    VirtualChannelCaps virtualchannel_caps2;

    RED_CHECK_EQUAL(virtualchannel_caps2.capabilityType, static_cast<uint16_t>(CAPSTYPE_VIRTUALCHANNEL));
    RED_CHECK_EQUAL(virtualchannel_caps2.len, static_cast<uint16_t>(CAPLEN_VIRTUALCHANNEL));

    RED_CHECK_EQUAL(static_cast<uint16_t>(CAPSTYPE_VIRTUALCHANNEL), stream.in_uint16_le());
    RED_CHECK_EQUAL(static_cast<uint16_t>(CAPLEN_VIRTUALCHANNEL), stream.in_uint16_le());
    virtualchannel_caps2.recv(stream, CAPLEN_VIRTUALCHANNEL);

    RED_CHECK_EQUAL(virtualchannel_caps2.flags, static_cast<uint16_t>(2));
    RED_CHECK_EQUAL(virtualchannel_caps2.VCChunkSize, static_cast<uint16_t>(56897));
}
