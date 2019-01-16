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
    Author(s): Christophe Grosjean, Raphael Zhou

    Unit test to RDP Orders coder/decoder
    Using lib boost functions for testing
*/

#define RED_TEST_MODULE TestOrderGlyphCache
#include "test_only/test_framework/redemption_unit_tests.hpp"


#include "utils/stream.hpp"
#include "core/RDP/orders/RDPOrdersSecondaryGlyphCache.hpp"

RED_AUTO_TEST_CASE(TestGlyphCache)
{
    uint8_t data[] = {
/* 0000 */ 0x1b, 0x01,  // orderLength = 283
/* 0002 */ 0x10, 0x00,  // extraFlags = CG_GLYPH_UNICODE_PRESENT
/* 0004 */ 0x03,        // orderType = TS_CACHE_GLYPH
/* 0005 */ 0x07,        // cacheId = 7
/* 0006 */ 0x0e,        // cGlyphs = 14
/* 0007 */ 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x07, 0x00, 0x08,  // ................
/* 0010 */ 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x01, 0x00, 0x00, 0x00, 0xfa, 0xff, 0x06,  // .|......|.......
/* 0020 */ 0x00, 0x06, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x7c, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xfa,  // ........|.......
/* 0030 */ 0xff, 0x06, 0x00, 0x06, 0x00, 0xcc, 0xcc, 0x78, 0x78, 0x30, 0x30, 0x00, 0x00, 0x03, 0x00, 0x00,  // .......xx00.....
/* 0040 */ 0x00, 0xfa, 0xff, 0x06, 0x00, 0x06, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0xc0, 0x7c, 0x00, 0x00, 0x04,  // .......x....|...
/* 0050 */ 0x00, 0x00, 0x00, 0xfa, 0xff, 0x04, 0x00, 0x06, 0x00, 0xd0, 0xf0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00,  // ................
/* 0060 */ 0x00, 0x05, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x04, 0x00, 0x08, 0x00, 0xc0, 0xc0, 0xf0, 0xc0, 0xc0,  // ................
/* 0070 */ 0xc0, 0xc0, 0x70, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,  // ..p.............
/* 0080 */ 0x00, 0x07, 0x00, 0x00, 0x00, 0xf7, 0xff, 0x06, 0x00, 0x09, 0x00, 0x0c, 0x0c, 0x0c, 0x7c, 0xcc,  // ..............|.
/* 0090 */ 0xcc, 0xcc, 0xcc, 0x7c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xfa, 0xff, 0x05, 0x00, 0x06,  // ...|............
/* 00a0 */ 0x00, 0x78, 0xc0, 0xf0, 0x78, 0x18, 0xf0, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x02,  // .x..x...........
/* 00b0 */ 0x00, 0x08, 0x00, 0xc0, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x0a, 0x00, 0x00, 0x00, 0xfa,  // ................
/* 00c0 */ 0xff, 0x06, 0x00, 0x06, 0x00, 0x78, 0xcc, 0xcc, 0xcc, 0xcc, 0x78, 0x00, 0x00, 0x0b, 0x00, 0x00,  // .....x....x.....
/* 00d0 */ 0x00, 0xfa, 0xff, 0x06, 0x00, 0x06, 0x00, 0xf8, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x0c,  // ................
/* 00e0 */ 0x00, 0xff, 0xff, 0xf8, 0xff, 0x0c, 0x00, 0x08, 0x00, 0x66, 0x60, 0x66, 0x60, 0x66, 0x60, 0x36,  // .........f`f`f`6
/* 00f0 */ 0xc0, 0x3f, 0xc0, 0x39, 0xc0, 0x19, 0x80, 0x19, 0x80, 0x0d, 0x00, 0x00, 0x00, 0xfa, 0xff, 0x08,  // .?.9............
/* 0100 */ 0x00, 0x06, 0x00, 0xdb, 0xdb, 0xdb, 0xff, 0x66, 0x66, 0x00, 0x00,                 // .......ff..
    };

    InStream in_stream(data);

    RDPSecondaryOrderHeader header(in_stream);

    RDPGlyphCache cmd;
    cmd.receive(in_stream, header);

    RED_CHECK_EQUAL(283, header.order_length);
    RED_CHECK_EQUAL(CG_GLYPH_UNICODE_PRESENT, header.flags);
    RED_CHECK_EQUAL(RDP::TS_CACHE_GLYPH, header.type);

    RED_CHECK_EQUAL(7, cmd.cacheId);
    RED_CHECK_EQUAL(14, cmd.cGlyphs);
}
