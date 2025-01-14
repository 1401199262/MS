//  shapi.h - Public API to the Symbol Handler
//
//    This file contains all types and APIs that are defined by
//    the Symbol Handler and are publicly accessible by other
//    components.
//
//    Before including this file, you must include cvtypes.h.


//  The master copy of this file resides in the CVINC project.
//    All Microsoft projects are required to use the master copy without
//    modification.  Modification of the master version or a copy
//    without consultation with all parties concerned is extremely
//    risky.

#ifndef _VC_VER_INC
#include "..\include\vcver.h"
#endif

#ifndef SH_API
#define SH_API

#define DECL_STR(n, v, s) n = v,
typedef enum {                  // Error returns from some SH functions
#include "sherror.h"
} SHE;
#undef DECL_STR

enum {
  sopNone  =  0,
  sopData  =  1,
  sopStack =  2,
  sopReg   =  4,
  sopLab   =  8,
  sopFcn   = 16,
  sopExact = 32
};
typedef short SOP;              // Symbol OPtions

typedef enum {
    fstNone,
    fstSymbol,
    fstPublic
} FST;                          // Function Symbol Type

typedef enum {
    fcdUnknown,
    fcdNear,
    fcdFar,
    fcdData
} FCD;                          // Function Call Distance (near/far/unknown)

typedef enum {
    fptUnknown,
    fptPresent,
    fptOmitted
} FPT;                          // Frame Pointer Type

typedef struct _ODR {
    FST     fst;
    FCD     fcd;
    FPT     fpt;
    WORD    cbProlog;
    DWORD   dwDeltaOff;
    LSZ     lszName;
} ODR;                          // OSDebug Return type
typedef ODR *LPODR;


typedef enum {
    astNone,
    astAddress,
    astRegister,
    astBaseOff
} AST;                          // Assembler symbol return Types

typedef struct _ASR {
    AST ast;
    union {
        struct {
            FCD  fcd;
            ADDR addr;
        };
        WORD   ireg;
        OFFSET off;
    };
} ASR;      // Assembler Symbol Return structure
typedef ASR *LPASR;

typedef HEMI            SHEMI;
typedef unsigned int    SHFLAG; // A TRUE/FALSE flag var
typedef SHFLAG *        PSHFLAG;
typedef SEGMENT         SHSEG;  // A segment/selector value
typedef UOFFSET         SHOFF;  // An offset value

typedef void *          HVOID;  // Generic handle type
typedef HIND            HMOD;   // A module handle
typedef HIND            HGRP;   // A group handle (sub group of module
                                //   currently either a seg or filename)
typedef HVOID           HPROC;  // A handle to a procedure
typedef HVOID           HBLK;   // A handle to a block.
typedef HVOID           HSF;    // A handle to source file table
typedef HIND            HEXE;   // An Executable file handle
typedef HVOID           HTYPE;  // A handle to a type
typedef HVOID           HSYM;   // A handle to a symbol
typedef HIND            HPDS;   // A handle to a process

typedef HSYM *          PHSYM;

typedef CV_typ_t        THIDX;

typedef struct CXT {
    ADDR  addr;
    HMOD  hMod;
    HGRP  hGrp;
    HPROC hProc;
    HBLK  hBlk;
} CXT;                          // General Symbol context pkt
typedef CXT *PCXT;

typedef struct CXF {
    CXT   cxt;
    FRAME Frame;
} CXF;                          // Symbol context pkt locked to a frame ptr
typedef CXF *PCXF;

typedef enum {
    SHFar,
    SHNear
} SHCALL;

typedef struct SHREG {
    unsigned short      hReg;
    union {
        unsigned char   Byte1;
        struct {
            unsigned short  Byte2;
            unsigned short  Byte2High;
        };
        struct {
            unsigned long Byte4;
            unsigned long Byte4High;
        };
        float  Byte4f;		// New field at DECs request
        double      Byte8;
        unsigned __int64 Byte8i;
        FLOAT10     Byte10;
    };
} SHREG;
typedef SHREG *PSHREG;

typedef struct _SLP {
    ADDR    addr;
    SHOFF   cb;
} SLP;                          // Source Line Pair (used by SLCAddrFromLine)
typedef SLP * LPSLP;

//  structure defining parameters of symbol to be searched for.  The address
//  of this structure is passed on the the EE's symbol compare routine.  Any
//  additional data required by the EE's routine must follow this structure.

typedef struct _SSTR {          // string with length byte and pointer to data
    LPB             lpName;     // pointer to the string itself
    unsigned char   cb;         // length byte
    unsigned char   searchmask; // mask to control symbol searching
    unsigned short  symtype;    // symbol types to be checked
    unsigned char * pRE;        // pointer to regular expression
} SSTR;
typedef SSTR *LPSSTR;

#define SSTR_proc       0x0001  // compare only procs with correct type
#define SSTR_data       0x0002  // compare only global data with correct type
#define SSTR_RE         0x0004  // compare using regular expression
#define SSTR_NoHash     0x0008  // do a linear search of the table
#define SSTR_symboltype 0x0010  // pass only symbols of symtype to the
                                //  comparison function.

#define SHpCXTFrompCXF(a)   (&((a)->cxt))
#define SHpFrameFrompCXF(a) (&(a)->Frame)
#define SHHMODFrompCXT(a)   ((a)->hMod)
#define SHHPROCFrompCXT(a)  ((a)->hProc)
#define SHHBLKFrompCXT(a)   ((a)->hBlk)
#define SHpADDRFrompCXT(a)  (&((a)->addr))
#define SHPAddrFromPCxf(a)  (SHpADDRFrompCXT(SHpCXTFrompCXF(a)))

#define SHIsCXTMod(a)       ((a)->hMod  && !(a)->hProc  && !(a)->hBlk)
#define SHIsCXTProc(a)      ((a)->hMod  &&  (a)->hProc  && !(a)->hBlk)
#define SHIsCXTBlk(a)       ((a)->hMod  &&  (a)->hProc  &&  (a)->hBlk)

#define SHHGRPFrompCXT(a)   ((a)->hGrp)

// comparison prototype
typedef SHFLAG  (FAR PASCAL *PFNCMP) (HVOID, HVOID, LSZ, SHFLAG);
typedef SHE     (FAR PASCAL *PFNVALIDATEEXE) (UINT, PVOID, LPSTR);
typedef BOOL    (FAR PASCAL *PFNVALIDATEDEBUGINFOFILE) (LPCSTR szFile, ULONG * errcode );

// definitions needed from pdb.h
typedef ULONG SIG;
typedef ULONG AGE;
struct PDB;
typedef struct PDB PDB;
typedef long EC;
#ifndef __PDB_INCLUDED__
enum { cbErrMax = 1024 };
#endif

typedef BOOL    (FAR PASCAL *PFNPDBOpenValidateEx) (
    /*const*/ char* szPDB,
    /*const*/ char* szPathOrig,
    /*const*/ char* szSearchPath,
    /*const*/ char* szMode,
    SIG sig,
    AGE age,
    EC* pec,
    char szError[cbErrMax],
    PDB** pppdb);


#define LPFNSYM     PASCAL *
#define LPFNSYMC    CDECL *

typedef struct omap_tag {
    DWORD       rva;
    DWORD       rvaTo;
} OMAP, *LPOMAP;

typedef struct {
    DWORD Offset;
    DWORD Size;
    DWORD Flags;
} SECSTART, *LPSECSTART;

typedef struct _tagDEBUGDATA {
    union {
        PIMAGE_RUNTIME_FUNCTION_ENTRY lpRtf;          // Runtime function table - fpo or pdata
    PFPO_DATA lpFpo;
    };
    DWORD       cRtf;           // Count of rtf entries
    LPOMAP      lpOmapFrom;     // Omap table - From Source
    DWORD       cOmapFrom;      // Count of omap entries - From Source
    LPOMAP      lpOmapTo;       // Omap table - To Source
    DWORD       cOmapTo;        // Count of omap entries - To Source
    LPSECSTART  lpSecStart;     // Original section table (pre-Lego)
    SHE         she;
	DWORD       dwOrigAlign;    // Original section alignment (pre-Lego)
} DEBUGDATA, *LPDEBUGDATA;

typedef struct _tagSEARCHDEBUGINFO {
    DWORD   cb;                         // doubles as version detection
    BOOL    fMainDebugFile;             // indicates "core" or "ancilliary" file
                                        // eg: main.exe has main.pdb and foo.lib->foo.pdb
    LSZ     szMod;                      // exe/dll
    LSZ     szLib;                      // lib if appropriate
    LSZ     szObj;                      // object file
    LSZ *   rgszTriedThese;             // list of ones that were tried,
                                        // NULL terminated list of LSZ's
    _TCHAR  szValidatedFile[_MAX_PATH]; // output of validated filename,
    PFNVALIDATEDEBUGINFOFILE
            pfnValidateDebugInfoFile;   // validation function
} SEARCHDEBUGINFO, *PSEARCHDEBUGINFO;

typedef struct _KNF {
    int   cb;

    PVOID (LPFNSYM lpfnMHAlloc)     (UINT);
    PVOID (LPFNSYM lpfnMHRealloc)   (PVOID, UINT);
    VOID  (LPFNSYM lpfnMHFree)      (PVOID);
    PVOID (LPFNSYM lpfnMHAllocHuge) (LONG, UINT);
    VOID  (LPFNSYM lpfnMHFreeHuge)  (PVOID);

    HDEP  (LPFNSYM lpfnMMAllocHmem) (UINT);
    VOID  (LPFNSYM lpfnMMFreeHmem)  (HDEP);
    PVOID (LPFNSYM lpfnMMLock)      (HDEP);
    VOID  (LPFNSYM lpfnMMUnlock)    (HDEP);

    HLLI  (LPFNSYM lpfnLLInit)      (DWORD, LLF, LPFNKILLNODE, LPFNFCMPNODE);
    HLLE  (LPFNSYM lpfnLLCreate)    (HLLI);
    VOID  (LPFNSYM lpfnLLAdd)       (HLLI, HLLE);
    VOID  (LPFNSYM lpfnLLAddHead)   (HLLI, HLLE);
    VOID  (LPFNSYM lpfnLLInsert)    (HLLI, HLLE, DWORD);
    BOOL  (LPFNSYM lpfnLLDelete)    (HLLI, HLLE);
    BOOL  (LPFNSYM lpfnLLRemove)    (HLLI, HLLE);
    DWORD (LPFNSYM lpfnLLDestroy)   (HLLI);
    HLLE  (LPFNSYM lpfnLLNext)      (HLLI, HLLE);
    HLLE  (LPFNSYM lpfnLLFind)      (HLLI, HLLE, PVOID, DWORD);
    HLLE  (LPFNSYM lpfnLLLast)      (HLLI);
    DWORD (LPFNSYM lpfnLLSize)      (HLLI);
    PVOID (LPFNSYM lpfnLLLock)      (HLLE);
    VOID  (LPFNSYM lpfnLLUnlock)    (HLLE);

    BOOL  (LPFNSYM lpfnLBPrintf)    (LPCH, LPCH, DWORD);
    BOOL  (LPFNSYM lpfnLBQuit)      (DWORD);

        UINT  (LPFNSYM lpfnSYOpen)      (LSZ);
    VOID  (LPFNSYM lpfnSYClose)     (UINT);
    UINT  (LPFNSYM lpfnSYReadFar)   (UINT, LPB, UINT);
    LONG  (LPFNSYM lpfnSYSeek)      (UINT, LONG, UINT);
    BOOL  (LPFNSYM lpfnSYFixupAddr) (PADDR);
    BOOL  (LPFNSYM lpfnSYUnFixupAddr)(PADDR);
    UINT  (LPFNSYM lpfnSYProcessor) (DWORD);

// Added/Changed for NT merge.

//    VOID  (LPFNSYM lpfn_searchenv)  (LSZ, LSZ, LSZ);
//    UINT  (LPFNSYMC lpfnsprintf)    (LSZ, LSZ, ...);
//    VOID  (LPFNSYM lpfn_splitpath)  (LSZ, LSZ, LSZ, LSZ, LSZ);
//    LSZ   (LPFNSYM lpfn_fullpath)   (LSZ, LSZ, UINT);
//    VOID  (LPFNSYM lpfn_makepath)   (LSZ, LSZ, LSZ, LSZ, LSZ);
//    UINT  (LPFNSYM lpfnstat)        (LSZ, LPCH);

    LONG  (LPFNSYM lpfnSYTell)          (UINT);
    UINT  (LPFNSYM lpfnSYFindExeFile)   (LSZ, LSZ, UINT, PVOID, PFNVALIDATEEXE);
    VOID  (LPFNSYM lpfnLoadedSymbols)   (SHE, HPID, LSZ);
    BOOL  (LPFNSYM lpfnSYGetDefaultShe) (LSZ, SHE *);

// Added for separate type pool work

    BOOL  (LPFNSYM pfnSYFindDebugInfoFile) ( PSEARCHDEBUGINFO );
    BOOL  (WINAPI * lpfnGetRegistryRoot)            ( LPTSTR, LPDWORD );

#ifdef NT_BUILD_ONLY
// Added for WinDbg back port 
    BOOL    (LPFNSYM lpfnSetProfileString)(LPCSTR, LPCSTR);
    BOOL    (LPFNSYM lpfnGetProfileString)(LPCSTR, LPSTR, ULONG, ULONG *);
#endif

} KNF;  // KerNel Functions exported to the Symbol Handler
typedef KNF *LPKNF;

typedef struct _SHF {
    int     cb;
    HPDS    (LPFNSYM pSHCreateProcess)      (VOID);
    VOID    (LPFNSYM pSHSetHpid)            (HPID);
    BOOL    (LPFNSYM pSHDeleteProcess)      (HPDS);
    HPDS    (LPFNSYM pSHChangeProcess)      (HPDS);
    SHE     (LPFNSYM pSHAddDll)             (LSZ, BOOL);          // Changed for NT
    SHE     (LPFNSYM pSHAddDllsToProcess)   (VOID);
    SHE     (LPFNSYM pSHLoadDll)            (LSZ, BOOL);
    VOID    (LPFNSYM pSHUnloadDll)          (HEXE);
    UOFFSET (LPFNSYM pSHGetDebugStart)      (HSYM);
    LSZ     (LPFNSYM pSHGetSymName)         (HSYM, LSZ);
    BOOL    (LPFNSYM pSHAddrFromHsym)       (PADDR, HSYM);        // Changed for NT
    HMOD    (LPFNSYM pSHHModGetNextGlobal)  (HEXE *, HMOD);
    int     (LPFNSYM pSHModelFromAddr)      (PADDR, LPW, LPB, UOFFSET *);
    int     (LPFNSYM pSHPublicNameToAddr)   (PADDR, PADDR, LSZ);
    LSZ     (LPFNSYM pSHGetSymbol)          (LPADDR, LPADDR, SOP, LPODR);
    BOOL    (LPFNSYM pSHGetPublicAddr)      (PADDR, LSZ);
    BOOL    (LPFNSYM pSHIsLabel)            (HSYM);

    VOID    (LPFNSYM pSHSetDebuggeeDir)     (LSZ);
//    VOID    (LPFNSYM pSHSetUserDir)         (LSZ);                // Deleted for NT
    BOOL    (LPFNSYM pSHAddrToLabel)        (PADDR, LSZ);

    int     (LPFNSYM pSHGetSymLoc)          (HSYM, LSZ, UINT, PCXT);
    BOOL    (LPFNSYM pSHFIsAddrNonVirtual)  (PADDR);
    BOOL    (LPFNSYM pSHIsFarProc)          (HSYM);

    HEXE    (LPFNSYM pSHGetNextExe)         (HEXE);
    HEXE    (LPFNSYM pSHHexeFromHmod)       (HMOD);
    HMOD    (LPFNSYM pSHGetNextMod)         (HEXE, HMOD);
    PCXT    (LPFNSYM pSHGetCxtFromHmod)     (HMOD, PCXT);
    PCXT    (LPFNSYM pSHSetCxt)             (PADDR, PCXT);
    PCXT    (LPFNSYM pSHSetCxtMod)          (PADDR, PCXT);
    HSYM    (LPFNSYM pSHFindNameInGlobal)   (HSYM,
                                             PCXT,
                                             LPSSTR,
                                             SHFLAG,
                                             PFNCMP,
                                             PCXT
                                            );
    HSYM    (LPFNSYM pSHFindNameInContext)  (HSYM,
                                             PCXT,
                                             LPSSTR,
                                             SHFLAG,
                                             PFNCMP,
                                             PCXT
                                            );
    HSYM    (LPFNSYM pSHGoToParent)         (PCXT, PCXT);
    HSYM    (LPFNSYM pSHHsymFromPcxt)       (PCXT);
    HSYM    (LPFNSYM pSHNextHsym)           (HMOD, HSYM);
    PCXF    (LPFNSYM pSHGetFuncCXF)         (PADDR, PCXF);
    LPCH    (LPFNSYM pSHGetModName)         (HMOD);
    LPCH    (LPFNSYM pSHGetExeName)         (HEXE);
    HEXE    (LPFNSYM pSHGethExeFromName)    (LPCH);
    UOFF32  (LPFNSYM pSHGetNearestHsym)     (PADDR, HMOD, int, PHSYM);
    SHFLAG  (LPFNSYM pSHIsInProlog)         (PCXT);
    SHFLAG  (LPFNSYM pSHIsAddrInCxt)        (PCXT, PADDR);
    SHFLAG  (LPFNSYM pSHCompareRE)          (LPCH, LPCH, BOOL);
    BOOL    (LPFNSYM pSHFindSymbol)         (LSZ, PADDR, LPASR);
    UOFF32  (LPFNSYM pPHGetNearestHsym)     (PADDR, HEXE, PHSYM);
    HSYM    (LPFNSYM pPHFindNameInPublics)  (HSYM, HEXE, LPSSTR, SHFLAG, PFNCMP);
    HTYPE   (LPFNSYM pTHGetTypeFromIndex)   (HMOD, THIDX);
    HTYPE   (LPFNSYM pTHGetNextType)        (HMOD, HTYPE);
    PVOID   (LPFNSYM pSHLpGSNGetTable)      (HEXE);
    BOOL    (LPFNSYM pSHCanDisplay)         (HSYM);

    //  Source Line handler API Exports

    BOOL    (LPFNSYM pSLLineFromAddr)       (LPADDR, LPW, SHOFF *, SHOFF *);
    BOOL    (LPFNSYM pSLFLineToAddr)        (HSF, WORD, LPADDR, SHOFF *, WORD *);
    LPCH    (LPFNSYM pSLNameFromHsf)        (HSF);
    LPCH    (LPFNSYM pSLNameFromHmod)       (HMOD, WORD);
    BOOL    (LPFNSYM pSLFQueryModSrc)       (HMOD);
    HMOD    (LPFNSYM pSLHmodFromHsf)        (HEXE, HSF);
    HSF     (LPFNSYM pSLHsfFromPcxt)        (PCXT);
    HSF     (LPFNSYM pSLHsfFromFile)        (HMOD, LSZ);

    int     (LPFNSYM pSLCAddrFromLine)      (HEXE, HMOD, LSZ, WORD, LPSLP *);
    VOID    (LPFNSYM pSHFree)               (PVOID);
    VOID    (LPFNSYM pSHUnloadSymbolHandler)(BOOL);
// REVIEW: piersh
#ifdef NT_BUILD_ONLY
    SHE     (LPFNSYM pSHGetExeTimeStamp)    (LPSTR, ULONG *);
#else
    SHE     (LPFNSYM pSHGetExeTimeStamp)    (LPSTR, ULONG* Time, ULONG* Check);
#endif
    VOID    (LPFNSYM pSHPdbNameFromExe)     (LSZ, LSZ, UINT);
    LPVOID  (LPFNSYM pSHGetDebugData)   (HEXE);
    BOOL    (LPFNSYM pSHIsThunk)            (HSYM, THUNK_ORDINAL*, ULONG* length);
    HSYM    (LPFNSYM pSHFindSymInExe)       (HEXE, LPSSTR, BOOL);
    HSYM    (LPFNSYM pSHFindSLink32)        (PCXT);
    BOOL    (LPFNSYM pSHIsDllLoaded)        (HEXE);

// Entries added for NT work.

    LSZ     (LPFNSYM pSHGetModule)          (PADDR, LSZ);
    PCXT    (LPFNSYM pSHGetCxtFromHexe)     (HEXE, PCXT);
    LPCH    (LPFNSYM pSHGetModNameFromHexe) (HEXE);
    LPCH    (LPFNSYM pSHGetSymFName)        (HEXE);
    HEXE    (LPFNSYM pSHGethExeFromModuleName) (LPCH);
    LSZ     (LPFNSYM pSHLszGetErrorText)    (SHE);
    BOOL    (LPFNSYM pSHWantSymbols)        (HEXE);
    HSYM    (LPFNSYM pSHFindNameInTypes)    ( PCXT, LPSSTR, SHFLAG, PFNCMP, PCXT );

// Entries added for separate type pools work
    BOOL    (LPFNSYM pTHAreTypesEqual)      (HMOD, CV_typ_t, CV_typ_t);

// LoadDllEx is for V7 only (right now)
	SHE     (LPFNSYM pSHLoadDllEx)          (LSZ	szName,
											 BOOL	fLoading,
											 ULONG	TimeStamp,
											 ULONG	CheckSum,
											 ULONG	LoadAddress,
											 HEXE*	lphexe);
											 
// Entries for Edit and Continue work
    LSZ     (LPFNSYM pSHGetSrcPath) (HMOD);
    LSZ     (LPFNSYM pSHGetObjPath) (HMOD);
    LSZ     (LPFNSYM pSHGetCompileEnv) (HMOD);
    BOOL    (LPFNSYM pSHModSupportsEC)(HMOD);
    BOOL    (LPFNSYM pSHExeSupportsEC) (HEXE);
    BOOL    (LPFNSYM pSHModLinkedFromLib) (HMOD);

    BOOL    (LPFNSYM pSHModFromHmod) ( HMOD, PHANDLE );	// get the underlying Pdb Mod
    BOOL    (LPFNSYM pSHFlushModSyms)( HMOD );	// flush symbols for mod
    void    (LPFNSYM pSHUseAltPdb) ( PFNPDBOpenValidateEx );

} SHF;  // Symbol Handler Functions
typedef SHF *LPSHF;

// FNSHINIT is the prototype for the SHInit function

typedef BOOL EXPCALL FNSHINIT(LPSHF *, LPKNF);

typedef FNSHINIT * LPFNSHINIT;
typedef FNSHINIT * PFNSHINIT;

typedef BOOL    (* LPFNSHUNINIT)(VOID);
typedef BOOL    (* LPFNSHSTARTBACKGROUND)(VOID);
typedef BOOL    (* LPFNSHSTOPBACKGROUND)(VOID);

// This is the only SH function that's actually exported from the DLL
FNSHINIT SHInit;

#endif // SH_API
