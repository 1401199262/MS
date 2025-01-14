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
*mbsbtype.c - Return type of byte within a string (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Return type of byte within a string (MBCS)
*
*Revision History:
*       11-19-92  KRS   Ported from 16-bit sources.
*       10-05-93  GJF   Replaced _CRTAPI1 with __cdecl.
*       04-15-93  CFW   Add _MB_CP_LOCK.
*       05-09-94  CFW   Optimize for SBCS.
*       05-19-94  CFW   Enable non-Win32.
*       09-11-97  GJF   Replaced __mbcodepage == 0 with _ISNOTMBCP.
*       04-03-98  GJF   Revised multithread support based on threadmbcinfo
*                       structs
*       10-02-03  AC    Added validation.
*       03-08-04  MSL   Fixed validation/return in len past end case
*       08-16-04  SJ    VSW#315314 - Asserting only if any char before the 
*                       one asked for is a '\0'
*
*******************************************************************************/

#ifdef  _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <mbdata.h>
#include <mbstring.h>
#include <mbctype.h>
#include <internal.h>
#include <locale.h>
#include <setlocal.h>


#define _MBBTYPE(p,c)   _mbbtype(p,c)

/***
* _mbsbtype - Return type of byte within a string
*
*Purpose:
*       Test byte within a string for MBCS char type.
*       This function requires the start of the string because
*       context must be taken into account.
*
*Entry:
*       const unsigned char *string = pointer to string
*       size_t len = position of the char in string
*
*Exit:
*       returns one of the following values:
*
*       _MBC_LEAD      = if 1st byte of MBCS char
*       _MBC_TRAIL     = if 2nd byte of MBCS char
*       _MBC_SINGLE    = valid single byte char
*
*       _MBC_ILLEGAL   = if illegal char
*
*Exceptions:
*       Returns _MBC_ILLEGAL if char is invalid. 
*       Calls invalid parameter if len is bigger than string length (and errno is set to EINVAL).
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _mbsbtype_l
(
        const unsigned char *string,
        size_t len,
        _locale_t plocinfo
)
{
        int chartype;
        _LocaleUpdate _loc_update(plocinfo);

        /* validation section */
        _VALIDATE_RETURN(string != NULL, EINVAL, _MBC_ILLEGAL);

        if (_loc_update.GetLocaleT()->mbcinfo->ismbcodepage == 0)
            return _MBC_SINGLE;

        chartype = _MBC_ILLEGAL;

        do 
        {
            /* If the char at the position asked for is a '\0' we return 
            _MBC_ILLEGAL. But, If any char before the position asked for is 
            a '\0', then we call invalid_param */
            
            if ((len == 0) && (*string == '\0'))
                return(_MBC_ILLEGAL);
                
            _VALIDATE_RETURN(*string != '\0', EINVAL, _MBC_ILLEGAL);
            chartype = _mbbtype_l(*string++, chartype, _loc_update.GetLocaleT());
        }  while (len--);

        return(chartype);
}

int (__cdecl _mbsbtype)(
        const unsigned char *string,
        size_t len
        )
{
        return _mbsbtype_l(string, len, NULL);
}

#endif  /* _MBCS */
