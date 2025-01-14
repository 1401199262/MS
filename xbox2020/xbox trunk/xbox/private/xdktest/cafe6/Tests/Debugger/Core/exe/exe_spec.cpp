///////////////////////////////////////////////////////////////////////////////
//	exe_spec.CPP
//											 
//	Created by :			
//		VCBU QA		

#include <process.h>
#include "stdafx.h"
#include "exe_spec.h"	

#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
				 	 
IMPLEMENT_TEST(Cexe_specialIDETest, CDebugTestSet, "Execution Special", -1, CexeSubSuite)
												 
void Cexe_specialIDETest::PreRun(void)
{
	// call the base class
	CTest::PreRun();
}

void Cexe_specialIDETest::Run()
{	 
	XSAFETY;
	
	if( SetProject("testgo01\\testgo01"))
	{	  
		bps.ClearAllBreakpoints();
		XSAFETY;
		GoExeDoesNotExist();
		XSAFETY;
		// TODO(michma): this has broken since chris' performance chagnes. 2/24/98
		//ExitWhileDebugging();
		XSAFETY;
	}

/*	It doesn't work now: the msg dialog is hidden behind the IDE and even Spy++ cannot see it	
	if (SetProject("dllapp\\incdll", PROJECT_DLL) && SetProject("dllapp\\powdll", PROJECT_DLL)
		&& SetProject("dllapp\\dec2", PROJECT_DLL) && SetProject("dllapp\\dllappx"))
	{
		XSAFETY;
		bps.ClearAllBreakpoints();
		EXPECT_TRUE(CannotFindDll());
		XSAFETY;
	}
*/
	if( SetProject("cons01\\cons01"))
	{	  
		XSAFETY;
		StdInOutRedirection();
		XSAFETY;
	}
 
	if( SetProject("dbga\\dbga"))
	{	  
		XSAFETY;
		if(!(GetSystem() & SYSTEM_NT))
		{
			NoSystemCodeStepping();
			XSAFETY;
		}

/* bug # 1347 is postponed for after V50
		TraceIntoWndProc();
		XSAFETY;
*/
	}

	// close the project in preparation for the next group of tests.
	COProject prj;
	prj.Attach();
	prj.Close();

}


///////////////////////////////////////////////////////////////////////////////
//	Test Cases
///////////////////////////////////////////////////////////////////////////////

BOOL Cexe_specialIDETest::GoExeDoesNotExist( )
{
 	LogTestHeader( "GoExeDoesNotExist" );
	m_pLog->RecordInfo( "Try to execute nonexisting PCTarget, correct the path and GO" );

	char* szTARGET_EXT = ".exe";
	char* szREMOTE_TARGET_EXT = "";

	CString stOldTarget = m_strProjectName + szTARGET_EXT ;
	CString stAbsPathTarget =  m_strProjectDir + "\\" + m_strProjectName + szTARGET_EXT ;
	CString stNewTarget = m_strProjectDir + "\\kit\\" + m_strProjectName + szTARGET_EXT;
	CString stBadTarget = m_strProjectDir + "\\..\\" + m_strProjectName + szTARGET_EXT ;

	// Rename the target and run
	CopyFile(stAbsPathTarget, m_strProjectDir + "\\foo" + szTARGET_EXT, FALSE);
	DeleteFile(stAbsPathTarget);
	EXPECT_TRUE(dbg.Go(NULL,NULL,NULL,0) );

	if( WaitMsgBoxText("does not exist", 10) )
	{
		WriteLog( PASSED, "01 : The MsgBox \"... does not exist...\" was found as expected" );
		MST.WButtonClick( "OK" ); // MST.WButtonClick( "No" );
	}
	else
	{
		if( WaitMsgBoxText("not found", 10) )
		{
			WriteLog( PASSED, "01 : The MsgBox \"...File not found\" was found as expected" );
			MST.WButtonClick( "OK" );
		}
		else
		{
			WriteLog( FAILED, "01 : The MsgBox \"...File not found\" was not found" );
			WriteLog( FAILED, "01 : The MsgBox \"... does not exist...\" was not found" );
			EXPECT_TRUE( FALSE );	 	
		}
	}
	
	// Set incorrect path\target 
	EXPECT_TRUE(proj.SetLocalTarget(stBadTarget) == ERROR_SUCCESS);
	EXPECT_TRUE(dbg.Go(NULL,NULL,NULL,0));

	if( WaitMsgBoxText("or do not exist", 10)) 
	{
		WriteLog( PASSED, "The MsgBox \"One or more targets are out of date...\" was found" );
		MST.WButtonClick( "No" );
	}

/*	EXPECT_TRUE( WaitMsgBoxText("not be opened", 10));
	EXPECT_TRUE( WaitMsgBoxText("not exist", 10));
	MST.WButtonClick( "OK" );
*/
 	if( WaitMsgBoxText("does not exist", 10) )
	{
		WriteLog( PASSED, "01 : The MsgBox \"... does not exist...\" was found as expected" );
		MST.WButtonClick( "OK" ); // MST.WButtonClick( "No" );
	}
	else
	{
		if( WaitMsgBoxText("not found", 10) )
		{
			WriteLog( PASSED, "01 : The MsgBox \"...File not found\" was found as expected" );
			MST.WButtonClick( "OK" );
		}
		else
		{
			WriteLog( FAILED, "01 : The MsgBox \"...File not found\" was not found" );
			WriteLog( FAILED, "01 : The MsgBox \"... does not exist...\" was not found" );
			EXPECT_TRUE( FALSE );	 	
		}
	}

	// Set correct new path\target and run
	CreateDirectory(m_strProjectDir + "\\kit", 0);          
	CopyFile(m_strProjectDir + "\\foo" + szTARGET_EXT, stNewTarget, FALSE);
	EXPECT(proj.SetLocalTarget(stNewTarget) == ERROR_SUCCESS );
	EXPECT_TRUE(dbg.Go(NULL,NULL,NULL,0) );

	if( WaitMsgBoxText("or do not exist", 10))
	{
		WriteLog( PASSED, "01 : The MsgBox \"One or more targets are out of date...\" was found as expected" );
		MST.WButtonClick( "No" );
		while(MST.WFndWndC("or do not exist", "Static", FW_PART));
	}

	// REVIEW (michma): why does UIWB.WaitForRun() never return?
	//UIWB.WaitForRun();
	// Sleep(10000);
	CString TitleRun = (CString)"[" + GetLocString(IDSS_DBG_RUN) + "]";
	MST.WFndWndWait(TitleRun, FW_PART, 10);

//	UIWB.WaitForTermination();
	dbg.Wait(WAIT_FOR_TERMINATION);

/*	EXPECT_TRUE( VerifyProjectRan() );
*/
	// Remove temporary file and dir
	DeleteFile(stNewTarget);
	RemoveDirectory(m_strProjectDir + "\\kit");         

	// Set correct AbsPath\target 
	EXPECT( proj.SetLocalTarget(stAbsPathTarget) == ERROR_SUCCESS );

	// Exe is still out of place
	EXPECT_TRUE(dbg.Go(NULL,NULL,NULL,0) );
	if( WaitMsgBoxText("does not exist", 10) )
	{
		WriteLog( PASSED, "The MsgBox \"... does not exist...\" was found as expected" );
		MST.WButtonClick( "OK" ); // MST.WButtonClick( "No" );
	}
	else
	{
		if( WaitMsgBoxText("not found", 10) )
		{
			WriteLog( PASSED, "The MsgBox \"...File not found\" was found as expected" );
			MST.WButtonClick( "OK" );
		}
		else
		{
			WriteLog( FAILED, "The MsgBox \"...File not found\" was not found" );
			WriteLog( FAILED, "The MsgBox \"... does not exist...\" was not found" );
			EXPECT_TRUE( FALSE );	 	
		}
	}

	// Exe is back in place
	CopyFile(m_strProjectDir + "\\foo" + szTARGET_EXT, stAbsPathTarget, FALSE);
	DeleteFile(m_strProjectDir + "\\foo" + szTARGET_EXT);
	Sleep(1000);
	EXPECT_TRUE( dbg.Go(NULL, NULL, NULL, WAIT_FOR_TERMINATION) );

	// Restore initial target
	EXPECT_TRUE( proj.SetLocalTarget(stOldTarget) == ERROR_SUCCESS );

	// Verify restoration is OK
	EXPECT_TRUE( dbg.Go(NULL, NULL, NULL, WAIT_FOR_TERMINATION) );

	return (TRUE);
}

BOOL Cexe_specialIDETest::CheckGeneralNetworkFailure( )
{
	if( WaitMsgBoxText("General Network Failure", 10) )
	{
		MST.DoKeys( KEY_Enter );
		return (FALSE);      
	}
	else
	{
		return (TRUE);      
	}
}


BOOL Cexe_specialIDETest::ExitWhileDebugging( )
{
 	LogTestHeader( "ExitWhileDebugging" );

	m_pLog->RecordInfo( "Restart, Exit IDE and verify: ExitWhileDebugging" );
	dbg.Restart();
	dbg.SetSteppingMode(SRC);  //Temporary workaround: bug #15556
	CString strLine = "{";

	if( UIWB.GetPcodeSwitch() )
		strLine = "int iGo = TestG();";

    EXPECT_TRUE( dbg.CurrentLineIs( strLine ) );
	UIWB.DoCommand(ID_APP_EXIT, DC_MNEMONIC);
	
	if( WaitMsgBoxText("command will stop the debugger", 10) )
	{
		WriteLog( PASSED, "01 : The MsgBox \"command will stop the debugger\" was found as expected" );
		MST.WButtonClick( "Cancel" );
	}
	else
	{
		WriteLog( FAILED, "01 : The MsgBox \"command will stop the debugger\" was not found as expected" );
		EXPECT_TRUE( FALSE );	 	
	}
 
	UIWB.DoCommand(ID_APP_EXIT, DC_MNEMONIC);

	if( WaitMsgBoxText("command will stop the debugger", 10) )
	{
		WriteLog( PASSED, "02 : The MsgBox \"command will stop the debugger\" was found as expected" );
		MST.WButtonClick( "OK" );
	}
	else
	{
		WriteLog( FAILED, "02 : The MsgBox \"command will stop the debugger\" was not found as expected" );
		EXPECT_TRUE( FALSE );	 	
	}

	if( WaitMsgBoxText("want to save", 10) )
	{
		MST.WButtonClick( "No" );
	}

	while(GetSubSuite()->GetIDE()->IsLaunched());

	// REVIEW(briancr): use RTTI for this cast, or at least to validate it
	GetSubSuite()->GetIDE()->Launch();
	WriteLog( PASSED, "03 : The IDE was closed and reopened as expected" );
	return (TRUE);
}


BOOL Cexe_specialIDETest::CannotFindDll( )
{
 	LogTestHeader( "CannotFindDll" );
	m_pLog->RecordInfo( "Try to start debugging exe which cannot find associated Dll, correct the path and debug" );

	// Verify we can start debugging
	dbg.Restart();
	EXPECT_TRUE( dbg.StepInto(1, NULL, NULL, "WinMain") );
	dbg.StopDebugging();

	// Rename dec2.dll to dec22.dll and start debugging
	CopyFile(m_strProjectDir + "\\dec2.dll", m_strProjectDir + "\\dec22.dll", FALSE);
	DeleteFile(m_strProjectDir + "\\dec2.dll");
	EXPECT_TRUE(dbg.StepInto(1,NULL,NULL,NULL,0) );

//	dbg.CancelMsg(MSG_DLL_NOT_FOUND);	 // it got out of the CODebug ?
/*	if( WaitMsgBoxText("not found", 10) )
	{
		WriteLog( PASSED, "01 : The MsgBox \"...not found...\" was found" );
		MST.WButtonClick( "OK" ); 
	}
	else
	{
		if( WaitMsgBoxText("Could not execute", 10) )
		{
			WriteLog( PASSED, "01 : The MsgBox \"Could not execute...\" was found as expected" );
			MST.DoKeys( KEY_Enter );
			if( WaitMsgBoxText("Cannot execute", 10) )
			{
				WriteLog( PASSED, "01 : The MsgBox \"Cannot execute...\" was found as expected" );
				MST.DoKeys( KEY_Enter );
			}
		}
		else
		{
			WriteLog( FAILED, "The MsgBox \"...not found\" was not found" );
			WriteLog( FAILED, "The MsgBox \"Could not execute...\" was not found" );
			EXPECT_TRUE( FALSE );	 	
		}
	}
	
	if( WaitMsgBoxText("not found", 10) )
	{
		WriteLog( PASSED, "01 : The MsgBox \"...not found...\" was found" );
		MST.WButtonClick( "OK" ); 
	}
	else
	{
		if( WaitMsgBoxText("Could not execute", 10) )
		{
			WriteLog( PASSED, "01 : The MsgBox \"Could not execute...\" was found as expected" );
			MST.DoKeys( KEY_Enter );
			if( WaitMsgBoxText("Cannot execute", 10) )
			{
				WriteLog( PASSED, "01 : The MsgBox \"Cannot execute...\" was found as expected" );
				MST.DoKeys( KEY_Enter );
			}
		}
		else
		{
			WriteLog( FAILED, "The MsgBox \"...not found\" was not found" );
			WriteLog( FAILED, "The MsgBox \"Could not execute...\" was not found" );
			EXPECT_TRUE( FALSE );	 	
		}
	}
*/	
/*	
	if( WaitMsgBoxText("Could not execute", 10) )
	{
		WriteLog( PASSED, "01 : The MsgBox \"Could not execute...\" was found as expected" );
		MST.DoKeys( KEY_Enter );
	}
	else
	{
		WriteLog( FAILED, "01 : The MsgBox \"Could not execute...\" was not found" );
		EXPECT_TRUE( FALSE );	 	
	}
*/	
	// Restore initial Dll
	CopyFile(m_strProjectDir + "\\dec22.dll", m_strProjectDir + "\\dec2.dll", FALSE);
	DeleteFile(m_strProjectDir + "\\dec22.dll");
	EXPECT_TRUE( dbg.StepInto(1, NULL, "{", "WinMain") );
	dbg.StopDebugging();

	return (TRUE);
}

BOOL Cexe_specialIDETest::StdInOutRedirection()
{
 	LogTestHeader( "StdInOutRedirection" );
	m_pLog->RecordInfo( "Redirect Std Input and Output to files and debug console app" );

	// Redirect std input from file cons01.inp and check values
	CString progargs = "< cons01.inp";
	EXPECT_TRUE( proj.SetProgramArguments(progargs) == ERROR_SUCCESS );

	UIWB.OpenFile( m_strProjectDir + "\\" + "cons01.cpp");
	EXPECT_TRUE( src.Find("cin >> Char;") );

	EXPECT_TRUE( dbg.StepToCursor());
	// REVIEW(michma - 6/8/99): we're seeing a weird timing problem on w2k where sometimes the console window of the debuggee is still in the foreground
	// even though the debugger is in break mode. doesn't happen manually.
	UIWB.Activate();
	EXPECT_TRUE(dbg.CurrentLineIs("cin >> Char;"));
	
	dbg.StepOver(1, NULL, NULL, NULL, 0);
	EXPECT_TRUE( cxx.ExpressionValueIs("Char=='a'",1) );

	dbg.StepOver(1, NULL, NULL, NULL, 0);
	EXPECT_TRUE( cxx.ExpressionValueIs("Array1d[1]",888) );

	EXPECT_TRUE( src.Find("cin >> String;") );

	EXPECT_TRUE( dbg.StepToCursor());
	// REVIEW(michma - 6/8/99): we're seeing a weird timing problem on w2k where sometimes the console window of the debuggee is still in the foreground
	// even though the debugger is in break mode. doesn't happen manually.
	UIWB.Activate();
	EXPECT_TRUE(dbg.CurrentLineIs("cin >> String;"));

	EXPECT_TRUE( cxx.ExpressionValueIs("Array2d[0][2]",7) );

	dbg.StepOver(1, NULL, NULL, NULL, 0);
	CString stringValue = "ADDRESS \"!ZYXWVUTSRQPONMLKJIHGFEDCBA\"";
	EXPECT_TRUE( cxx.ExpressionValueIs("String",stringValue) );

	dbg.StopDebugging();

	// Redirect std output into file cons01.out
	progargs = "> cons01.out";
	EXPECT_TRUE( proj.SetProgramArguments(progargs) == ERROR_SUCCESS );

	DeleteFile(m_strProjectDir + "\\cons01.out");
	UIWB.OpenFile( m_strProjectDir + "\\" + "cons01.cpp");
	EXPECT_TRUE( src.Find("cout << Char;") );
	
	EXPECT_TRUE( dbg.StepToCursor());
	// REVIEW(michma - 6/8/99): we're seeing a weird timing problem on w2k where sometimes the console window of the debuggee is still in the foreground
	// even though the debugger is in break mode. doesn't happen manually.
	UIWB.Activate();
	EXPECT_TRUE(dbg.CurrentLineIs("cout << Char;"));

	EXPECT_TRUE( dbg.StepOver(1, NULL, "cout << Array1d[0] << Array1d[1] << Array1d[2] ;") );
	EXPECT_TRUE( src.Find("cin >> Char;") );
	
	EXPECT_TRUE( dbg.StepToCursor());
	// REVIEW(michma - 6/8/99): we're seeing a weird timing problem on w2k where sometimes the console window of the debuggee is still in the foreground
	// even though the debugger is in break mode. doesn't happen manually.
	UIWB.Activate();
	EXPECT_TRUE(dbg.CurrentLineIs("cin >> Char;"));

	dbg.StopDebugging();

	// Redirect both std input (cons01.inp) and output (cons01.out)
	// and check values and output
	DeleteFile(m_strProjectDir + "\\cons01.out");
	progargs = "< cons01.inp > cons01.out";
	EXPECT_TRUE( proj.SetProgramArguments(progargs) == ERROR_SUCCESS );

	UIWB.OpenFile( m_strProjectDir + "\\" + "cons01.cpp");
	EXPECT_TRUE( src.Find("cin >> Char;") );
	EXPECT_TRUE( dbg.StepToCursor(0, NULL, "cin >> Char;") );
	dbg.StepOver(1, NULL, NULL, NULL, 0);
	EXPECT_TRUE( cxx.ExpressionValueIs("Char=='a'",1) );

	dbg.StepOver(1, NULL, NULL, NULL, 0);
	EXPECT_TRUE( cxx.ExpressionValueIs("Array1d[1]",888) );

	EXPECT_TRUE( src.Find("cin >> String;") );
	EXPECT_TRUE( dbg.StepToCursor(0, NULL, "cin >> String;") );
	EXPECT_TRUE( cxx.ExpressionValueIs("Array2d[0][2]",7) );

	dbg.StepOver(1, NULL, NULL, NULL, 0);
	EXPECT_TRUE( cxx.ExpressionValueIs("String",stringValue) );

	EXPECT_TRUE( dbg.Go(NULL, NULL, NULL, WAIT_FOR_TERMINATION) );
	UIWB.OpenFile( m_strProjectDir + "\\" + "cons01.out");
	EXPECT_TRUE( src.Find("s111222333112233445566778899this is a string") );

	return (TRUE);
}

BOOL Cexe_specialIDETest::TraceIntoWndProc()
{
 	LogTestHeader( "TraceIntoWndProc" );
	bps.ClearAllBreakpoints();

	UIWB.OpenFile( m_strProjectDir + "\\" + "main2.c");
	EXPECT_TRUE( src.Find("SendMessage(") );
	EXPECT_TRUE( dbg.StepToCursor(0, NULL, "SendMessage(") );
//	EXPECT_TRUE( dbg.StepInto(1, NULL, "{", "WndProc") );
	EXPECT_TRUE( dbg.StepInto(1, NULL, "{") );
	EXPECT_TRUE( stk.CurrentFunctionIs("WndProc")	);
	EXPECT_TRUE( dbg.StepInto(1, NULL, "SAVELABEL(TagCallOtherMain);", "WndProc") );

	dbg.StopDebugging();
	return (TRUE);
}

BOOL Cexe_specialIDETest::NoSystemCodeStepping()
{
 	LogTestHeader( "NoSystemCodeStepping" );
	bps.ClearAllBreakpoints();
	BOOL ret = TRUE;

	UIWB.OpenFile( m_strProjectDir + "\\" + "main2.c");
	EXPECT_TRUE( src.Find("SAVELABEL(TagCreateCase);") );
	EXPECT_TRUE( dbg.StepToCursor(0, NULL, "SAVELABEL(TagCreateCase);", "WndProc") );
	EXPECT_TRUE( dbg.StepOut(1, NULL, NULL, NULL, 0) );

//	if( WaitMsgBoxText("Cannot step through system code", 280) )
	if( WaitMsgBoxText("Can't trace into system DLLs", 280) )
	{
		WriteLog( PASSED, "The MsgBox \"Can't trace into system DLLs\" was found as expected" );
		MST.WButtonClick( "OK" ); 
	}
	else
	{
		WriteLog( FAILED, "The MsgBox \"Can't trace into system DLLs\" was not found" );
		ret = FALSE;	 	
	}
	dbg.StopDebugging();
	return ret;
}
