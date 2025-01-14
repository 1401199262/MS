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
*memmove_s.c - contains memmove_s routine
*
*Purpose:
*       memmove_s() copies a source memory buffer to a destination buffer.
*       Overlapping buffers are treated specially, to avoid propagation.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <internal.h>

/***
*memmove - Copy source buffer to destination buffer
*
*Purpose:
*       memmove() copies a source memory buffer to a destination memory buffer.
*       This routine recognize overlapping buffers to avoid propagation.
*
*       For cases where propagation is not a problem, memcpy_s() can be used.
*
*Entry:
*       void *dst = pointer to destination buffer
*       size_t sizeInBytes = size in bytes of the destination buffer
*       const void *src = pointer to source buffer
*       size_t count = number of bytes to copy
*
*Exit:
*       Returns 0 if everything is ok, else return the error code.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*       On error, the error code is returned. Nothing is written to the destination buffer.
*
*******************************************************************************/

errno_t __cdecl memmove_s(
    void * dst,
    size_t sizeInBytes,
    const void * src,
    size_t count
)
{
    if (count == 0)
    {
        /* nothing to do */
        return 0;
    }

    /* validation section */
    _VALIDATE_RETURN_ERRCODE(dst != NULL, EINVAL);
    _VALIDATE_RETURN_ERRCODE(src != NULL, EINVAL);
    _VALIDATE_RETURN_ERRCODE(sizeInBytes >= count, ERANGE);

    memmove(dst, src, count);
    return 0;
}
