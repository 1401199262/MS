# Microsoft Developer Studio Project File - Name="prim" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=prim - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Prim.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Prim.mak" CFG="prim - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "prim - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "prim - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "prim - Win32 Hybrid" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/devel/3dswin/src/maxsdk/Samples/Objects", RJPBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "prim - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G6 /MD /W3 /O2 /I "..\..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /Yu"prim.h" /Fd"prim.pdb" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib delayimp.lib /nologo /base:"0x12000000" /subsystem:windows /dll /machine:I386 /out:"..\..\..\maxsdk\plugin\prim.dlo" /release /delayload:"edmodel.dll"

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /Fd"prim.pdb" /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /ZI /Od /I "..\..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /Yu"prim.h" /Fd"prim.pdb" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib delayimp.lib /nologo /base:"0x12000000" /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\maxsdk\plugin\prim.dlo" /delayload:"edmodel.dll"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\prim___W"
# PROP BASE Intermediate_Dir ".\prim___W"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Hybrid"
# PROP Intermediate_Dir ".\Hybrid"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /Gm /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"prim.h" /Fd"prim.pdb" /c
# ADD CPP /nologo /G6 /MD /W3 /Gm /ZI /Od /I "..\..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /Yu"prim.h" /Fd"prim.pdb" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\maxsdk\plugin\prim.dlo"
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib delayimp.lib /nologo /base:"0x12000000" /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\maxsdk\plugin\prim.dlo" /delayload:"edmodel.dll"

!ENDIF 

# Begin Target

# Name "prim - Win32 Release"
# Name "prim - Win32 Debug"
# Name "prim - Win32 Hybrid"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\3DSURFER.CPP
# End Source File
# Begin Source File

SOURCE=.\arc.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\bones.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\boolobj.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\boxobj.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\camera.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\circle.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cone.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cyl.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\donut.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ellipse.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\gridobj.cpp
# End Source File
# Begin Source File

SOURCE=.\hedraobj.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\helix.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\light.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\mkhedra.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\morphcnt.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\morphobj.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ngon.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\patchgrd.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\prim.cpp
# ADD CPP /Yc"prim.h"
# End Source File
# Begin Source File

SOURCE=.\prim.def
# End Source File
# Begin Source File

SOURCE=.\prim.rc
# End Source File
# Begin Source File

SOURCE=.\prothelp.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rain.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rectangl.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sample.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sphere.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\spline.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\star.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tapehelp.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\target.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\teapatch.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\teapot.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\teavert.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\text.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\torus.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tripatch.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tube.cpp

!IF  "$(CFG)" == "prim - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "prim - Win32 Debug"

# ADD CPP /Yu"prim.h"

!ELSEIF  "$(CFG)" == "prim - Win32 Hybrid"

# ADD CPP /Yu"prim.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Lib\BMM.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\core.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\edmodel.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\geom.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\gfx.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\mesh.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\Particle.lib
# End Source File
# Begin Source File

SOURCE=..\..\Lib\maxutil.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\Paramblk2.lib
# End Source File
# Begin Source File

SOURCE=..\..\lib\MNMath.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\3DSURFER.H
# End Source File
# Begin Source File

SOURCE=.\camera.h
# End Source File
# Begin Source File

SOURCE=.\light.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\mesh.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\MNMesh.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\object.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\point3.h
# End Source File
# Begin Source File

SOURCE=.\PRIM.H
# End Source File
# Begin Source File

SOURCE=.\prothelp.h
# End Source File
# Begin Source File

SOURCE=.\TAPEHELP.H
# End Source File
# Begin Source File

SOURCE=.\target.h
# End Source File
# Begin Source File

SOURCE=.\tea_util.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CROSSHR1.CUR
# End Source File
# Begin Source File

SOURCE=.\dmtlbut.bmp
# End Source File
# Begin Source File

SOURCE=.\fov.bmp
# End Source File
# Begin Source File

SOURCE=.\HYBRIDKE.BMP
# End Source File
# Begin Source File

SOURCE=.\locksep.bmp
# End Source File
# Begin Source File

SOURCE=.\MASK_HYB.BMP
# End Source File
# Begin Source File

SOURCE=.\pushpin.bmp
# End Source File
# Begin Source File

SOURCE=.\TEXT_BUT.BMP
# End Source File
# Begin Source File

SOURCE=.\TEXT_MAS.BMP
# End Source File
# End Group
# End Target
# End Project
