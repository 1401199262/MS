//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft shared
// source or premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license agreement,
// you are not authorized to use this source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the SOURCE.RTF on your install media or the root of your tools installation.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/***
*ctype.c - _ctype definition file
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       _ctype definition file of character classification data.  This file
*       initializes the array used by the character classification macros
*       in ctype.h.
*
*Revision History:
*       06-08-89  PHG   Module created, based on asm version
*       08-28-89  JCR   Corrected _ctype declaration to match ctype.h
*       04-06-90  GJF   Added #include <cruntime.h>. Also, fixed the copyright.
*       10-08-91  ETC   _ctype table is unsigned short under _INTL.
*       11-11-91  ETC   Declare _pctype and _pwctype under _INTL.
*       12-16-91  ETC   Make ctype table width independent of _INTL, use
*                       _NEWCTYPETABLE for short table, else char.
*       04-06-92  KRS   Remove _INTL switches.
*       01-19-03  CFW   Move to _NEWCTYPETABLE, remove switch.
*       04-06-93  SKS   Change _VARTYPE1 to nothing
*       04-11-94  GJF   Made definitions of _p[w]ctype conditional on ndef
*                       DLL_FOR_WIN32S.
*       05-13-99  PML   Remove Win32s
*       09-06-00  GB    Introduced _wctype and made _ctype to const
*       01-29-01  GB    Added _func function version of data variable used in
*                       msvcprt.lib to work with STATIC_CPPLIB
*       11-12-01  GB    Added support for new locale implementation.
*       04-25-02  GB    Removed definition of _ctype from this file, Made a
*                       new asm file _ctypev.asm
*       05-06-02  GB    Replaced _ctype by __newctype.
*
*******************************************************************************/

#include <cruntime.h>
#include <windows.h>
#include <ctype.h>
#include <setlocal.h>
#include <wchar.h>
#include <mtdll.h>

#ifdef _WIN32_WCE
// Moved from fpw32/conv/fpinit.c so that bootcrt can make use of these.
int _fltused = 0x9875;
int _ldused = 0x9873;
#endif

const unsigned short *_pctype = __newctype+128;  /* pointer to table for char's      */
const unsigned short *_pwctype = _wctype+1;    /* pointer to table for wchar_t's   */

_CRTIMP const unsigned short *__cdecl __pwctype_func(void)
{
    return _pwctype;
}

_CRTIMP const unsigned short *__cdecl __pctype_func(void)
{
#if _PTD_LOCALE_SUPPORT
    /*
     * Note that we don't need _LocaleUpdate in this function.
     * The main reason being, that this is a leaf function in
     * locale usage terms.
     */
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
    return ptloci->pctype;
#else
#ifdef _WCE_FULLCRT
    const unsigned short* retval;
    _mlock(_SETLOCALE_LOCK);
    __try 
    {
         retval = __ptlocinfo->pctype;
    }
    __finally
    {
         _munlock(_SETLOCALE_LOCK);
    }
    return retval;
#else
    return _pctype;
#endif
#endif  /* _PTD_LOCALE_SUPPORT */
}

#ifdef _WCE_BOOTCRT
// BOOTCRT will always have __locale_changed = 0 to use
// the US locale.
int __locale_changed = 0;
#endif

const unsigned short _wctype[] = {
    0,                              /* -1 EOF   */
    _CONTROL ,                      /* 00 (NUL) */
    _CONTROL ,                      /* 01 (SOH) */
    _CONTROL ,                      /* 02 (STX) */
    _CONTROL ,                      /* 03 (ETX) */
    _CONTROL ,                      /* 04 (EOT) */
    _CONTROL ,                      /* 05 (ENQ) */
    _CONTROL ,                      /* 06 (ACK) */
    _CONTROL ,                      /* 07 (BEL) */
    _CONTROL ,                      /* 08 (BS)  */
    _SPACE | _CONTROL | _BLANK ,    /* 09 (HT)  */
    _SPACE | _CONTROL ,             /* 0A (LF)  */
    _SPACE | _CONTROL ,             /* 0B (VT)  */
    _SPACE | _CONTROL ,             /* 0C (FF)  */
    _SPACE | _CONTROL ,             /* 0D (CR)  */
    _CONTROL ,                      /* 0E (SI)  */
    _CONTROL ,                      /* 0F (SO)  */
    _CONTROL ,                      /* 10 (DLE) */
    _CONTROL ,                      /* 11 (DC1) */
    _CONTROL ,                      /* 12 (DC2) */
    _CONTROL ,                      /* 13 (DC3) */
    _CONTROL ,                      /* 14 (DC4) */
    _CONTROL ,                      /* 15 (NAK) */
    _CONTROL ,                      /* 16 (SYN) */
    _CONTROL ,                      /* 17 (ETB) */
    _CONTROL ,                      /* 18 (CAN) */
    _CONTROL ,                      /* 19 (EM)  */
    _CONTROL ,                      /* 1A (SUB) */
    _CONTROL ,                      /* 1B (ESC) */
    _CONTROL ,                      /* 1C (FS)  */
    _CONTROL ,                      /* 1D (GS)  */
    _CONTROL ,                      /* 1E (RS)  */
    _CONTROL ,                      /* 1F (US)  */
    _SPACE | _BLANK ,               /* 20 SPACE */
    _PUNCT ,                        /* 21 !     */
    _PUNCT ,                        /* 22 "     */
    _PUNCT ,                        /* 23 #     */
    _PUNCT ,                        /* 24 $     */
    _PUNCT ,                        /* 25 %     */
    _PUNCT ,                        /* 26 &     */
    _PUNCT ,                        /* 27 '     */
    _PUNCT ,                        /* 28 (     */
    _PUNCT ,                        /* 29 )     */
    _PUNCT ,                        /* 2A *     */
    _PUNCT ,                        /* 2B +     */
    _PUNCT ,                        /* 2C ,     */
    _PUNCT ,                        /* 2D -     */
    _PUNCT ,                        /* 2E .     */
    _PUNCT ,                        /* 2F /     */
    _DIGIT | _HEX ,                 /* 30 0     */
    _DIGIT | _HEX ,                 /* 31 1     */
    _DIGIT | _HEX ,                 /* 32 2     */
    _DIGIT | _HEX ,                 /* 33 3     */
    _DIGIT | _HEX ,                 /* 34 4     */
    _DIGIT | _HEX ,                 /* 35 5     */
    _DIGIT | _HEX ,                 /* 36 6     */
    _DIGIT | _HEX ,                 /* 37 7     */
    _DIGIT | _HEX ,                 /* 38 8     */
    _DIGIT | _HEX ,                 /* 39 9     */
    _PUNCT ,                        /* 3A :     */
    _PUNCT ,                        /* 3B ;     */
    _PUNCT ,                        /* 3C <     */
    _PUNCT ,                        /* 3D =     */
    _PUNCT ,                        /* 3E >     */
    _PUNCT ,                        /* 3F ?     */
    _PUNCT ,                        /* 40 @     */
    _UPPER | _HEX | C1_ALPHA ,      /* 41 A     */
    _UPPER | _HEX | C1_ALPHA ,      /* 42 B     */
    _UPPER | _HEX | C1_ALPHA ,      /* 43 C     */
    _UPPER | _HEX | C1_ALPHA ,      /* 44 D     */
    _UPPER | _HEX | C1_ALPHA ,      /* 45 E     */
    _UPPER | _HEX | C1_ALPHA ,      /* 46 F     */
    _UPPER | C1_ALPHA ,             /* 47 G     */
    _UPPER | C1_ALPHA ,             /* 48 H     */
    _UPPER | C1_ALPHA ,             /* 49 I     */
    _UPPER | C1_ALPHA ,             /* 4A J     */
    _UPPER | C1_ALPHA ,             /* 4B K     */
    _UPPER | C1_ALPHA ,             /* 4C L     */
    _UPPER | C1_ALPHA ,             /* 4D M     */
    _UPPER | C1_ALPHA ,             /* 4E N     */
    _UPPER | C1_ALPHA ,             /* 4F O     */
    _UPPER | C1_ALPHA ,             /* 50 P     */
    _UPPER | C1_ALPHA ,             /* 51 Q     */
    _UPPER | C1_ALPHA ,             /* 52 R     */
    _UPPER | C1_ALPHA ,             /* 53 S     */
    _UPPER | C1_ALPHA ,             /* 54 T     */
    _UPPER | C1_ALPHA ,             /* 55 U     */
    _UPPER | C1_ALPHA ,             /* 56 V     */
    _UPPER | C1_ALPHA ,             /* 57 W     */
    _UPPER | C1_ALPHA ,             /* 58 X     */
    _UPPER | C1_ALPHA ,             /* 59 Y     */
    _UPPER | C1_ALPHA ,             /* 5A Z     */
    _PUNCT ,                        /* 5B [     */
    _PUNCT ,                        /* 5C \     */
    _PUNCT ,                        /* 5D ]     */
    _PUNCT ,                        /* 5E ^     */
    _PUNCT ,                        /* 5F _     */
    _PUNCT ,                        /* 60 `     */
    _LOWER | _HEX | C1_ALPHA ,      /* 61 a     */
    _LOWER | _HEX | C1_ALPHA ,      /* 62 b     */
    _LOWER | _HEX | C1_ALPHA ,      /* 63 c     */
    _LOWER | _HEX | C1_ALPHA ,      /* 64 d     */
    _LOWER | _HEX | C1_ALPHA ,      /* 65 e     */
    _LOWER | _HEX | C1_ALPHA ,      /* 66 f     */
    _LOWER | C1_ALPHA ,             /* 67 g     */
    _LOWER | C1_ALPHA ,             /* 68 h     */
    _LOWER | C1_ALPHA ,             /* 69 i     */
    _LOWER | C1_ALPHA ,             /* 6A j     */
    _LOWER | C1_ALPHA ,             /* 6B k     */
    _LOWER | C1_ALPHA ,             /* 6C l     */
    _LOWER | C1_ALPHA ,             /* 6D m     */
    _LOWER | C1_ALPHA ,             /* 6E n     */
    _LOWER | C1_ALPHA ,             /* 6F o     */
    _LOWER | C1_ALPHA ,             /* 70 p     */
    _LOWER | C1_ALPHA ,             /* 71 q     */
    _LOWER | C1_ALPHA ,             /* 72 r     */
    _LOWER | C1_ALPHA ,             /* 73 s     */
    _LOWER | C1_ALPHA ,             /* 74 t     */
    _LOWER | C1_ALPHA ,             /* 75 u     */
    _LOWER | C1_ALPHA ,             /* 76 v     */
    _LOWER | C1_ALPHA ,             /* 77 w     */
    _LOWER | C1_ALPHA ,             /* 78 x     */
    _LOWER | C1_ALPHA ,             /* 79 y     */
    _LOWER | C1_ALPHA ,             /* 7A z     */
    _PUNCT ,                        /* 7B {     */
    _PUNCT ,                        /* 7C |     */
    _PUNCT ,                        /* 7D }     */
    _PUNCT ,                        /* 7E ~     */
    _CONTROL ,                      /* 7F (DEL) */
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _CONTROL ,
    _SPACE | _BLANK ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _DIGIT | _PUNCT ,
    _DIGIT | _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _DIGIT | _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _PUNCT ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _PUNCT ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _UPPER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _PUNCT ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _LOWER | C1_ALPHA ,
    _UPPER | C1_ALPHA
};
const unsigned short __newctype[384] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 
        0,                      /* -1 EOF   */
        _CONTROL,               /* 00 (NUL) */
        _CONTROL,               /* 01 (SOH) */
        _CONTROL,               /* 02 (STX) */
        _CONTROL,               /* 03 (ETX) */
        _CONTROL,               /* 04 (EOT) */
        _CONTROL,               /* 05 (ENQ) */
        _CONTROL,               /* 06 (ACK) */
        _CONTROL,               /* 07 (BEL) */
        _CONTROL,               /* 08 (BS)  */
        _SPACE+_CONTROL,        /* 09 (HT)  */
        _SPACE+_CONTROL,        /* 0A (LF)  */
        _SPACE+_CONTROL,        /* 0B (VT)  */
        _SPACE+_CONTROL,        /* 0C (FF)  */
        _SPACE+_CONTROL,        /* 0D (CR)  */
        _CONTROL,               /* 0E (SI)  */
        _CONTROL,               /* 0F (SO)  */
        _CONTROL,               /* 10 (DLE) */
        _CONTROL,               /* 11 (DC1) */
        _CONTROL,               /* 12 (DC2) */
        _CONTROL,               /* 13 (DC3) */
        _CONTROL,               /* 14 (DC4) */
        _CONTROL,               /* 15 (NAK) */
        _CONTROL,               /* 16 (SYN) */
        _CONTROL,               /* 17 (ETB) */
        _CONTROL,               /* 18 (CAN) */
        _CONTROL,               /* 19 (EM)  */
        _CONTROL,               /* 1A (SUB) */
        _CONTROL,               /* 1B (ESC) */
        _CONTROL,               /* 1C (FS)  */
        _CONTROL,               /* 1D (GS)  */
        _CONTROL,               /* 1E (RS)  */
        _CONTROL,               /* 1F (US)  */
        _SPACE+_BLANK,          /* 20 SPACE */
        _PUNCT,                 /* 21 !     */
        _PUNCT,                 /* 22 "     */
        _PUNCT,                 /* 23 #     */
        _PUNCT,                 /* 24 $     */
        _PUNCT,                 /* 25 %     */
        _PUNCT,                 /* 26 &     */
        _PUNCT,                 /* 27 '     */
        _PUNCT,                 /* 28 (     */
        _PUNCT,                 /* 29 )     */
        _PUNCT,                 /* 2A *     */
        _PUNCT,                 /* 2B +     */
        _PUNCT,                 /* 2C ,     */
        _PUNCT,                 /* 2D -     */
        _PUNCT,                 /* 2E .     */
        _PUNCT,                 /* 2F /     */
        _DIGIT+_HEX,            /* 30 0     */
        _DIGIT+_HEX,            /* 31 1     */
        _DIGIT+_HEX,            /* 32 2     */
        _DIGIT+_HEX,            /* 33 3     */
        _DIGIT+_HEX,            /* 34 4     */
        _DIGIT+_HEX,            /* 35 5     */
        _DIGIT+_HEX,            /* 36 6     */
        _DIGIT+_HEX,            /* 37 7     */
        _DIGIT+_HEX,            /* 38 8     */
        _DIGIT+_HEX,            /* 39 9     */
        _PUNCT,                 /* 3A :     */
        _PUNCT,                 /* 3B ;     */
        _PUNCT,                 /* 3C <     */
        _PUNCT,                 /* 3D =     */
        _PUNCT,                 /* 3E >     */
        _PUNCT,                 /* 3F ?     */
        _PUNCT,                 /* 40 @     */
        _UPPER+_HEX,            /* 41 A     */
        _UPPER+_HEX,            /* 42 B     */
        _UPPER+_HEX,            /* 43 C     */
        _UPPER+_HEX,            /* 44 D     */
        _UPPER+_HEX,            /* 45 E     */
        _UPPER+_HEX,            /* 46 F     */
        _UPPER,                 /* 47 G     */
        _UPPER,                 /* 48 H     */
        _UPPER,                 /* 49 I     */
        _UPPER,                 /* 4A J     */
        _UPPER,                 /* 4B K     */
        _UPPER,                 /* 4C L     */
        _UPPER,                 /* 4D M     */
        _UPPER,                 /* 4E N     */
        _UPPER,                 /* 4F O     */
        _UPPER,                 /* 50 P     */
        _UPPER,                 /* 51 Q     */
        _UPPER,                 /* 52 R     */
        _UPPER,                 /* 53 S     */
        _UPPER,                 /* 54 T     */
        _UPPER,                 /* 55 U     */
        _UPPER,                 /* 56 V     */
        _UPPER,                 /* 57 W     */
        _UPPER,                 /* 58 X     */
        _UPPER,                 /* 59 Y     */
        _UPPER,                 /* 5A Z     */
        _PUNCT,                 /* 5B [     */
        _PUNCT,                 /* 5C \     */
        _PUNCT,                 /* 5D ]     */
        _PUNCT,                 /* 5E ^     */
        _PUNCT,                 /* 5F _     */
        _PUNCT,                 /* 60 `     */
        _LOWER+_HEX,            /* 61 a     */
        _LOWER+_HEX,            /* 62 b     */
        _LOWER+_HEX,            /* 63 c     */
        _LOWER+_HEX,            /* 64 d     */
        _LOWER+_HEX,            /* 65 e     */
        _LOWER+_HEX,            /* 66 f     */
        _LOWER,                 /* 67 g     */
        _LOWER,                 /* 68 h     */
        _LOWER,                 /* 69 i     */
        _LOWER,                 /* 6A j     */
        _LOWER,                 /* 6B k     */
        _LOWER,                 /* 6C l     */
        _LOWER,                 /* 6D m     */
        _LOWER,                 /* 6E n     */
        _LOWER,                 /* 6F o     */
        _LOWER,                 /* 70 p     */
        _LOWER,                 /* 71 q     */
        _LOWER,                 /* 72 r     */
        _LOWER,                 /* 73 s     */
        _LOWER,                 /* 74 t     */
        _LOWER,                 /* 75 u     */
        _LOWER,                 /* 76 v     */
        _LOWER,                 /* 77 w     */
        _LOWER,                 /* 78 x     */
        _LOWER,                 /* 79 y     */
        _LOWER,                 /* 7A z     */
        _PUNCT,                 /* 7B {     */
        _PUNCT,                 /* 7C |     */
        _PUNCT,                 /* 7D }     */
        _PUNCT,                 /* 7E ~     */
        _CONTROL,               /* 7F (DEL) */
        /* and the rest are 0... */
};

const unsigned char __newclmap[384] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
        0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
        0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
        0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
        0x00,   /* 00 (NUL) */
        0x01,   /* 01 (SOH) */
        0x02,   /* 02 (STX) */
        0x03,   /* 03 (ETX) */
        0x04,   /* 04 (EOT) */
        0x05,   /* 05 (ENQ) */
        0x06,   /* 06 (ACK) */
        0x07,   /* 07 (BEL) */
        0x08,   /* 08 (BS)  */
        0x09,   /* 09 (HT)  */
        0x0A,   /* 0A (LF)  */
        0x0B,   /* 0B (VT)  */
        0x0C,   /* 0C (FF)  */
        0x0D,   /* 0D (CR)  */
        0x0E,   /* 0E (SI)  */
        0x0F,   /* 0F (SO)  */
        0x10,   /* 10 (DLE) */
        0x11,   /* 11 (DC1) */
        0x12,   /* 12 (DC2) */
        0x13,   /* 13 (DC3) */
        0x14,   /* 14 (DC4) */
        0x15,   /* 15 (NAK) */
        0x16,   /* 16 (SYN) */
        0x17,   /* 17 (ETB) */
        0x18,   /* 18 (CAN) */
        0x19,   /* 19 (EM)  */
        0x1A,   /* 1A (SUB) */
        0x1B,   /* 1B (ESC) */
        0x1C,   /* 1C (FS)  */
        0x1D,   /* 1D (GS)  */
        0x1E,   /* 1E (RS)  */
        0x1F,   /* 1F (US)  */
        0x20,   /* 20 SPACE */
        0x21,   /* 21 !     */
        0x22,   /* 22 "     */
        0x23,   /* 23 #     */
        0x24,   /* 24 $     */
        0x25,   /* 25 %     */
        0x26,   /* 26 &     */
        0x27,   /* 27 '     */
        0x28,   /* 28 (     */
        0x29,   /* 29 )     */
        0x2A,   /* 2A *     */
        0x2B,   /* 2B +     */
        0x2C,   /* 2C ,     */
        0x2D,   /* 2D -     */
        0x2E,   /* 2E .     */
        0x2F,   /* 2F /     */
        0x30,   /* 30 0     */
        0x31,   /* 31 1     */
        0x32,   /* 32 2     */
        0x33,   /* 33 3     */
        0x34,   /* 34 4     */
        0x35,   /* 35 5     */
        0x36,   /* 36 6     */
        0x37,   /* 37 7     */
        0x38,   /* 38 8     */
        0x39,   /* 39 9     */
        0x3A,   /* 3A :     */
        0x3B,   /* 3B ;     */
        0x3C,   /* 3C <     */
        0x3D,   /* 3D =     */
        0x3E,   /* 3E >     */
        0x3F,   /* 3F ?     */
        0x40,   /* 40 @     */
        0x61,   /* 41 A     */
        0x62,   /* 42 B     */
        0x63,   /* 43 C     */
        0x64,   /* 44 D     */
        0x65,   /* 45 E     */
        0x66,   /* 46 F     */
        0x67,   /* 47 G     */
        0x68,   /* 48 H     */
        0x69,   /* 49 I     */
        0x6A,   /* 4A J     */
        0x6B,   /* 4B K     */
        0x6C,   /* 4C L     */
        0x6D,   /* 4D M     */
        0x6E,   /* 4E N     */
        0x6F,   /* 4F O     */
        0x70,   /* 50 P     */
        0x71,   /* 51 Q     */
        0x72,   /* 52 R     */
        0x73,   /* 53 S     */
        0x74,   /* 54 T     */
        0x75,   /* 55 U     */
        0x76,   /* 56 V     */
        0x77,   /* 57 W     */
        0x78,   /* 58 X     */
        0x79,   /* 59 Y     */
        0x7A,   /* 5A Z     */
        0x5B,   /* 5B [     */
        0x5C,   /* 5C \     */
        0x5D,   /* 5D ]     */
        0x5E,   /* 5E ^     */
        0x5F,   /* 5F _     */
        0x60,   /* 60 `     */
        0x61,   /* 61 a     */
        0x62,   /* 62 b     */
        0x63,   /* 63 c     */
        0x64,   /* 64 d     */
        0x65,   /* 65 e     */
        0x66,   /* 66 f     */
        0x67,   /* 67 g     */
        0x68,   /* 68 h     */
        0x69,   /* 69 i     */
        0x6A,   /* 6A j     */
        0x6B,   /* 6B k     */
        0x6C,   /* 6C l     */
        0x6D,   /* 6D m     */
        0x6E,   /* 6E n     */
        0x6F,   /* 6F o     */
        0x70,   /* 70 p     */
        0x71,   /* 71 q     */
        0x72,   /* 72 r     */
        0x73,   /* 73 s     */
        0x74,   /* 74 t     */
        0x75,   /* 75 u     */
        0x76,   /* 76 v     */
        0x77,   /* 77 w     */
        0x78,   /* 78 x     */
        0x79,   /* 79 y     */
        0x7A,   /* 7A z     */
        0x7B,   /* 7B {     */
        0x7C,   /* 7C |     */
        0x7D,   /* 7D }     */
        0x7E,   /* 7E ~     */
        0x7F,   /* 7F (DEL) */
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
        0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
        0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
        0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};

const unsigned char __newcumap[384] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
        0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
        0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
        0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
        0x00,   /* 00 (NUL) */
        0x01,   /* 01 (SOH) */
        0x02,   /* 02 (STX) */
        0x03,   /* 03 (ETX) */
        0x04,   /* 04 (EOT) */
        0x05,   /* 05 (ENQ) */
        0x06,   /* 06 (ACK) */
        0x07,   /* 07 (BEL) */
        0x08,   /* 08 (BS)  */
        0x09,   /* 09 (HT)  */
        0x0A,   /* 0A (LF)  */
        0x0B,   /* 0B (VT)  */
        0x0C,   /* 0C (FF)  */
        0x0D,   /* 0D (CR)  */
        0x0E,   /* 0E (SI)  */
        0x0F,   /* 0F (SO)  */
        0x10,   /* 10 (DLE) */
        0x11,   /* 11 (DC1) */
        0x12,   /* 12 (DC2) */
        0x13,   /* 13 (DC3) */
        0x14,   /* 14 (DC4) */
        0x15,   /* 15 (NAK) */
        0x16,   /* 16 (SYN) */
        0x17,   /* 17 (ETB) */
        0x18,   /* 18 (CAN) */
        0x19,   /* 19 (EM)  */
        0x1A,   /* 1A (SUB) */
        0x1B,   /* 1B (ESC) */
        0x1C,   /* 1C (FS)  */
        0x1D,   /* 1D (GS)  */
        0x1E,   /* 1E (RS)  */
        0x1F,   /* 1F (US)  */
        0x20,   /* 20 SPACE */
        0x21,   /* 21 !     */
        0x22,   /* 22 "     */
        0x23,   /* 23 #     */
        0x24,   /* 24 $     */
        0x25,   /* 25 %     */
        0x26,   /* 26 &     */
        0x27,   /* 27 '     */
        0x28,   /* 28 (     */
        0x29,   /* 29 )     */
        0x2A,   /* 2A *     */
        0x2B,   /* 2B +     */
        0x2C,   /* 2C ,     */
        0x2D,   /* 2D -     */
        0x2E,   /* 2E .     */
        0x2F,   /* 2F /     */
        0x30,   /* 30 0     */
        0x31,   /* 31 1     */
        0x32,   /* 32 2     */
        0x33,   /* 33 3     */
        0x34,   /* 34 4     */
        0x35,   /* 35 5     */
        0x36,   /* 36 6     */
        0x37,   /* 37 7     */
        0x38,   /* 38 8     */
        0x39,   /* 39 9     */
        0x3A,   /* 3A :     */
        0x3B,   /* 3B ;     */
        0x3C,   /* 3C <     */
        0x3D,   /* 3D =     */
        0x3E,   /* 3E >     */
        0x3F,   /* 3F ?     */
        0x40,   /* 40 @     */
        0x41,   /* 41 A     */
        0x42,   /* 42 B     */
        0x43,   /* 43 C     */
        0x44,   /* 44 D     */
        0x45,   /* 45 E     */
        0x46,   /* 46 F     */
        0x47,   /* 47 G     */
        0x48,   /* 48 H     */
        0x49,   /* 49 I     */
        0x4A,   /* 4A J     */
        0x4B,   /* 4B K     */
        0x4C,   /* 4C L     */
        0x4D,   /* 4D M     */
        0x4E,   /* 4E N     */
        0x4F,   /* 4F O     */
        0x50,   /* 50 P     */
        0x51,   /* 51 Q     */
        0x52,   /* 52 R     */
        0x53,   /* 53 S     */
        0x54,   /* 54 T     */
        0x55,   /* 55 U     */
        0x56,   /* 56 V     */
        0x57,   /* 57 W     */
        0x58,   /* 58 X     */
        0x59,   /* 59 Y     */
        0x5A,   /* 5A Z     */
        0x5B,   /* 5B [     */
        0x5C,   /* 5C \     */
        0x5D,   /* 5D ]     */
        0x5E,   /* 5E ^     */
        0x5F,   /* 5F _     */
        0x60,   /* 60 `     */
        0x41,   /* 61 a     */
        0x42,   /* 62 b     */
        0x43,   /* 63 c     */
        0x44,   /* 64 d     */
        0x45,   /* 65 e     */
        0x46,   /* 66 f     */
        0x47,   /* 67 g     */
        0x48,   /* 68 h     */
        0x49,   /* 69 i     */
        0x4A,   /* 6A j     */
        0x4B,   /* 6B k     */
        0x4C,   /* 6C l     */
        0x4D,   /* 6D m     */
        0x4E,   /* 6E n     */
        0x4F,   /* 6F o     */
        0x50,   /* 70 p     */
        0x51,   /* 71 q     */
        0x52,   /* 72 r     */
        0x53,   /* 73 s     */
        0x54,   /* 74 t     */
        0x55,   /* 75 u     */
        0x56,   /* 76 v     */
        0x57,   /* 77 w     */
        0x58,   /* 78 x     */
        0x59,   /* 79 y     */
        0x5A,   /* 7A z     */
        0x7B,   /* 7B {     */
        0x7C,   /* 7C |     */
        0x7D,   /* 7D }     */
        0x7E,   /* 7E ~     */
        0x7F,   /* 7F (DEL) */
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
        0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
        0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
        0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};
