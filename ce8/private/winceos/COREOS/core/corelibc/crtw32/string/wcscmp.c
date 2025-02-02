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
*wcscmp.c - routine to compare two wchar_t strings (for equal, less, or greater)
*
*	Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*	Compares two wide-character strings, determining their lexical order.
*
*Revision History:
*	09-09-91  ETC	Created from strcmp.c.
*	04-07-92  KRS	Updated and ripped out _INTL switches.
*	04-06-93  SKS	Replace _CRTAPI* with __cdecl
*       02-07-94  CFW   POSIXify.
*
*******************************************************************************/

#ifndef _POSIX_

#include <cruntime.h>
#include <string.h>

#if defined(_M_X64) || defined(_M_IX86) || defined(_M_ARM)
#pragma warning(disable:4163)
#pragma function(wcscmp)
#endif

/***
*wcscmp - compare two wchar_t strings,
*	 returning less than, equal to, or greater than
*
*Purpose:
*	wcscmp compares two wide-character strings and returns an integer
*	to indicate whether the first is less than the second, the two are
*	equal, or whether the first is greater than the second.
*
*	Comparison is done wchar_t by wchar_t on an UNSIGNED basis, which is to
*	say that Null wchar_t(0) is less than any other character.
*
*Entry:
*	const wchar_t * src - string for left-hand side of comparison
*	const wchar_t * dst - string for right-hand side of comparison
*
*Exit:
*	returns -1 if src <  dst
*	returns  0 if src == dst
*	returns +1 if src >  dst
*
*Exceptions:
*
*******************************************************************************/

int __cdecl wcscmp (
	const wchar_t * src,
	const wchar_t * dst
	)
{
	int ret = 0 ;

	while( ! (ret = (int)(*src - *dst)) && *dst)
		++src, ++dst;

	if ( ret < 0 )
		ret = -1 ;
	else if ( ret > 0 )
		ret = 1 ;

	return( ret );
}

#endif /* _POSIX_ */
