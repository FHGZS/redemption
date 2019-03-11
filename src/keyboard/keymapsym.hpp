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
   Copyright (C) Wallix 2012-2013
   Author(s): Christophe Grosjean, Dominique Lafages

   header file. KeymapSym object for keymap translation from RDP to X (VNC)
*/

#pragma once

#include <cstdint>

enum KS_Symbols {

    XK_EuroSign                         =     0x20AC,

    KS_Bksp  = 0xff08,
    KS_Tab   = 0xff09,
//    Linefeed                         65290     0xff0a
    KS_Linefeed                         =     0xff0a,
//    Clear                            65291     0xff0b
    KS_Clear                            =     0xff0b,
    KS_Ret   = 0xff0d,
    KS_Pause = 0xff13,
//    Scroll_Lock                      65300     0xff14
    KS_Scroll_Lock                      =     0xff14,
//    Sys_Req                          65301     0xff15
    KS_Sys_Req                          =     0xff15,
    KS_Esc =     0xff1b,
//    Multi_key                        65312     0xff20
    KS_Multi_key                        =     0xff20,
//    Kanji                            65313     0xff21
    KS_Kanji                            =     0xff21,
    KS_Home =  0xff50,
    KS_Left =  0xff51,
    KS_Up   =  0xff52,
    KS_Right = 0xff53,
    KS_Down  = 0xff54,
    KS_Prior = 0xff55,
    KS_Next  = 0xff56,
    KS_End   = 0xff57,
//    Begin                            65368     0xff58
    KS_Begin                            =     0xff58,
//    Select                           65376     0xff60
    KS_Select                           =     0xff60,
    KS_Print                            =     0xff61,
//    Execute                          65378     0xff62
    KS_Execute                          =     0xff62,
    KS_Ins =  0xff63,
//    Undo                             65381     0xff65
    KS_Undo                             =     0xff65,
//    Redo                             65382     0xff66
    KS_Redo                             =     0xff66,
    KS_Menu = 0xff67,
//    Find                             65384     0xff68
    KS_Find                             =     0xff68,
//    Cancel                           65385     0xff69
    KS_Cancel                           =     0xff69,
//    Help                             65386     0xff6a
    KS_Help                             =     0xff6a,
//    Break                            65387     0xff6b
    KS_Break                            =     0xff6b,
//    Hebrew_switch                    65406     0xff7e
    KS_Hebrew_switch                    =     0xff7e,
//    Num_Lock                         65407     0xff7f
    KS_Num_Lock                         =     0xff7f,
//    KP_Space                         65408     0xff80
    KS_KP_Space                         =     0xff80,
//    KP_Tab                           65417     0xff89
    KS_KP_Tab                           =     0xff89,
    KS_KP_Enter  = 0xff8d,
//    KP_F1                            65425     0xff91
    KS_KP_F1                            =     0xff91,
//    KP_F2                            65426     0xff92
    KS_KP_F2                            =     0xff92,
//    KP_F3                            65427     0xff93
    KS_KP_F3                            =     0xff93,
//    KP_F4                            65428     0xff94
    KS_KP_F4                            =     0xff94,
//    KP_Multiply                      65450     0xffaa
    KS_KP_Mul = 0xffaa,
    KS_KP_Add = 0xffab,
//    KP_Separator                     65452     0xffac
    KS_KP_Separator                     =     0xffac,
//    KP_Subtract                      65453     0xffad
    KS_KP_Subtract                      =     0xffad,
//    KP_Decimal                       65454     0xffae
    KS_KP_Decimal                       =     0xffae,
//    KP_Divide                        65455     0xffaf
    KS_KP_Divide                        =     0xffaf,
    KS_KP_0 = 0xffb0,
    KS_KP_1 = 0xffb1,
    KS_KP_2 = 0xffb2,
    KS_KP_3 = 0xffb3,
    KS_KP_4 = 0xffb4,
    KS_KP_5 = 0xffb5,
    KS_KP_6 = 0xffb6,
    KS_KP_7 = 0xffb7,
    KS_KP_8 = 0xffb8,
    KS_KP_9 = 0xffb9,
    KS_KP_Equal = 0xffbd,
//    F1                               65470     0xffbe
    KS_F1                               =     0xffbe,
//    F2                               65471     0xffbf
    KS_F2                               =     0xffbf,
//    F3                               65472     0xffc0
    KS_F3                               =     0xffc0,
//    F4                               65473     0xffc1
    KS_F4                               =     0xffc1,
//    F5                               65474     0xffc2
    KS_F5                               =     0xffc2,
//    F6                               65475     0xffc3
    KS_F6                               =     0xffc3,
//    F7                               65476     0xffc4
    KS_F7                               =     0xffc4,
//    F8                               65477     0xffc5
    KS_F8                               =     0xffc5,
//    F9                               65478     0xffc6
    KS_F9                               =     0xffc6,
//    F10                              65479     0xffc7
    KS_F10                              =     0xffc7,
//    L1                               65480     0xffc8
    KS_L1                               =     0xffc8,
//    L2                               65481     0xffc9
    KS_L2                               =     0xffc9,
//    L3                               65482     0xffca
    KS_L3                               =     0xffca,
//    L4                               65483     0xffcb
    KS_L4                               =     0xffcb,
//    L5                               65484     0xffcc
    KS_L5                               =     0xffcc,
//    L6                               65485     0xffcd
    KS_L6                               =     0xffcd,
//    L7                               65486     0xffce
    KS_L7                               =     0xffce,
 //    L8                               65487     0xffcf
    KS_L8                               =     0xffcf,
//    L9                               65488     0xffd0
    KS_L9                               =     0xffd0,
//    L10                              65489     0xffd1
    KS_L10                              =     0xffd1,
//    R1                               65490     0xffd2
    KS_R1                               =     0xffd2,
//    R2                               65491     0xffd3
    KS_R2                               =     0xffd3,
//    R3                               65492     0xffd4
    KS_R3                               =     0xffd4,
//    R4                               65493     0xffd5
    KS_R4                               =     0xffd5,
//    R5                               65494     0xffd6
    KS_R5                               =     0xffd6,
//    R6                               65495     0xffd7
    KS_R6                               =     0xffd7,
//    R7                               65496     0xffd8
    KS_R7                               =     0xffd8,
//    R8                               65497     0xffd9
    KS_R8                               =     0xffd9,
//    R9                               65498     0xffda
    KS_R9                               =     0xffda,
//    R10                              65499     0xffdb
    KS_R10                              =     0xffdb,
//    R11                              65500     0xffdc
    KS_R11                              =     0xffdc,
//    R12                              65501     0xffdd
    KS_R12                              =     0xffdd,
//    F33                              65502     0xffde
    KS_F33                              =     0xffde,
//    R14                              65503     0xffdf
    KS_R14                              =     0xffdf,
//    R15                              65504     0xffe0
    KS_R15                              =     0xffe0,
//    Shift_L                          65505     0xffe1
    KS_Shift_L                          =     0xffe1,
    KS_Shift_R  =  0xffe2,
    KS_Ctrl_L   =  0xffe3,
    KS_Ctrl_R   =  0xffe4,
    KS_Caps_Lock =  0xffe5,
//    Shift_Lock                       65510     0xffe6
    KS_Shift_Lock                       =     0xffe6,
//    Meta_L                           65511     0xffe7
    KS_Meta_L                           =     0xffe7,
//    Meta_R                           65512     0xffe8
    KS_Meta_R                           =     0xffe8,
    KS_Alt_L  =   0xffe9,
    KS_Alt_Gr =   0xffea,
//    App                              65373     0xff5d
    KS_App                              =     0xff5d,
    KS_Win_L = 0xffeb, // Actually Super_L, Win_L is 0xff5b but doesn' t work with VNC
    KS_Win_R = 0xffec, // Actually Super_R, Win_L is 0xff5c but doesn' t work with VNC 

//    Hyper_L                          65517     0xffed
    KS_Hyper_L                          =     0xffed,
//    Hyper_R                          65518     0xffee
    KS_Hyper_R                          =     0xffee,

    KS_Del = 0xffff
};

// KEYSYMS
// =======
//    space                               32     0x0020
//    exclam                              33     0x0021
//    quotedbl                            34     0x0022
//    numbersign                          35     0x0023
//    dollar                              36     0x0024
//    percent                             37     0x0025
//    ampersand                           38     0x0026
//    quoteright                          39     0x0027
//    parenleft                           40     0x0028
//    parenright                          41     0x0029
//    asterisk                            42     0x002a
//    plus                                43     0x002b
//    comma                               44     0x002c
//    minus                               45     0x002d
//    period                              46     0x002e
//    slash                               47     0x002f
//    0                                   48     0x0030
//    1                                   49     0x0031
//    2                                   50     0x0032
//    3                                   51     0x0033
//    4                                   52     0x0034
//    5                                   53     0x0035
//    6                                   54     0x0036
//    7                                   55     0x0037
//    8                                   56     0x0038
//    9                                   57     0x0039
//    colon                               58     0x003a
//    semicolon                           59     0x003b
//    less                                60     0x003c
//    equal                               61     0x003d
//    greater                             62     0x003e
//    question                            63     0x003f
//    at                                  64     0x0040
//    A                                   65     0x0041
//    B                                   66     0x0042
//    C                                   67     0x0043
//    D                                   68     0x0044
//    E                                   69     0x0045
//    F                                   70     0x0046
//    G                                   71     0x0047
//    H                                   72     0x0048
//    I                                   73     0x0049
//    J                                   74     0x004a
//    K                                   75     0x004b
//    L                                   76     0x004c
//    M                                   77     0x004d
//    N                                   78     0x004e
//    O                                   79     0x004f
//    P                                   80     0x0050
//    Q                                   81     0x0051
//    R                                   82     0x0052
//    S                                   83     0x0053
//    T                                   84     0x0054
//    U                                   85     0x0055
//    V                                   86     0x0056
//    W                                   87     0x0057
//    X                                   88     0x0058
//    Y                                   89     0x0059
//    Z                                   90     0x005a
//    bracketleft                         91     0x005b
//    backslash                           92     0x005c
//    bracketright                        93     0x005d
//    asciicircum                         94     0x005e
//    underscore                          95     0x005f
//    quoteleft                           96     0x0060
//    a                                   97     0x0061
//    b                                   98     0x0062
//    c                                   99     0x0063
//    d                                  100     0x0064
//    e                                  101     0x0065
//    f                                  102     0x0066
//    g                                  103     0x0067
//    h                                  104     0x0068
//    i                                  105     0x0069
//    j                                  106     0x006a
//    k                                  107     0x006b
//    l                                  108     0x006c
//    m                                  109     0x006d
//    n                                  110     0x006e
//    o                                  111     0x006f
//    p                                  112     0x0070
//    q                                  113     0x0071
//    r                                  114     0x0072
//    s                                  115     0x0073
//    t                                  116     0x0074
//    u                                  117     0x0075
//    v                                  118     0x0076
//    w                                  119     0x0077
//    x                                  120     0x0078
//    y                                  121     0x0079
//    z                                  122     0x007a
//    braceleft                          123     0x007b
//    bar                                124     0x007c
//    braceright                         125     0x007d
//    asciitilde                         126     0x007e
//    nobreakspace                       160     0x00a0
//    exclamdown                         161     0x00a1
//    cent                               162     0x00a2
//    sterling                           163     0x00a3
//    currency                           164     0x00a4
//    yen                                165     0x00a5
//    brokenbar                          166     0x00a6
//    section                            167     0x00a7
//    diaeresis                          168     0x00a8
//    copyright                          169     0x00a9
//    ordfeminine                        170     0x00aa
//    guillemotleft                      171     0x00ab
//    notsign                            172     0x00ac
//    hyphen                             173     0x00ad
//    registered                         174     0x00ae
//    macron                             175     0x00af
//    degree                             176     0x00b0
//    plusminus                          177     0x00b1
//    twosuperior                        178     0x00b2
//    threesuperior                      179     0x00b3
//    acute                              180     0x00b4
//    mu                                 181     0x00b5
//    paragraph                          182     0x00b6
//    periodcentered                     183     0x00b7
//    cedilla                            184     0x00b8
//    onesuperior                        185     0x00b9
//    masculine                          186     0x00ba
//    guillemotright                     187     0x00bb
//    onequarter                         188     0x00bc
//    onehalf                            189     0x00bd
//    threequarters                      190     0x00be
//    questiondown                       191     0x00bf
//    Agrave                             192     0x00c0
//    Aacute                             193     0x00c1
//    Acircumflex                        194     0x00c2
//    Atilde                             195     0x00c3
//    Adiaeresis                         196     0x00c4
//    Aring                              197     0x00c5
//    AE                                 198     0x00c6
//    Ccedilla                           199     0x00c7
//    Egrave                             200     0x00c8
//    Eacute                             201     0x00c9
//    Ecircumflex                        202     0x00ca
//    Ediaeresis                         203     0x00cb
//    Igrave                             204     0x00cc
//    Iacute                             205     0x00cd
//    Icircumflex                        206     0x00ce
//    Idiaeresis                         207     0x00cf
//    Eth                                208     0x00d0
//    Ntilde                             209     0x00d1
//    Ograve                             210     0x00d2
//    Oacute                             211     0x00d3
//    Ocircumflex                        212     0x00d4
//    Otilde                             213     0x00d5
//    Odiaeresis                         214     0x00d6
//    multiply                           215     0x00d7
//    Ooblique                           216     0x00d8
//    Ugrave                             217     0x00d9
//    Uacute                             218     0x00da
//    Ucircumflex                        219     0x00db
//    Udiaeresis                         220     0x00dc
//    Yacute                             221     0x00dd
//    Thorn                              222     0x00de
//    ssharp                             223     0x00df
//    agrave                             224     0x00e0
//    aacute                             225     0x00e1
//    acircumflex                        226     0x00e2
//    atilde                             227     0x00e3
//    adiaeresis                         228     0x00e4
//    aring                              229     0x00e5
//    ae                                 230     0x00e6
//    ccedilla                           231     0x00e7
//    egrave                             232     0x00e8
//    eacute                             233     0x00e9
//    ecircumflex                        234     0x00ea
//    ediaeresis                         235     0x00eb
//    igrave                             236     0x00ec
//    iacute                             237     0x00ed
//    icircumflex                        238     0x00ee
//    idiaeresis                         239     0x00ef
//    eth                                240     0x00f0
//    ntilde                             241     0x00f1
//    ograve                             242     0x00f2
//    oacute                             243     0x00f3
//    ocircumflex                        244     0x00f4
//    otilde                             245     0x00f5
//    odiaeresis                         246     0x00f6
//    division                           247     0x00f7
//    oslash                             248     0x00f8
//    ugrave                             249     0x00f9
//    uacute                             250     0x00fa
//    ucircumflex                        251     0x00fb
//    udiaeresis                         252     0x00fc
//    yacute                             253     0x00fd
//    thorn                              254     0x00fe
//    ydiaeresis                         255     0x00ff
//    Aogonek                            417     0x01a1
//    breve                              418     0x01a2
//    Lstroke                            419     0x01a3
//    Lcaron                             421     0x01a5
//    Sacute                             422     0x01a6
//    Scaron                             425     0x01a9
//    Scedilla                           426     0x01aa
//    Tcaron                             427     0x01ab
//    Zacute                             428     0x01ac
//    Zcaron                             430     0x01ae
//    Zabovedot                          431     0x01af
//    aogonek                            433     0x01b1
//    ogonek                             434     0x01b2
//    lstroke                            435     0x01b3
//    lcaron                             437     0x01b5
//    sacute                             438     0x01b6
//    caron                              439     0x01b7
//    scaron                             441     0x01b9
//    scedilla                           442     0x01ba
//    tcaron                             443     0x01bb
//    zacute                             444     0x01bc
//    doubleacute                        445     0x01bd
//    zcaron                             446     0x01be
//    zabovedot                          447     0x01bf
//    Racute                             448     0x01c0
//    Abreve                             451     0x01c3
//    Cacute                             454     0x01c6
//    Ccaron                             456     0x01c8
//    Eogonek                            458     0x01ca
//    Ecaron                             460     0x01cc
//    Dcaron                             463     0x01cf
//    Nacute                             465     0x01d1
//    Ncaron                             466     0x01d2
//    Odoubleacute                       469     0x01d5
//    Rcaron                             472     0x01d8
//    Uring                              473     0x01d9
//    Udoubleacute                       475     0x01db
//    Tcedilla                           478     0x01de
//    racute                             480     0x01e0
//    abreve                             483     0x01e3
//    cacute                             486     0x01e6
//    ccaron                             488     0x01e8
//    eogonek                            490     0x01ea
//    ecaron                             492     0x01ec
//    dcaron                             495     0x01ef
//    nacute                             497     0x01f1
//    ncaron                             498     0x01f2
//    odoubleacute                       501     0x01f5
//    rcaron                             504     0x01f8
//    uring                              505     0x01f9
//    udoubleacute                       507     0x01fb
//    tcedilla                           510     0x01fe
//    abovedot                           511     0x01ff
//    Hstroke                            673     0x02a1
//    Hcircumflex                        678     0x02a6
//    Iabovedot                          681     0x02a9
//    Gbreve                             683     0x02ab
//    Jcircumflex                        684     0x02ac
//    hstroke                            689     0x02b1
//    hcircumflex                        694     0x02b6
//    idotless                           697     0x02b9
//    gbreve                             699     0x02bb
//    jcircumflex                        700     0x02bc
//    Cabovedot                          709     0x02c5
//    Ccircumflex                        710     0x02c6
//    Gabovedot                          725     0x02d5
//    Gcircumflex                        728     0x02d8
//    Ubreve                             733     0x02dd
//    Scircumflex                        734     0x02de
//    cabovedot                          741     0x02e5
//    ccircumflex                        742     0x02e6
//    gabovedot                          757     0x02f5
//    gcircumflex                        760     0x02f8
//    ubreve                             765     0x02fd
//    scircumflex                        766     0x02fe
//    kappa                              930     0x03a2
//    Rcedilla                           931     0x03a3
//    Itilde                             933     0x03a5
//    Lcedilla                           934     0x03a6
//    Emacron                            938     0x03aa
//    Gcedilla                           939     0x03ab
//    Tslash                             940     0x03ac
//    rcedilla                           947     0x03b3
//    itilde                             949     0x03b5
//    lcedilla                           950     0x03b6
//    emacron                            954     0x03ba
//    gacute                             955     0x03bb
//    tslash                             956     0x03bc
//    ENG                                957     0x03bd
//    eng                                959     0x03bf
//    Amacron                            960     0x03c0
//    Iogonek                            967     0x03c7
//    Eabovedot                          972     0x03cc
//    Imacron                            975     0x03cf
//    Ncedilla                           977     0x03d1
//    Omacron                            978     0x03d2
//    Kcedilla                           979     0x03d3
//    Uogonek                            985     0x03d9
//    Utilde                             989     0x03dd
//    Umacron                            990     0x03de
//    amacron                            992     0x03e0
//    iogonek                            999     0x03e7
//    eabovedot                         1004     0x03ec
//    imacron                           1007     0x03ef
//    ncedilla                          1009     0x03f1
//    omacron                           1010     0x03f2
//    kcedilla                          1011     0x03f3
//    uogonek                           1017     0x03f9
//    utilde                            1021     0x03fd
//    umacron                           1022     0x03fe
//    overline                          1150     0x047e
//    kana_fullstop                     1185     0x04a1
//    kana_openingbracket               1186     0x04a2
//    kana_closingbracket               1187     0x04a3
//    kana_comma                        1188     0x04a4
//    kana_middledot                    1189     0x04a5
//    kana_WO                           1190     0x04a6
//    kana_a                            1191     0x04a7
//    kana_i                            1192     0x04a8
//    kana_u                            1193     0x04a9
//    kana_e                            1194     0x04aa
//    kana_o                            1195     0x04ab
//    kana_ya                           1196     0x04ac
//    kana_yu                           1197     0x04ad
//    kana_yo                           1198     0x04ae
//    kana_tu                           1199     0x04af
//    prolongedsound                    1200     0x04b0
//    kana_A                            1201     0x04b1
//    kana_I                            1202     0x04b2
//    kana_U                            1203     0x04b3
//    kana_E                            1204     0x04b4
//    kana_O                            1205     0x04b5
//    kana_KA                           1206     0x04b6
//    kana_KI                           1207     0x04b7
//    kana_KU                           1208     0x04b8
//    kana_KE                           1209     0x04b9
//    kana_KO                           1210     0x04ba
//    kana_SA                           1211     0x04bb
//    kana_SHI                          1212     0x04bc
//    kana_SU                           1213     0x04bd
//    kana_SE                           1214     0x04be
//    kana_SO                           1215     0x04bf
//    kana_TA                           1216     0x04c0
//    kana_TI                           1217     0x04c1
//    kana_TU                           1218     0x04c2
//    kana_TE                           1219     0x04c3
//    kana_TO                           1220     0x04c4
//    kana_NA                           1221     0x04c5
//    kana_NI                           1222     0x04c6
//    kana_NU                           1223     0x04c7
//    kana_NE                           1224     0x04c8
//    kana_NO                           1225     0x04c9
//    kana_HA                           1226     0x04ca
//    kana_HI                           1227     0x04cb
//    kana_HU                           1228     0x04cc
//    kana_HE                           1229     0x04cd
//    kana_HO                           1230     0x04ce
//    kana_MA                           1231     0x04cf
//    kana_MI                           1232     0x04d0
//    kana_MU                           1233     0x04d1
//    kana_ME                           1234     0x04d2
//    kana_MO                           1235     0x04d3
//    kana_YA                           1236     0x04d4
//    kana_YU                           1237     0x04d5
//    kana_YO                           1238     0x04d6
//    kana_RA                           1239     0x04d7
//    kana_RI                           1240     0x04d8
//    kana_RU                           1241     0x04d9
//    kana_RE                           1242     0x04da
//    kana_RO                           1243     0x04db
//    kana_WA                           1244     0x04dc
//    kana_N                            1245     0x04dd
//    voicedsound                       1246     0x04de
//    semivoicedsound                   1247     0x04df
//    Arabic_comma                      1452     0x05ac
//    Arabic_semicolon                  1467     0x05bb
//    Arabic_question_mark              1471     0x05bf
//    Arabic_hamza                      1473     0x05c1
//    Arabic_maddaonalef                1474     0x05c2
//    Arabic_hamzaonalef                1475     0x05c3
//    Arabic_hamzaonwaw                 1476     0x05c4
//    Arabic_hamzaunderalef             1477     0x05c5
//    Arabic_hamzaonyeh                 1478     0x05c6
//    Arabic_alef                       1479     0x05c7
//    Arabic_beh                        1480     0x05c8
//    Arabic_tehmarbuta                 1481     0x05c9
//    Arabic_teh                        1482     0x05ca
//    Arabic_theh                       1483     0x05cb
//    Arabic_jeem                       1484     0x05cc
//    Arabic_hah                        1485     0x05cd
//    Arabic_khah                       1486     0x05ce
//    Arabic_dal                        1487     0x05cf
//    Arabic_thal                       1488     0x05d0
//    Arabic_ra                         1489     0x05d1
//    Arabic_zain                       1490     0x05d2
//    Arabic_seen                       1491     0x05d3
//    Arabic_sheen                      1492     0x05d4
//    Arabic_sad                        1493     0x05d5
//    Arabic_dad                        1494     0x05d6
//    Arabic_tah                        1495     0x05d7
//    Arabic_zah                        1496     0x05d8
//    Arabic_ain                        1497     0x05d9
//    Arabic_ghain                      1498     0x05da
//    Arabic_tatweel                    1504     0x05e0
//    Arabic_feh                        1505     0x05e1
//    Arabic_qaf                        1506     0x05e2
//    Arabic_kaf                        1507     0x05e3
//    Arabic_lam                        1508     0x05e4
//    Arabic_meem                       1509     0x05e5

//    Arabic_noon                       1510     0x05e6
//    Arabic_heh                        1511     0x05e7
//    Arabic_waw                        1512     0x05e8
//    Arabic_alefmaksura                1513     0x05e9
//    Arabic_yeh                        1514     0x05ea
//    Arabic_fathatan                   1515     0x05eb
//    Arabic_dammatan                   1516     0x05ec
//    Arabic_kasratan                   1517     0x05ed
//    Arabic_fatha                      1518     0x05ee
//    Arabic_damma                      1519     0x05ef
//    Arabic_kasra                      1520     0x05f0
//    Arabic_shadda                     1521     0x05f1
//    Arabic_sukun                      1522     0x05f2
//    Serbian_dje                       1697     0x06a1
//    Macedonia_gje                     1698     0x06a2
//    Cyrillic_io                       1699     0x06a3
//    Ukranian_je                       1700     0x06a4
//    Macedonia_dse                     1701     0x06a5
//    Ukranian_i                        1702     0x06a6
//    Ukranian_yi                       1703     0x06a7
//    Serbian_je                        1704     0x06a8
//    Serbian_lje                       1705     0x06a9
//    Serbian_nje                       1706     0x06aa
//    Serbian_tshe                      1707     0x06ab
//    Macedonia_kje                     1708     0x06ac
//    Byelorussian_shortu               1710     0x06ae
//    Serbian_dze                       1711     0x06af
//    numerosign                        1712     0x06b0
//    Serbian_DJE                       1713     0x06b1
//    Macedonia_GJE                     1714     0x06b2
//    Cyrillic_IO                       1715     0x06b3
//    Ukranian_JE                       1716     0x06b4
//    Macedonia_DSE                     1717     0x06b5
//    Ukranian_I                        1718     0x06b6
//    Ukranian_YI                       1719     0x06b7
//    Serbian_JE                        1720     0x06b8
//    Serbian_LJE                       1721     0x06b9
//    Serbian_NJE                       1722     0x06ba
//    Serbian_TSHE                      1723     0x06bb
//    Macedonia_KJE                     1724     0x06bc
//    Byelorussian_SHORTU               1726     0x06be
//    Serbian_DZE                       1727     0x06bf
//    Cyrillic_yu                       1728     0x06c0
//    Cyrillic_a                        1729     0x06c1
//    Cyrillic_be                       1730     0x06c2
//    Cyrillic_tse                      1731     0x06c3
//    Cyrillic_de                       1732     0x06c4
//    Cyrillic_ie                       1733     0x06c5
//    Cyrillic_ef                       1734     0x06c6
//    Cyrillic_ghe                      1735     0x06c7
//    Cyrillic_ha                       1736     0x06c8
//    Cyrillic_i                        1737     0x06c9
//    Cyrillic_shorti                   1738     0x06ca
//    Cyrillic_ka                       1739     0x06cb
//    Cyrillic_el                       1740     0x06cc
//    Cyrillic_em                       1741     0x06cd
//    Cyrillic_en                       1742     0x06ce
//    Cyrillic_o                        1743     0x06cf
//    Cyrillic_pe                       1744     0x06d0
//    Cyrillic_ya                       1745     0x06d17f
//    Cyrillic_er                       1746     0x06d2
//    Cyrillic_es                       1747     0x06d3
//    Cyrillic_te                       1748     0x06d4
//    Cyrillic_u                        1749     0x06d5
//    Cyrillic_zhe                      1750     0x06d6
//    Cyrillic_ve                       1751     0x06d7
//    Cyrillic_softsign                 1752     0x06d8
//    Cyrillic_yeru                     1753     0x06d9
//    Cyrillic_ze                       1754     0x06da
//    Cyrillic_sha                      1755     0x06db
//    Cyrillic_e                        1756     0x06dc
//    Cyrillic_shcha                    1757     0x06dd
//    Cyrillic_che                      1758     0x06de
//    Cyrillic_hardsign                 1759     0x06df
//    Cyrillic_YU                       1760     0x06e0
//    Cyrillic_A                        1761     0x06e1
//    Cyrillic_BE                       1762     0x06e2
//    Cyrillic_TSE                      1763     0x06e3
//    Cyrillic_DE                       1764     0x06e4
//    Cyrillic_IE                       1765     0x06e5
//    Cyrillic_EF                       1766     0x06e6
//    Cyrillic_GHE                      1767     0x06e7
//    Cyrillic_HA                       1768     0x06e8
//    Cyrillic_I                        1769     0x06e9
//    Cyrillic_SHORTI                   1770     0x06ea
//    Cyrillic_KA                       1771     0x06eb
//    Cyrillic_EL                       1772     0x06ec
//    Cyrillic_EM                       1773     0x06ed
//    Cyrillic_EN                       1774     0x06ee
//    Cyrillic_O                        1775     0x06ef
//    Cyrillic_PE                       1776     0x06f0
//    Cyrillic_YA                       1777     0x06f1
//    Cyrillic_ER                       1778     0x06f2
//    Cyrillic_ES                       1779     0x06f3
//    Cyrillic_TE                       1780     0x06f4
//    Cyrillic_U                        1781     0x06f5
//    Cyrillic_ZHE                      1782     0x06f6
//    Cyrillic_VE                       1783     0x06f7
//    Cyrillic_SOFTSIGN                 1784     0x06f8
//    Cyrillic_YERU                     1785     0x06f9
//    Cyrillic_ZE                       1786     0x06fa
//    Cyrillic_SHA                      1787     0x06fb
//    Cyrillic_E                        1788     0x06fc
//    Cyrillic_SHCHA                    1789     0x06fd
//    Cyrillic_CHE                      1790     0x06fe
//    Cyrillic_HARDSIGN                 1791     0x06ff
//    Greek_ALPHAaccent                 1953     0x07a1
//    Greek_EPSILONaccent               1954     0x07a2
//    Greek_ETAaccent                   1955     0x07a3
//    Greek_IOTAaccent                  1956     0x07a4
//    Greek_IOTAdiaeresis               1957     0x07a5
//    Greek_IOTAaccentdiaeresis         1958     0x07a6
//    Greek_OMICRONaccent               1959     0x07a7
//    Greek_UPSILONaccent               1960     0x07a8
//    Greek_UPSILONdieresis             1961     0x07a9
//    Greek_UPSILONaccentdieresis       1962     0x07aa
//    Greek_OMEGAaccent                 1963     0x07ab
//    Greek_alphaaccent                 1969     0x07b1
//    Greek_epsilonaccent               1970     0x07b2
//    Greek_etaaccent                   1971     0x07b3
//    Greek_iotaaccent                  1972     0x07b4
//    Greek_iotadieresis                1973     0x07b5
//    Greek_iotaaccentdieresis          1974     0x07b6
//    Greek_omicronaccent               1975     0x07b77f
//    Greek_upsilonaccent               1976     0x07b8
//    Greek_upsilondieresis             1977     0x07b9
//    Greek_upsilonaccentdieresis       1978     0x07ba
//    Greek_omegaaccent                 1979     0x07bb
//    Greek_ALPHA                       1985     0x07c1
//    Greek_BETA                        1986     0x07c2
//    Greek_GAMMA                       1987     0x07c3
//    Greek_DELTA                       1988     0x07c4
//    Greek_EPSILON                     1989     0x07c5
//    Greek_ZETA                        1990     0x07c6
//    Greek_ETA                         1991     0x07c7
//    Greek_THETA                       1992     0x07c8
//    Greek_IOTA                        1993     0x07c9
//    Greek_KAPPA                       1994     0x07ca
//    Greek_LAMBDA                      1995     0x07cb
//    Greek_MU                          1996     0x07cc
//    Greek_NU                          1997     0x07cd
//    Greek_XI                          1998     0x07ce
//    Greek_OMICRON                     1999     0x07cf
//    Greek_PI                          2000     0x07d0
//    Greek_RHO                         2001     0x07d1
//    Greek_SIGMA                       2002     0x07d2
//    Greek_TAU                         2004     0x07d4
//    Greek_UPSILON                     2005     0x07d5
//    Greek_PHI                         2006     0x07d6
//    Greek_CHI                         2007     0x07d7
//    Greek_PSI                         2008     0x07d8
//    Greek_OMEGA                       2009     0x07d9
//    Greek_alpha                       2017     0x07e1
//    Greek_beta                        2018     0x07e2
//    Greek_gamma                       2019     0x07e3
//    Greek_delta                       2020     0x07e4
//    Greek_epsilon                     2021     0x07e5
//    Greek_zeta                        2022     0x07e6
//    Greek_eta                         2023     0x07e7
//    Greek_theta                       2024     0x07e8
//    Greek_iota                        2025     0x07e9
//    Greek_kappa                       2026     0x07ea
//    Greek_lambda                      2027     0x07eb
//    Greek_mu                          2028     0x07ec
//    Greek_nu                          2029     0x07ed
//    Greek_xi                          2030     0x07ee
//    Greek_omicron                     2031     0x07ef
//    Greek_pi                          2032     0x07f0
//    Greek_rho                         2033     0x07f1
//    Greek_sigma                       2034     0x07f2
//    Greek_finalsmallsigma             2035     0x07f3
//    Greek_tau                         2036     0x07f4
//    Greek_upsilon                     2037     0x07f5
//    Greek_phi                         2038     0x07f6
//    Greek_chi                         2039     0x07f7
//    Greek_psi                         2040     0x07f8
//    Greek_omega                       2041     0x07f9
//    leftradical                       2209     0x08a1
//    topleftradical                    2210     0x08a2
//    horizconnector                    2211     0x08a3
//    topintegral                       2212     0x08a4
//    botintegral                       2213     0x08a5
//    vertconnector                     2214     0x08a6
//    topleftsqbracket                  2215     0x08a7
//    botleftsqbracket                  2216     0x08a8
//    toprightsqbracket                 2217     0x08a9
//    botrightsqbracket                 2218     0x08aa
//    topleftparens                     2219     0x08ab
//    botleftparens                     2220     0x08ac
//    toprightparens                    2221     0x08ad
//    botrightparens                    2222     0x08ae
//    leftmiddlecurlybrace              2223     0x08af
//    rightmiddlecurlybrace             2224     0x08b0
//    topleftsummation                  2225     0x08b1
//    botleftsummation                  2226     0x08b2
//    topvertsummationconnector         2227     0x08b3
//    botvertsummationconnector         2228     0x08b4
//    toprightsummation                 2229     0x08b5
//    botrightsummation                 2230     0x08b6
//    rightmiddlesummation              2231     0x08b7

//    lessthanequal                     2236     0x08bc
//    notequal                          2237     0x08bd
//    greaterthanequal                  2238     0x08be
//    integral                          2239     0x08bf
//    therefore                         2240     0x08c0
//    variation                         2241     0x08c1
//    infinity                          2242     0x08c2
//    nabla                             2245     0x08c5
//    approximate                       2248     0x08c8
//    similarequal                      2249     0x08c9
//    ifonlyif                          2253     0x08cd
//    implies                           2254     0x08ce
//    identical                         2255     0x08cf
//    radical                           2262     0x08d6
//    include "in                        2266     0x08da
//    include "                          2267     0x08db
//    intersection                      2268     0x08dc
//    union                             2269     0x08dd
//    logicaland                        2270     0x08de
//    logicalor                         2271     0x08df
//    partialderivative                 2287     0x08ef
//    function                          2294     0x08f6
//    leftarrow                         2299     0x08fb
//    uparrow                           2300     0x08fc
//    rightarrow                        2301     0x08fd
//    downarrow                         2302     0x08fe
//    blank                             2527     0x09df
//    soliddiamond                      2528     0x09e0
//    checkerboard                      2529     0x09e1
//    ht                                2530     0x09e2
//    ff                                2531     0x09e3
//    cr                                2532     0x09e4
//    lf                                2533     0x09e5
//    nl                                2536     0x09e8
//    vt                                2537     0x09e9
//    lowrightcorner                    2538     0x09ea
//    uprightcorner                     2539     0x09eb
//    upleftcorner                      2540     0x09ec
//    lowleftcorner                     2541     0x09ed
//    crossinglines                     2542     0x09ee
//    horizlinescan1                    2543     0x09ef
//    horizlinescan3                    2544     0x09f0
//    horizlinescan5                    2545     0x09f1
//    horizlinescan7                    2546     0x09f2
//    horizlinescan9                    2547     0x09f3
//    leftt                             2548     0x09f4
//    rightt                            2549     0x09f5
//    bott                              2550     0x09f6
//    topt                              2551     0x09f7
//    vertbar                           2552     0x09f8
//    emspace                           2721     0x0aa1
//    enspace                           2722     0x0aa2
//    em3space                          2723     0x0aa3
//    em4space                          2724     0x0aa4
//    digitspace                        2725     0x0aa5
//    punctspace                        2726     0x0aa6
//    thinspace                         2727     0x0aa7
//    hairspace                         2728     0x0aa8
//    emdash                            2729     0x0aa9
//    endash                            2730     0x0aaa
//    signifblank                       2732     0x0aac
//    ellipsis                          2734     0x0aae
//    doubbaselinedot                   2735     0x0aaf
//    onethird                          2736     0x0ab0
//    twothirds                         2737     0x0ab1
//    onefifth                          2738     0x0ab2
//    twofifths                         2739     0x0ab3
//    threefifths                       2740     0x0ab4
//    fourfifths                        2741     0x0ab5
//    onesixth                          2742     0x0ab6
//    fivesixths                        2743     0x0ab7
//    careof                            2744     0x0ab8
//    figdash                           2747     0x0abb
//    leftanglebracket                  2748     0x0abc
//    decimalpoint                      2749     0x0abd
//    rightanglebracket                 2750     0x0abe
//    marker                            2751     0x0abf
//    oneeighth                         2755     0x0ac3
//    threeeighths                      2756     0x0ac4
//    fiveeighths                       2757     0x0ac5
//    seveneighths                      2758     0x0ac6
//    trademark                         2761     0x0ac9
//    signaturemark                     2762     0x0aca
//    trademarkincircle                 2763     0x0acb
//    leftopentriangle                  2764     0x0acc
//    rightopentriangle                 2765     0x0acd
//    emopencircle                      2766     0x0ace
//    emopenrectangle                   2767     0x0acf
//    leftsinglequotemark               2768     0x0ad0
//    rightsinglequotemark              2769     0x0ad1
//    leftdoublequotemark               2770     0x0ad2
//    rightdoublequotemark              2771     0x0ad3
//    prescription                      2772     0x0ad4
//    minutes                           2774     0x0ad6
//    seconds                           2775     0x0ad7
//    latincross                        2777     0x0ad9
//    hexagram                          2778     0x0ada
//    filledrectbullet                  2779     0x0adb
//    filledlefttribullet               2780     0x0adc
//    filledrighttribullet              2781     0x0add
//    emfilledcircle                    2782     0x0ade
//    emfilledrect                      2783     0x0adf
//    enopencircbullet                  2784     0x0ae0
//    enopensquarebullet                2785     0x0ae1
//    openrectbullet                    2786     0x0ae2
//    opentribulletup                   2787     0x0ae3
//    opentribulletdown                 2788     0x0ae4
//    openstar                          2789     0x0ae5
//    enfilledcircbullet                2790     0x0ae6
//    enfilledsqbullet                  2791     0x0ae7
//    filledtribulletup                 2792     0x0ae8
//    filledtribulletdown               2793     0x0ae9
//    leftpointer                       2794     0x0aea
//    rightpointer                      2795     0x0aeb
//    club                              2796     0x0aec
//    diamond                           2797     0x0aed
//    heart                             2798     0x0aee
//    maltesecross                      2800     0x0af0
//    dagger                            2801     0x0af1
//    doubledagger                      2802     0x0af2
//    checkmark                         2803     0x0af3
//    ballotcross                       2804     0x0af4
//    musicalsharp                      2805     0x0af5
//    musicalflat                       2806     0x0af6
//    malesymbol                        2807     0x0af7
//    femalesymbol                      2808     0x0af8
//    telephone                         2809     0x0af9
//    telephonerecorder                 2810     0x0afa
//    phonographcopyright               2811     0x0afb
//    caret                             2812     0x0afc
//    singlelowquotemark                2813     0x0afd
//    doublelowquotemark                2814     0x0afe
//    cursor                            2815     0x0aff
//    leftcaret                         2979     0x0ba3
//    rightcaret                        2982     0x0ba6
//    downcaret                         2984     0x0ba8
//    upcaret                           2985     0x0ba9
//    overbar                           3008     0x0bc0
//    downtack                          3010     0x0bc2
//    upshoe                            3011     0x0bc3
//    downstile                         3012     0x0bc4
//    underbar                          3014     0x0bc6
//    jot                               3018     0x0bca
//    quad                              3020     0x0bcc
//    uptack                            3022     0x0bce
//    circle                            3023     0x0bcf
//    upstile                           3027     0x0bd3
//    downshoe                          3030     0x0bd6
//    rightshoe                         3032     0x0bd8
//    leftshoe                          3034     0x0bda
//    lefttack                          3036     0x0bdc
//    righttack                         3068     0x0bfc
//    hebrew_aleph                      3296     0x0ce0
//    hebrew_beth                       3297     0x0ce1
//    hebrew_gimmel                     3298     0x0ce2
//    hebrew_daleth                     3299     0x0ce3
//    hebrew_he                         3300     0x0ce4
//    hebrew_waw                        3301     0x0ce5
//    hebrew_zayin                      3302     0x0ce6
//    hebrew_het                        3303     0x0ce7
//    hebrew_teth                       3304     0x0ce8
//    hebrew_yod                        3305     0x0ce9
//    hebrew_finalkaph                  3306     0x0cea
//    hebrew_kaph                       3307     0x0ceb
//    hebrew_lamed                      3308     0x0cec
//    hebrew_finalmem                   3309     0x0ced
//    hebrew_mem                        3310     0x0cee
//    hebrew_finalnun                   3311     0x0cef
//    hebrew_nun                        3312     0x0cf0
//    hebrew_samekh                     3313     0x0cf1
//    hebrew_ayin                       3314     0x0cf2
//    hebrew_finalpe                    3315     0x0cf3
//    hebrew_pe                         3316     0x0cf4
//    hebrew_finalzadi                  3317     0x0cf5
//    hebrew_zadi                       3318     0x0cf6
//    hebrew_kuf                        3319     0x0cf7
//    hebrew_resh                       3320     0x0cf8
//    hebrew_shin                       3321     0x0cf9
//    hebrew_taf                        3322     0x0cfa

struct KeymapSym
{
    enum {
           KBDFLAGS_EXTENDED = 0x0100
         , KBDFLAGS_DOWN     = 0x4000
         , KBDFLAGS_RELEASE  = 0x8000
    };

    enum {
           SCROLLLOCK  = 0x01
         , NUMLOCK     = 0x02
         , CAPSLOCK    = 0x04
         , FLG_SHIFT   = 0x08
         , FLG_CTRL    = 0x10
         , FLG_ALT     = 0x20
         , FLG_WINDOWS = 0x40
         , FLG_ALTGR   = 0x80
    };

    enum {
           LEFT_SHIFT  = 0x36
         , RIGHT_SHIFT = 0x2A
         , LEFT_CTRL   = 0x1D
         , RIGHT_CTRL  = 0x9D
         , LEFT_ALT    = 0x38
         , RIGHT_ALT   = 0xB8
    };



    // keyboard info
    int keys_down[256];  // key states 0 up 1 down (0..127 plain keys, 128..255 extended keys)

    int key_flags;          // scroll_lock = 1, num_lock = 2, caps_lock = 4,
                            // shift = 8, ctrl = 16, Alt = 32,
                            // Windows = 64, AltGr = 128

    enum {
        SIZE_KEYBUF_SYM = 200 // we are sending along many modifiers and the actual number of keys can become large
    };

    enum {
        KEVENT_KEY,
        KEVENT_TAB,
        KEVENT_BACKTAB,
        KEVENT_ENTER,
        KEVENT_ESC,
        KEVENT_DELETE,
        KEVENT_BACKSPACE,
        KEVENT_LEFT_ARROW,
        KEVENT_RIGHT_ARROW,
        KEVENT_UP_ARROW,
        KEVENT_DOWN_ARROW,
        KEVENT_HOME,
        KEVENT_END,
        KEVENT_PGUP,
        KEVENT_PGDOWN
    };

    struct KeySym
    {
        KeySym() : sym(0), down(0) {}

        KeySym(uint32_t sym, uint8_t down) 
            : sym(sym)
            , down(down)
        {
        }
        uint32_t sym;
        uint8_t down;
    };

    uint32_t ibuf_sym; // first free position
    uint32_t nbuf_sym; // number of char in char buffer
    KeySym buffer_sym[SIZE_KEYBUF_SYM]; // actual char buffer

    uint8_t dead_key;

    enum {
        DEADKEY_NONE,
        DEADKEY_CIRC,
        DEADKEY_UML,
        DEADKEY_GRAVE,
        DEADKEY_TILDE
    };

    int keylayout;
    bool is_unix;
    bool is_apple;
    uint32_t verbose;
    int last_sym;

    typedef int KeyLayout_t[128];

    // keylayout working tables (X11 mode : begins in 8e position.)
    KeyLayout_t keylayout_WORK_noshift_sym;
    KeyLayout_t keylayout_WORK_shift_sym;
    KeyLayout_t keylayout_WORK_altgr_sym;
    KeyLayout_t keylayout_WORK_capslock_sym;
    KeyLayout_t keylayout_WORK_shiftcapslock_sym;

    explicit KeymapSym(int keylayout, int key_flags, bool is_unix, bool is_apple, int verbose);

    void synchronize(uint16_t param1);

// The TS_KEYBOARD_EVENT structure is a standard T.128 Keyboard Event (see [T128] section
// 8.18.2). RDP keyboard input is restricted to keyboard scancodes, unlike the code-point or virtual
// codes supported in T.128 (a scancode is an 8-bit value specifying a key location on the keyboard).
// The server accepts a scancode value and translates it into the correct character depending on the
// language locale and keyboard layout used in the session.

// keyboardFlags (2 bytes): A 16-bit, unsigned integer. The flags describing the keyboard event.

// +--------------------------+------------------------------------------------+
// | 0x0100 KBDFLAGS_EXTENDED | The keystroke message contains an extended     |
// |                          | scancode. For enhanced 101-key and 102-key     |
// |                          | keyboards, extended keys include "he right     |
// |                          | ALT and right CTRL keys on the main section    |
// |                          | of the keyboard; the INS, DEL, HOME, END,      |
// |                          | PAGE UP, PAGE DOWN and ARROW keys in the       |
// |                          | clusters to the left of the numeric keypad;    |
// |                          | and the Divide ("/") and ENTER keys in the     |
// |                          | numeric keypad.                                |
// +--------------------------+------------------------------------------------+
// | 0x4000 KBDFLAGS_DOWN     | Indicates that the key was down prior to this  |
// |                          | event.                                         |
// +--------------------------+------------------------------------------------+
// | 0x8000 KBDFLAGS_RELEASE  | The absence of this flag indicates a key-down  |
// |                          | event, while its presence indicates a          |
// |                          | key-release event.                             |
// +--------------------------+------------------------------------------------+

// keyCode (2 bytes): A 16-bit, unsigned integer. The scancode of the key which
// triggered the event.

    void event(int device_flags, long keycode);

    void remove_modifiers();
    void putback_modifiers();
    void key_event(int device_flags, long keycode);
    void apple_keyboard_translation(int device_flags, long keycode);
    KeySym get_key(const uint16_t keyboardFlags, const uint16_t keyCode);

    // Push only sym
    void push_sym(KeySym sym);

    uint32_t get_sym(uint8_t & downflag);

    uint32_t nb_sym_available() const;

    bool is_caps_locked() const;

    bool is_scroll_locked() const;

    bool is_num_locked() const;

    void toggle_num_lock(bool on);

    bool is_left_shift_pressed() const;

    bool is_right_shift_pressed() const;

    bool is_shift_pressed() const;

    bool is_left_ctrl_pressed() const;

    bool is_right_ctrl_pressed() const;

    bool is_ctrl_pressed() const;

    bool is_left_alt_pressed() const;

    // altgr key or ctrl+alt
    bool is_altgr_pressed() const;

    // altgr key
    bool is_right_alt_pressed() const;

    bool is_alt_pressed() const;

    void init_layout_sym(int keyb);

    const KeyLayout_t * select_layout();

}; // STRUCT - KeymapSym