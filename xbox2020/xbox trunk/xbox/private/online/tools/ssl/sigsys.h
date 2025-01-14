/*-----------------------------------------------------------------------------
* Copyright (C) Microsoft Corporation, 1995 - 1996.
* All rights reserved.
*
* This file is part of the Microsoft Private Communication Technology
* reference implementation, version 1.0
*
* The Private Communication Technology reference implementation, version 1.0
* ("PCTRef"), is being provided by Microsoft to encourage the development and
* enhancement of an open standard for secure general-purpose business and
* personal communications on open networks.  Microsoft is distributing PCTRef
* at no charge irrespective of whether you use PCTRef for non-commercial or
* commercial use.
*
* Microsoft expressly disclaims any warranty for PCTRef and all derivatives of
* it.  PCTRef and any related documentation is provided "as is" without
* warranty of any kind, either express or implied, including, without
* limitation, the implied warranties or merchantability, fitness for a
* particular purpose, or noninfringement.  Microsoft shall have no obligation
* to provide maintenance, support, upgrades or new releases to you or to anyone
* receiving from you PCTRef or your modifications.  The entire risk arising out
* of use or performance of PCTRef remains with you.
*
* Please see the file LICENSE.txt,
* or http://pct.microsoft.com/pct/pctlicen.txt
* for more information on licensing.
*
* Please see http://pct.microsoft.com/pct/pct.htm for The Private
* Communication Technology Specification version 1.0 ("PCT Specification")
*
* 1/23/96
*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* RSA Public Key Cryptosystem, RC4, MD2, MD5 and RSA are trademarks
* of RSA Data Security, Inc.
*----------------------------------------------------------------------------*/


#ifndef __SIGSYS_H__
#define __SIGSYS_H__




typedef BOOL
( WINAPI * SignatureSignFn)(
    PUCHAR          pData,
    DWORD           cbData,
    PUCHAR          pSigned,
    DWORD           *pcbSigned,
    PctPrivateKey    *pKey);


typedef BOOL
( WINAPI * SignatureVerifyFn)(
    PUCHAR          pData,
	DWORD           cbData,
    PUCHAR          pSigned,
    DWORD           cbSigned,
    PctPublicKey    *pKey);



typedef struct _SignatureSystem {
    SigSpec   Spec;
    SignatureSignFn Sign;
    SignatureVerifyFn Verify;
} SignatureSystem, * PSignatureSystem;





/*
 * instantiations of systems
 */
extern const SignatureSystem sigRSAMD2;
extern const SignatureSystem sigRSAMD5;
extern const SignatureSystem sigRSASHA;
extern const SignatureSystem sigRSASHAMD5;

#endif /* __SIGSYS_H__ */
