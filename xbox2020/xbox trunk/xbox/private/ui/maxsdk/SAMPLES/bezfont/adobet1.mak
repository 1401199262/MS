# Microsoft Developer Studio Generated NMAKE File, Based on Adobet1.dsp
!IF "$(CFG)" == ""
CFG=adobet1 - Win32 Release
!MESSAGE No configuration specified. Defaulting to adobet1 - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "adobet1 - Win32 Release" && "$(CFG)" !=\
 "adobet1 - Win32 Debug" && "$(CFG)" != "adobet1 - Win32 Hybrid"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Adobet1.mak" CFG="adobet1 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "adobet1 - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "adobet1 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "adobet1 - Win32 Hybrid" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "adobet1 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\maxsdk\plugin\adobet1.dlf"

!ELSE 

ALL : "..\..\..\maxsdk\plugin\adobet1.dlf"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\adobet1.obj"
	-@erase "$(INTDIR)\adobet1.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\adobet1.exp"
	-@erase "$(OUTDIR)\adobet1.lib"
	-@erase "..\..\..\maxsdk\plugin\adobet1.dlf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G6 /MD /W3 /O2 /I "..\..\include" /D "NDEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\adobet1.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Adobet1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /release /base:"0x10280000" /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\adobet1.pdb" /machine:I386 /def:".\adobet1.def"\
 /out:"..\..\..\maxsdk\plugin\adobet1.dlf" /implib:"$(OUTDIR)\adobet1.lib" 
DEF_FILE= \
	".\adobet1.def"
LINK32_OBJS= \
	"$(INTDIR)\adobet1.obj" \
	"$(INTDIR)\adobet1.res" \
	"..\..\Lib\core.lib" \
	"..\..\Lib\geom.lib" \
	"..\..\Lib\gfx.lib" \
	"..\..\Lib\mesh.lib" \
	"..\..\Lib\maxutil.lib"

"..\..\..\maxsdk\plugin\adobet1.dlf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "adobet1 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\maxsdk\plugin\adobet1.dlf"

!ELSE 

ALL : "..\..\..\maxsdk\plugin\adobet1.dlf"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\adobet1.obj"
	-@erase "$(INTDIR)\adobet1.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\adobet1.exp"
	-@erase "$(OUTDIR)\adobet1.lib"
	-@erase "$(OUTDIR)\adobet1.pdb"
	-@erase "..\..\..\maxsdk\plugin\adobet1.dlf"
	-@erase "..\..\..\maxsdk\plugin\adobet1.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G6 /MDd /W3 /Gm /Zi /Od /I "..\..\include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\adobet1.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Adobet1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /base:"0x10280000" /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\adobet1.pdb" /debug /machine:I386 /def:".\adobet1.def"\
 /out:"..\..\..\maxsdk\plugin\adobet1.dlf" /implib:"$(OUTDIR)\adobet1.lib" 
DEF_FILE= \
	".\adobet1.def"
LINK32_OBJS= \
	"$(INTDIR)\adobet1.obj" \
	"$(INTDIR)\adobet1.res" \
	"..\..\Lib\core.lib" \
	"..\..\Lib\geom.lib" \
	"..\..\Lib\gfx.lib" \
	"..\..\Lib\mesh.lib" \
	"..\..\Lib\maxutil.lib"

"..\..\..\maxsdk\plugin\adobet1.dlf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "adobet1 - Win32 Hybrid"

OUTDIR=.\Hybrid
INTDIR=.\Hybrid

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\maxsdk\plugin\adobet1.dlf"

!ELSE 

ALL : "..\..\..\maxsdk\plugin\adobet1.dlf"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\adobet1.obj"
	-@erase "$(INTDIR)\adobet1.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\adobet1.exp"
	-@erase "$(OUTDIR)\adobet1.lib"
	-@erase "$(OUTDIR)\adobet1.pdb"
	-@erase "..\..\..\maxsdk\plugin\adobet1.dlf"
	-@erase "..\..\..\maxsdk\plugin\adobet1.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G6 /MD /W3 /Gm /Zi /Od /I "..\..\include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "WIN32_LEAN_AND_MEAN" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Hybrid/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\adobet1.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Adobet1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /base:"0x10280000" /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\adobet1.pdb" /debug /machine:I386 /def:".\adobet1.def"\
 /out:"..\..\..\maxsdk\plugin\adobet1.dlf" /implib:"$(OUTDIR)\adobet1.lib" 
DEF_FILE= \
	".\adobet1.def"
LINK32_OBJS= \
	"$(INTDIR)\adobet1.obj" \
	"$(INTDIR)\adobet1.res" \
	"..\..\Lib\core.lib" \
	"..\..\Lib\geom.lib" \
	"..\..\Lib\gfx.lib" \
	"..\..\Lib\mesh.lib" \
	"..\..\Lib\maxutil.lib"

"..\..\..\maxsdk\plugin\adobet1.dlf" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "adobet1 - Win32 Release" || "$(CFG)" ==\
 "adobet1 - Win32 Debug" || "$(CFG)" == "adobet1 - Win32 Hybrid"
SOURCE=.\adobet1.cpp

!IF  "$(CFG)" == "adobet1 - Win32 Release"

DEP_CPP_ADOBE=\
	"..\..\include\acolor.h"\
	"..\..\include\animtbl.h"\
	"..\..\include\appio.h"\
	"..\..\include\assert1.h"\
	"..\..\include\bezfont.h"\
	"..\..\include\bitarray.h"\
	"..\..\include\box2.h"\
	"..\..\include\box3.h"\
	"..\..\include\buildver.h"\
	"..\..\include\captypes.h"\
	"..\..\include\channels.h"\
	"..\..\include\cmdmode.h"\
	"..\..\include\color.h"\
	"..\..\include\control.h"\
	"..\..\include\coreexp.h"\
	"..\..\include\custcont.h"\
	"..\..\include\dbgprint.h"\
	"..\..\include\dpoint3.h"\
	"..\..\include\euler.h"\
	"..\..\include\evuser.h"\
	"..\..\include\export.h"\
	"..\..\include\gencam.h"\
	"..\..\include\genhier.h"\
	"..\..\include\genlight.h"\
	"..\..\include\genshape.h"\
	"..\..\include\geom.h"\
	"..\..\include\geomlib.h"\
	"..\..\include\gfloat.h"\
	"..\..\include\gfx.h"\
	"..\..\include\gfxlib.h"\
	"..\..\include\gutil.h"\
	"..\..\include\hitdata.h"\
	"..\..\include\hold.h"\
	"..\..\include\impapi.h"\
	"..\..\include\impexp.h"\
	"..\..\include\imtl.h"\
	"..\..\include\inode.h"\
	"..\..\include\interval.h"\
	"..\..\include\ioapi.h"\
	"..\..\include\iparamb.h"\
	"..\..\include\ipoint2.h"\
	"..\..\include\ipoint3.h"\
	"..\..\include\lockid.h"\
	"..\..\include\log.h"\
	"..\..\include\matrix2.h"\
	"..\..\include\matrix3.h"\
	"..\..\include\Max.h"\
	"..\..\include\maxapi.h"\
	"..\..\include\maxcom.h"\
	"..\..\include\maxtess.h"\
	"..\..\include\maxtypes.h"\
	"..\..\include\mesh.h"\
	"..\..\include\meshlib.h"\
	"..\..\include\mouseman.h"\
	"..\..\include\mstshape.h"\
	"..\..\include\mtl.h"\
	"..\..\include\nametab.h"\
	"..\..\include\object.h"\
	"..\..\include\objmode.h"\
	"..\..\include\patch.h"\
	"..\..\include\patchlib.h"\
	"..\..\include\patchobj.h"\
	"..\..\include\plugapi.h"\
	"..\..\include\plugin.h"\
	"..\..\include\point2.h"\
	"..\..\include\point3.h"\
	"..\..\include\point4.h"\
	"..\..\include\polyshp.h"\
	"..\..\include\ptrvec.h"\
	"..\..\include\quat.h"\
	"..\..\include\ref.h"\
	"..\..\include\render.h"\
	"..\..\include\rtclick.h"\
	"..\..\include\sceneapi.h"\
	"..\..\include\shape.h"\
	"..\..\include\shphier.h"\
	"..\..\include\shpsels.h"\
	"..\..\include\snap.h"\
	"..\..\include\soundobj.h"\
	"..\..\include\spline3d.h"\
	"..\..\include\splshape.h"\
	"..\..\include\stack.h"\
	"..\..\include\stack3.h"\
	"..\..\include\strbasic.h"\
	"..\..\include\strclass.h"\
	"..\..\include\tab.h"\
	"..\..\include\trig.h"\
	"..\..\include\triobj.h"\
	"..\..\include\units.h"\
	"..\..\include\utilexp.h"\
	"..\..\include\utillib.h"\
	"..\..\include\vedge.h"\
	"..\..\include\winutil.h"\
	".\type1.h"\
	

"$(INTDIR)\adobet1.obj" : $(SOURCE) $(DEP_CPP_ADOBE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "adobet1 - Win32 Debug"


"$(INTDIR)\adobet1.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "adobet1 - Win32 Hybrid"

DEP_CPP_ADOBE=\
	"..\..\include\acolor.h"\
	"..\..\include\animtbl.h"\
	"..\..\include\appio.h"\
	"..\..\include\assert1.h"\
	"..\..\include\bezfont.h"\
	"..\..\include\bitarray.h"\
	"..\..\include\box2.h"\
	"..\..\include\box3.h"\
	"..\..\include\buildver.h"\
	"..\..\include\captypes.h"\
	"..\..\include\channels.h"\
	"..\..\include\cmdmode.h"\
	"..\..\include\color.h"\
	"..\..\include\control.h"\
	"..\..\include\coreexp.h"\
	"..\..\include\custcont.h"\
	"..\..\include\dbgprint.h"\
	"..\..\include\dpoint3.h"\
	"..\..\include\euler.h"\
	"..\..\include\evuser.h"\
	"..\..\include\export.h"\
	"..\..\include\gencam.h"\
	"..\..\include\genhier.h"\
	"..\..\include\genlight.h"\
	"..\..\include\genshape.h"\
	"..\..\include\geom.h"\
	"..\..\include\geomlib.h"\
	"..\..\include\gfloat.h"\
	"..\..\include\gfx.h"\
	"..\..\include\gfxlib.h"\
	"..\..\include\gutil.h"\
	"..\..\include\hitdata.h"\
	"..\..\include\hold.h"\
	"..\..\include\impapi.h"\
	"..\..\include\impexp.h"\
	"..\..\include\imtl.h"\
	"..\..\include\inode.h"\
	"..\..\include\interval.h"\
	"..\..\include\ioapi.h"\
	"..\..\include\iparamb.h"\
	"..\..\include\ipoint2.h"\
	"..\..\include\ipoint3.h"\
	"..\..\include\lockid.h"\
	"..\..\include\log.h"\
	"..\..\include\matrix2.h"\
	"..\..\include\matrix3.h"\
	"..\..\include\Max.h"\
	"..\..\include\maxapi.h"\
	"..\..\include\maxcom.h"\
	"..\..\include\maxtess.h"\
	"..\..\include\maxtypes.h"\
	"..\..\include\mesh.h"\
	"..\..\include\meshlib.h"\
	"..\..\include\mouseman.h"\
	"..\..\include\mstshape.h"\
	"..\..\include\mtl.h"\
	"..\..\include\nametab.h"\
	"..\..\include\object.h"\
	"..\..\include\objmode.h"\
	"..\..\include\patch.h"\
	"..\..\include\patchlib.h"\
	"..\..\include\patchobj.h"\
	"..\..\include\plugapi.h"\
	"..\..\include\plugin.h"\
	"..\..\include\point2.h"\
	"..\..\include\point3.h"\
	"..\..\include\point4.h"\
	"..\..\include\polyshp.h"\
	"..\..\include\ptrvec.h"\
	"..\..\include\quat.h"\
	"..\..\include\ref.h"\
	"..\..\include\render.h"\
	"..\..\include\rtclick.h"\
	"..\..\include\sceneapi.h"\
	"..\..\include\shape.h"\
	"..\..\include\shphier.h"\
	"..\..\include\shpsels.h"\
	"..\..\include\snap.h"\
	"..\..\include\soundobj.h"\
	"..\..\include\spline3d.h"\
	"..\..\include\splshape.h"\
	"..\..\include\stack.h"\
	"..\..\include\stack3.h"\
	"..\..\include\strbasic.h"\
	"..\..\include\strclass.h"\
	"..\..\include\tab.h"\
	"..\..\include\trig.h"\
	"..\..\include\triobj.h"\
	"..\..\include\units.h"\
	"..\..\include\utilexp.h"\
	"..\..\include\utillib.h"\
	"..\..\include\vedge.h"\
	"..\..\include\winutil.h"\
	".\type1.h"\
	

"$(INTDIR)\adobet1.obj" : $(SOURCE) $(DEP_CPP_ADOBE) "$(INTDIR)"


!ENDIF 

SOURCE=.\adobet1.rc

"$(INTDIR)\adobet1.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

