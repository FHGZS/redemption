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

   Product name : redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2012
   Author(s) : Christophe Grosjean

   RDP Licence Layer Unit Tests

*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestLic
#include <boost/test/auto_unit_test.hpp>

#define LOGPRINT
#include "log.hpp"

#include "stream.hpp"
#include "RDP/lic.hpp"

BOOST_AUTO_TEST_CASE(Test_lic_new_licence)
{
    uint8_t indata[] = {
        // non encrypted headers
           0x03, 0x03, 0x07, 0x08, 0x09, 0x00, 0xef, 0x07,
        // here begins encrypted data
/* 0000 */ 0xde, 0x18, 0x8f, 0x41, 0xad, 0x37, 0x5d, 0x3a, 0xe7, 0x6f, 0x7d, 0x15, 0x38, 0x1d, 0xfd, 0x0b,  // ...A.7]:.o}.8...
/* 0010 */ 0xb3, 0x34, 0x1c, 0xcc, 0x9d, 0xcc, 0xc7, 0x97, 0x45, 0x2b, 0x00, 0x32, 0x4e, 0x96, 0x3c, 0x9e,  // .4......E+.2N.<.
/* 0020 */ 0xda, 0xcc, 0x1a, 0x07, 0x1e, 0x9f, 0xd6, 0xc5, 0x27, 0x7a, 0xd1, 0xcb, 0x10, 0xfc, 0x61, 0x19,  // ........'z....a.
/* 0030 */ 0xe4, 0x34, 0xea, 0x2d, 0xcb, 0x62, 0xf1, 0xc0, 0x39, 0x4f, 0xea, 0x05, 0xd4, 0x71, 0x61, 0x50,  // .4.-.b..9O...qaP
/* 0040 */ 0xfd, 0xa0, 0xad, 0x58, 0x4a, 0x48, 0xec, 0xa3, 0x0a, 0x39, 0x3a, 0x8a, 0xfd, 0x1d, 0x32, 0x9e,  // ...XJH...9:...2.
/* 0050 */ 0xc8, 0x7d, 0xf1, 0x16, 0xf2, 0x2c, 0xc4, 0x1e, 0xa4, 0x70, 0x95, 0x5d, 0x93, 0xc3, 0x5d, 0xb3,  // .}...,...p.]..].
/* 0060 */ 0x55, 0x15, 0x7b, 0x81, 0x40, 0xcc, 0x48, 0x7a, 0xd8, 0xb2, 0xbc, 0xbd, 0x82, 0x46, 0x13, 0x66,  // U.{.@.Hz.....F.f
/* 0070 */ 0x23, 0x57, 0x88, 0xd3, 0x66, 0xa7, 0x66, 0x3b, 0x04, 0xcf, 0xf1, 0xb6, 0xc4, 0x32, 0xee, 0x41,  // #W..f.f;.....2.A
/* 0080 */ 0xda, 0x78, 0x62, 0xeb, 0xb1, 0x97, 0xad, 0xeb, 0x49, 0xac, 0x46, 0xb0, 0xf6, 0x3d, 0xc5, 0xe7,  // .xb.....I.F..=..
/* 0090 */ 0x2e, 0x51, 0x14, 0xfb, 0x76, 0xc9, 0xef, 0x52, 0xe5, 0x6d, 0x3e, 0x36, 0x05, 0xdb, 0x97, 0xc7,  // .Q..v..R.m>6....
/* 00a0 */ 0xd9, 0x1e, 0xf4, 0x08, 0x08, 0x6c, 0x1f, 0xbb, 0x29, 0xe8, 0x24, 0x36, 0x85, 0x43, 0x32, 0x91,  // .....l..).$6.C2.
/* 00b0 */ 0x0a, 0x27, 0xd8, 0xae, 0x5d, 0x35, 0x73, 0x0d, 0x6a, 0xcb, 0x59, 0xa0, 0x7e, 0x9b, 0x5b, 0x8d,  // .'..]5s.j.Y.~.[.
/* 00c0 */ 0x00, 0x60, 0xa1, 0xa2, 0xb9, 0xf8, 0x49, 0xd2, 0x9f, 0x5c, 0x09, 0x98, 0x3e, 0x1b, 0xa4, 0x34,  // .`....I.....>..4
/* 00d0 */ 0xd7, 0xd3, 0x29, 0x38, 0x95, 0x85, 0x86, 0x2e, 0xc7, 0x09, 0xd5, 0x34, 0x47, 0x1c, 0xd7, 0x70,  // ..)8.......4G..p
/* 00e0 */ 0xde, 0x3b, 0xb9, 0x07, 0x11, 0xaa, 0x99, 0xfd, 0x0a, 0xd9, 0x42, 0x94, 0x39, 0x76, 0xb9, 0xce,  // .;........B.9v..
/* 00f0 */ 0x50, 0xf3, 0xd5, 0x61, 0x47, 0x9a, 0x55, 0x75, 0x1d, 0x32, 0x75, 0x0e, 0xa7, 0xe1, 0x8e, 0x64,  // P..aG.Uu.2u....d
/* 0100 */ 0x38, 0x6a, 0xac, 0x3c, 0xcd, 0x59, 0xe2, 0x14, 0x27, 0xa8, 0x11, 0x10, 0xc4, 0x16, 0xca, 0x0b,  // 8j.<.Y..'.......
/* 0110 */ 0xd2, 0x8a, 0x7f, 0x59, 0x8e, 0x64, 0x37, 0x2d, 0xdf, 0xac, 0x17, 0xc7, 0x76, 0xd2, 0x76, 0x42,  // ...Y.d7-....v.vB
/* 0120 */ 0x33, 0xeb, 0xf5, 0x9e, 0x00, 0xce, 0x02, 0x30, 0x0e, 0x15, 0xbf, 0x58, 0x32, 0x11, 0x38, 0x2e,  // 3......0...X2.8.
/* 0130 */ 0x04, 0x16, 0x7a, 0x8b, 0xb2, 0xe0, 0x93, 0x89, 0x7e, 0x8e, 0xdc, 0x74, 0x06, 0xfe, 0xa7, 0x77,  // ..z.....~..t...w
/* 0140 */ 0x11, 0x48, 0x90, 0xae, 0x67, 0xa5, 0x9d, 0xc1, 0x99, 0x32, 0x3d, 0x08, 0x6e, 0xb5, 0xfa, 0x94,  // .H..g....2=.n...
/* 0150 */ 0xd5, 0x9b, 0x85, 0x2e, 0xed, 0x1e, 0x8e, 0x8e, 0xf7, 0x6b, 0x0d, 0x1b, 0xb6, 0xe4, 0xb7, 0xdf,  // .........k......
/* 0160 */ 0x46, 0xde, 0x93, 0xf7, 0xce, 0x1e, 0x91, 0xce, 0x96, 0x27, 0x5a, 0x55, 0x93, 0xbb, 0xf2, 0xc1,  // F........'ZU....
/* 0170 */ 0xaf, 0xe3, 0x90, 0x81, 0x08, 0x5e, 0xc1, 0xe9, 0x12, 0x23, 0xee, 0x56, 0xa3, 0x55, 0x31, 0x00,  // .....^...#.V.U1.
/* 0180 */ 0xe1, 0x7c, 0x54, 0xa5, 0x65, 0xbe, 0xdb, 0xe0, 0x45, 0xc3, 0x28, 0x48, 0xd7, 0xcb, 0x88, 0xae,  // .|T.e...E.(H....
/* 0190 */ 0x3d, 0x91, 0x3e, 0xc8, 0x6c, 0xf0, 0x4a, 0xec, 0x5a, 0x3e, 0x6f, 0xac, 0xf6, 0xcb, 0xde, 0xa7,  // =.>.l.J.Z>o.....
/* 01a0 */ 0x03, 0x7e, 0x5b, 0x8d, 0xab, 0x94, 0x75, 0x12, 0xfa, 0x87, 0x5e, 0xc2, 0xd2, 0x13, 0xad, 0x5e,  // .~[...u...^....^
/* 01b0 */ 0xcb, 0xa8, 0x1a, 0x71, 0xb5, 0xb5, 0x62, 0xcf, 0xf7, 0x6a, 0x10, 0x81, 0xd1, 0xba, 0x57, 0x8b,  // ...q..b..j....W.
/* 01c0 */ 0xe1, 0xa5, 0xd1, 0x99, 0xfb, 0x5c, 0xd4, 0x2f, 0x13, 0x63, 0x92, 0x2e, 0x83, 0x49, 0x12, 0xec,  // ......./.c...I..
/* 01d0 */ 0xf7, 0x1d, 0x9f, 0x19, 0xd6, 0x45, 0x6b, 0x6b, 0x4c, 0x0a, 0xa5, 0xe7, 0xff, 0xe5, 0x93, 0x63,  // .....EkkL......c
/* 01e0 */ 0x63, 0x33, 0x14, 0x48, 0x7a, 0x0d, 0x8a, 0xe6, 0x5c, 0xab, 0xa6, 0x54, 0xbf, 0x94, 0x2b, 0xdc,  // c3.Hz......T..+.
/* 01f0 */ 0x1c, 0x95, 0x32, 0xf2, 0x07, 0xfe, 0x35, 0x77, 0xbf, 0x41, 0x27, 0x18, 0x5b, 0x76, 0x0d, 0xe9,  // ..2...5w.A'.[v..
/* 0200 */ 0x4e, 0x69, 0x93, 0x11, 0xca, 0x8f, 0x53, 0x7d, 0xaa, 0xf1, 0x48, 0x34, 0x81, 0xff, 0xae, 0x43,  // Ni....S}..H4...C
/* 0210 */ 0xca, 0x60, 0x4e, 0x01, 0x95, 0xe3, 0xf1, 0xe5, 0xfe, 0xc1, 0xf9, 0xb7, 0x24, 0x38, 0x5b, 0xcd,  // .`N.........$8[.
/* 0220 */ 0xbb, 0xfb, 0xb5, 0xef, 0x90, 0xff, 0x63, 0x8d, 0x16, 0xca, 0xec, 0xb2, 0x55, 0x5e, 0xa0, 0x3e,  // ......c.....U^.>
/* 0230 */ 0x41, 0xa5, 0x61, 0x89, 0xd4, 0xb1, 0x57, 0x2c, 0x00, 0xc6, 0x18, 0x44, 0x68, 0x2d, 0xfb, 0x54,  // A.a...W,...Dh-.T
/* 0240 */ 0x68, 0xaa, 0x50, 0x51, 0x5f, 0xf0, 0x40, 0x64, 0x36, 0xc7, 0xa9, 0x99, 0x91, 0x95, 0x31, 0x39,  // h.PQ_.@d6.....19
/* 0250 */ 0x0d, 0x25, 0xa1, 0x23, 0xf9, 0xad, 0xa2, 0x5d, 0xcf, 0x34, 0x29, 0xdb, 0x97, 0xfc, 0xd7, 0x3a,  // .%.#...].4)....:
/* 0260 */ 0xb7, 0x16, 0xdf, 0x41, 0x19, 0x15, 0x17, 0x3d, 0x67, 0x09, 0x62, 0x1c, 0x82, 0xac, 0x44, 0x80,  // ...A...=g.b...D.
/* 0270 */ 0x37, 0xd1, 0x39, 0x81, 0x4e, 0x56, 0xff, 0x66, 0x14, 0xa5, 0xb9, 0x53, 0x60, 0xbe, 0x9c, 0x1b,  // 7.9.NV.f...S`...
/* 0280 */ 0x01, 0xb7, 0xd8, 0x3e, 0x41, 0xff, 0xd1, 0x8c, 0x9f, 0x3f, 0x5a, 0xb2, 0x05, 0x95, 0x2e, 0x45,  // ...>A....?Z....E
/* 0290 */ 0x72, 0xc6, 0xa9, 0x9b, 0xce, 0x31, 0x84, 0xc0, 0xd9, 0xe5, 0x96, 0x22, 0xd3, 0x34, 0x28, 0x2a,  // r....1.....".4(*
/* 02a0 */ 0x06, 0x85, 0x1f, 0xb2, 0x5b, 0xf9, 0x48, 0x64, 0x0c, 0x73, 0x71, 0xb0, 0xb7, 0xcf, 0x79, 0x75,  // ....[.Hd.sq...yu
/* 02b0 */ 0x4b, 0x89, 0x09, 0x7d, 0xea, 0x76, 0xa7, 0x72, 0x0b, 0x0c, 0xdb, 0x56, 0xa6, 0xe8, 0x11, 0x4c,  // K..}.v.r...V...L
/* 02c0 */ 0x3e, 0xa3, 0xde, 0xc9, 0x0a, 0x87, 0x37, 0x24, 0x2b, 0x5a, 0x26, 0x95, 0xd3, 0x72, 0xe4, 0x43,  // >.....7$+Z&..r.C
/* 02d0 */ 0x20, 0x1d, 0xc0, 0x9b, 0x85, 0x69, 0xf4, 0x89, 0xb6, 0x4b, 0xeb, 0x67, 0x3d, 0x9d, 0x21, 0x3e,  //  ....i...K.g=.!>
/* 02e0 */ 0x36, 0xfa, 0x0a, 0xfb, 0xc9, 0xe7, 0xb3, 0x36, 0xde, 0x66, 0xbc, 0xf7, 0x01, 0x23, 0x7b, 0x3f,  // 6......6.f...#{?
/* 02f0 */ 0xa4, 0x8c, 0x68, 0xa9, 0x70, 0x1d, 0x6c, 0xd2, 0xbb, 0x53, 0x97, 0x1e, 0xb0, 0xaa, 0x9c, 0x2d,  // ..h.p.l..S.....-
/* 0300 */ 0x3a, 0xb7, 0x18, 0xfe, 0x01, 0xc5, 0xcf, 0x67, 0xba, 0x6d, 0xe1, 0x06, 0x8d, 0x9d, 0x8f, 0x46,  // :......g.m.....F
/* 0310 */ 0x8c, 0x17, 0x53, 0x5b, 0x88, 0x81, 0x48, 0x05, 0x18, 0x48, 0x5e, 0x8a, 0xf5, 0x82, 0x93, 0x47,  // ..S[..H..H^....G
/* 0320 */ 0x5d, 0x1c, 0xba, 0x83, 0x79, 0x68, 0x07, 0xc2, 0x2f, 0x05, 0xa7, 0x81, 0x52, 0xc1, 0x40, 0x73,  // ]...yh../...R.@s
/* 0330 */ 0xa5, 0x8d, 0x1e, 0x25, 0x74, 0x78, 0x09, 0x09, 0xf9, 0x4a, 0x24, 0x39, 0x31, 0x2f, 0x48, 0xb4,  // ...%tx...J$91/H.
/* 0340 */ 0x5d, 0x9b, 0x52, 0x76, 0xdc, 0x6a, 0xb7, 0x3b, 0x58, 0x0b, 0x1d, 0xfe, 0xe3, 0xf6, 0xaf, 0x58,  // ].Rv.j.;X......X
/* 0350 */ 0x83, 0x57, 0x4b, 0xb3, 0x55, 0x31, 0xbb, 0x7e, 0x1c, 0x49, 0x52, 0x35, 0xd1, 0xed, 0xbb, 0x49,  // .WK.U1.~.IR5...I
/* 0360 */ 0x05, 0x9e, 0xa8, 0xd6, 0x4c, 0x70, 0x7b, 0x8c, 0x05, 0x95, 0xec, 0xf4, 0xce, 0x06, 0x55, 0x78,  // ....Lp{.......Ux
/* 0370 */ 0xfa, 0x19, 0x07, 0xd9, 0x93, 0x0b, 0xf7, 0x38, 0xc9, 0x21, 0x16, 0x54, 0x5a, 0x54, 0x4d, 0xa4,  // .......8.!.TZTM.
/* 0380 */ 0xba, 0xd6, 0x3e, 0xb9, 0xe0, 0x05, 0x9d, 0x95, 0x43, 0x5b, 0x3b, 0xb2, 0x7b, 0xc3, 0xa8, 0xdf,  // ..>.....C[;.{...
/* 0390 */ 0x9d, 0x09, 0xdb, 0xd1, 0xee, 0x79, 0xd7, 0x56, 0x52, 0x49, 0xa5, 0x08, 0xf6, 0x8e, 0x8a, 0x4a,  // .....y.VRI.....J
/* 03a0 */ 0xe1, 0x77, 0xa2, 0x98, 0x04, 0x1a, 0xb0, 0xa4, 0x3e, 0xb5, 0x44, 0xdc, 0xba, 0xbf, 0xcd, 0x0c,  // .w......>.D.....
/* 03b0 */ 0x62, 0x75, 0x67, 0x49, 0x05, 0x0d, 0xc8, 0x73, 0x53, 0xd7, 0x8d, 0x7d, 0x35, 0x0d, 0xea, 0x8e,  // bugI...sS..}5...
/* 03c0 */ 0x09, 0x81, 0x41, 0x66, 0xd6, 0x5f, 0x45, 0x06, 0x45, 0x07, 0xeb, 0xe7, 0x51, 0x09, 0x0e, 0x2f,  // ..Af._E.E...Q../
/* 03d0 */ 0x55, 0x7c, 0x02, 0x88, 0xd3, 0x8c, 0x55, 0x1a, 0x27, 0x73, 0x5b, 0xb2, 0x06, 0x54, 0x3f, 0x81,  // U|....U.'s[..T?.
/* 03e0 */ 0xf5, 0x1f, 0x3b, 0x66, 0x3f, 0x88, 0xd4, 0x98, 0x8a, 0x55, 0x9e, 0x4b, 0xff, 0xe8, 0x22, 0xc0,  // ..;f?....U.K..".
/* 03f0 */ 0x35, 0xf4, 0x6c, 0x7b, 0x5f, 0x47, 0xb2, 0xbc, 0x97, 0x8c, 0x89, 0xbc, 0xbd, 0xf9, 0xfc, 0x65,  // 5.l{_G.........e
/* 0400 */ 0x9a, 0xc6, 0x98, 0xbf, 0xdd, 0x04, 0x8f, 0x50, 0x63, 0xaa, 0x16, 0x84, 0xff, 0x03, 0x8b, 0xd6,  // .......Pc.......
/* 0410 */ 0xb8, 0x72, 0x46, 0x18, 0x04, 0x2b, 0xe4, 0xcc, 0x3d, 0x2c, 0x18, 0xc6, 0x9c, 0x35, 0x84, 0x7d,  // .rF..+..=,...5.}
/* 0420 */ 0x8f, 0xa6, 0x43, 0x48, 0x88, 0x9f, 0xe7, 0xf7, 0x3b, 0x1f, 0x4a, 0xf5, 0xe6, 0x84, 0x95, 0x5e,  // ..CH....;.J....^
/* 0430 */ 0x40, 0x5b, 0xf1, 0x49, 0x00, 0x46, 0x52, 0xd3, 0x18, 0x8a, 0x01, 0xfd, 0x98, 0xd5, 0xaf, 0x8c,  // @[.I.FR.........
/* 0440 */ 0x29, 0xb8, 0x59, 0xd6, 0x92, 0x40, 0xe7, 0x52, 0xeb, 0x5c, 0x8e, 0xda, 0x45, 0xea, 0x18, 0x74,  // ).Y..@.R....E..t
/* 0450 */ 0xa0, 0xed, 0xb5, 0xab, 0x96, 0xa0, 0x77, 0x5e, 0x86, 0x6e, 0x2d, 0xe4, 0xab, 0xb7, 0xd5, 0xa1,  // ......w^.n-.....
/* 0460 */ 0x6a, 0x73, 0x92, 0x81, 0xf6, 0x1c, 0x24, 0x22, 0xe8, 0x58, 0x9d, 0xf4, 0xc8, 0xe6, 0x3c, 0x18,  // js....$".X....<.
/* 0470 */ 0x85, 0x9f, 0x60, 0xe7, 0x78, 0x5a, 0x03, 0xed, 0xf7, 0x23, 0x20, 0xd9, 0x41, 0x6a, 0x32, 0x0c,  // ..`.xZ...# .Aj2.
/* 0480 */ 0xc6, 0x5d, 0x95, 0x18, 0x68, 0x79, 0xb6, 0xfd, 0x94, 0x7f, 0x98, 0xd2, 0x40, 0xa4, 0x9a, 0xc2,  // .]..hy......@...
/* 0490 */ 0x25, 0x2c, 0x9e, 0xa5, 0xac, 0xb1, 0xa5, 0x5e, 0x8c, 0x62, 0xd7, 0x15, 0x53, 0xe1, 0xf3, 0xc7,  // %,.....^.b..S...
/* 04a0 */ 0x45, 0x6a, 0xa1, 0x81, 0x71, 0x75, 0x66, 0x17, 0x8f, 0x2a, 0x1e, 0xcf, 0xcd, 0xd4, 0xdd, 0x0f,  // Ej..quf..*......
/* 04b0 */ 0x87, 0x66, 0x2d, 0xad, 0x18, 0x5b, 0x8e, 0x41, 0xf4, 0x25, 0xc0, 0x18, 0x22, 0x58, 0x99, 0xfb,  // .f-..[.A.%.."X..
/* 04c0 */ 0x07, 0x4a, 0x73, 0x93, 0xea, 0x92, 0x13, 0xfd, 0x79, 0xff, 0x4d, 0xaf, 0x4f, 0xa4, 0xc0, 0xc6,  // .Js.....y.M.O...
/* 04d0 */ 0xea, 0x7b, 0x6b, 0xb8, 0xa7, 0xfe, 0x7d, 0x6f, 0xea, 0x9c, 0x80, 0x42, 0x5c, 0x6d, 0x6e, 0x1d,  // .{k...}o...B.mn.
/* 04e0 */ 0xe8, 0xe3, 0x04, 0x02, 0x03, 0xdc, 0x54, 0x65, 0x06, 0xee, 0xc1, 0xc8, 0xf8, 0xf4, 0x37, 0x6c,  // ......Te......7l
/* 04f0 */ 0x8b, 0x35, 0x82, 0x20, 0x0b, 0x56, 0x44, 0xe6, 0x15, 0x8b, 0xd4, 0x9b, 0x35, 0xfe, 0x1f, 0x0e,  // .5. .VD.....5...
/* 0500 */ 0x1d, 0xf2, 0xa9, 0xf5, 0xc3, 0x93, 0x60, 0xe6, 0x21, 0x0d, 0x59, 0x10, 0x0f, 0xea, 0xeb, 0x5e,  // ......`.!.Y....^
/* 0510 */ 0x38, 0x2d, 0xb9, 0x8a, 0xf5, 0x25, 0xd3, 0x91, 0x11, 0xd6, 0x3a, 0x33, 0x2c, 0xf8, 0x15, 0xe3,  // 8-...%....:3,...
/* 0520 */ 0x4b, 0xeb, 0x1c, 0x89, 0x4c, 0x16, 0x29, 0x72, 0x02, 0x85, 0x38, 0x19, 0xe3, 0x6c, 0x26, 0x2f,  // K...L.)r..8..l&/
/* 0530 */ 0x6a, 0xb4, 0xa4, 0x0b, 0xa8, 0xd3, 0xd0, 0x22, 0x91, 0x62, 0x63, 0x40, 0xda, 0x26, 0x0a, 0x33,  // j......".bc@.&.3
/* 0540 */ 0x8e, 0xa2, 0xdc, 0x34, 0x90, 0x05, 0x78, 0xec, 0xcd, 0x0e, 0xd7, 0x7a, 0x60, 0x54, 0xe1, 0xc7,  // ...4..x....z`T..
/* 0550 */ 0x51, 0xe2, 0x6e, 0xae, 0x90, 0xae, 0xed, 0x00, 0xa4, 0xe0, 0xa6, 0xb2, 0x1d, 0x14, 0x45, 0x27,  // Q.n...........E'
/* 0560 */ 0xb6, 0xb2, 0x14, 0x36, 0xf0, 0x60, 0x87, 0x9e, 0x2e, 0x0f, 0xbf, 0x42, 0x8e, 0xc7, 0x3d, 0xc7,  // ...6.`.....B..=.
/* 0570 */ 0xaf, 0xd1, 0xe8, 0x19, 0xaf, 0xe7, 0x47, 0x5c, 0xc2, 0xcc, 0xf5, 0x2a, 0xd1, 0x9c, 0x74, 0x47,  // ......G....*..tG
/* 0580 */ 0xae, 0x2c, 0xcf, 0xca, 0xa2, 0xed, 0x38, 0x26, 0x83, 0x70, 0xd6, 0xdd, 0x15, 0x3e, 0xb1, 0x71,  // .,....8&.p...>.q
/* 0590 */ 0x92, 0x1b, 0xe5, 0x5b, 0xc5, 0xae, 0x0a, 0xa4, 0x4f, 0x3c, 0x81, 0x1b, 0x00, 0x36, 0x02, 0x0c,  // ...[....O<...6..
/* 05a0 */ 0xc4, 0xce, 0x45, 0x3f, 0x0a, 0x15, 0xdf, 0x72, 0x06, 0x02, 0xde, 0x69, 0x8c, 0xe0, 0x02, 0xf0,  // ..E?...r...i....
/* 05b0 */ 0xa4, 0x06, 0x21, 0x43, 0xf8, 0xf7, 0xb4, 0x79, 0x09, 0x7b, 0x43, 0x46, 0x1a, 0xe4, 0x71, 0x3d,  // ..!C...y.{CF..q=
/* 05c0 */ 0xc3, 0xe7, 0xc2, 0xd4, 0x36, 0x1e, 0x61, 0xcd, 0x19, 0x1b, 0x7e, 0x20, 0x8a, 0x92, 0xec, 0x58,  // ....6.a...~ ...X
/* 05d0 */ 0xaa, 0xb2, 0xdb, 0xb5, 0x71, 0x8f, 0x65, 0x52, 0x09, 0x57, 0x98, 0x50, 0x2a, 0x03, 0xc5, 0xbe,  // ....q.eR.W.P*...
/* 05e0 */ 0xe4, 0xd1, 0x58, 0x40, 0xb8, 0xba, 0xb0, 0x0f, 0xd8, 0xbb, 0x6f, 0xc6, 0xdf, 0x9a, 0x4f, 0x60,  // ..X@......o...O`
/* 05f0 */ 0xbe, 0x28, 0x28, 0x7e, 0xb4, 0x36, 0xbc, 0xbb, 0xed, 0x20, 0x6c, 0x7d, 0x8f, 0xcd, 0xd5, 0xf5,  // .((~.6... l}....
/* 0600 */ 0x1e, 0x5e, 0x1e, 0x6d, 0x0a, 0xba, 0x59, 0x89, 0x75, 0x42, 0x59, 0x4c, 0xce, 0x7a, 0x4c, 0xe4,  // .^.m..Y.uBYL.zL.
/* 0610 */ 0x21, 0x7e, 0x32, 0x06, 0x8a, 0x5e, 0x9d, 0x66, 0x3c, 0x7a, 0x25, 0x67, 0xe2, 0xf9, 0x28, 0xa8,  // !~2..^.f<z%g..(.
/* 0620 */ 0x71, 0xad, 0x08, 0xb3, 0xe4, 0xff, 0xde, 0xde, 0x8d, 0x35, 0x6b, 0xf4, 0x67, 0xfd, 0x53, 0x65,  // q........5k.g.Se
/* 0630 */ 0xe2, 0x36, 0xab, 0x38, 0xa8, 0x51, 0x37, 0x9e, 0xb5, 0xf7, 0x0a, 0xde, 0x17, 0x22, 0xcb, 0x03,  // .6.8.Q7......"..
/* 0640 */ 0x15, 0xf2, 0x9d, 0x2a, 0x6d, 0x42, 0x5d, 0x54, 0x95, 0x5b, 0x92, 0x91, 0xd6, 0xdb, 0x66, 0xaa,  // ...*mB]T.[....f.
/* 0650 */ 0x78, 0x6d, 0x61, 0x00, 0x9e, 0x8a, 0xb7, 0xa7, 0x23, 0x7f, 0xd9, 0xb0, 0xa5, 0x7d, 0x03, 0x83,  // xma.....#....}..
/* 0660 */ 0xaf, 0x26, 0x8b, 0xbb, 0x3d, 0x40, 0xbe, 0x14, 0x08, 0xdc, 0x48, 0xc2, 0xdc, 0x03, 0x20, 0xe0,  // .&..=@....H... .
/* 0670 */ 0x95, 0xc0, 0xad, 0x67, 0x79, 0xed, 0x09, 0xc6, 0xc3, 0xa2, 0x7d, 0x3b, 0x08, 0x82, 0x1c, 0x15,  // ...gy.....};....
/* 0680 */ 0x5c, 0xae, 0x1d, 0x1f, 0xe3, 0x64, 0xe3, 0x9e, 0xab, 0x14, 0x56, 0xc9, 0x9f, 0xfb, 0xc8, 0xb8,  // .....d....V.....
/* 0690 */ 0xf4, 0x31, 0x0d, 0xd7, 0x92, 0x80, 0x8f, 0xdb, 0x7e, 0x78, 0xd4, 0xd9, 0x79, 0xc0, 0x52, 0xa8,  // .1......~x..y.R.
/* 06a0 */ 0x9f, 0x8c, 0x48, 0xa1, 0x88, 0xed, 0x78, 0xc8, 0xc6, 0x76, 0x4a, 0x4f, 0xa5, 0xa5, 0xa6, 0xac,  // ..H...x..vJO....
/* 06b0 */ 0x7d, 0x3f, 0x41, 0x88, 0xae, 0xdf, 0xfa, 0xdb, 0xc0, 0xdc, 0x61, 0xf9, 0x56, 0x52, 0xac, 0xf6,  // }?A.......a.VR..
/* 06c0 */ 0xc9, 0x25, 0x03, 0x74, 0xd7, 0x2c, 0x22, 0xee, 0xa9, 0xb6, 0x27, 0x66, 0x8b, 0x12, 0x09, 0x4e,  // .%.t.,"...'f...N
/* 06d0 */ 0x6e, 0x7a, 0x9f, 0xbe, 0x4e, 0xe4, 0xaf, 0xc1, 0xda, 0x6b, 0x04, 0xc5, 0xdd, 0x79, 0x10, 0x32,  // nz..N....k...y.2
/* 06e0 */ 0xcb, 0xbd, 0x12, 0x7b, 0xd9, 0x3c, 0x92, 0x3b, 0x2a, 0x7d, 0x52, 0xf7, 0xd6, 0x73, 0x5a, 0x06,  // ...{.<.;*}R..sZ.
/* 06f0 */ 0xd1, 0xf9, 0x5f, 0xa9, 0x69, 0x8c, 0x04, 0x23, 0xb8, 0xe6, 0x2e, 0xce, 0x0d, 0xea, 0x07, 0x30,  // .._.i..#.......0
/* 0700 */ 0xfd, 0x00, 0x3a, 0x0b, 0x61, 0xdd, 0xb1, 0xb4, 0x96, 0x9a, 0xa9, 0x0e, 0xa6, 0xce, 0xee, 0x98,  // ..:.a...........
/* 0710 */ 0x1c, 0xd5, 0x58, 0x40, 0x1b, 0x34, 0x4a, 0x56, 0xb3, 0xf6, 0xa7, 0xbc, 0x97, 0xe9, 0x85, 0x9b,  // ..X@.4JV........
/* 0720 */ 0x10, 0x43, 0xdf, 0xa6, 0x35, 0xb4, 0x0e, 0x41, 0xa5, 0xae, 0x56, 0x28, 0x2c, 0xcc, 0x10, 0x1b,  // .C..5..A..V(,...
/* 0730 */ 0x71, 0x36, 0x60, 0x1f, 0xfc, 0x4b, 0xc3, 0x4a, 0x1e, 0xb5, 0x51, 0xd9, 0xde, 0xb7, 0x5b, 0x9f,  // q6`..K.J..Q...[.
/* 0740 */ 0x73, 0xb2, 0xa6, 0x6d, 0xa0, 0xdf, 0xad, 0xc6, 0xbe, 0x73, 0xa5, 0x4f, 0x90, 0x13, 0x08, 0xb3,  // s..m.....s.O....
/* 0750 */ 0xcf, 0xeb, 0x5e, 0x7e, 0x5c, 0x6e, 0x99, 0x5a, 0xa0, 0x36, 0x75, 0x03, 0x82, 0x20, 0xa5, 0x74,  // ..^~.n.Z.6u.. .t
/* 0760 */ 0x8c, 0x1e, 0xe4, 0x4b, 0xcb, 0x77, 0x97, 0x4d, 0x33, 0x55, 0xfb, 0x2d, 0xcb, 0x09, 0xd2, 0x57,  // ...K.w.M3U.-...W
/* 0770 */ 0xdd, 0x69, 0xe9, 0x37, 0x55, 0xc6, 0xf5, 0x0c, 0x54, 0x02, 0xc4, 0x7c, 0xe7, 0x9f, 0xc6, 0x1f,  // .i.7U...T..|....
/* 0780 */ 0x9d, 0x20, 0x2d, 0x4a, 0x7a, 0x47, 0xb2, 0x8f, 0x4e, 0x17, 0x88, 0x1e, 0xa1, 0x29, 0xee, 0x26,  // . -JzG..N....).&
/* 0790 */ 0x79, 0x53, 0x2a, 0x2f, 0x8b, 0x38, 0xde, 0x29, 0xf2, 0xa4, 0xcd, 0x39, 0x29, 0xc2, 0x0a, 0x01,  // yS*/.8.)...9)...
/* 07a0 */ 0x7f, 0xb0, 0x5e, 0x79, 0x84, 0x8d, 0xb7, 0x9c, 0x2e, 0x15, 0x40, 0x35, 0x17, 0xd0, 0xea, 0x7e,  // ..^y......@5...~
/* 07b0 */ 0x8e, 0xa4, 0xba, 0xf9, 0x06, 0xa1, 0xfc, 0xa8, 0xdc, 0x74, 0xdb, 0x99, 0x5e, 0xda, 0xbd, 0xff,  // .........t..^...
/* 07c0 */ 0x43, 0xb9, 0xd7, 0x5c, 0xc0, 0x62, 0xbe, 0xae, 0x16, 0xbe, 0x60, 0x7c, 0x2c, 0x8f, 0xbd, 0x53,  // C....b....`|,..S
/* 07d0 */ 0x42, 0x8c, 0x98, 0x57, 0x73, 0xde, 0xa0, 0x94, 0x6f, 0x7a, 0xd1, 0x30, 0xce, 0x26, 0xe2, 0xbf,  // B..Ws...oz.0.&..
/* 07e0 */ 0xde, 0x9f, 0x6f, 0x6c, 0x8f, 0x8c, 0x0c, 0x89, 0x2f, 0x7d, 0xce, 0x1f, 0x2f, 0xaf, 0x37,     // ..ol..../}../.7
        // MACData trailer
        0xed, 0xe8, 0xbf, 0xd6, 0x13, 0xa0, 0xf5, 0x80, 0x4a, 0xe5, 0xff, 0x85, 0x16, 0xfa, 0xcb, 0x1f
};

    BStream stream(2048);
    memcpy(stream.data, indata, sizeof(indata) - 1);
    stream.end += sizeof(indata);

    uint8_t license_key[16] = {};
    LIC::NewLicense_Recv lic(stream, license_key);
    BOOST_CHECK_EQUAL((uint8_t)LIC::NEW_LICENSE, (uint8_t)lic.wMsgType);
    BOOST_CHECK_EQUAL((uint8_t)3, (uint8_t)lic.bVersion);
    BOOST_CHECK_EQUAL((uint16_t)2055, (uint16_t)lic.wMsgSize);

    BOOST_CHECK_EQUAL((uint16_t)9, lic.licenseInfo.wBlobType);
    BOOST_CHECK_EQUAL((uint16_t)2031, lic.licenseInfo.wBlobLen);

    BOOST_CHECK_EQUAL((uint32_t)6, (lic.licenseInfo.dwVersion >> 16) & 0xFFFF); // major
    BOOST_CHECK_EQUAL((uint32_t)0, lic.licenseInfo.dwVersion & 0xFFFF);         // minor
    BOOST_CHECK_EQUAL((uint32_t)14, lic.licenseInfo.cbScope);
    BOOST_CHECK_EQUAL(0, memcmp("\x6d\x69\x63\x72\x6f\x73\x6f\x66\x74\x2e\x63\x6f\x6d\x00", lic.licenseInfo.pbScope, lic.licenseInfo.cbScope));
    BOOST_CHECK_EQUAL((uint32_t)44, lic.licenseInfo.cbCompanyName);

    BOOST_CHECK_EQUAL(0, memcmp(
        /* 0000 */ "\x4d\x00\x69\x00\x63\x00\x72\x00\x6f\x00\x73\x00\x6f\x00\x66\x00" //M.i.c.r.o.s.o.f.
        /* 0010 */ "\x74\x00\x20\x00\x43\x00\x6f\x00\x72\x00\x70\x00\x6f\x00\x72\x00" //t. .C.o.r.p.o.r.
        /* 0020 */ "\x61\x00\x74\x00\x69\x00\x6f\x00\x6e\x00\x00\x00"                 //a.t.i.o.n...
        , lic.licenseInfo.pbCompanyName, lic.licenseInfo.cbCompanyName));
    BOOST_CHECK_EQUAL((uint32_t)8, lic.licenseInfo.cbProductId);
//    BOOST_CHECK_EQUAL(0, memcmp("", lic.licenseInfo.pbProductId, lic.licenseInfo.cbProductId));
//    BOOST_CHECK_EQUAL((uint32_t)0, lic.licenseInfo.cbLicenseInfo);
//    BOOST_CHECK_EQUAL(0, memcmp("", lic.licenseInfo.pbLicenseInfo, lic.licenseInfo.cbLicenseInfo));
}


