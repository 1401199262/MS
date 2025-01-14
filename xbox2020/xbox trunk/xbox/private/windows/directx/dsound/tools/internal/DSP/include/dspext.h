
/**
*	DSPEXT - DSP COFF definition extensions
**/

struct opthdr {	/* original (old) linker optional header */
	long modsize;	/* module size */
	long datasize;	/* raw data size */
	long endstr;	/* end expression string */
	long secnt;	/* logical section count */
	long ctrcnt;	/* counter count */
	long relocnt;	/* relocation entry count */
	long lnocnt;	/* line number entry count */
	long bufcnt;	/* buffer count */
	long ovlcnt;	/* overlay count */
	long dumstk;	/* debugger dummy stack size */
};
#define OPTHDR struct opthdr
#define OPTHSZ (sizeof(OPTHDR))

/*
  NOTE:	Currently the linker depends on the fields modsize through
  	ovlcnt to be in the same relative position in both opthdr
	and opthdr2.
*/

struct opthdr2 {	/* linker optional header */
	long modsize;	/* module size */
	long datasize;	/* raw data size */
	long endstr;	/* end expression string */
	long secnt;	/* logical section count */
	long ctrcnt;	/* counter count */
	long relocnt;	/* relocation entry count */
	long lnocnt;	/* line number entry count */
	long bufcnt;	/* buffer count */
	long ovlcnt;	/* overlay count */
	long majver;	/* major version number */
	long minver;	/* minor version number */
	long revno;	/* revision number */
	long optflags;	/* optional header flags */
	long sditot;	/* count of SDIs in module */
};
#define OPTHDR2 struct opthdr2
#define OPTH2SZ (sizeof(OPTHDR2))

union xcnhdr {	/* extended COFF section header */
	union {
		char _s_name[SYMNMLEN];
		struct {
			long	_s_zeroes;
			long	_s_offset;
		} _s_n;
	} _n;
	SCNHDR	_s;
};
#define XCNHDR	union xcnhdr
#define XCNHSZ	sizeof (XCNHDR)

#define scn_name	xcn->_n._s_name
#define scn_zeroes	xcn->_n._s_n._s_zeroes
#define scn_offset	xcn->_n._s_n._s_offset
#define scn_paddr	xcn->_s.s_paddr.w0.u
#define scn_pmap	xcn->_s.s_paddr.w1.mape
#define scn_vaddr	xcn->_s.s_vaddr.w0.u
#define scn_vmap	xcn->_s.s_vaddr.w1.mape
#define scn_size	xcn->_s.s_size
#define scn_scnptr	xcn->_s.s_scnptr
#define scn_relptr	xcn->_s.s_relptr
#define scn_lnnoptr	xcn->_s.s_lnnoptr
#define scn_nreloc	xcn->_s.s_nreloc
#define scn_nlnno	xcn->_s.s_nlnno
#define scn_flags	xcn->_s.s_flags

struct mematt {		/* memory attribute structure */
	enum memory_map mspace; /* memory space */
	enum memory_map mmap;   /* memory mapping */
	long mcntr;             /* counter */
	long mclass;		/* memory class */
};

union auxlnk {	/* original (old) linker auxiliary symbol structure */
	struct {
		long secno;
		long rsecno;
		long mem;
		long flags;
		union {
			struct {
				long bufcnt;
				long buftyp;
				long buflim;
			} buf;
			struct {
				long ovlcnt;
				long ovlmem;
				long ovlstr;
			} ovl;
		} bufovl;
	} aux;
	SYMENT filler;	/* to fill out to size of SYMENT */
};
#define AUXLNK union auxlnk
#define AUXLSZ (sizeof(AUXLNK))

/*
  NOTE:	Currently the linker depends on the fields secno and
  	rsecno to be in the same relative position in both auxlnk
	and auxlnk1.
*/

union auxlnk1 {	/* linker first auxiliary symbol structure */
	struct {
		long secno;
		long rsecno;
		long flags;
		struct mematt mem;
	} aux;
	SYMENT filler;	/* to fill out to size of SYMENT */
};
#define AUXLNK1 union auxlnk1
#define AUXL1SZ (sizeof(AUXLNK1))

union auxlnk2 {	/* linker second auxiliary symbol structure */
	union {
		struct {
			long bufcnt;
			long buftyp;
			long buflim;
		} buf;
		struct {
			struct mematt ovlmem;
			long ovlcnt;
			long ovlstr;
			unsigned long ovloff;
		} ovl;
	} bufovl;
	SYMENT filler;	/* to fill out to size of SYMENT */
};
#define AUXLNK2 union auxlnk2
#define AUXL2SZ (sizeof(AUXLNK2))

#define F_CC	0x00010000L	/* object file produced via C compiler */
#define F_SDI   0x00020000L	/* object file contains span-dep. instr. */

#define C_SECT  128L		/* special class for nested section symbols */
#define C_SDI	129L		/* special class for SDI symbol record */

#define T_MOD	1L		/* special module file type (maps to T_ARG) */

#define _TXT	".txt"		/* .txt section name */
#define _DAT	".dat"		/* .dat section name */
#define _CMT	".cmt"		/* .cmt comment symbol name */
#define _SDI	".sdi"		/* .sdi span-dependent instruction name */
#define _BS	".bs"		/* .bs begin section symbol name */
#define _ES	".es"		/* .es end section symbol name */
#define _TX	".tx"		/* .tx section debug name */
#define _DA	".da"		/* .da section debug name */
#define _XR	".xr"		/* .xr xref symbol name */
#define _BM	".bm"		/* .bm begin macro symbol name */
#define _EM	".em"		/* .em end macro symbol name */
