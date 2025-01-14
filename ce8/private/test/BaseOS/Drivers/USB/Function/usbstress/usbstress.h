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
//******************************************************************************
//
//
// This source code is licensed under Microsoft Shared Source License
// Version 1.0 for Windows CE.
// For a copy of the license visit http://go.microsoft.com/fwlink/?LinkId=3223.
//******************************************************************************

/*++

Module Name:  
    usbstress.h

Abstract:  
    USB Stress Test Header
    Modified from usb\function\usbtest.h
    
Notes: 

    Created - smoidu 08/28/2007
    Modified -

    smoidu 04/04/2007 - code cleanup, suspend/resume
    
--*/

#ifndef __USBSTRESS_H
#define __USBSTRESS_H
#include <tux.h>
#include <kato.h>
#include <ceddk.h>
#include <usbtypes.h>
#include "loadfn.h"
#include "clicker.h"

class UsbClientDrv;

// ----------------------------------------------------------------------------
//
// Debugger
//
// ----------------------------------------------------------------------------
#ifdef DEBUG

//
// These defines must match the ZONE_* defines
//
#define INIT_ERR        1
#define INIT_WARN       2
#define INIT_FUNC       4
#define INIT_DETAIL     8
#define INIT_INI        16
#define INIT_USB        32
#define INIT_IO         64
#define INIT_WEIRD      128
#define INIT_EVENTS             256
#define INIT_USB_CONTROL 0x200
#define INIT_USB_BULK 0x400
#define INIT_USB_INTERRUPT 0x800
#define INIT_USB_USB_ISOCH 0x1000
#define INIT_THREAD 0x2000

#define DEVICE_TO_HOST_MASK  0x80

extern DBGPARAM dpCurSettings;

#endif                // DEBUG

#define DEVICE_TO_HOST_MASK  0x80

// --Test Harness related definitions--

extern CKato *g_pKato;
extern SPS_SHELL_INFO *g_pShellInfo;

// Suggested log verbosities
#define LOG_EXCEPTION      0
#define LOG_FAIL                2
#define LOG_ABORT             4
#define LOG_SKIP                6
#define LOG_NOT_IMPLEMENTED    8
#define LOG_PASS               10
#define LOG_DETAIL            12
#define LOG_COMMENT        14

#define LOG_VERBOSITY_LEVEL 13


// --physical memory allocation  related definitions--
#define MY_PHYS_MEM_SIZE 0x000A8000/PAGE_SIZE
typedef struct _PhysMemNode {
    LONG lSize;
    DWORD dwVirtMem;
} PhysMemNode, *PPhysMemNode;


// -- Endpoint type strings --
static LPCTSTR g_szEPTypeStrings[5] = {
    _T("USB_ENDPOINT_TYPE_CONTROL"),
    _T("USB_ENDPOINT_TYPE_ISOCH"),
    _T("USB_ENDPOINT_TYPE_BULK"),
    _T("USB_ENDPOINT_TYPE_INTERRUPT"),
    _T("USB_ENDPOINT_TYPE_UNKNOWN")
};


// -- User input (command line) --
typedef struct _USB_PARAM {
    DWORD dwSelectDevice;
    BOOL fDoSuspendResume;    // use suspend resume functionality on the host/function
    DWORD dwConnectDisconnectProbability; // probability of connect/disconnect occuring during transfer
    TCHAR clickerParams; // parameters for clicker init function
    DWORD dwClients;    // clients we are trying to emulate
    DWORD dwDuration;
    BOOL fClicker;
} USB_PARAM, *LPUSB_PARAM;

BOOL ParseCommandLine(LPUSB_PARAM lpUsbParam, LPCTSTR lpCommandLine);

//-- test commands (communicate with function side) --

//test request commands
#define TEST_REQUEST_BULKIN    1
#define TEST_REQUEST_BULKOUT    2
#define TEST_REQUEST_INTIN        3
#define TEST_REQUEST_INTOUT    4
#define TEST_REQUEST_ISOCHIN    5
#define TEST_REQUEST_ISOCHOUT    6
#define TEST_REQUEST_RECONFIG    7
#define TEST_REQUEST_DATALPBK  8
#define TEST_REQUEST_LPBKINFO   9
#define TEST_REQUEST_PAIRNUM   10
#define TEST_REQUEST_SHORTSTRESS    11
#define TEST_REQUEST_VARYLENTRAN    12

#define TEST_REQUEST_EP0TESTIN    20
#define TEST_REQUEST_EP0TESTOUT    21
#define TEST_REQUEST_EP0TESTINST    22
#define TEST_REQUEST_EP0TESTOUTST    23
#define TEST_REQUEST_EP0ZEROINLEN   24

#define TEST_SPREQ_DATAPIPESTALL    101
#define TEST_SPREQ_DATAPIPESHORT    102

//choices of test device configuration/interface/speed
#define CONFIG_DEFAULT  1
#define CONFIG_ALTERNATIVE  2
#define INTERFACE_DEFAULT 1
#define INTERFACE_ALTERNATIVE 2
#define SPEED_ALL_SPEED        0
#define SPEED_FULL_SPEED        1
#define SPEED_HIGH_SPEED        2

#define USB_REQUEST_CLASS               0x20

//this is the definition of the request structure (mainly for a protocol to commnicate between 
//test driver on host controller side and function drive side
#include <pshpack1.h>
typedef struct _USB_TDEVICE_REQUEST {
    UCHAR bmRequestType;
    UCHAR bRequest;
    USHORT wValue;
    USHORT wIndex;
    USHORT wLength;
} USB_TDEVICE_REQUEST, *PUSB_TDEVICE_REQUEST, *LPUSB_TDEVICE_REQUEST;

typedef struct _USB_TDEVICE_RECONFIG {
    UCHAR ucConfig;
    UCHAR ucInterface;
    UCHAR ucSpeed;
    USHORT wBulkPkSize;
    USHORT wIntrPkSize;
    USHORT wIsochPkSize;
    USHORT wControlPkSize;
} USB_TDEVICE_RECONFIG, *PUSB_TDEVICE_RECONFIG, *LPUSB_TDEVICE_RECONFIG;

typedef struct _USB_TDEVICE_LPBKPAIR {
    UCHAR ucOutEp;
    UCHAR ucInEp;
    UCHAR ucType;
} USB_TDEVICE_LPBKPAIR, *PUSB_TDEVICE_LPBKPAIR;

typedef struct _USB_TDEVICE_DATALPBK {
    UCHAR uOutEP;
    USHORT wBlockSize;
    USHORT wNumofBlocks;
    USHORT usAlignment;
    DWORD dwStartVal;
} USB_TDEVICE_DATALPBK, *PUSB_TDEVICE_DATALPBK, *LPUSB_TDEVICE_DATALPBK;
#include <poppack.h>

//--test device information--
typedef struct _USB_TDEVICE_LPBKINFO {
    UCHAR uNumOfPipePairs;
    UCHAR uNumofSinglePipes;
    USB_TDEVICE_LPBKPAIR pLpbkPairs[1];
} USB_TDEVICE_LPBKINFO, *PUSB_TDEVICE_LPBKINFO;

typedef struct _USB_TDEVICE_VARYLENLPBK {
    USHORT uOutEP;
    USHORT wNumofPackets;
    USHORT pusTransLens[1];
} USB_TDEVICE_VARYLENLPBK, *PUSB_TDEVICE_VARYLENLPBK, *LPUSB_TDEVICE_VARYLENLPBK;

#include <pshpack1.h>
typedef struct _USB_TDEVICE_STALLDATAPIPE {
    UCHAR uOutEP;
    UCHAR uStallRepeat;
    USHORT wBlockSize;
} USB_TDEVICE_STALLDATAPIPE, *PUSB_TDEVICE_STALLDATAPIPE, *LPUSB_TDEVICE_STALLDATAPIPE;
#include <poppack.h>

#define MAX_CLIENTS 4

extern PUSB_TDEVICE_LPBKINFO g_pTstDevLpbkInfo[MAX_CLIENTS];
extern PBOOL g_pTstDevPipeStatus[MAX_CLIENTS];    //we allow to connect to up to 4 test devices 

//-- test related definitions --
#define TEST_LPBK_NORMAL    0
#define TEST_LPBK_PHYMEM    1
#define TEST_LPBK_PHYMEMALIGN    2
#define TEST_LPBK_NORMALSHORT    3
#define TEST_LPBK_STRESSSHORT     5
#define TEST_LPBK_ZEROLEN            6

#define TEST_WAIT_TIME  10*60*1000    //10minutes
#define TEST_DATA_SIZE 0x400
#define DATA_LOOP_TIMES  16
#define HIGHSPEED_MULTI_PACKETS_MASK 0x1800
#define PACKETS_PART_MASK                       0x7ff


#ifdef DEBUG
#define DBG_ERR        DEBUGZONE(0)
#define DBG_WARN       DEBUGZONE(1)
#define DBG_FUNC       DEBUGZONE(2)
#define DBG_DETAIL     DEBUGZONE(3)
#define DBG_INI        DEBUGZONE(4)
#define DBG_USB        DEBUGZONE(5)
#define DBG_IO         DEBUGZONE(6)
#define DBG_WEIRD      DEBUGZONE(7)
#define DBG_EVENTS        DEBUGZONE(8)
#define DBG_CONTROL        DEBUGZONE(9)
#define DBG_BULK        DEBUGZONE(10)
#define DBG_INTERRUPT    DEBUGZONE(11)
#define DBG_ISOCH        DEBUGZONE(12)
#define DBG_THREAD        DEBUGZONE(13)
#endif


//--funciton prototypes--
#define SET_DEV_NAME_BUFF_SIZE 64
VOID SetDeviceName(LPTSTR Buff, USHORT uID, LPCUSB_ENDPOINT pOutEP, LPCUSB_ENDPOINT pInEP);

//--APIs about register/unregister test driver to USBD.dll--
BOOL RegLoadUSBTestDll(LPTSTR szDllName, BOOL bUnload);
HWND ShowDialogBox(LPCTSTR szPromptString);
VOID DeleteDialogBox(HWND hDiagWnd);
//--vendor transfer for communicating with test device
BOOL SendVendorTransfer(UsbClientDrv * pDriverPtr, BOOL bIn, PUSB_TDEVICE_REQUEST pUtdr, LPVOID pBuffer, PDWORD cbBytes);
//--get test device information--
PUSB_TDEVICE_LPBKINFO GetTestDeviceLpbkInfo(UsbClientDrv * pClientDrv);

// client types
#define SERIAL_CLIENT 1
#define STORAGE_CLIENT (1<<1)
#define RNDIS_CLIENT (1<<2)
#define OVERLOAD_CLIENT (1<<4)

#define DEFAULT_TEST_TIME 5

#endif
