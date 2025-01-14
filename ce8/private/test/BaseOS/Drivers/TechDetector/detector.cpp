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
// --------------------------------------------------------------------
//                                                                     
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A      
// PARTICULAR PURPOSE.                                                 
//                                                                     
// --------------------------------------------------------------------
//
//
// Example technology detector for TestKit
//
//

#include "detector.h"

//
// This is the name of the binary and should be changed to something
// like "Touch_detector.dll".  You will need to update the sources
// file to match.
//

CKato *g_pKato = NULL;
SPS_SHELL_INFO *g_pShellInfo;

TESTPROCAPI DetectorTest( UINT, TPPARAM, LPFUNCTION_TABLE_ENTRY ); 


// --------------------------------------------------------------------
// TUX Function table
//
// Update the text "Example Technology Detector" to be the name of the
// tech you want to detect.  For example "Touch detector" or "Mouse detector"
//
// You do not need to change the name of the function DetectorTest
//

extern "C" {
    FUNCTION_TABLE_ENTRY g_lpFTE[] = {
    TEXT( "Example Technology Detector" ), 0, 0, BVT_BASE+0, DetectorTest,  
    {NULL, 0, 0, 0, NULL}
    };
}


//
// IsTechPresent
//
// This helper function should be updated with whatever code is needed to 
// actually detect if your technology is present.  
//
// Called By:
//      DetectorTest
//
// Return Values:
//      TRUE    tech was detected
//      FALSE   tech is not detected
//

BOOL IsTechPresent(void)
{
    BOOL bFlag = FALSE;

    //Put your detection code here... and change bFlag based on if you
    // find the technology.  For example a mouse detector looks for the
    // mouse driver.
    // For this example we assume we find the driver :)
    bFlag = TRUE;

    // inform the logs if we found the technology
    if(bFlag)
        DETAIL("Successfully detected Tech X");                      
    else
        WARN("Did not detect Tech X at line %d in file %s");

    return bFlag;
}


//
// DetectorTest
//
// This test is called by TUX directly and so must handle the standard TUX messages.  
//
// To make things as simple as possible we suggest putting any acutal detection code in 
// the support function IsTechPresent.
//
// No changes are required to this function.
//
// Returns:
//      TPR_PASS    if tech was detected
//      TPR_FAIL    if tech was not detected
//

TESTPROCAPI DetectorTest(UINT uMsg, TPPARAM tpParam, LPFUNCTION_TABLE_ENTRY lpFTE)
{
    UNREFERENCED_PARAMETER(tpParam);
    UNREFERENCED_PARAMETER(lpFTE);

    INT TP_Status = TPR_FAIL;

    // skip any call to us that is not a request to execute
    if (uMsg != TPM_EXECUTE)
        return TPR_NOT_HANDLED;

    DETAIL("Detecting ...");                      
    if(IsTechPresent())
        TP_Status = TPR_PASS;

    return TP_Status;
}


//
// ShellProc
//
// Standard simplified shellproc for supporting calls from TUX
//
// This function should need no modification.
//

SHELLPROCAPI ShellProc(UINT uMsg, SPPARAM spParam) 
{
    switch (uMsg) 
    {
        case SPM_LOAD_DLL: 
        {
            ((LPSPS_LOAD_DLL)spParam)->fUnicode = TRUE;
            // Get/Create our global logging object.
            g_pKato = (CKato*)KatoGetDefaultObject();            
            return SPR_HANDLED;
        }

        case SPM_SHELL_INFO: 
        {        
            // Store a pointer to our shell info for later use.
            g_pShellInfo = (LPSPS_SHELL_INFO)spParam;
            if( g_pShellInfo->szDllCmdLine && *g_pShellInfo->szDllCmdLine )
            {
                // Display our Dlls command line if we have one.
                g_pKato->Log( LOG_DETAIL, _T("Command Line: \"%s\"."), g_pShellInfo->szDllCmdLine);
            } 
            // get the global pointer to our HINSTANCE
            //g_hInstance = g_pShellInfo->hLib;                        
            return SPR_HANDLED;
        }

        case SPM_REGISTER: 
        {
            ((LPSPS_REGISTER)spParam)->lpFunctionTable = g_lpFTE;
            return SPR_HANDLED | SPF_UNICODE;
        }

        case SPM_UNLOAD_DLL: 
        case SPM_START_SCRIPT: 
        case SPM_STOP_SCRIPT: 
            return SPR_HANDLED;


        case SPM_BEGIN_GROUP: 
        {
            g_pKato->BeginLevel(0, TEXT("BEGIN GROUP: %s"), TARGETNAME);                      
            return SPR_HANDLED;
        }

        case SPM_END_GROUP: 
        {
           g_pKato->EndLevel(TEXT("END GROUP: %s"), TARGETNAME);         
           return SPR_HANDLED;
        }

        case SPM_BEGIN_TEST: 
        {
           // Start our logging level.
            LPSPS_BEGIN_TEST pBT = (LPSPS_BEGIN_TEST)spParam;
            g_pKato->BeginLevel(pBT->lpFTE->dwUniqueID, 
                                TEXT("BEGIN TEST: \"%s\", Threads=%u, Seed=%u"),
                                pBT->lpFTE->lpDescription, pBT->dwThreadCount,
                                pBT->dwRandomSeed);
           return SPR_HANDLED;
        }

        case SPM_END_TEST: 
        {
            // End our logging level.
            LPSPS_END_TEST pET = (LPSPS_END_TEST)spParam;
            g_pKato->EndLevel(TEXT("END TEST: \"%s\", %s, Time=%u.%03u"),
                            pET->lpFTE->lpDescription,
                            pET->dwResult == TPR_SKIP ? TEXT("SKIPPED") :
                            pET->dwResult == TPR_PASS ? TEXT("PASSED") :
                            pET->dwResult == TPR_FAIL ? TEXT("FAILED") : TEXT("ABORTED"),
                            pET->dwExecutionTime / 1000, pET->dwExecutionTime % 1000);    
            return SPR_HANDLED;
        }

        case SPM_EXCEPTION: 
        {
            g_pKato->Log(LOG_EXCEPTION, TEXT("Exception occurred in %s!"),__WFILE__);
            return SPR_HANDLED;
        }
    }
    return SPR_NOT_HANDLED;
}
