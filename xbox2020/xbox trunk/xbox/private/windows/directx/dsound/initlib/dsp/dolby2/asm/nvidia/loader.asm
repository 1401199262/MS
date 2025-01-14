; ******************************************************************
;
; 	Unpublished work.  Copyright 2000 Dolby Laboratories Inc.
; 	All Rights Reserved.
;												
;	File:			loader.asm
;												
;   Contents:		Loader for Dolby Game Encoder
;
;	Author:			jmw
;											
;	Revision:		1.00b
;											
;	History:
;
;		Rev 1.00	10/11/2000 7:23:25 AM		jmw
;	          Created.
;               Rev 1.00b       12/11/2000 4:11:12 PM           Ethan Brodsky
;        	  brought DMA code over from Dolby/v1.20 package
;
;
; ******************************************************************

	page	132,66,3,3
	opt		cre,loc,cc,cex,mex,mu,so

	title	'Dolby Loader'

	section	Loader

;**** include files ************************************************

	nolist
	include 'ldr_sip.inc'
	include 'eputil.h'    ; EP constants
	include 'dmautil.h'   ; DMA constants
	list

;**** equates ******************************************************

	;DEFINE	ERRCHK	'1'			; Enable error checking

	IF		!@DEF('RTSYS')
	DEFINE	CHKSUM	'0'			; Enable program checksum
	ENDIF

	DEFINE	PREAMBLE	'1'		; Enable AC3 SPDIF Preamble
	DEFINE	ZEROFILL	'1'		; Enable AC3 SPDIF Zero Fill


;**** macros ******************************************************


debug_wait macro
_wait_for_flag
    move    p:debug_enable,a
    cmp     #0,a
    beq     _wait_no_debug
    move    p:debug_wait_flag,a
    cmp     #0,a
	beq     _wait_for_flag
_wait_no_debug
    move    #0,x0
    move    x0,p:debug_wait_flag
	endm

;**** program code *************************************************

	xref	LdrTable


	org		p(100):				; Loader Runtime Counter is 100

;******************************************************************************
;*										  
;*   Subroutine Name:	Loader
;*								  
;*   Description:		Loader
;*										      
;*   Input:				a	- Buffer ID word
;*						b	- Loader action
;*						r0	- DSP memory address
;*						n0	- number of words to transfer
;*						n1	- System memory offset
;*
;*   Output:			a	- error return code: nonzero if error
;*
;*   Modifies:			
;*
;*	 Locals:			
;*
;*	 Stack Space Used:	
;*										      
;******************************************************************************

loader:
	move	#LdrTable,r2			; set up loader table base
	move	a1,n2					; set up loader table offset
    move    #1,n5                   ; set Flag indicating DMAs are blocking type
	

;	Check Buffer ID to see what to do, check for errors along the way

	IF		@DEF('ERRCHK')
	cmp		#LID_MIN,a				; is ID too small?
	blt		<abort_id				; if so, abort
	cmp		#LID_MAX,a				; is ID too big?
	bgt		<abort_id				; if so, abort
	ENDIF


	cmp		#LID_LOADER_INIT,a		; loader init request?
	beq		<loader_init		    ; if so, go init the loader

	move	(r2)+n2					; point into table (halfway)
	move	(r2)+n2					; point into table
	move	p:(r2)+,r1				; get system memory address
							; (or FIFO index, in case of ac3/rtlt)

	cmp		#LID_BUF_DATA,a			; is ID a heap data buffer?
	bge		<heap_id				; if so, go parse heap buffer params
	cmp		#LID_MAX_PROG,a			; is ID a program?
	ble		<load_program			; if so, go load program
	cmp		#LID_BUF_AC3,a			; is ID for output AC-3?
	beq		<save_ac3				; if so, save AC-3
	cmp		#LID_CONFIG_LIST,a		; is ID system configuration data?
	beq		<load_config			; if so, go load configuration list
	cmp		#LID_PING_PONG,a		; should we toggle the PCM input ping pong
	beq		<toggle_ping_pong

	cmp		#LID_AC3_ZEROS,a		; is ID for zero fill of AC-3 output buffer?
	beq		<SPDIFzerofill			; if so, go zero fill
	cmp		#LID_AC3_PREAMBLE,a		; is ID to write AC3 S/PDIF preamble?
	beq		<SPDIFpreamble			; if so, go write SPDIF preamble

	cmp		#LID_BUF_LTRT,a
	beq		<save_ltrt				; save Lt/Rt

	bra		<load_pcm				; if so, load pcm

;***********************************************************************
;	Data storage for loader variables

ldr_heap_ptr		dc		0		; current heap pointer
ldr_heap_size		dc		0		; current heap size
	IF		@DEF('CHKSUM')
ldr_r0_copy			dc		0		; copy of r0 input argument
ldr_n0_copy			dc		0		; copy of n0
	ENDIF	; @DEF('CHKSUM')

debug_wait_flag			dc      0		;
debug_enable            dc      1

dmaNode	                dc	    0
dmaNode1                dc	    0
dmaNode2	            dc	    0
dmaNode3	            dc	    0
dmaNode4	            dc	    0
dmaNode5	            dc	    0
dmaNode6	            dc	    0



;***********************************************************************
;	Load configuration data from system
;		uses input DSP address, gets length from loader table
;		ignores input action, offset, and length

load_config:

	move	p:(r2)+,a				; get length from Sys Mem
	move	n0,x0					; copy max length to x0
	cmp		x0,a a1,n0				; compare two lengths. preload length
	IF		@DEF('ERRCHK')
	bgt		<abort_length			; if SysMem length to large, fail
	ENDIF

	bsr		<CopySysToDSPX			; copy list

	bra		<done

;***********************************************************************
;	Load a program
;		uses input DSP address, gets length from loader table
;		ignores input action, offset, and length
;		error checking for program checksum

load_program:
	move	p:(r2)+,n0				; get program size
	IF		@DEF('CHKSUM')
	move	r0,p:ldr_r0_copy		; save r0
	move	n0,p:ldr_n0_copy		; save n0
	ENDIF	; @DEF('CHKSUM')
	bsr		<CopySysToDSPP			; copy program
	IF		@DEF('CHKSUM')
	clr		a						; init checksum
	move	p:ldr_n0_copy,n0		; restore n0
	move	p:ldr_r0_copy,r0		; restore r0
	dor		n0,checksumlp
	move	p:(r0)+,x0				; get DSP word
	add		x0,a					; add to checksum
checksumlp:
	move	#>0,a2					; clear msword, use as return code
	rts								; return from here
	ELSE
	bra		<done					; exit
	ENDIF



;***********************************************************************
;	Toggle ping pong buffer on PCM input

toggle_ping_pong:
	move	p:LdrTable+LDR_PINGPONG_OFFSET,x0
	bchg	#8,x0                               
	move	x0,p:LdrTable+LDR_PINGPONG_OFFSET

	bra		<done					; exit

;***********************************************************************
;	Load input PCM data
;		uses input DSP address, length, offset
;		ignores input action
;		error checking for buffer overflow

load_pcm:

;	The following lines convert an interleave buffer index into an offset
;	suitable for contiguous audio buffers.  

	move	n1,b					; get interleave buffer index
	asl		#9,b,b					; multiply by 512
	move	r1,x0					; rewritten to avoid pipeline stall
	add		x0,b
    move	p:LdrTable+LDR_PINGPONG_OFFSET,a
	add		b,a
	move	a1,r1

load_pcm_block0:
	bsr		<CopySysToDSPX			; copy buffer data

	bra		<done					; exit




;***********************************************************************
;	Save output Lt/Rt data
;		uses input DSP address, length, offset
;		ignores input action
;		error checking for buffer overflow
;	Note:	FIFO index is in r1 instead of system memory address

save_ltrt:

	;ignore the second save_ltrt, we do interleave
	move	n1,a
	cmp		#0,a
	bne		<done

	move	#1,n2						;This flag indicates number of channels -1 to interleave
	move	#0,n4                       ;This flag indicates null ouput
	bsr		<CopyDSPXToFIFO


	bra		<done					; exit

;***********************************************************************
;	Save output AC-3 data
;		uses input DSP address, length, offset
;		ignores input action
;		error checking for buffer overflow
;	Note:	FIFO index is in r1 instead of system memory address

save_ac3:

;;;;;; Note: Dolby memory-based code
;;;;;	move	(r1)+n1					; adjust output pointer
;       Note: XXXXX TODO - this might be a problem - we've switched to a FIFO,
;             so we can't support the n1 offset into the buffer.  Hopefully
;             Dolby will be writing everything in order...
;;;;;	bsr		<CopyDSPXToSys			; copy buffer data


	move	#0,n2						;This flag indicates number of channels -1 to interleave
	move	#0,n4                       ;This flag indicates null ouput
    ;move    #0,n5                       ;Clear blocking flag
	bsr		<CopyDSPXToFIFO
;	Note: 	this currently blocks until DMA completion...  If we're working
;		with a FIFO, will it complete quickly or slowly?  If slowly,
;		we probably need to make this non-blocking.  In which case we
;		need to have multiple DMA nodes instead of using a single node
;		for everything...

	bra		<done					; exit




;***********************************************************************
; Zero Fill AC-3 output buffer
;	uses length (n0), offset (n1)


SPDIFzerofill:
	IF		@DEF('ZEROFILL')

	move	#0,n2						;This flag indicates number of channels -1 to interleave
	move	#1,n4                       ;This flag indicates null ouput
    move    #0,n5                       ;Clear blocking flag
	bsr		<CopyDSPXToFIFO

    ENDIF
	bra		<done					; exit




;***********************************************************************
; Load preamble from DSP X memory
;	(essentially the same routine as used for "save_ac3")
;	uses input DSP address (r0), offset (n1), length (n0)

SPDIFpreamble:

	IF		@DEF('PREAMBLE')

	move	#0,n2						;This flag indicates number of channels -1 to interleave
	move	#0,n4                       ;This flag indicates null ouput
	bsr		<CopyDSPXToFIFO
;	Note: 	this currently blocks until DMA completion...  If we're working
;		with a FIFO, will it complete quickly or slowly?  If slowly,
;		we probably need to make this non-blocking.  In which case we
;		need to have multiple DMA nodes instead of using a single node
;		for everything...

    ENDIF
	bra		<done					; exit


;***********************************************************************
;	Heap handler

heap_jmptbl		dc		CopySysToDSPX,CopyDSPXToSys
				dc		CopySysToDSPY,CopyDSPYToSys
				dc		CopySysToDSPP,CopyDSPPToSys

heap_id:
	cmp		#LDR_ACT_ALLOC,b		; is action alloc?
	beq		<alloc_heap				; if so, skip ahead

;***********************************************************************
;	Heap access code
;		Transfers data to/from allocated data buffers
;		Uses all input parameters
;		Checks for following errors:
;			Buffer not yet allocated
;			Invalid length
;			Invalid offset
;			Buffer overflow

access_heap:
	IF		@DEF('ERRCHK')
	tst		b r1,a					; is action too small?, copy buffer pointer
	ble		<abort_action			; if so, abort
	cmp		#LDR_ACT_MAX,b			; is action too big?
	bgt		<abort_action			; if so, abort
	tst		a n0,a					; is the buffer allocated?	get length
	beq		<abort_alloc			; if not, abort
;	move	n0,a					; get length (no longer needed, done above)
	tst		a a1,x0					; is length valid?
	ble		<abort_length			; if not, abort
	move	n1,a					; get offset
	tst		a						; is offset valid?
	blt		<abort_length			; if not, abort
	add		x0,a					; add together
	move	p:(r2)+,x0				; get buffer size
	cmp		x0,a					; check for overflow
	bgt		<abort_length			; if so, abort
	ENDIF

	move	b1,n4					; set up table index
	move	#heap_jmptbl-1,r4		; set up jump table base
	move	(r1)+n1					; adjust pointer
	move	p:(r4+n4),r6			; get jump address
	jsr		(r6)					; call handler routine
	bra		<done

;***********************************************************************
;	Heap allocation code
;		Allocates new data buffer from heap
;		Uses input length, ignores other parameters
;		Checks for following errors:
;			Buffer already allocated
;			Requested length <= 0
;			Heap underflow

alloc_heap:
	IF		@DEF('ERRCHK')
	move	r1,a					; copy buffer pointer
	tst		a n0,b					; is the buffer allocated?, copy length
	bne		<abort_alloc			; if so, abort
	tst		b						; is the length valid?
	ble		<abort_alloc			; if not, abort
	ELSE
	move	n0,b					; copy length
	ENDIF

	move	p:ldr_heap_size,a		; get current heap size
	move	p:ldr_heap_ptr,x0		; get current heap pointer
	sub		b,a						; update heap size
	blt		<abort_alloc			; if underflow, abort
	add		x0,b (r2)-				; update heap pointer, back up table ptr
	move	x0,p:(r2)+				; save buffer pointer to loader table
	move	n0,p:(r2)+				; save buffer size to loader table
	move	a1,p:ldr_heap_size		; save heap size
	move	b1,p:ldr_heap_ptr		; save heap pointer

;***********************************************************************
;	Exit points

done:

	clr		a						; no error status
	rts

abort_alloc:
	move	#>LDR_ERR_ALLOC,a		; set up error status
	rts
	IF		@DEF('ERRCHK')
abort_id:
	move	#>LDR_ERR_ID,a			; set up error status
	rts
abort_action:
	move	#>LDR_ERR_ACTION,a		; set up error status
	rts
abort_length:
	move	#>LDR_ERR_LENGTH,a		; set up error status
	rts
	ENDIF
												  
;***********************************************************************
;	Loader initialization code
;		Copies loader table from system memory to DSP memory
;		Clears dynamic table entries
;		Initializes local heap variables

loader_init:
; Note: we no longer need to copy the loader table, since the RM prepositions
;       it in system memory so that it is automatically loaded by the DSP
;       bootstrap.
;	move	#LdrTable,r0				; point to loader table
;	move	#LDR_SYSTABLE_SIZE,n0			; set up length
;	move	#SYSMEMTABLEBASE,r1		   	; point to SysMem loader table
;	bsr		<CopySysToDSPP			; copy loader table
	move	p:LdrTable+LDR_HEAP_ADDR,x0		; get heap address
	move	x0,p:ldr_heap_ptr			; save to local memory
	move	p:LdrTable+LDR_HEAP_SIZE,x0		; get heap size
	move	x0,p:ldr_heap_size			; save to local memory
	clr	a #LdrTable+LDR_HEAP_ADDR,r0		; set up pointer
	dor	#LDR_NUM_DATA*2,clrlp
	move	a,p:(r0)+						; clear table value
clrlp:


	bsr	DMAInit					; one-time DMA initialization
	
	bra	<done					; exit

;******************************************************************************
;*										  
;*   Subroutine Name:	CopySysToDSPX
;*								  
;*   Description:	Copy from System Memory to DSP x: space
;*										      
;*   Input:		r0 	- DSP address (x:)
;*			n0	- number of words to copy
;*			r1	- System Memory address
;*										      
;******************************************************************************

CopySysToDSPX:
	IF		@DEF('RTSYS')
	jsr		set_timing
	ENDIF

; Note: Dolby-only testing code
;	dor		n0,systoxlp
;	move	x:(r1)+,x0
;	move	x0,x:(r0)+
;systoxlp:

	move	#dmaNode,r4			; r4 <- address of dma node
	
	move	r0,a1
	add 	#kEPMemory_Base_XRAM,a 		; adjust DSP address for X base
	move	a1,r0
	
	jsr	DMANewNode_Read			; create DMA node
	
	jsr	DMAStartNodeP_Wait		; initiate DMA 
						; (and wait for completion)

	IF		@DEF('RTSYS')
	jsr		clr_timing
	ENDIF
	rts

;******************************************************************************
;*										  
;*   Subroutine Name:	CopyDSPXToSys
;*								  
;*   Description:	Copy from DSP x: space to System Memory
;*										      
;*   Input:		r0 	- DSP address (x:)
;*			n0	- number of words to copy
;*			r1	- System Memory address
;*										      
;******************************************************************************

CopyDSPXToSys:
	IF		@DEF('RTSYS')
	jsr		set_timing
	ENDIF
	
; Note: Dolby-only testing code
;	dor		n0,xtosyslp
;	move	x:(r0)+,x0
;	move	x0,x:(r1)+
;xtosyslp:

	move	#dmaNode,r4			; r4 <- address of dma node
	
	move	r0,a1
	add 	#kEPMemory_Base_XRAM,a 		; adjust DSP address for X base
	move	a1,r0
	
	jsr	DMANewNode_Write		; create DMA node
	
	jsr	DMAStartNodeP_Wait		; initiate DMA 
						; (and wait for completion)
        
	IF		@DEF('RTSYS')
	jsr		clr_timing
	ENDIF
	rts

;******************************************************************************
;*										  
;*   Subroutine Name:	CopyDSPXToFIFO
;*								  
;*   Description:	Copy from DSP x: space to FIFO
;*									      
;*   Input:		r0 	- DSP address (x:)
;*			n0	- number of words to copy
;*			r1	- FIFO index
;*			n2  - (Channels -1) to interleave
;*			n4  - Flag indicating NULL ouput
;*										      
;******************************************************************************

CopyDSPXToFIFO:

	move	#dmaNode,r4			; r4 <- address of dma node
	
	move	r0,a1
	add 	#kEPMemory_Base_XRAM,a 		; adjust DSP address for X base
	move	a1,r0
	
	move	n2,a
	cmp		#0,a
	bne		CopyDSPXToFIFO_Interleaved
	jsr	    DMANewNode_WriteFIFO		; create DMA node
	bra		CopyDSPXToFIFOStart

CopyDSPXToFIFO_Interleaved:	
	jsr	    DMANewNode_WriteFIFOInterleaved		; create DMA node

CopyDSPXToFIFOStart:
	jsr	    DMAStartNodeP_Wait		; initiate DMA 
						; (and wait for completion)
        
	rts

;******************************************************************************
;*										  
;*   Subroutine Name:	CopySysToDSPY
;*								  
;*   Description:	Copy from System Memory to DSP y: space
;*		       							      
;*   Input:	 	r0 	- DSP address (y:)
;*			n0	- number of words to copy
;*			r1	- System Memory address
;*										      
;******************************************************************************

CopySysToDSPY:

; Note: Dolby-only testing code
;	dor		n0,systoylp
;	move	x:(r1)+,x0
;	move	x0,y:(r0)+
;systoylp:

	move	#dmaNode,r4			; r4 <- address of dma node
	
	move	r0,a1
	add 	#kEPMemory_Base_YRAM,a 		; adjust DSP address for Y base
	move	a1,r0
	
	jsr	DMANewNode_Read			; create DMA node
	
	jsr	DMAStartNodeP_Wait		; initiate DMA 
						; (and wait for completion)

	rts

;******************************************************************************
;*										  
;*   Subroutine Name:	CopyDSPYToSys
;*								  
;*   Description:	Copy from DSP y: space to System Memory
;*										      
;*   Input:		r0 	- DSP address (y:)
;*	   		n0	- number of words to copy
;*			r1	- System Memory address
;*										      
;******************************************************************************

CopyDSPYToSys:

; Note: Dolby-only testing code
;	dor		n0,ytosyslp
;	move	y:(r0)+,x0
;	move	x0,x:(r1)+
;ytosyslp:


	move	#dmaNode,r4			; r4 <- address of dma node
	
	move	r0,a1
	add 	#kEPMemory_Base_YRAM,a 		; adjust DSP address for Y base
	move	a1,r0
	
	jsr	DMANewNode_Write		; create DMA node
	
	jsr	DMAStartNodeP_Wait		; initiate DMA 
						; (and wait for completion)

	rts

;******************************************************************************
;*										  
;*   Subroutine Name:	CopySysToDSPP
;*								  
;*   Description:	Copy from System Memory to DSP p: space
;*										      
;*   Input:	 	r0 	- DSP address (p:)
;*			n0	- number of words to copy
;*			r1	- System Memory address
;*										      
;******************************************************************************

CopySysToDSPP:

; Note: Dolby-only testing code
;	dor		n0,systoplp
;	move	x:(r1)+,x0
;	move	x0,p:(r0)+
;	nop
;systoplp:

	move	#dmaNode,r4			; r4 <- address of dma node
	
	move	r0,a1
	add 	#kEPMemory_Base_PRAM,a 		; adjust DSP address for Y base
	move	a1,r0
	
	jsr	DMANewNode_Read			; create DMA node
	
	jsr	DMAStartNodeP_Wait		; initiate DMA 
						; (and wait for completion)

	rts

;******************************************************************************
;*										  
;*   Subroutine Name:	CopyDSPPToSys
;*								  
;*   Description:	Copy from DSP p: space to System Memory
;*										      
;*   Input:	       	r0 	- DSP address (p:)
;*		  	n0	- number of words to copy
;*		       	r1	- System Memory address
;*										      
;******************************************************************************

CopyDSPPToSys:
	
; Note: Dolby-only testing code
;	dor		n0,ptosyslp
;	move	p:(r0)+,x0
;	move	x0,x:(r1)+
;ptosyslp:

	move	#dmaNode,r4			; r4 <- address of dma node
	
	move	r0,a1
	add 	#kEPMemory_Base_PRAM,a 		; adjust DSP address for Y base
	move	a1,r0
	
	jsr	DMANewNode_Write		; create DMA node
	
	jsr	DMAStartNodeP_Wait		; initiate DMA 
						; (and wait for completion)
	rts
	
	
;************************************************************
; DMANewNode_Read
;
;	This DMA Read node is configured for the System
;	Memory -> DSP transfer
;	This node is also set up to be the last node in
;	the list, so it is self-linked with the EndOfList
;	bit set
;
;	Parameters:  	r4	node DSP PRAM address
;			r0	DSP Memory address
;			r1	System Memory address
;           n0      Length
;************************************************************
DMANewNode_Read

	jsr 	DMAWaitForIdle			    ; Wait fro DMA to be idle

; Word0
	move	r4,r5			; r5 <- copy of node base 
	
	move	r4,a1
	and		#>kDMANode_EOLMask,a	; Clear reserved upper bits
	or		#>kDMANode_EOL,a 	; set EOL bit
	
    movem   a1,p:(r5)+		; Link to itself			
; Word1
    move    #>DMA_BASE_READ1,a1
	movem	a1,p:(r5)+
; Word2
	movem	n0,p:(r5)+		; Length       	
; Word3
	movem	r0,p:(r5)+		; DSP RAM address (System memory space)	
; Word4						
	move	r1,a
	lsl		#2,a					
	movem	a1,p:(r5)+		; System memory address


 	rts
; ---- end DMANewNode_Read


;************************************************************
; DMANewNode_Write     	
;
;	This DMA Write node is configured for the DSP -> SM
;	transfer 
;	This node is also set up to be the last node in
;	the list, so it is self-linked with the EndOfList
;	bit set
;
;	Parameters:  	r4	node DSP PRAM address
;			r0	DSP Memory address 
;			r1	System Memory address
;			n0	Length
;************************************************************
DMANewNode_Write	

	jsr 	DMAWaitForIdle			    ; Wait fro DMA to be idle

; Word0
	move	r4,r5			; r5 <- copy of node base 
	
	move	r4,a1
	and		#>kDMANode_EOLMask,a	; Clear reserved upper bits
	or		#>kDMANode_EOL,a	; set EOL bit
    	
; Word0
	move    a1,p:(r5)+		; Link to itself			

; Word1
    	move    #>DMA_BASE_WRITE1,a1
	movem	a1,p:(r5)+			
; Word2
	movem	n0,p:(r5)+		; Length
; Word3
	movem	r0,p:(r5)+		; DSP RAM address (System memory space)	
; Word4	
	move	r1,a
	lsl		#2,a					
	movem	a1,p:(r5)+		; System memory address

 	rts
; ---- end DMANewNode_Write ----

;************************************************************
; DMANewNode_WriteFIFO
;
;	This DMA Write node is configured for the DSP -> FIFO
;	transfer 
;	This node is also set up to be the last node in
;	the list, so it is self-linked with the EndOfList
;	bit set
;
;	Parameters:  	r4	node DSP PRAM address
;			r0	DSP Memory address 
;			r1	FIFO number
;			n0	Length
;			n4  NULL output flag
;************************************************************
DMANewNode_WriteFIFO

	jsr 	DMAWaitForIdle			    ; Wait fro DMA to be idle

; Word0
	move	r4,r5			; r5 <- copy of node base 
	
	move	r4,a1
	and		#>kDMANode_EOLMask,a	; Clear reserved upper bits
	or		#>kDMANode_EOL,a	; Set EOL bit
    	
	move    a1,p:(r5)+		; Link to itself			

; Word1
    move    #>DMA_BASE_WRITEFIFO1,a1 	; start with "write to FIFO/16-bit" control word
	move	r1,b			; FIFO index from r1
    lsl	    #5,b			; shift FIFO index smBufId bits
	move	b,x1
	or	    x1,a			; put FIFO index into control word
	movem	a1,p:(r5)+		; write control word
; Word2
	movem	n0,p:(r5)+			
; Word3
	move	n4,a			; Null Flag
    lsl	    #14,a
	move	r0,x0
	add		x0,a			; Add DSP Address
	movem	a1,p:(r5)+		; DSP RAM address (System memory space)	
; Word4 is no longer used

 	rts
; ---- end DMANewNode_WriteFIFO ----


;************************************************************
; DMANewNode_WriteFIFOInterleaved
;
;	This DMA Write node is configured for the DSP -> FIFO
;	transfer 
;	This node is also set up to be the last node in
;	the list, so it is self-linked with the EndOfList
;	bit set
;
;	Parameters:  	r4	node DSP PRAM address
;			r0	DSP Memory address 
;			r1	FIFO number
;			n0	Length
;			n2  Channels per block
;			n4  NULL output flag
;************************************************************
DMANewNode_WriteFIFOInterleaved

	jsr 	DMAWaitForIdle			    ; Wait fro DMA to be idle

; Word0
	move	r4,r5			; r5 <- copy of node base 
	
	move	r4,a1
	and 	#>kDMANode_EOLMask,a	; Clear reserved upper bits
	or		#>kDMANode_EOL,a	; Set EOL bit
	move    a1,p:(r5)+		; Link to itself			

; Word1
    move    #>DMA_BASE_WRITEFIFO2,a1 	; start with "write to FIFO/16-bit interleaved" control word
	move	r1,b			; FIFO index from r1
    lsl	    #5,b			; shift FIFO index smBufId bits
	move	b,x1
	or	    x1,a			; put FIFO index into control word

	move	n0,b			; Set Incr value
    lsl	    #14,b			; shift incr value bits (Increment by size of a single channel buffer)
	move	b,x1
	or	    x1,a			; put FIFO index into control word
	movem	a1,p:(r5)+		; write control word
; Word2
	move	n0,b			; Set Length value
    lsl	    #4,b			; shift Length bits (Increment by size of a single channel buffer)
	move	n2,x1
	add		x1,b			; Add samples per block
	movem	b1,p:(r5)+		; write control word
; Word3
	move	n4,a			; Null Flag
    lsl	    #14,a
	move	r0,x0
	add		x0,a			; Add DSP Address
	movem	a1,p:(r5)+		; DSP RAM address (System memory space)	
; Word4 is no longer used

 	rts
; ---- end DMANewNode_WriteFIFO ----





;************************************************************
; DMAWaitForStop	
;
;		Alters DMA Running state 
;************************************************************
DMAWaitForIdle 


; Wait for DMA engine to enter Idle state: 0 = Idle, 1 = Running
	jset 	#kDMA_RunningBit,x:DMA_CONTROL_REGISTER,DMAWaitForIdle

; Clear DMA EndofList, Bit 7
	movep    #$80,x:INTERRUPT_REGISTER	; "Write-1-To-Clear-One" 

 	rts
; ---- end DMAStopAndFreeze ----

;************************************************************
; DMAStart
;
;		Alters DMA Running state 
;************************************************************
DMAStart 


; Write "run" command
    movep    #>kDMA_ActionRequest_Start,x:DMA_CONTROL_REGISTER	

; Wait for DMA engine to enter Running state: 0 = Idle, 1 = Running
DMAUnFreezeAndStart_WaitForStart
;	jset	#kDMAErrorBit,x:INTERRUPT_REGISTER,DMAError
	jclr 	#kDMA_RunningBit,x:DMA_CONTROL_REGISTER,DMAUnFreezeAndStart_WaitForStart

 	rts
; ---- end DMAUnFreezeAndStart ----

;************************************************************
; DMAWaitForEOL		Wait for DMA EOL interrupt and clear register bit
;************************************************************
DMAWaitForEOL 

; Poll for DMA EOL interrupt
DMAWaitForEOL_Poll
	jset	#kDMAErrorBit,x:INTERRUPT_REGISTER,DMAError
	jclr 	#kDMAEndOfListBit,x:INTERRUPT_REGISTER,DMAWaitForEOL_Poll

; Clear DMA EndofList, Bit 7
	movep    #$80,x:INTERRUPT_REGISTER	; "Write-1-To-Clear-One" 

    ;debug_wait


 	rts
; ---- end DMAWaitForEOL ----

;************************************************************
; DMAStartNodeP_Wait	
;
;	Replace start block, initiate transfer and Wait for
;				completion
;
;	This function assumes a single DMA node that is
;	setup as the EOL block.
;
;	Parameters:  	 r4	next node DSP PRAM address
;************************************************************
DMAStartNodeP_Wait 
	
	move	r4,a
	add		#kEPMemory_Base_PRAM,a			; block is in PRAM 
	movep	a,x:DMA_NEXT_BLOCK_CONTROL_REGISTER
	
	jsr 	DMAStart			        ; Start transfer
	
    clr     b
    move    n5,b
    cmp     #0,b
    beq     DMAStartNodeP_NoWait

	jsr 	DMAWaitForEOL				; Wait for completion

DMAStartNodeP_NoWait
 	rts
; ---- end DMAStartNodeX_Wait ----


;******************************************************************************
;*
;*   Subroutine Name:	DMAInit             
;*
;*   Description:		Inits the EP for DMA (based on Gints' GP code)
;*
;******************************************************************************
;
;

DMAInit:
	;GPClearInterruptRegister
	movep    #$FFF,x:INTERRUPT_REGISTER	 ; "Write-1-To-Clear-One"
	
	;GPClearProcessingControlRegister
	movep    #$1,x:PROCESSING_CONTROL_REGISTER	 ; "Set Idle bit"	

	;GPClearSampleControlRegister
	movep    #$0,x:SAMPLE_CONTROL_REGISTER	 
	
	;abort any DMA transfers
;    movep    #>kDMA_ActionRequest_Abort,x:DMA_CONTROL_REGISTER

    move    #$4000,x0	
	move    x0,x:DMA_START_BLOCK_CONTROL_REGISTER
	move    x0,x:DMA_NEXT_BLOCK_CONTROL_REGISTER

	;GPClearDMAConfigRegister 
	movep    #>0,x:DMA_CONFIGURATION_REGISTER

	rts


DMAError
	bra	DMAError




;** Loader Status

;status_active	dc	0
;status_ID		dc	-1
;status_action	dc	-1
;status_dspaddr	dc	-1
;status_length	dc	-1
;status_sysaddr	dc	-1
;status_wait		dc	0

	endsec
	end
