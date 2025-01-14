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
////////////////////////////////////////////////////////////////////////////////
//
//  TUXTEST TUX DLL
//  Copyright (c) Microsoft Corporation
//
//  Module:
//          Declares the TUX function table and test function prototypes EXCEPT
//          when included by globals.cpp, in which case it DEFINES the function
//          table.
//
//  Revision History:
//
////////////////////////////////////////////////////////////////////////////////

#if (!defined(__FT_H__) || defined(__GLOBALS_CPP__))
#ifndef __FT_H__
#define __FT_H__
#endif

////////////////////////////////////////////////////////////////////////////////
// Local macros

#ifdef __DEFINE_FTE__
#undef BEGIN_FTE
#undef FTE
#undef FTH
#undef END_FTE
#define BEGIN_FTE FUNCTION_TABLE_ENTRY g_lpFTE[] = {
#define FTH(a, b) { TEXT(b), a, 0, 0, NULL },
#define FTE(a, b, c, d, e) { TEXT(b), a, d, c, e },
#define END_FTE { NULL, 0, 0, 0, NULL } };
#else // __DEFINE_FTE__
#ifdef __GLOBALS_CPP__
#define BEGIN_FTE
#else // __GLOBALS_CPP__
#define BEGIN_FTE extern FUNCTION_TABLE_ENTRY g_lpFTE[];
#endif // __GLOBALS_CPP__
#define FTH(a, b)
#define FTE(a, b, c, d, e) TESTPROCAPI e(UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY);
#define END_FTE
#endif // __DEFINE_FTE__

//Test Base
#define PDA_BASE 100     //Base Test Number for PDA Power Model Tests 
#define DEFAULT_BASE 200 //Base Test Number for DEFAULT Power Model Tests

///////////////////////////////////////////////////////////////////////////////
// TUX Function table
//  To create the function table and function prototypes, two macros are
//  available:
//
//      FTH(level, description)
//          (Function Table Header) Used for entries that don't have functions,
//          entered only as headers (or comments) into the function table.
//
//      FTE(level, description, code, param, function)
//          (Function Table Entry) Used for all functions. DON'T use this macro
//          if the "function" field is NULL. In that case, use the FTH macro.
//
//  You must not use the TEXT or _T macros here. This is done by the FTH and FTE
//  macros.
//
//  In addition, the table must be enclosed by the BEGIN_FTE and END_FTE macros.

BEGIN_FTE

FTE(0, "Power Manager State Transition tests",                  1, 0, UsageMessage)

FTH(0, "Power Manager State Transition Tests for the PDA power model")
FTE(1,     "Test On state",                                     PDA_BASE + 1, 0, Tst_TransitionOn_PDA)
FTE(1,     "Test transitions to and from BacklightOff state",   PDA_BASE + 2, 0, Tst_TransitionBacklightOff_PDA)
FTE(1,     "Test transitions to and from UserIdle state",       PDA_BASE + 3, 0, Tst_TransitionUserIdle_PDA)
FTE(1,     "Test transitions to and from ScreenOff state",      PDA_BASE + 4, 0, Tst_TransitionScreenOff_PDA)

FTH(0, "Power Manager State Transition Tests for the DEFAULT power model")
FTE(1,     "Test On state  ",                                   DEFAULT_BASE + 1, 0, Tst_TransitionOn_DEFAULT)
FTE(1,     "Test transitions to and from UserIdle state  ",     DEFAULT_BASE + 2, 0, Tst_TransitionUserIdle_DEFAULT)
FTE(1,     "Test transitions to and from SystemIdle state ",    DEFAULT_BASE + 3, 0, Tst_TransitionSystemIdle_DEFAULT)
FTE(1,     "Test transitions to and from Suspend state ",       DEFAULT_BASE + 4, 0, Tst_TransitionSuspend_DEFAULT)

END_FTE

////////////////////////////////////////////////////////////////////////////////

#endif // !__FT_H__ || __GLOBALS_CPP__
