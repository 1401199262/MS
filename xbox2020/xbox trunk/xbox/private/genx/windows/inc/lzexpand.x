/*
** lzdos.h - Public interface to LZEXP?.LIB.
** Copyright (C) 1992-2001 Microsoft Corporation.  All Rights Reserved.

*/

#ifndef _LZEXPAND_
#define _LZEXPAND_

#ifdef __cplusplus
extern "C" {
#endif

/*
** Error Return Codes
*/

#define LZERROR_BADINHANDLE   (-1)  /* invalid input handle */
#define LZERROR_BADOUTHANDLE  (-2)  /* invalid output handle */
#define LZERROR_READ          (-3)  /* corrupt compressed file format */
#define LZERROR_WRITE         (-4)  /* out of space for output file */
#define LZERROR_GLOBALLOC     (-5)  /* insufficient memory for LZFile struct */
#define LZERROR_GLOBLOCK      (-6)  /* bad global handle */
#define LZERROR_BADVALUE      (-7)  /* input parameter out of acceptable range*/
#define LZERROR_UNKNOWNALG    (-8)  /* compression algorithm not recognized */


/*
** Prototypes
*/

INT
APIENTRY
LZStart(
	VOID
	);

VOID
APIENTRY
LZDone(
	VOID
	);

LONG
APIENTRY
CopyLZFile(
	INT,
	INT
	);

LONG
APIENTRY
LZCopy(
	INT,
	INT
	);

INT
APIENTRY
LZInit(
	INT
	);

INT
APIENTRY
GetExpandedName%(
	LPTSTR%,
	LPTSTR%
	);

INT
APIENTRY
LZOpenFile%(
	LPTSTR%,
	LPOFSTRUCT,
	WORD
	);

LONG
APIENTRY
LZSeek(
	INT,
	LONG,
	INT
	);

INT
APIENTRY
LZRead(
	INT,
	LPSTR,
	INT
	);

VOID
APIENTRY
LZClose(
	INT
	);

#ifdef __cplusplus
}
#endif


#endif // _LZEXPAND_
