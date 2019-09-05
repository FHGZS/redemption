
#pragma once

#include "keylayout_r.hpp"

namespace x80020405{ 

const static int LCID = 0x20405;

const static char * const locale_name = "cs-CZ.programmers";

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
    { 0x002d, 0xc },
    { 0x003d, 0xd },
    { 0x0008, 0xe },
    { 0x0009, 0xf },
    { 0x0071, 0x10 },
    { 0x0077, 0x11 },
    { 0x0065, 0x12 },
    { 0x0072, 0x13 },
    { 0x0074, 0x14 },
    { 0x0079, 0x15 },
    { 0x0075, 0x16 },
    { 0x0069, 0x17 },
    { 0x006f, 0x18 },
    { 0x0070, 0x19 },
    { 0x005b, 0x1a },
    { 0x005d, 0x1b },
    { 0x000d, 0x1c },
    { 0x0061, 0x1e },
    { 0x0073, 0x1f },
    { 0x0064, 0x20 },
    { 0x0066, 0x21 },
    { 0x0067, 0x22 },
    { 0x0068, 0x23 },
    { 0x006a, 0x24 },
    { 0x006b, 0x25 },
    { 0x006c, 0x26 },
    { 0x003b, 0x27 },
    { 0x0027, 0x28 },
    { 0x0060, 0x29 },
    { 0x005c, 0x2b },
    { 0x007a, 0x2c },
    { 0x0078, 0x2d },
    { 0x0063, 0x2e },
    { 0x0076, 0x2f },
    { 0x0062, 0x30 },
    { 0x006e, 0x31 },
    { 0x006d, 0x32 },
    { 0x002c, 0x33 },
    { 0x002e, 0x34 },
    { 0x002f, 0x35 },
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
    { 0x005c, 0x56 },
    { 0x002f, 0x62 },
    { 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t shift
{
    { 0x001b, 0x1 },
    { 0x0021, 0x2 },
    { 0x0040, 0x3 },
    { 0x0023, 0x4 },
    { 0x0024, 0x5 },
    { 0x0025, 0x6 },
    { 0x005e, 0x7 },
    { 0x0026, 0x8 },
    { 0x002a, 0x9 },
    { 0x0028, 0xa },
    { 0x0029, 0xb },
    { 0x005f, 0xc },
    { 0x002b, 0xd },
    { 0x0008, 0xe },
    { 0x0051, 0x10 },
    { 0x0057, 0x11 },
    { 0x0045, 0x12 },
    { 0x0052, 0x13 },
    { 0x0054, 0x14 },
    { 0x0059, 0x15 },
    { 0x0055, 0x16 },
    { 0x0049, 0x17 },
    { 0x004f, 0x18 },
    { 0x0050, 0x19 },
    { 0x007b, 0x1a },
    { 0x007d, 0x1b },
    { 0x000d, 0x1c },
    { 0x0041, 0x1e },
    { 0x0053, 0x1f },
    { 0x0044, 0x20 },
    { 0x0046, 0x21 },
    { 0x0047, 0x22 },
    { 0x0048, 0x23 },
    { 0x004a, 0x24 },
    { 0x004b, 0x25 },
    { 0x004c, 0x26 },
    { 0x003a, 0x27 },
    { 0x0022, 0x28 },
    { 0x007e, 0x29 },
    { 0x007c, 0x2b },
    { 0x005a, 0x2c },
    { 0x0058, 0x2d },
    { 0x0043, 0x2e },
    { 0x0056, 0x2f },
    { 0x0042, 0x30 },
    { 0x004e, 0x31 },
    { 0x004d, 0x32 },
    { 0x003c, 0x33 },
    { 0x003e, 0x34 },
    { 0x003f, 0x35 },
    { 0x002a, 0x37 },
    { 0x0020, 0x39 },
    { 0x002d, 0x4a },
    { 0x002b, 0x4e },
    { 0x002c, 0x53 },
    { 0x007c, 0x56 },
    { 0x007f, 0x63 },
    { 0x000d, 0x64 },
    { 0x002f, 0x68 },
};


const Keylayout_r::KeyLayoutMap_t altGr
{
    { 0x001b, 0x1 },
    { 0x002b, 0x2 },
    { 0x011b, 0x3 },
    { 0x0161, 0x4 },
    { 0x010d, 0x5 },
    { 0x0159, 0x6 },
    { 0x017e, 0x7 },
    { 0x00fd, 0x8 },
    { 0x00e1, 0x9 },
    { 0x00ed, 0xa },
    { 0x00e9, 0xb },
    { 0x003d, 0xc },
    { 0x00b4, 0xd },
    { 0x0008, 0xe },
    { 0x0009, 0xf },
    { 0x20ac, 0x12 },
    { 0x00fa, 0x1a },
    { 0x0029, 0x1b },
    { 0x000d, 0x1c },
    { 0x016f, 0x27 },
    { 0x00a7, 0x28 },
    { 0x003b, 0x29 },
    { 0x00a8, 0x2b },
    { 0x003f, 0x33 },
    { 0x003a, 0x34 },
    { 0x002d, 0x35 },
    { 0x002a, 0x37 },
    { 0x002d, 0x4a },
    { 0x002b, 0x4e },
    { 0x00df, 0x56 },
    { 0x002f, 0x62 },
    { 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t shiftAltGr
{
    { 0x001b, 0x1 },
    { 0x0025, 0xc },
    { 0x02c7, 0xd },
    { 0x0008, 0xe },
    { 0x0009, 0xf },
    { 0x002f, 0x1a },
    { 0x0028, 0x1b },
    { 0x000d, 0x1c },
    { 0x0022, 0x27 },
    { 0x0021, 0x28 },
    { 0x00b0, 0x29 },
    { 0x005e, 0x2b },
    { 0x00d7, 0x33 },
    { 0x00f7, 0x34 },
    { 0x005f, 0x35 },
    { 0x002a, 0x37 },
    { 0x002d, 0x4a },
    { 0x002b, 0x4e },
    { 0x02dd, 0x56 },
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
    { 0x002d, 0xc },
    { 0x003d, 0xd },
    { 0x0008, 0xe },
    { 0x0009, 0xf },
    { 0x0051, 0x10 },
    { 0x0057, 0x11 },
    { 0x0045, 0x12 },
    { 0x0052, 0x13 },
    { 0x0054, 0x14 },
    { 0x0059, 0x15 },
    { 0x0055, 0x16 },
    { 0x0049, 0x17 },
    { 0x004f, 0x18 },
    { 0x0050, 0x19 },
    { 0x005b, 0x1a },
    { 0x005d, 0x1b },
    { 0x000d, 0x1c },
    { 0x0041, 0x1e },
    { 0x0053, 0x1f },
    { 0x0044, 0x20 },
    { 0x0046, 0x21 },
    { 0x0047, 0x22 },
    { 0x0048, 0x23 },
    { 0x004a, 0x24 },
    { 0x004b, 0x25 },
    { 0x004c, 0x26 },
    { 0x003b, 0x27 },
    { 0x0027, 0x28 },
    { 0x0060, 0x29 },
    { 0x005c, 0x2b },
    { 0x005a, 0x2c },
    { 0x0058, 0x2d },
    { 0x0043, 0x2e },
    { 0x0056, 0x2f },
    { 0x0042, 0x30 },
    { 0x004e, 0x31 },
    { 0x004d, 0x32 },
    { 0x002c, 0x33 },
    { 0x002e, 0x34 },
    { 0x002f, 0x35 },
    { 0x002a, 0x37 },
    { 0x0020, 0x39 },
    { 0x002d, 0x4a },
    { 0x002b, 0x4e },
    { 0x002c, 0x53 },
    { 0x005c, 0x56 },
    { 0x002f, 0x62 },
    { 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t capslock_shift
{
    { 0x001b, 0x1 },
    { 0x0021, 0x2 },
    { 0x0040, 0x3 },
    { 0x0023, 0x4 },
    { 0x0024, 0x5 },
    { 0x0025, 0x6 },
    { 0x005e, 0x7 },
    { 0x0026, 0x8 },
    { 0x002a, 0x9 },
    { 0x0028, 0xa },
    { 0x0029, 0xb },
    { 0x005f, 0xc },
    { 0x002b, 0xd },
    { 0x0008, 0xe },
    { 0x0009, 0xf },
    { 0x0071, 0x10 },
    { 0x0077, 0x11 },
    { 0x0065, 0x12 },
    { 0x0072, 0x13 },
    { 0x0074, 0x14 },
    { 0x0079, 0x15 },
    { 0x0075, 0x16 },
    { 0x0069, 0x17 },
    { 0x006f, 0x18 },
    { 0x0070, 0x19 },
    { 0x007b, 0x1a },
    { 0x007d, 0x1b },
    { 0x000d, 0x1c },
    { 0x0061, 0x1e },
    { 0x0073, 0x1f },
    { 0x0064, 0x20 },
    { 0x0066, 0x21 },
    { 0x0067, 0x22 },
    { 0x0068, 0x23 },
    { 0x006a, 0x24 },
    { 0x006b, 0x25 },
    { 0x006c, 0x26 },
    { 0x003a, 0x27 },
    { 0x0022, 0x28 },
    { 0x007e, 0x29 },
    { 0x007c, 0x2b },
    { 0x007a, 0x2c },
    { 0x0078, 0x2d },
    { 0x0063, 0x2e },
    { 0x0076, 0x2f },
    { 0x0062, 0x30 },
    { 0x006e, 0x31 },
    { 0x006d, 0x32 },
    { 0x003c, 0x33 },
    { 0x003e, 0x34 },
    { 0x003f, 0x35 },
    { 0x002a, 0x37 },
    { 0x0020, 0x39 },
    { 0x002d, 0x4a },
    { 0x002b, 0x4e },
    { 0x002c, 0x53 },
    { 0x007c, 0x56 },
    { 0x002f, 0x62 },
    { 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t capslock_altGr
{
    { 0x001b, 0x1 },
    { 0x002b, 0x2 },
    { 0x011b, 0x3 },
    { 0x0161, 0x4 },
    { 0x010d, 0x5 },
    { 0x0159, 0x6 },
    { 0x017e, 0x7 },
    { 0x00fd, 0x8 },
    { 0x00e1, 0x9 },
    { 0x00ed, 0xa },
    { 0x00e9, 0xb },
    { 0x003d, 0xc },
    { 0x00b4, 0xd },
    { 0x0008, 0xe },
    { 0x0009, 0xf },
    { 0x20ac, 0x12 },
    { 0x00fa, 0x1a },
    { 0x0029, 0x1b },
    { 0x000d, 0x1c },
    { 0x016f, 0x27 },
    { 0x00a7, 0x28 },
    { 0x003b, 0x29 },
    { 0x00a8, 0x2b },
    { 0x003f, 0x33 },
    { 0x003a, 0x34 },
    { 0x002d, 0x35 },
    { 0x002a, 0x37 },
    { 0x002d, 0x4a },
    { 0x002b, 0x4e },
    { 0x00df, 0x56 },
    { 0x002f, 0x62 },
    { 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t capslock_shiftAltGr
{
    { 0x001b, 0x1 },
    { 0x0025, 0xc },
    { 0x02c7, 0xd },
    { 0x0008, 0xe },
    { 0x0009, 0xf },
    { 0x002f, 0x1a },
    { 0x0028, 0x1b },
    { 0x000d, 0x1c },
    { 0x0022, 0x27 },
    { 0x0021, 0x28 },
    { 0x00b0, 0x29 },
    { 0x005e, 0x2b },
    { 0x00d7, 0x33 },
    { 0x00f7, 0x34 },
    { 0x005f, 0x35 },
    { 0x002a, 0x37 },
    { 0x002d, 0x4a },
    { 0x002b, 0x4e },
    { 0x02dd, 0x56 },
    { 0x002f, 0x62 },
    { 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t ctrl
{
    { 0x001b, 0x1 },
    { 0x0008, 0xe },
    { 0x0009, 0xf },
    { 0x001b, 0x1b },
    { 0x000d, 0x1c },
    { 0x001d, 0x27 },
    { 0x001c, 0x2b },
    { 0x002a, 0x37 },
    { 0x0020, 0x39 },
    { 0x002d, 0x4a },
    { 0x002b, 0x4e },
    { 0x002f, 0x62 },
    { 0x000d, 0x64 },
};


const Keylayout_r::KeyLayoutMap_t deadkeys
{
    { 0xb4, 0xd},
    { 0x59, 0xdd},
    { 0x53, 0x15a},
    { 0x52, 0x154},
    { 0x55, 0xda},
    { 0x4c, 0x139},
    { 0x4e, 0x143},
    { 0x4f, 0xd3},
    { 0x7a, 0x17a},
    { 0x65, 0xe9},
    { 0x63, 0x107},
    { 0x61, 0xe1},
    { 0x69, 0xed},
    { 0x49, 0xcd},
    { 0x20, 0xb4},
    { 0x41, 0xc1},
    { 0x43, 0x106},
    { 0x45, 0xc9},
    { 0x5a, 0x179},
    { 0x6f, 0xf3},
    { 0x6e, 0x144},
    { 0x6c, 0x13a},
    { 0x73, 0x15b},
    { 0x72, 0x155},
    { 0x75, 0xfa},
    { 0x79, 0xfd},
    { 0x2c7, 0xd},
    { 0x7a, 0x17e},
    { 0x43, 0x10c},
    { 0x6e, 0x148},
    { 0x6c, 0x13e},
    { 0x53, 0x160},
    { 0x52, 0x158},
    { 0x54, 0x164},
    { 0x72, 0x159},
    { 0x44, 0x10e},
    { 0x73, 0x161},
    { 0x45, 0x11a},
    { 0x20, 0x2c7},
    { 0x64, 0x10f},
    { 0x65, 0x11b},
    { 0x63, 0x10d},
    { 0x5a, 0x17d},
    { 0x4c, 0x13d},
    { 0x4e, 0x147},
    { 0x74, 0x165},
    { 0xb0, 0x29},
    { 0x41, 0xc5},
    { 0x20, 0xb0},
    { 0x75, 0x16f},
    { 0x55, 0x16e},
    { 0x61, 0xe5},
    { 0xa8, 0x2b},
    { 0x6f, 0xf6},
    { 0x49, 0xcf},
    { 0x59, 0x178},
    { 0x41, 0xc4},
    { 0x45, 0xcb},
    { 0x55, 0xdc},
    { 0x20, 0xa8},
    { 0x65, 0xeb},
    { 0x75, 0xfc},
    { 0x61, 0xe4},
    { 0x79, 0xff},
    { 0x4f, 0xd6},
    { 0x69, 0xef},
    { 0x5e, 0x2b},
    { 0x6f, 0xf4},
    { 0x49, 0xce},
    { 0x20, 0x5e},
    { 0x41, 0xc2},
    { 0x45, 0xca},
    { 0x55, 0xdb},
    { 0x65, 0xea},
    { 0x75, 0xfb},
    { 0x61, 0xe2},
    { 0x4f, 0xd4},
    { 0x69, 0xee},
    { 0x2dd, 0x56},
    { 0x6f, 0x151},
    { 0x20, 0x2dd},
    { 0x75, 0x171},
    { 0x4f, 0x150},
    { 0x55, 0x170},
};


const static uint8_t nbDeadkeys = 6;

}

static const Keylayout_r keylayout_x80020405( x80020405::LCID
                                 , x80020405::locale_name
                                 , x80020405::noMod
                                 , x80020405::shift
                                 , x80020405::altGr
                                 , x80020405::shiftAltGr
                                 , x80020405::ctrl
                                 , x80020405::capslock_noMod
                                 , x80020405::capslock_shift
                                 , x80020405::capslock_altGr
                                 , x80020405::capslock_shiftAltGr
                                 , x80020405::deadkeys
                                 , x80020405::nbDeadkeys

);

