
#pragma once

#include "keylayout_r.hpp"

namespace x80010408{ 

const static int LCID = 0x10408;

const static char * const locale_name = "el-GR.220";

const Keylayout_r::KeyLayoutMap_t noMod
{
	{ 0x001b, 0x1 },
	{ 0x0031, 0x2 },
	{ 0x0032, 0x3 },
	{ 0x0033, 0x4 },
	{ 0x0034, 0x5 },
	{ 0x0035, 0x6 },
	{ 0x0036, 0x7 },
	{ 0x0037, 0x8 },
	{ 0x0038, 0x9 },
	{ 0x0039, 0xa },
	{ 0x0030, 0xb },
	{ 0x0027, 0xc },
	{ 0x005d, 0xd },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x037e, 0x10 },
	{ 0x03c2, 0x11 },
	{ 0x03b5, 0x12 },
	{ 0x03c1, 0x13 },
	{ 0x03c4, 0x14 },
	{ 0x03c5, 0x15 },
	{ 0x03b8, 0x16 },
	{ 0x03b9, 0x17 },
	{ 0x03bf, 0x18 },
	{ 0x03c0, 0x19 },
	{ 0x002b, 0x1a },
	{ 0x007d, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x03b1, 0x1e },
	{ 0x03c3, 0x1f },
	{ 0x03b4, 0x20 },
	{ 0x03c6, 0x21 },
	{ 0x03b3, 0x22 },
	{ 0x03b7, 0x23 },
	{ 0x03be, 0x24 },
	{ 0x03ba, 0x25 },
	{ 0x03bb, 0x26 },
	{ 0x0384, 0x27 },
	{ 0x00a8, 0x28 },
	{ 0x00bd, 0x29 },
	{ 0x0023, 0x2b },
	{ 0x03b6, 0x2c },
	{ 0x03c7, 0x2d },
	{ 0x03c8, 0x2e },
	{ 0x03c9, 0x2f },
	{ 0x03b2, 0x30 },
	{ 0x03bd, 0x31 },
	{ 0x03bc, 0x32 },
	{ 0x002c, 0x33 },
	{ 0x002e, 0x34 },
	{ 0x002d, 0x35 },
	{ 0x002a, 0x37 },
	{ 0x0020, 0x39 },
	{ 0x0037, 0x47 },
	{ 0x0038, 0x48 },
	{ 0x0039, 0x49 },
	{ 0x002d, 0x4a },
	{ 0x0034, 0x4b },
	{ 0x0035, 0x4c },
	{ 0x0036, 0x4d },
	{ 0x002b, 0x4e },
	{ 0x0031, 0x4f },
	{ 0x0032, 0x50 },
	{ 0x0033, 0x51 },
	{ 0x0030, 0x52 },
	{ 0x002c, 0x53 },
	{ 0x003c, 0x56 },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t shift
{
	{ 0x001b, 0x1 },
	{ 0x0021, 0x2 },
	{ 0x0022, 0x3 },
	{ 0x00a3, 0x4 },
	{ 0x0024, 0x5 },
	{ 0x0025, 0x6 },
	{ 0x0026, 0x7 },
	{ 0x002f, 0x8 },
	{ 0x0028, 0x9 },
	{ 0x0029, 0xa },
	{ 0x003d, 0xb },
	{ 0x003f, 0xc },
	{ 0x005b, 0xd },
	{ 0x0008, 0xe },
	{ 0x003a, 0x10 },
	{ 0x007e, 0x11 },
	{ 0x0395, 0x12 },
	{ 0x03a1, 0x13 },
	{ 0x03a4, 0x14 },
	{ 0x03a5, 0x15 },
	{ 0x0398, 0x16 },
	{ 0x0399, 0x17 },
	{ 0x039f, 0x18 },
	{ 0x03a0, 0x19 },
	{ 0x002a, 0x1a },
	{ 0x007b, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x0391, 0x1e },
	{ 0x03a3, 0x1f },
	{ 0x0394, 0x20 },
	{ 0x03a6, 0x21 },
	{ 0x0393, 0x22 },
	{ 0x0397, 0x23 },
	{ 0x039e, 0x24 },
	{ 0x039a, 0x25 },
	{ 0x039b, 0x26 },
	{ 0x00a8, 0x27 },
	{ 0x0385, 0x28 },
	{ 0x00b1, 0x29 },
	{ 0x0040, 0x2b },
	{ 0x0396, 0x2c },
	{ 0x03a7, 0x2d },
	{ 0x03a8, 0x2e },
	{ 0x03a9, 0x2f },
	{ 0x0392, 0x30 },
	{ 0x039d, 0x31 },
	{ 0x039c, 0x32 },
	{ 0x003b, 0x33 },
	{ 0x003a, 0x34 },
	{ 0x005f, 0x35 },
	{ 0x002a, 0x37 },
	{ 0x0020, 0x39 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002c, 0x53 },
	{ 0x003e, 0x56 },
	{ 0x007f, 0x63 },
	{ 0x000d, 0x64 },
	{ 0x002f, 0x68 },
};


const Keylayout_r::KeyLayoutMap_t altGr
{
	{ 0x001b, 0x1 },
	{ 0x00b2, 0x3 },
	{ 0x00b3, 0x4 },
	{ 0x00a3, 0x5 },
	{ 0x00a7, 0x6 },
	{ 0x00b6, 0x7 },
	{ 0x00a4, 0x9 },
	{ 0x00a6, 0xa },
	{ 0x00b0, 0xb },
	{ 0x00b1, 0xc },
	{ 0x00bd, 0xd },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x20ac, 0x12 },
	{ 0x00ae, 0x13 },
	{ 0x00a5, 0x15 },
	{ 0x00ab, 0x1a },
	{ 0x00bb, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x0385, 0x27 },
	{ 0x0385, 0x28 },
	{ 0x00ac, 0x2b },
	{ 0x00a9, 0x2e },
	{ 0x002a, 0x37 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t shiftAltGr
{
	{ 0x001b, 0x1 },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x000d, 0x1c },
	{ 0x002a, 0x37 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t capslock_noMod
{
	{ 0x001b, 0x1 },
	{ 0x0031, 0x2 },
	{ 0x0032, 0x3 },
	{ 0x0033, 0x4 },
	{ 0x0034, 0x5 },
	{ 0x0035, 0x6 },
	{ 0x0036, 0x7 },
	{ 0x0037, 0x8 },
	{ 0x0038, 0x9 },
	{ 0x0039, 0xa },
	{ 0x0030, 0xb },
	{ 0x0027, 0xc },
	{ 0x005d, 0xd },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x037e, 0x10 },
	{ 0x03c2, 0x11 },
	{ 0x0395, 0x12 },
	{ 0x03a1, 0x13 },
	{ 0x03a4, 0x14 },
	{ 0x03a5, 0x15 },
	{ 0x0398, 0x16 },
	{ 0x0399, 0x17 },
	{ 0x039f, 0x18 },
	{ 0x03a0, 0x19 },
	{ 0x002b, 0x1a },
	{ 0x007d, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x0391, 0x1e },
	{ 0x03a3, 0x1f },
	{ 0x0394, 0x20 },
	{ 0x03a6, 0x21 },
	{ 0x0393, 0x22 },
	{ 0x0397, 0x23 },
	{ 0x039e, 0x24 },
	{ 0x039a, 0x25 },
	{ 0x039b, 0x26 },
	{ 0x0384, 0x27 },
	{ 0x00a8, 0x28 },
	{ 0x00bd, 0x29 },
	{ 0x0023, 0x2b },
	{ 0x0396, 0x2c },
	{ 0x03a7, 0x2d },
	{ 0x03a8, 0x2e },
	{ 0x03a9, 0x2f },
	{ 0x0392, 0x30 },
	{ 0x039d, 0x31 },
	{ 0x039c, 0x32 },
	{ 0x002c, 0x33 },
	{ 0x002e, 0x34 },
	{ 0x002d, 0x35 },
	{ 0x002a, 0x37 },
	{ 0x0020, 0x39 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002c, 0x53 },
	{ 0x003c, 0x56 },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t capslock_shift
{
	{ 0x001b, 0x1 },
	{ 0x0021, 0x2 },
	{ 0x0022, 0x3 },
	{ 0x00a3, 0x4 },
	{ 0x0024, 0x5 },
	{ 0x0025, 0x6 },
	{ 0x0026, 0x7 },
	{ 0x002f, 0x8 },
	{ 0x0028, 0x9 },
	{ 0x0029, 0xa },
	{ 0x003d, 0xb },
	{ 0x003f, 0xc },
	{ 0x005b, 0xd },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x003a, 0x10 },
	{ 0x007e, 0x11 },
	{ 0x03b5, 0x12 },
	{ 0x03c1, 0x13 },
	{ 0x03c4, 0x14 },
	{ 0x03c5, 0x15 },
	{ 0x03b8, 0x16 },
	{ 0x03b9, 0x17 },
	{ 0x03bf, 0x18 },
	{ 0x03c0, 0x19 },
	{ 0x002a, 0x1a },
	{ 0x007b, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x03b1, 0x1e },
	{ 0x03c3, 0x1f },
	{ 0x03b4, 0x20 },
	{ 0x03c6, 0x21 },
	{ 0x03b3, 0x22 },
	{ 0x03b7, 0x23 },
	{ 0x03be, 0x24 },
	{ 0x03ba, 0x25 },
	{ 0x03bb, 0x26 },
	{ 0x00a8, 0x27 },
	{ 0x0385, 0x28 },
	{ 0x00b1, 0x29 },
	{ 0x0040, 0x2b },
	{ 0x03b6, 0x2c },
	{ 0x03c7, 0x2d },
	{ 0x03c8, 0x2e },
	{ 0x03c9, 0x2f },
	{ 0x03b2, 0x30 },
	{ 0x03bd, 0x31 },
	{ 0x03bc, 0x32 },
	{ 0x003b, 0x33 },
	{ 0x003a, 0x34 },
	{ 0x005f, 0x35 },
	{ 0x002a, 0x37 },
	{ 0x0020, 0x39 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002c, 0x53 },
	{ 0x003e, 0x56 },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t capslock_altGr
{
	{ 0x001b, 0x1 },
	{ 0x00b2, 0x3 },
	{ 0x00b3, 0x4 },
	{ 0x00a3, 0x5 },
	{ 0x00a7, 0x6 },
	{ 0x00b6, 0x7 },
	{ 0x00a4, 0x9 },
	{ 0x00a6, 0xa },
	{ 0x00b0, 0xb },
	{ 0x00b1, 0xc },
	{ 0x00bd, 0xd },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x20ac, 0x12 },
	{ 0x00ae, 0x13 },
	{ 0x00a5, 0x15 },
	{ 0x00ab, 0x1a },
	{ 0x00bb, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x0385, 0x27 },
	{ 0x0385, 0x28 },
	{ 0x00ac, 0x2b },
	{ 0x00a9, 0x2e },
	{ 0x002a, 0x37 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t capslock_shiftAltGr
{
	{ 0x001b, 0x1 },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x000d, 0x1c },
	{ 0x002a, 0x37 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t ctrl
{
	{ 0x001b, 0x1 },
	{ 0x001d, 0xd },
	{ 0x0008, 0xe },
	{ 0x0009, 0xf },
	{ 0x001b, 0x1a },
	{ 0x001d, 0x1b },
	{ 0x000d, 0x1c },
	{ 0x002a, 0x37 },
	{ 0x0020, 0x39 },
	{ 0x002d, 0x4a },
	{ 0x002b, 0x4e },
	{ 0x002f, 0x62 },
	{ 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t deadkeys
{
	{ 0x384, 0x27},
	{ 0x395, 0x388},
	{ 0x20, 0x384},
	{ 0x3c9, 0x3ce},
	{ 0x3a9, 0x38f},
	{ 0x3a5, 0x38e},
	{ 0x397, 0x389},
	{ 0x399, 0x38a},
	{ 0x3c5, 0x3cd},
	{ 0x3b5, 0x3ad},
	{ 0x3b7, 0x3ae},
	{ 0x3bf, 0x3cc},
	{ 0x3b1, 0x3ac},
	{ 0x39f, 0x38c},
	{ 0x3b9, 0x3af},
	{ 0x391, 0x386},
	{ 0xa8, 0x27},
	{ 0x3a5, 0x3ab},
	{ 0x20, 0xa8},
	{ 0x3b9, 0x3ca},
	{ 0x399, 0x3aa},
	{ 0x3c5, 0x3cb},
	{ 0x385, 0x28},
	{ 0x20, 0x385},
	{ 0x3b9, 0x390},
	{ 0x3c5, 0x3b0},
};


const static uint8_t nbDeadkeys = 3;

}

static const Keylayout_r keylayout_x80010408( x80010408::LCID
                                 , x80010408::locale_name
                                 , x80010408::noMod
                                 , x80010408::shift
                                 , x80010408::altGr
                                 , x80010408::shiftAltGr
                                 , x80010408::ctrl
                                 , x80010408::capslock_noMod
                                 , x80010408::capslock_shift
                                 , x80010408::capslock_altGr
                                 , x80010408::capslock_shiftAltGr
                                 , x80010408::deadkeys
                                 , x80010408::nbDeadkeys

);

