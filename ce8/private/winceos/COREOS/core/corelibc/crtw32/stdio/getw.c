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
*getw.c - read a binary word from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _getw() - gets a binary integer from a stream
*
*Revision History:
*       09-02-83  RN    initial version
*       11-09-87  JCR   Multi-thread support
*       12-11-87  JCR   Added "_LOAD_DS" to declaration
*       01-13-88  JCR   Removed unnecessary calls to mthread fileno/feof/ferror
*       05-27-88  PHG   Merged DLL and normal versions
*                       Fixed bug if EOF occurs in middle of word being read
*       06-15-88  JCR   Near reference to _iob[] entries; improve REG variables
*       08-24-88  GJF   Don't use FP_OFF() macro for the 386
*       08-18-89  GJF   Clean up, now specific to OS/2 2.0 (i.e., 386 flat
*                       model). Also fixed copyright and indents.
*       02-15-90  GJF   Fixed copyright
*       03-19-90  GJF   Made calling type _CALLTYPE1, added #include
*                       <cruntime.h> and removed #include <register.h>.
*       07-23-90  SBM   Replaced <assertm.h> by <assert.h>
*       08-13-90  SBM   Compiles cleanly with -W3
*       10-02-90  GJF   New-style function declarator.
*       01-21-91  GJF   ANSI naming.
*       04-06-93  SKS   Replace _CRTAPI* with __cdecl
*       09-06-94  CFW   Replace MTHREAD with _MT.
*       02-06-94  CFW   assert -> _ASSERTE.
*       03-07-95  GJF   _[un]lock_str macros now take FILE * arg.
*       03-02-98  GJF   Exception-safe locking.
*       09-11-03  SJ    Secure CRT Work - Assertions & Validations
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>

/***
*int _getw(stream) - read an int from a stream
*
*Purpose:
*       get n bytes (n=sizeof(int)); OR them together in proper order; high
*       byte first. check for EOF between getc's.
*       this routine should be machine independent.
*
*Entry:
*       FILE *stream - stream to read integer from
*
*Exit:
*       returns the int read from the stream
*       returns EOF if fails (but this is a legit int value, so
*       should test feof() or ferror().
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _getw (
        FILE *str
        )
{
    FILE *stream;
    int bytecount = sizeof(int);
    int word;
    char *byteptr = (char *)&word;
    int retval;

    _VALIDATE_RETURN((str != NULL), EINVAL, EOF);

    /* Init stream pointer */
    stream = str;

    _lock_str(stream);
    __try {
        while (bytecount--)
            *byteptr++ = (char)_getc_nolock(stream);
        retval = ((feof(stream) || ferror(stream)) ? EOF : word);
    }
    __finally {
        _unlock_str(stream);
    }

    return(retval);
}
