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
*xtoa.c - convert integers/longs to ASCII string
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       The module has code to convert integers/longs to ASCII strings.  See
*
*Revision History:
*       06-06-89  PHG   Module created, based on asm version
*       03-06-90  GJF   Fixed calling type, added #include <cruntime.h> and
*                       fixed copyright.
*       03-23-90  GJF   Made xtoa() _CALLTYPE4.
*       09-27-90  GJF   New-style function declarators.
*       01-21-91  GJF   ANSI naming.
*       04-06-93  SKS   Replace _CRTAPI* with _cdecl
*       01-19-96  BWT   Add __int64 versions.
*       09-22-97  GJF   Added negation handling to x64toa.
*       05-11-02  BWT   Convert normalize the code so it can be
*                       compiled for wide as well.
*       08-18-03  AC    Added safe versions (_itoa_s, etc.)
*       10-13-03  SJ    Split into two files : VSW#168181
*       03-10-04  AC    Return ERANGE when buffer is too small
*       04-25-05  AC    Added debug filling
*                       VSW#2459
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <limits.h>
#include <tchar.h>
#include <internal.h>
#include <internal_securecrt.h>

#ifdef _UNICODE
#define xtox_s     xtow_s
#define _itox_s    _itow_s
#define _ltox_s    _ltow_s
#define _ultox_s   _ultow_s
#define x64tox_s   x64tow_s
#define _i64tox_s  _i64tow_s
#define _ui64tox_s _ui64tow_s
#define xtox       xtow
#define _itox      _itow
#define _ltox      _ltow
#define _ultox     _ultow
#define x64tox     x64tow
#define _i64tox    _i64tow
#define _ui64tox   _ui64tow
#else
#define xtox_s     xtoa_s
#define _itox_s    _itoa_s
#define _ltox_s    _ltoa_s
#define _ultox_s   _ultoa_s
#define x64tox_s   x64toa_s
#define _i64tox_s  _i64toa_s
#define _ui64tox_s _ui64toa_s
#define xtox       xtoa
#define _itox      _itoa
#define _ltox      _ltoa
#define _ultox     _ultoa
#define x64tox     x64toa
#define _i64tox    _i64toa
#define _ui64tox   _ui64toa
#endif

/***
*char *_itoa_s, *_ltoa_s, *_ultoa_s(val, buf, sizeInTChars, radix) - convert binary int to ASCII
*       string
*
*Purpose:
*       Converts an int to a character string.
*
*Entry:
*       val - number to be converted (int, long or unsigned long)
*       char *buf - ptr to buffer to place result
*       size_t sizeInTChars - size of the destination buffer
*       int radix - base to convert into
*
*Exit:
*       Fills in space pointed to by buf with string result.
*       Returns the errno_t: err != 0 means that something went wrong, and
*       an empty string (buf[0] = 0) is returned.
*
*Exceptions:
*	    Input parameters and buffer length are validated. 
*       Refer to the validation section of the function.
*
*******************************************************************************/

/* helper routine that does the main job. */
#ifdef _SECURE_ITOA
static errno_t __stdcall xtox_s
        (
        unsigned long val,
        TCHAR *buf,
        size_t sizeInTChars,
        unsigned radix,
        int is_neg
        )
#else
static void __stdcall xtox 
        (
        unsigned long val,
        TCHAR *buf,
        unsigned radix,
        int is_neg
        )
#endif
{
        TCHAR *p;                /* pointer to traverse string */
        TCHAR *firstdig;         /* pointer to first digit */
        TCHAR temp;              /* temp char */
        unsigned digval;         /* value of digit */
#ifdef _SECURE_ITOA
        size_t length;           /* current length of the string */

        /* validation section */
        _VALIDATE_RETURN_ERRCODE(buf != NULL, EINVAL);
        _VALIDATE_RETURN_ERRCODE(sizeInTChars > 0, EINVAL);
        _RESET_STRING(buf, sizeInTChars);
        _VALIDATE_RETURN_ERRCODE(sizeInTChars > (size_t)(is_neg ? 2 : 1), ERANGE);
        _VALIDATE_RETURN_ERRCODE(2 <= radix && radix <= 36, EINVAL);
        length = 0;
        
#endif
        p = buf;

        if (is_neg) {
            /* negative, so output '-' and negate */
            *p++ = _T('-');
#ifdef _SECURE_ITOA            
            length++;
#endif            
            val = (unsigned long)(-(long)val);
        }

        firstdig = p;           /* save pointer to first digit */

        do {
            digval = (unsigned) (val % radix);
            val /= radix;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (TCHAR) (digval - 10 + _T('a'));  /* a letter */
            else
                *p++ = (TCHAR) (digval + _T('0'));       /* a digit */
#ifndef _SECURE_ITOA
        } while (val > 0);
#else
            length++;
        } while (val > 0 && length < sizeInTChars);

        /* Check for buffer overrun */
        if (length >= sizeInTChars)
        {
            buf[0] = '\0';
            _VALIDATE_RETURN_ERRCODE(length < sizeInTChars, ERANGE);
        }
#endif
        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

        *p-- = _T('\0');            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */
#ifdef _SECURE_ITOA
        return 0;
#endif        
}

/* Actual functions just call conversion helper with neg flag set correctly,
   and return pointer to buffer. */

#ifdef _SECURE_ITOA
errno_t __cdecl _itox_s (
        int val,
        TCHAR *buf,
        size_t sizeInTChars,
        int radix
        )
{
        errno_t e = 0;

        if (radix == 10 && val < 0)
            e = xtox_s((unsigned long)val, buf, sizeInTChars, radix, 1);
        else
            e = xtox_s((unsigned long)(unsigned int)val, buf, sizeInTChars, radix, 0);
        
        return e;
}

errno_t __cdecl _ltox_s (
        long val,
        TCHAR *buf,
        size_t sizeInTChars,
        int radix
        )
{
        return xtox_s((unsigned long)val, buf, sizeInTChars, radix, (radix == 10 && val < 0));
}

errno_t __cdecl _ultox_s (
        unsigned long val,
        TCHAR *buf,
        size_t sizeInTChars,
        int radix
        )
{
        return xtox_s(val, buf, sizeInTChars, radix, 0);
}

#else

/***
*char *_itoa, *_ltoa, *_ultoa(val, buf, radix) - convert binary int to ASCII
*       string
*
*Purpose:
*       Converts an int to a character string.
*
*Entry:
*       val - number to be converted (int, long or unsigned long)
*       int radix - base to convert into
*       char *buf - ptr to buffer to place result
*
*Exit:
*       fills in space pointed to by buf with string result
*       returns a pointer to this buffer
*
*Exceptions:
*	    Input parameters are validated. The buffer is assumed to be big enough to
*       contain the string. Refer to the validation section of the function.
*
*******************************************************************************/

/* Actual functions just call conversion helper with neg flag set correctly,
   and return pointer to buffer. */

TCHAR * __cdecl _itox (
        int val,
        TCHAR *buf,
        int radix
        )
{
        if (radix == 10 && val < 0)
            xtox((unsigned long)val, buf, radix, 1);
        else
            xtox((unsigned long)(unsigned int)val, buf, radix, 0);
        return buf;
}

TCHAR * __cdecl _ltox (
        long val,
        TCHAR *buf,
        int radix
        )
{
        xtox((unsigned long)val, buf, radix, (radix == 10 && val < 0));
        return buf;
}

TCHAR * __cdecl _ultox (
        unsigned long val,
        TCHAR *buf,
        int radix
        )
{
        xtox(val, buf, radix, 0);
        return buf;
}

#endif

#ifndef _NO_INT64

/***
*char *_i64toa_s(val, buf, sizeInTChars, radix) - convert binary int to ASCII
*       string
*
*Purpose:
*       Converts an int64 to a character string.
*
*Entry:
*       val - number to be converted
*       char *buf - ptr to buffer to place result
*       size_t sizeInTChars - size of the destination buffer
*       int radix - base to convert into
*
*Exit:
*       Fills in space pointed to by buf with string result.
*       Returns the errno_t: err != 0 means that something went wrong, and
*       an empty string (buf[0] = 0) is returned.
*
*Exceptions:
*       Input parameters and buffer length are validated. 
*       Refer to the validation section of the function.
*
*******************************************************************************/

#ifdef _SECURE_ITOA
static errno_t __fastcall x64tox_s 
        (/* stdcall is faster and smaller... Might as well use it for the helper. */
        unsigned __int64 val,
        TCHAR *buf,
        size_t sizeInTChars,
        unsigned radix,
        int is_neg
        )
#else
static void __fastcall x64tox 
        (/* stdcall is faster and smaller... Might as well use it for the helper. */
        unsigned __int64 val,
        TCHAR *buf,
        unsigned radix,
        int is_neg
        )
#endif
{
        TCHAR *p;                /* pointer to traverse string */
        TCHAR *firstdig;         /* pointer to first digit */
        TCHAR temp;              /* temp char */
        unsigned digval;         /* value of digit */
#ifdef _SECURE_ITOA
        size_t length;           /* current length of the string */

        /* validation section */
        _VALIDATE_RETURN_ERRCODE(buf != NULL, EINVAL);
        _VALIDATE_RETURN_ERRCODE(sizeInTChars > 0, EINVAL);
        _RESET_STRING(buf, sizeInTChars);
        _VALIDATE_RETURN_ERRCODE(sizeInTChars > (size_t)(is_neg ? 2 : 1), ERANGE);
        _VALIDATE_RETURN_ERRCODE(2 <= radix && radix <= 36, EINVAL);
        length = 0;
#endif
        p = buf;

        if ( is_neg )
        {
            *p++ = _T('-');         /* negative, so output '-' and negate */
#ifdef _SECURE_ITOA            
            length++;
#endif
            val = (unsigned __int64)(-(__int64)val);
        }

        firstdig = p;           /* save pointer to first digit */

        do {
            digval = (unsigned) (val % radix);
            val /= radix;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (TCHAR) (digval - 10 + _T('a'));  /* a letter */
            else
                *p++ = (TCHAR) (digval + _T('0'));       /* a digit */

#ifndef _SECURE_ITOA
        } while (val > 0);
#else        
            length++;
        } while (val > 0 && length < sizeInTChars);

        /* Check for buffer overrun */
        if (length >= sizeInTChars)
        {
            buf[0] = '\0';
            _VALIDATE_RETURN_ERRCODE(length < sizeInTChars, ERANGE);
        }
#endif
        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

        *p-- = _T('\0');            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */

#ifdef _SECURE_ITOA
        return 0;
#endif        
}

#ifdef _SECURE_ITOA

/* Actual functions just call conversion helper with neg flag set correctly,
   and return pointer to buffer. */

errno_t __cdecl _i64tox_s (
        __int64 val,
        TCHAR *buf,
        size_t sizeInTChars,
        int radix
        )
{
        return x64tox_s((unsigned __int64)val, buf, sizeInTChars, radix, (radix == 10 && val < 0));
}

errno_t __cdecl _ui64tox_s (
        unsigned __int64 val,
        TCHAR *buf,
        size_t sizeInTChars,
        int radix
        )
{
        return x64tox_s(val, buf, sizeInTChars, radix, 0);
}

#else

/***
*char *_i64toa(val, buf, radix) - convert binary int to ASCII
*       string
*
*Purpose:
*       Converts an int64 to a character string.
*
*Entry:
*       val - number to be converted
*       int radix - base to convert into
*       char *buf - ptr to buffer to place result
*
*Exit:
*       fills in space pointed to by buf with string result
*       returns a pointer to this buffer
*
*Exceptions:
*	    Input parameters are validated. The buffer is assumed to be big enough to
*       contain the string. Refer to the validation section of the function.
*
*******************************************************************************/

/* Actual functions just call conversion helper with neg flag set correctly,
   and return pointer to buffer. */

TCHAR * __cdecl _i64tox (
        __int64 val,
        TCHAR *buf,
        int radix
        )
{
        x64tox((unsigned __int64)val, buf, radix, (radix == 10 && val < 0));
        return buf;
}

TCHAR * __cdecl _ui64tox (
        unsigned __int64 val,
        TCHAR *buf,
        int radix
        )
{
        x64tox(val, buf, radix, 0);
        return buf;
}

#endif

#endif /* _NO_INT64 */
