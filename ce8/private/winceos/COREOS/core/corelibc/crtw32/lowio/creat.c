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
*creat.c - create a new file or truncate existing file
*
*	Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*	defines _creat() - create new file
*
*Revision History:
*	06-08-89  PHG	Module created, based on asm version
*	03-12-90  GJF	Made calling type _CALLTYPE1, added #include
*			<cruntime.h>, fixed compiler warning and fixed the
*			copyright. Also, cleaned up the formatting a bit.
*	09-28-90  GJF	New-style function declarator.
*	01-16-91  GJF	ANSI naming.
*	04-06-93  SKS	Replace _CRTAPI* with __cdecl
*	11-01-93  CFW	Enable Unicode variant.
*	09-28-04  AC	Modified call to _sopen_s.
*
*******************************************************************************/

#include <cruntime.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <tchar.h>

/***
*int _creat(path, pmode) - create a new file
*
*Purpose:
*	If file specified does not exist, _creat creates a new file
*	with the given permission setting and opens it for writing.
*	If the file already exists and its permission allows writing,
*	_creat truncates it to 0 length and open it for writing.
*	The only Xenix mode bit supprted by DOS is user write (S_IWRITE).
*
*Entry:
*	_TSCHAR *path - filename to create
*	int pmode - permission mode setting for new file
*
*Exit:
*	returns handle for created file
*	returns -1 and sets errno if fails.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _tcreat (
	const _TSCHAR *path,
	int pmode
	)
{
	/* creat is just the same as open... */
	int fh = -1;
	errno_t e = _tsopen_s(&fh, path, _O_CREAT + _O_TRUNC + _O_RDWR, _SH_DENYNO, pmode);
	return (e == 0 ? fh : -1);
}
