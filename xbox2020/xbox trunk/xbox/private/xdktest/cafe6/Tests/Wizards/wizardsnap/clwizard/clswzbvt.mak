# Microsoft Visual C++ Generated NMAKE File, Format Version 20053
# MSVCPRJ: version 2.00.4178
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Win32 (80x86) Debug
!MESSAGE No configuration specified.  Defaulting to Win32 (80x86) Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 (80x86) Release" && "$(CFG)" != "Win32 (80x86) Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "CLSWZBVT.MAK" CFG="Win32 (80x86) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 (80x86) Release" (based on "Win32 (x86) Application")
!MESSAGE "Win32 (80x86) Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 (80x86) Debug"
MTL=MkTypLib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 (80x86) Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : .\WinRel\CLSWZBVT.exe .\WinRel\CLSWZBVT.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE CPP /nologo /MD /W3 /GX /YX /O2 /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /c
# ADD CPP /nologo /MT /W3 /GX /YX /O2 /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /c
CPP_PROJ=/nologo /MT /W3 /GX /YX /O2 /D "NDEBUG" /D "_WINDOWS" /D "_MBCS"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"CLSWZBVT.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\WinRel/
# ADD BASE RSC /l 0x1 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"SNIFF.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# SUBTRACT BSC32 /nologo /Iu
BSC32_FLAGS=/o$(OUTDIR)/"CLSWZBVT.bsc" 
BSC32_SBRS= \
	.\WinRel\STDAFX.SBR \
	.\WinRel\CLWCASES.SBR \
	.\WinRel\CLWSNIF.SBR \
	.\WinRel\uictlwz.sbr

.\WinRel\CLSWZBVT.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib mfc30.lib mfco30.lib mfcd30.lib compob32.lib storag32.lib ole2w32.lib ole232.lib ole2di32.lib /NOLOGO /SUBSYSTEM:windows /machine:$(PROCESSOR_ARCHITECTURE)
# SUBTRACT BASE LINK32 /PDB:none
# ADD LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib nafxcw.lib ctl3d32.lib cafe.lib wbdrv.lib t3bas32.lib t3run32.lib t3scrn32.lib t3ctrl32.lib /NOLOGO /SUBSYSTEM:windows /machine:$(PROCESSOR_ARCHITECTURE)
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib nafxcw.lib ctl3d32.lib cafe.lib wbdrv.lib t3bas32.lib t3run32.lib\
 t3scrn32.lib t3ctrl32.lib /NOLOGO /SUBSYSTEM:windows /INCREMENTAL:no\
 /PDB:$(OUTDIR)/"CLSWZBVT.pdb" /machine:$(PROCESSOR_ARCHITECTURE) /OUT:$(OUTDIR)/"CLSWZBVT.exe" 
DEF_FILE=
LINK32_OBJS= \
	.\WinRel\SNIFF.res \
	.\WinRel\STDAFX.OBJ \
	.\WinRel\CLWCASES.OBJ \
	.\WinRel\CLWSNIF.OBJ \
	.\WinRel\uictlwz.obj

.\WinRel\CLSWZBVT.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 (80x86) Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : .\WinDebug\CLSWZBVT.exe .\WinDebug\CLSWZBVT.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE CPP /nologo /MD /W3 /GX /Zi /YX /Od /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /c
# ADD CPP /nologo /MT /W3 /GX /Zi /YX /Od /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /c
CPP_PROJ=/nologo /MT /W3 /GX /Zi /YX /Od /D "_DEBUG" /D "_WINDOWS" /D "_MBCS"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"CLSWZBVT.pch" /Fo$(INTDIR)/\
 /Fd$(OUTDIR)/"CLSWZBVT.pdb" /c 
CPP_OBJS=.\WinDebug/
# ADD BASE RSC /l 0x1 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"SNIFF.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BASE BSC32 /Iu
# SUBTRACT BSC32 /nologo /Iu
BSC32_FLAGS=/o$(OUTDIR)/"CLSWZBVT.bsc" 
BSC32_SBRS= \
	.\WinDebug\STDAFX.SBR \
	.\WinDebug\CLWCASES.SBR \
	.\WinDebug\CLWSNIF.SBR \
	.\WinDebug\uictlwz.sbr

.\WinDebug\CLSWZBVT.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib mfc30d.lib mfco30d.lib mfcd30d.lib compob32.lib storag32.lib ole2w32.lib ole232.lib ole2di32.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /machine:$(PROCESSOR_ARCHITECTURE)
# SUBTRACT BASE LINK32 /PDB:none
# ADD LINK32 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib cafed.lib wbdrvd.lib t3bas32.lib t3run32.lib t3scrn32.lib t3ctrl32.lib nafxcwd.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /machine:$(PROCESSOR_ARCHITECTURE)
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib cafed.lib wbdrvd.lib t3bas32.lib t3run32.lib t3scrn32.lib\
 t3ctrl32.lib nafxcwd.lib /NOLOGO /SUBSYSTEM:windows /INCREMENTAL:yes\
 /PDB:$(OUTDIR)/"CLSWZBVT.pdb" /DEBUG /machine:$(PROCESSOR_ARCHITECTURE)\
 /OUT:$(OUTDIR)/"CLSWZBVT.exe" 
DEF_FILE=
LINK32_OBJS= \
	.\WinDebug\SNIFF.res \
	.\WinDebug\STDAFX.OBJ \
	.\WinDebug\CLWCASES.OBJ \
	.\WinDebug\CLWSNIF.OBJ \
	.\WinDebug\uictlwz.obj

.\WinDebug\CLSWZBVT.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

MTL_PROJ=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\SNIFF.RC
DEP_SNIFF=\
	\DOLTEST\CAFE\WB\wb.rc\
	\DOLTEST\CAFE\SRC\CAFE.RC\
	\DOLTEST\CAFE\SRC\RES\MAIN.ICO\
	\DOLTEST\CAFE\SRC\RES\CATS.ICO\
	\DOLTEST\CAFE\SRC\RES\AFX.ICO\
	\DOLTEST\CAFE\SRC\RES\CAFE.ICO\
	\DOLTEST\CAFE\SRC\RES\SNOUT.ICO\
	\DOLTEST\CAFE\SRC\RES\PIG2.ICO\
	\DOLTEST\CAFE\SRC\RES\UTENSIL.ICO\
	\DOLTEST\CAFE\SRC\RES\UTENSILX.ICO\
	\DOLTEST\CAFE\SRC\RES\UTENSIL3.ICO\
	\DOLTEST\CAFE\SRC\RES\CHEESE.ICO\
	\DOLTEST\CAFE\SRC\RES\PORTDOC.ICO\
	\DOLTEST\CAFE\SRC\RES\TOOLBAR.BMP\
	\DOLTEST\CAFE\SRC\RES\CAFE.RC2

!IF  "$(CFG)" == "Win32 (80x86) Release"

.\WinRel\SNIFF.res :  $(SOURCE)  $(DEP_SNIFF) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 (80x86) Debug"

.\WinDebug\SNIFF.res :  $(SOURCE)  $(DEP_SNIFF) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\STDAFX.CPP
DEP_STDAF=\
	.\STDAFX.H\
	\DOLTEST\CAFE\WB\WB.H\
	\DOLTEST\CAFE\SRC\CAFE.H\
	\DOLTEST\CAFE\WB\wbdrv.h\
	\DOLTEST\CAFE\WB\wbdebug.h\
	\DOLTEST\CAFE\WB\REGDLG.H\
	\DOLTEST\CAFE\WB\ENVDLG.H\
	\DOLTEST\CAFE\WB\wbutil.h\
	\DOLTEST\CAFE\WB\FILETABL.H\
	\DOLTEST\CAFE\WB\registry.h\
	\DOLTEST\CAFE\WB\SYM\MENUIDS.H\
	\DOLTEST\CAFE\WB\SYM\DOCKIDS.H\
	\DOLTEST\CAFE\WB\SYM\QCQP.H\
	\DOLTEST\CAFE\WB\WB_PROPS.H\
	\DOLTEST\CAFE\WB\SYM\shrdres.h\
	\DOLTEST\CAFE\WB\SYM\projprop.h\
	\DOLTEST\CAFE\WB\SYM\slob.h\
	\DOLTEST\CAFE\WB\uwbframe.h\
	\DOLTEST\CAFE\WB\UEDITOR.H\
	\DOLTEST\CAFE\WB\URESEDIT.H\
	\DOLTEST\CAFE\WB\URESTABL.H\
	\DOLTEST\CAFE\WB\URESBRWS.H\
	\DOLTEST\CAFE\WB\USTREDIT.H\
	\DOLTEST\CAFE\WB\UACCEDIT.H\
	\DOLTEST\CAFE\WB\UVEREDIT.H\
	\DOLTEST\CAFE\WB\UDLGEDIT.H\
	\DOLTEST\CAFE\WB\uimgedit.h\
	\DOLTEST\CAFE\WB\UMENEDIT.H\
	\DOLTEST\CAFE\WB\USYMDLG.H\
	\DOLTEST\CAFE\WB\UCOMMDLG.H\
	\DOLTEST\CAFE\WB\UAPPWIZ.H\
	\DOLTEST\CAFE\WB\UPROPWND.H\
	\DOLTEST\CAFE\WB\UPRJPROP.H\
	\DOLTEST\CAFE\WB\UPRJFILE.H\
	\DOLTEST\CAFE\WB\UDOCKWND.H\
	\DOLTEST\CAFE\WB\UOUTPUT.H\
	\DOLTEST\CAFE\WB\UBPDLG.H\
	\DOLTEST\CAFE\WB\UFINDINF.H\
	\DOLTEST\CAFE\WB\UFINDREP.H\
	\DOLTEST\CAFE\WB\UTBARDLG.H\
	\DOLTEST\CAFE\WB\UTOOLBAR.H\
	\DOLTEST\CAFE\WB\UITARGET.H\
	\DOLTEST\CAFE\WB\UITABDLG.H\
	\DOLTEST\CAFE\WB\UIOPTDLG.H\
	\DOLTEST\CAFE\WB\UPRJTDLG.H\
	\DOLTEST\CAFE\WB\UCUSTDLG.H\
	\DOLTEST\CAFE\WB\ucwzdlg.h\
	\DOLTEST\CAFE\WB\PARSE.H\
	\DOLTEST\CAFE\WB\UIQW.H\
	\DOLTEST\CAFE\WB\UIBP.H\
	\DOLTEST\CAFE\WB\UIMEM.H\
	\DOLTEST\CAFE\WB\UIWATCH.H\
	\DOLTEST\CAFE\WB\UILOCALS.H\
	\DOLTEST\CAFE\WB\uireg.h\
	\DOLTEST\CAFE\WB\UICP.H\
	\DOLTEST\CAFE\WB\UIDAM.H\
	\DOLTEST\CAFE\WB\UIEXCPT.H\
	\DOLTEST\CAFE\WB\UISTACK.H\
	\DOLTEST\CAFE\WB\UITHREAD.H\
	\DOLTEST\CAFE\WB\UIBROWSE.H\
	\DOLTEST\CAFE\WB\UIWBMSG.H\
	\DOLTEST\CAFE\WB\UIWBDLG.H\
	\DOLTEST\CAFE\WB\COFILE.H\
	\DOLTEST\CAFE\WB\COEE.H\
	\DOLTEST\CAFE\WB\COBP.H\
	\DOLTEST\CAFE\WB\COMEM.H\
	\DOLTEST\CAFE\WB\CODEBUG.H\
	\DOLTEST\CAFE\WB\COCP.H\
	\DOLTEST\CAFE\WB\COREG.H\
	\DOLTEST\CAFE\WB\costack.h\
	\DOLTEST\CAFE\WB\COTHREAD.H\
	\DOLTEST\CAFE\WB\COEXCPT.H\
	\DOLTEST\CAFE\WB\CODAM.H\
	\DOLTEST\CAFE\WB\COSOURCE.H\
	\DOLTEST\CAFE\WB\COWRKSPC.H\
	\DOLTEST\CAFE\WB\COPRJECT.H\
	\DOLTEST\CAFE\WB\CORESSCR.H\
	\DOLTEST\CAFE\WB\COBROWSE.H\
	\DOLTEST\CAFE\SRC\MSTWRAP.H\
	\DOLTEST\CAFE\SRC\IMEWRAP.H\
	\DOLTEST\CAFE\SRC\EXCEPT.H\
	\DOLTEST\CAFE\SRC\LOG.H\
	\DOLTEST\CAFE\SRC\MAINFRM.H\
	\DOLTEST\CAFE\SRC\PORTDOC.H\
	\DOLTEST\CAFE\SRC\PORTVIEW.H\
	\DOLTEST\CAFE\SRC\FINDDLG.H\
	\DOLTEST\CAFE\SRC\TESTSET.H\
	\DOLTEST\CAFE\SRC\SCRIPTS.H\
	\DOLTEST\CAFE\SRC\TESTDRV.H\
	\DOLTEST\CAFE\SRC\TESTUTIL.H\
	\DOLTEST\CAFE\SRC\PERFDATA.H\
	\DOLTEST\CAFE\SRC\debug.h\
	\DOLTEST\CAFE\SRC\HOTKEY.H\
	\DOLTEST\CAFE\SRC\UWINDOW.H\
	\DOLTEST\CAFE\SRC\UDIALOG.H\
	\DOLTEST\CAFE\SRC\UMSGBOX.H\
	\DOLTEST\CAFE\SRC\UMAINFRM.H\
	\DOLTEST\CAFE\SRC\COCLIP.H\
	\DOLTEST\CAFE\WB\SYM\props.h\
	\DOLTEST\CAFE\INC\T3CTRL.H\
	\DOLTEST\CAFE\INC\T3SCRN.H\
	\DOLTEST\CAFE\INC\IME32.H

!IF  "$(CFG)" == "Win32 (80x86) Release"

.\WinRel\STDAFX.OBJ :  $(SOURCE)  $(DEP_STDAF) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 (80x86) Debug"

.\WinDebug\STDAFX.OBJ :  $(SOURCE)  $(DEP_STDAF) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CLWCASES.CPP
DEP_CLWCA=\
	.\STDAFX.H\
	.\CLWCASES.H\
	\DOLTEST\CAFE\WB\WB.H\
	\DOLTEST\CAFE\SRC\CAFE.H\
	\DOLTEST\CAFE\WB\wbdrv.h\
	\DOLTEST\CAFE\WB\wbdebug.h\
	\DOLTEST\CAFE\WB\REGDLG.H\
	\DOLTEST\CAFE\WB\ENVDLG.H\
	\DOLTEST\CAFE\WB\wbutil.h\
	\DOLTEST\CAFE\WB\FILETABL.H\
	\DOLTEST\CAFE\WB\registry.h\
	\DOLTEST\CAFE\WB\SYM\MENUIDS.H\
	\DOLTEST\CAFE\WB\SYM\DOCKIDS.H\
	\DOLTEST\CAFE\WB\SYM\QCQP.H\
	\DOLTEST\CAFE\WB\WB_PROPS.H\
	\DOLTEST\CAFE\WB\SYM\shrdres.h\
	\DOLTEST\CAFE\WB\SYM\projprop.h\
	\DOLTEST\CAFE\WB\SYM\slob.h\
	\DOLTEST\CAFE\WB\uwbframe.h\
	\DOLTEST\CAFE\WB\UEDITOR.H\
	\DOLTEST\CAFE\WB\URESEDIT.H\
	\DOLTEST\CAFE\WB\URESTABL.H\
	\DOLTEST\CAFE\WB\URESBRWS.H\
	\DOLTEST\CAFE\WB\USTREDIT.H\
	\DOLTEST\CAFE\WB\UACCEDIT.H\
	\DOLTEST\CAFE\WB\UVEREDIT.H\
	\DOLTEST\CAFE\WB\UDLGEDIT.H\
	\DOLTEST\CAFE\WB\uimgedit.h\
	\DOLTEST\CAFE\WB\UMENEDIT.H\
	\DOLTEST\CAFE\WB\USYMDLG.H\
	\DOLTEST\CAFE\WB\UCOMMDLG.H\
	\DOLTEST\CAFE\WB\UAPPWIZ.H\
	\DOLTEST\CAFE\WB\UPROPWND.H\
	\DOLTEST\CAFE\WB\UPRJPROP.H\
	\DOLTEST\CAFE\WB\UPRJFILE.H\
	\DOLTEST\CAFE\WB\UDOCKWND.H\
	\DOLTEST\CAFE\WB\UOUTPUT.H\
	\DOLTEST\CAFE\WB\UBPDLG.H\
	\DOLTEST\CAFE\WB\UFINDINF.H\
	\DOLTEST\CAFE\WB\UFINDREP.H\
	\DOLTEST\CAFE\WB\UTBARDLG.H\
	\DOLTEST\CAFE\WB\UTOOLBAR.H\
	\DOLTEST\CAFE\WB\UITARGET.H\
	\DOLTEST\CAFE\WB\UITABDLG.H\
	\DOLTEST\CAFE\WB\UIOPTDLG.H\
	\DOLTEST\CAFE\WB\UPRJTDLG.H\
	\DOLTEST\CAFE\WB\UCUSTDLG.H\
	\DOLTEST\CAFE\WB\ucwzdlg.h\
	\DOLTEST\CAFE\WB\PARSE.H\
	\DOLTEST\CAFE\WB\UIQW.H\
	\DOLTEST\CAFE\WB\UIBP.H\
	\DOLTEST\CAFE\WB\UIMEM.H\
	\DOLTEST\CAFE\WB\UIWATCH.H\
	\DOLTEST\CAFE\WB\UILOCALS.H\
	\DOLTEST\CAFE\WB\uireg.h\
	\DOLTEST\CAFE\WB\UICP.H\
	\DOLTEST\CAFE\WB\UIDAM.H\
	\DOLTEST\CAFE\WB\UIEXCPT.H\
	\DOLTEST\CAFE\WB\UISTACK.H\
	\DOLTEST\CAFE\WB\UITHREAD.H\
	\DOLTEST\CAFE\WB\UIBROWSE.H\
	\DOLTEST\CAFE\WB\UIWBMSG.H\
	\DOLTEST\CAFE\WB\UIWBDLG.H\
	\DOLTEST\CAFE\WB\COFILE.H\
	\DOLTEST\CAFE\WB\COEE.H\
	\DOLTEST\CAFE\WB\COBP.H\
	\DOLTEST\CAFE\WB\COMEM.H\
	\DOLTEST\CAFE\WB\CODEBUG.H\
	\DOLTEST\CAFE\WB\COCP.H\
	\DOLTEST\CAFE\WB\COREG.H\
	\DOLTEST\CAFE\WB\costack.h\
	\DOLTEST\CAFE\WB\COTHREAD.H\
	\DOLTEST\CAFE\WB\COEXCPT.H\
	\DOLTEST\CAFE\WB\CODAM.H\
	\DOLTEST\CAFE\WB\COSOURCE.H\
	\DOLTEST\CAFE\WB\COWRKSPC.H\
	\DOLTEST\CAFE\WB\COPRJECT.H\
	\DOLTEST\CAFE\WB\CORESSCR.H\
	\DOLTEST\CAFE\WB\COBROWSE.H\
	\DOLTEST\CAFE\SRC\MSTWRAP.H\
	\DOLTEST\CAFE\SRC\IMEWRAP.H\
	\DOLTEST\CAFE\SRC\EXCEPT.H\
	\DOLTEST\CAFE\SRC\LOG.H\
	\DOLTEST\CAFE\SRC\MAINFRM.H\
	\DOLTEST\CAFE\SRC\PORTDOC.H\
	\DOLTEST\CAFE\SRC\PORTVIEW.H\
	\DOLTEST\CAFE\SRC\FINDDLG.H\
	\DOLTEST\CAFE\SRC\TESTSET.H\
	\DOLTEST\CAFE\SRC\SCRIPTS.H\
	\DOLTEST\CAFE\SRC\TESTDRV.H\
	\DOLTEST\CAFE\SRC\TESTUTIL.H\
	\DOLTEST\CAFE\SRC\PERFDATA.H\
	\DOLTEST\CAFE\SRC\debug.h\
	\DOLTEST\CAFE\SRC\HOTKEY.H\
	\DOLTEST\CAFE\SRC\UWINDOW.H\
	\DOLTEST\CAFE\SRC\UDIALOG.H\
	\DOLTEST\CAFE\SRC\UMSGBOX.H\
	\DOLTEST\CAFE\SRC\UMAINFRM.H\
	\DOLTEST\CAFE\SRC\COCLIP.H\
	\DOLTEST\CAFE\WB\SYM\props.h\
	\DOLTEST\CAFE\INC\T3CTRL.H\
	\DOLTEST\CAFE\INC\T3SCRN.H\
	\DOLTEST\CAFE\INC\IME32.H

!IF  "$(CFG)" == "Win32 (80x86) Release"

.\WinRel\CLWCASES.OBJ :  $(SOURCE)  $(DEP_CLWCA) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 (80x86) Debug"

.\WinDebug\CLWCASES.OBJ :  $(SOURCE)  $(DEP_CLWCA) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CLWSNIF.CPP
DEP_CLWSN=\
	.\STDAFX.H\
	.\CLWSNIF.H\
	.\CLWCASES.H\
	\DOLTEST\CAFE\WB\WB.H\
	\DOLTEST\CAFE\SRC\CAFE.H\
	\DOLTEST\CAFE\WB\wbdrv.h\
	\DOLTEST\CAFE\WB\wbdebug.h\
	\DOLTEST\CAFE\WB\REGDLG.H\
	\DOLTEST\CAFE\WB\ENVDLG.H\
	\DOLTEST\CAFE\WB\wbutil.h\
	\DOLTEST\CAFE\WB\FILETABL.H\
	\DOLTEST\CAFE\WB\registry.h\
	\DOLTEST\CAFE\WB\SYM\MENUIDS.H\
	\DOLTEST\CAFE\WB\SYM\DOCKIDS.H\
	\DOLTEST\CAFE\WB\SYM\QCQP.H\
	\DOLTEST\CAFE\WB\WB_PROPS.H\
	\DOLTEST\CAFE\WB\SYM\shrdres.h\
	\DOLTEST\CAFE\WB\SYM\projprop.h\
	\DOLTEST\CAFE\WB\SYM\slob.h\
	\DOLTEST\CAFE\WB\uwbframe.h\
	\DOLTEST\CAFE\WB\UEDITOR.H\
	\DOLTEST\CAFE\WB\URESEDIT.H\
	\DOLTEST\CAFE\WB\URESTABL.H\
	\DOLTEST\CAFE\WB\URESBRWS.H\
	\DOLTEST\CAFE\WB\USTREDIT.H\
	\DOLTEST\CAFE\WB\UACCEDIT.H\
	\DOLTEST\CAFE\WB\UVEREDIT.H\
	\DOLTEST\CAFE\WB\UDLGEDIT.H\
	\DOLTEST\CAFE\WB\uimgedit.h\
	\DOLTEST\CAFE\WB\UMENEDIT.H\
	\DOLTEST\CAFE\WB\USYMDLG.H\
	\DOLTEST\CAFE\WB\UCOMMDLG.H\
	\DOLTEST\CAFE\WB\UAPPWIZ.H\
	\DOLTEST\CAFE\WB\UPROPWND.H\
	\DOLTEST\CAFE\WB\UPRJPROP.H\
	\DOLTEST\CAFE\WB\UPRJFILE.H\
	\DOLTEST\CAFE\WB\UDOCKWND.H\
	\DOLTEST\CAFE\WB\UOUTPUT.H\
	\DOLTEST\CAFE\WB\UBPDLG.H\
	\DOLTEST\CAFE\WB\UFINDINF.H\
	\DOLTEST\CAFE\WB\UFINDREP.H\
	\DOLTEST\CAFE\WB\UTBARDLG.H\
	\DOLTEST\CAFE\WB\UTOOLBAR.H\
	\DOLTEST\CAFE\WB\UITARGET.H\
	\DOLTEST\CAFE\WB\UITABDLG.H\
	\DOLTEST\CAFE\WB\UIOPTDLG.H\
	\DOLTEST\CAFE\WB\UPRJTDLG.H\
	\DOLTEST\CAFE\WB\UCUSTDLG.H\
	\DOLTEST\CAFE\WB\ucwzdlg.h\
	\DOLTEST\CAFE\WB\PARSE.H\
	\DOLTEST\CAFE\WB\UIQW.H\
	\DOLTEST\CAFE\WB\UIBP.H\
	\DOLTEST\CAFE\WB\UIMEM.H\
	\DOLTEST\CAFE\WB\UIWATCH.H\
	\DOLTEST\CAFE\WB\UILOCALS.H\
	\DOLTEST\CAFE\WB\uireg.h\
	\DOLTEST\CAFE\WB\UICP.H\
	\DOLTEST\CAFE\WB\UIDAM.H\
	\DOLTEST\CAFE\WB\UIEXCPT.H\
	\DOLTEST\CAFE\WB\UISTACK.H\
	\DOLTEST\CAFE\WB\UITHREAD.H\
	\DOLTEST\CAFE\WB\UIBROWSE.H\
	\DOLTEST\CAFE\WB\UIWBMSG.H\
	\DOLTEST\CAFE\WB\UIWBDLG.H\
	\DOLTEST\CAFE\WB\COFILE.H\
	\DOLTEST\CAFE\WB\COEE.H\
	\DOLTEST\CAFE\WB\COBP.H\
	\DOLTEST\CAFE\WB\COMEM.H\
	\DOLTEST\CAFE\WB\CODEBUG.H\
	\DOLTEST\CAFE\WB\COCP.H\
	\DOLTEST\CAFE\WB\COREG.H\
	\DOLTEST\CAFE\WB\costack.h\
	\DOLTEST\CAFE\WB\COTHREAD.H\
	\DOLTEST\CAFE\WB\COEXCPT.H\
	\DOLTEST\CAFE\WB\CODAM.H\
	\DOLTEST\CAFE\WB\COSOURCE.H\
	\DOLTEST\CAFE\WB\COWRKSPC.H\
	\DOLTEST\CAFE\WB\COPRJECT.H\
	\DOLTEST\CAFE\WB\CORESSCR.H\
	\DOLTEST\CAFE\WB\COBROWSE.H\
	\DOLTEST\CAFE\SRC\MSTWRAP.H\
	\DOLTEST\CAFE\SRC\IMEWRAP.H\
	\DOLTEST\CAFE\SRC\EXCEPT.H\
	\DOLTEST\CAFE\SRC\LOG.H\
	\DOLTEST\CAFE\SRC\MAINFRM.H\
	\DOLTEST\CAFE\SRC\PORTDOC.H\
	\DOLTEST\CAFE\SRC\PORTVIEW.H\
	\DOLTEST\CAFE\SRC\FINDDLG.H\
	\DOLTEST\CAFE\SRC\TESTSET.H\
	\DOLTEST\CAFE\SRC\SCRIPTS.H\
	\DOLTEST\CAFE\SRC\TESTDRV.H\
	\DOLTEST\CAFE\SRC\TESTUTIL.H\
	\DOLTEST\CAFE\SRC\PERFDATA.H\
	\DOLTEST\CAFE\SRC\debug.h\
	\DOLTEST\CAFE\SRC\HOTKEY.H\
	\DOLTEST\CAFE\SRC\UWINDOW.H\
	\DOLTEST\CAFE\SRC\UDIALOG.H\
	\DOLTEST\CAFE\SRC\UMSGBOX.H\
	\DOLTEST\CAFE\SRC\UMAINFRM.H\
	\DOLTEST\CAFE\SRC\COCLIP.H\
	\DOLTEST\CAFE\WB\SYM\props.h\
	\DOLTEST\CAFE\INC\T3CTRL.H\
	\DOLTEST\CAFE\INC\T3SCRN.H\
	\DOLTEST\CAFE\INC\IME32.H

!IF  "$(CFG)" == "Win32 (80x86) Release"

.\WinRel\CLWSNIF.OBJ :  $(SOURCE)  $(DEP_CLWSN) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 (80x86) Debug"

.\WinDebug\CLWSNIF.OBJ :  $(SOURCE)  $(DEP_CLWSN) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\DOLTEST\CAFE\WB\uictlwz.CPP
DEP_UICTL=\
	\DOLTEST\CAFE\WB\WB.H\
	\DOLTEST\CAFE\WB\uictlwz.h\
	\DOLTEST\CAFE\SRC\CAFE.H\
	\DOLTEST\CAFE\WB\wbdrv.h\
	\DOLTEST\CAFE\WB\wbdebug.h\
	\DOLTEST\CAFE\WB\REGDLG.H\
	\DOLTEST\CAFE\WB\ENVDLG.H\
	\DOLTEST\CAFE\WB\wbutil.h\
	\DOLTEST\CAFE\WB\FILETABL.H\
	\DOLTEST\CAFE\WB\registry.h\
	\DOLTEST\CAFE\WB\SYM\MENUIDS.H\
	\DOLTEST\CAFE\WB\SYM\DOCKIDS.H\
	\DOLTEST\CAFE\WB\SYM\QCQP.H\
	\DOLTEST\CAFE\WB\WB_PROPS.H\
	\DOLTEST\CAFE\WB\SYM\shrdres.h\
	\DOLTEST\CAFE\WB\SYM\projprop.h\
	\DOLTEST\CAFE\WB\SYM\slob.h\
	\DOLTEST\CAFE\WB\uwbframe.h\
	\DOLTEST\CAFE\WB\UEDITOR.H\
	\DOLTEST\CAFE\WB\URESEDIT.H\
	\DOLTEST\CAFE\WB\URESTABL.H\
	\DOLTEST\CAFE\WB\URESBRWS.H\
	\DOLTEST\CAFE\WB\USTREDIT.H\
	\DOLTEST\CAFE\WB\UACCEDIT.H\
	\DOLTEST\CAFE\WB\UVEREDIT.H\
	\DOLTEST\CAFE\WB\UDLGEDIT.H\
	\DOLTEST\CAFE\WB\uimgedit.h\
	\DOLTEST\CAFE\WB\UMENEDIT.H\
	\DOLTEST\CAFE\WB\USYMDLG.H\
	\DOLTEST\CAFE\WB\UCOMMDLG.H\
	\DOLTEST\CAFE\WB\UAPPWIZ.H\
	\DOLTEST\CAFE\WB\UPROPWND.H\
	\DOLTEST\CAFE\WB\UPRJPROP.H\
	\DOLTEST\CAFE\WB\UPRJFILE.H\
	\DOLTEST\CAFE\WB\UDOCKWND.H\
	\DOLTEST\CAFE\WB\UOUTPUT.H\
	\DOLTEST\CAFE\WB\UBPDLG.H\
	\DOLTEST\CAFE\WB\UFINDINF.H\
	\DOLTEST\CAFE\WB\UFINDREP.H\
	\DOLTEST\CAFE\WB\UTBARDLG.H\
	\DOLTEST\CAFE\WB\UTOOLBAR.H\
	\DOLTEST\CAFE\WB\UITARGET.H\
	\DOLTEST\CAFE\WB\UITABDLG.H\
	\DOLTEST\CAFE\WB\UIOPTDLG.H\
	\DOLTEST\CAFE\WB\UPRJTDLG.H\
	\DOLTEST\CAFE\WB\UCUSTDLG.H\
	\DOLTEST\CAFE\WB\ucwzdlg.h\
	\DOLTEST\CAFE\WB\odbcdlgs.h\
	\DOLTEST\CAFE\WB\PARSE.H\
	\DOLTEST\CAFE\WB\UIQW.H\
	\DOLTEST\CAFE\WB\UIBP.H\
	\DOLTEST\CAFE\WB\UIMEM.H\
	\DOLTEST\CAFE\WB\UIWATCH.H\
	\DOLTEST\CAFE\WB\UILOCALS.H\
	\DOLTEST\CAFE\WB\uireg.h\
	\DOLTEST\CAFE\WB\UICP.H\
	\DOLTEST\CAFE\WB\UIDAM.H\
	\DOLTEST\CAFE\WB\UIEXCPT.H\
	\DOLTEST\CAFE\WB\UISTACK.H\
	\DOLTEST\CAFE\WB\UITHREAD.H\
	\DOLTEST\CAFE\WB\UIBROWSE.H\
	\DOLTEST\CAFE\WB\UIWBMSG.H\
	\DOLTEST\CAFE\WB\UIWBDLG.H\
	\DOLTEST\CAFE\WB\COFILE.H\
	\DOLTEST\CAFE\WB\COEE.H\
	\DOLTEST\CAFE\WB\COBP.H\
	\DOLTEST\CAFE\WB\COMEM.H\
	\DOLTEST\CAFE\WB\CODEBUG.H\
	\DOLTEST\CAFE\WB\COCP.H\
	\DOLTEST\CAFE\WB\COREG.H\
	\DOLTEST\CAFE\WB\costack.h\
	\DOLTEST\CAFE\WB\COTHREAD.H\
	\DOLTEST\CAFE\WB\COEXCPT.H\
	\DOLTEST\CAFE\WB\CODAM.H\
	\DOLTEST\CAFE\WB\COSOURCE.H\
	\DOLTEST\CAFE\WB\COWRKSPC.H\
	\DOLTEST\CAFE\WB\COPRJECT.H\
	\DOLTEST\CAFE\WB\CORESSCR.H\
	\DOLTEST\CAFE\WB\COBROWSE.H\
	\DOLTEST\CAFE\SRC\MSTWRAP.H\
	\DOLTEST\CAFE\SRC\IMEWRAP.H\
	\DOLTEST\CAFE\SRC\EXCEPT.H\
	\DOLTEST\CAFE\SRC\LOG.H\
	\DOLTEST\CAFE\SRC\MAINFRM.H\
	\DOLTEST\CAFE\SRC\PORTDOC.H\
	\DOLTEST\CAFE\SRC\PORTVIEW.H\
	\DOLTEST\CAFE\SRC\FINDDLG.H\
	\DOLTEST\CAFE\SRC\TESTSET.H\
	\DOLTEST\CAFE\SRC\SCRIPTS.H\
	\DOLTEST\CAFE\SRC\TESTDRV.H\
	\DOLTEST\CAFE\SRC\TESTUTIL.H\
	\DOLTEST\CAFE\SRC\PERFDATA.H\
	\DOLTEST\CAFE\SRC\debug.h\
	\DOLTEST\CAFE\SRC\HOTKEY.H\
	\DOLTEST\CAFE\SRC\UWINDOW.H\
	\DOLTEST\CAFE\SRC\UDIALOG.H\
	\DOLTEST\CAFE\SRC\UMSGBOX.H\
	\DOLTEST\CAFE\SRC\UMAINFRM.H\
	\DOLTEST\CAFE\SRC\COCLIP.H\
	\DOLTEST\CAFE\WB\SYM\props.h\
	\DOLTEST\CAFE\INC\T3CTRL.H\
	\DOLTEST\CAFE\INC\T3SCRN.H\
	\DOLTEST\CAFE\INC\IME32.H

!IF  "$(CFG)" == "Win32 (80x86) Release"

.\WinRel\uictlwz.obj :  $(SOURCE)  $(DEP_UICTL) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 (80x86) Debug"

.\WinDebug\uictlwz.obj :  $(SOURCE)  $(DEP_UICTL) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ENDIF 

# End Source File
# End Group
# End Project
################################################################################
