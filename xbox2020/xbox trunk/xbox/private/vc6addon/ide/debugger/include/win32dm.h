/*++

Copyright (c) 1992  Microsoft Corporation

Module Name:

    win32dm.h

Abstract:

    This file contains structures used by the Win32 em/dm which
    are exposed to the debugger shell.

    This file should go away.  It is the collection point for
    non-portable stuff in windbg's shell.

Author:

    Kent Forschmiedt (kentf) 20-Oct-1993

Environment:

    Win32, User Mode

--*/



#if 0
//
// BUGBUG kentf this is probably going away
//
// RIP reporting structure
//
typedef struct _NT_RIP {
    BPR         bpr;
    ULONG       ulErrorCode;
    ULONG       ulErrorLevel;
} NT_RIP; // RIP Return
typedef NT_RIP FAR *LPNT_RIP;

#endif


//
// Packet for DebugActiveProcess
//
// BUGBUG kentf this needs to be generic.  it can't be private to Win32.
//
typedef struct _DAP {
    DWORD dwProcessId;
    HANDLE hEventGo;
} DAP;
typedef DAP FAR * LPDAP;


//
// System services provided by win32 em/dm
//

typedef enum _WIN32_SSVC {
    ssvcGetStackFrame           = FIRST_PRIVATE_SSVC,
    ssvcGetThreadContext,
    ssvcSetThreadContext,
    ssvcGetProcessHandle,
    ssvcGetThreadHandle,
    ssvcGetPrompt,
    ssvcCustomCommand,
    ssvcGeneric
};


//
// the kernel debugger reserves the first 255 subtypes
//
// the subtypes that are defined here are applicable to all
// dms that exist today.
//
#define IG_TRANSLATE_ADDRESS     256
#define IG_WATCH_TIME            257
#define IG_WATCH_TIME_STOP       258
#define IG_WATCH_TIME_RECALL     259
#define IG_WATCH_TIME_PROCS      260
#define IG_DM_PARAMS             261
#define IG_THREAD_INFO           262
#define IG_TASK_LIST             263
#define IG_RELOAD                264
#define IG_PAGEIN                265
#define IG_CHANGE_PROC           266

typedef struct _IOCTLGENERIC {
    DWORD   ioctlSubType;
    DWORD   length;
    char    data[0];
} IOCTLGENERIC, *PIOCTLGENERIC;

typedef struct _PROMPTMSG {
    DWORD   len;
    BYTE    szPrompt[];
} PROMPTMSG; // GetPrompt return
typedef PROMPTMSG * LPPROMPTMSG;

typedef struct _TASK_LIST {
    DWORD   dwProcessId;
    char    ProcessName[16];
} TASK_LIST, *PTASK_LIST;


typedef struct _DMSYM {
    ADDR    AddrSym;
    DWORD   Ra;
    char fname[];
} DMSYM;
typedef DMSYM *PDMSYM, *LPDMSYM;


