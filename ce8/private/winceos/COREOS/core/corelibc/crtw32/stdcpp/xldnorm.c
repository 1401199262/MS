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
/* _LDnorm function -- IEEE 754 version */
#include "xmath.h"
_C_STD_BEGIN

 #if _DLONG == 0
	/* not needed -- 64-bit */

 #elif _DLONG == 1
_CRTIMP2_PURE short __CLRCALL_PURE_OR_CDECL _LDnorm(_Lval *ps)
	{	/* normalize long double fraction -- 80-bit */
	short xchar;
	unsigned short sign = ps->_Sh[_L0];

	xchar = 0;
	for (ps->_Sh[_L0] = 0; ps->_Sh[_L0] == 0 && ps->_Sh[_L1] < 0x100;
		xchar -= 16)
		{	/* shift left by 16 */
		ps->_Sh[_L0] = ps->_Sh[_L1];
		ps->_Sh[_L1] = ps->_Sh[_L2], ps->_Sh[_L2] = ps->_Sh[_L3];
		ps->_Sh[_L3] = ps->_Sh[_L4], ps->_Sh[_L4] = 0;
		}
	if (ps->_Sh[_L0] == 0)
		for (; ps->_Sh[_L1] < (1U << _LOFF); --xchar)
			{	/* shift left by 1 */
			ps->_Sh[_L1] = ps->_Sh[_L1] << 1 | ps->_Sh[_L2] >> 15;
			ps->_Sh[_L2] = ps->_Sh[_L2] << 1 | ps->_Sh[_L3] >> 15;
			ps->_Sh[_L3] = ps->_Sh[_L3] << 1 | ps->_Sh[_L4] >> 15;
			ps->_Sh[_L4] <<= 1;
			}
	for (; ps->_Sh[_L0] != 0; ++xchar)
		{	/* shift right by 1 */
		ps->_Sh[_L4] = ps->_Sh[_L4] >> 1 | ps->_Sh[_L3] << 15;
		ps->_Sh[_L3] = ps->_Sh[_L3] >> 1 | ps->_Sh[_L2] << 15;
		ps->_Sh[_L2] = ps->_Sh[_L2] >> 1 | ps->_Sh[_L1] << 15;
		ps->_Sh[_L1] = ps->_Sh[_L1] >> 1 | ps->_Sh[_L0] << 15;
		ps->_Sh[_L0] >>= 1;
		}
	ps->_Sh[_L0] = sign;
	return (xchar);
	}

 #else	/* 1 < _DLONG */
_CRTIMP2_PURE short __CLRCALL_PURE_OR_CDECL _LDnorm(_Lval *ps)
	{	/* normalize long double fraction -- 128-bit SPARC */
	short xchar;
	unsigned short sign = ps->_Sh[_L0];

	xchar = 1;
	if (ps->_Sh[_L1] != 0 || ps->_Sh[_L2] != 0 || ps->_Sh[_L3] != 0
		|| ps->_Sh[_L4] != 0 || ps->_Sh[_L5] != 0 || ps->_Sh[_L6] != 0
		|| ps->_Sh[_L7] != 0)
		{	/* nonzero, scale */
		for (ps->_Sh[_L0] = 0; ps->_Sh[_L0] == 0 && ps->_Sh[_L1] < 0x100;
			xchar -= 16)
			{	/* shift left by 16 */
			ps->_Sh[_L0] = ps->_Sh[_L1];
			ps->_Sh[_L1] = ps->_Sh[_L2], ps->_Sh[_L2] = ps->_Sh[_L3];
			ps->_Sh[_L3] = ps->_Sh[_L4], ps->_Sh[_L4] = ps->_Sh[_L5];
			ps->_Sh[_L5] = ps->_Sh[_L6], ps->_Sh[_L6] = ps->_Sh[_L7];
			ps->_Sh[_L7] = 0;
			}
		for (; ps->_Sh[_L0] == 0; --xchar)
			{	/* shift left by 1 */
			ps->_Sh[_L0] = ps->_Sh[_L0] << 1 | ps->_Sh[_L1] >> 15;
			ps->_Sh[_L1] = ps->_Sh[_L1] << 1 | ps->_Sh[_L2] >> 15;
			ps->_Sh[_L2] = ps->_Sh[_L2] << 1 | ps->_Sh[_L3] >> 15;
			ps->_Sh[_L3] = ps->_Sh[_L3] << 1 | ps->_Sh[_L4] >> 15;
			ps->_Sh[_L4] = ps->_Sh[_L4] << 1 | ps->_Sh[_L5] >> 15;
			ps->_Sh[_L5] = ps->_Sh[_L5] << 1 | ps->_Sh[_L6] >> 15;
			ps->_Sh[_L6] = ps->_Sh[_L6] << 1 | ps->_Sh[_L7] >> 15;
			ps->_Sh[_L7] <<= 1;
			}
		for (; 1 < ps->_Sh[_L0]; ++xchar)
			{	/* shift right by 1 */
			ps->_Sh[_L7] = ps->_Sh[_L7] >> 1 | ps->_Sh[_L6] << 15;
			ps->_Sh[_L6] = ps->_Sh[_L6] >> 1 | ps->_Sh[_L5] << 15;
			ps->_Sh[_L5] = ps->_Sh[_L5] >> 1 | ps->_Sh[_L4] << 15;
			ps->_Sh[_L4] = ps->_Sh[_L4] >> 1 | ps->_Sh[_L3] << 15;
			ps->_Sh[_L3] = ps->_Sh[_L3] >> 1 | ps->_Sh[_L2] << 15;
			ps->_Sh[_L2] = ps->_Sh[_L2] >> 1 | ps->_Sh[_L1] << 15;
			ps->_Sh[_L1] = ps->_Sh[_L1] >> 1 | ps->_Sh[_L0] << 15;
			ps->_Sh[_L0] >>= 1;
			}
		}
	ps->_Sh[_L0] = sign;
	return (xchar);
	}
 #endif /* _DLONG */

_C_STD_END

/*
 * Copyright (c) 1992-2007 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V5.03:0009 */
