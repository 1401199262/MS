// FILE:			library\hardware\mpeg3dec\specific\sti4600r.h
// AUTHOR:		Viona
// COPYRIGHT:	(c) 1995 VIONA Development GmbH.  All Rights Reserved.
// CREATED:		05.12.95
//
// PURPOSE:		
//
// HISTORY:
#ifndef STI4600R_H
#define STI4600R_H

#define I4600REG_VERSION		0x00
#define I4600REG_IDENT			0x01

#define I4600REG_FBADDRL		0x02
#define I4600REG_FBADDRH		0x03
#define I4600REG_FBDATA			0x04

#define I4600REG_SFREQ			0x05
#define I4600REG_EMPH			0x06

#define I4600REG_INTEL			0x07
#define I4600REG_INTEH			0x08
#define I4600REG_INTL			0x09
#define I4600REG_INTH			0x0a
#define I4600REG_SETINT			0x0b

#define I4600INTIDX_SYNC		0
#define I4600INTIDX_HEADER		1
#define I4600INTIDX_ERROR		2
#define I4600INTIDX_SFREQ		3
#define I4600INTIDX_DEEMPH		4
#define I4600INTIDX_BOF			5
#define I4600INTIDX_PTS			6
#define I4600INTIDX_ANC			7
#define I4600INTIDX_PCM			8
#define I4600INTIDX_FBFULL		9
#define I4600INTIDX_FBEMPTY	10
#define I4600INTIDX_FIFO		11
#define I4600INTIDX_BREAK		15

#define I4600REG_SINSETUP		0x0c

#define I4600IDX_INPUTSETUP	0, 2
#define I4600IDX_REQPOL			2

#define I4600INSET_PARALLEL	0
#define I4600INSET_SERIAL		1
#define I4600INSET_ANALOG		3

#define I4600REG_CANSETUP		0x0d

#define I4600CANSET_PADDING	0
#define I4600CANSET_LEFT1		1
#define I4600CANSET_FALLEDGE	2
#define I4600CANSET_SLOTCNT32	3

#define I4600REG_DATAIN			0x0e
#define I4600REG_ERROR			0x0f
#define I4600REG_RESET			0x10

#define I4600REG_PLLSYS			0x11

#define I4600IDXPLL_BYPASS		6
#define I4600IDXPLL_DISABLED	5
#define I4600IDXPLL_DIV			0, 4

#define I4600REG_PLLPCM			0x12

#define I4600REG_PLAY			0x13
#define I4600REG_MUTE			0x14
#define I4600REG_REQ				0x15
#define I4600REG_ACK				0x16
#define I4600REG_RUN				0x72

#define I4600REG_SYNCSTATUS	0x40

#define I4600IDX_FRAMESTATUS	0, 2

#define I4600FRMSTAT_SEARCH	0
#define I4600FRMSTAT_WAIT		1
#define I4600FRMSTAT_SYNC		2

#define I4600IDX_PACKETSTATUS	2, 2

#define I4600PCKSTAT_SEARCH	0
#define I4600PCKSTAT_WAIT		1
#define I4600PCKSTAT_SYNC		2

#define I4600REG_ANCCOUNT		0x41
#define I4600REG_HEAD24			0x42
#define I4600REG_HEAD16			0x43
#define I4600REG_HEAD8			0x44
#define I4600REG_HEAD0			0x45
#define I4600REG_PTS33			0x46
#define I4600REG_PTS24			0x47
#define I4600REG_PTS16			0x48
#define I4600REG_PTS8			0x49
#define I4600REG_PTS0			0x4a
#define I4600REG_USER1			0x4b

#define I4600REG_STREAMSEL		0x4c

#define I4600STRSEL_PES			0
#define I4600STRSEL_PESDVD		1
#define I4600STRSEL_PACKMPEG1	2
#define I4600STRSEL_ELEMNTRY	3

#define I4600REG_DECODESEL		0x4d

#define I4600DECSEL_AC3			0
#define I4600DECSEL_MPEG		1
#define I4600DECSEL_MPEGEXT	2                                                                
#define I4600DECSEL_LPCM		3

#define I4600REG_PACKETLOCK	0x4f
#define I4600REG_AUDIOIDEN		0x50
#define I4600REG_AUDIOID		0x51
#define I4600REG_AUDIOIDEXT	0x52
#define I4600REG_SYNCLOCK		0x53

#define I4600REG_PCMDIVIDER	0x54
#define I4600REG_PCMCONFIG		0x55
#define I4600REG_PCMCANCEL		0x7e

#define I4600IDX_PCMORDER		6
#define I4600IDX_PCMDIFF		5
#define I4600IDX_INVLRCLK		4
#define I4600IDX_I2S				3	// doku says i2c ???
#define I4600IDX_INVSCLK		2
#define I4600IDX_PCMPREC		0, 2

#define I4600PCMPREC_16			0
#define I4600PCMPREC_18			1
#define I4600PCMPREC_20			2
#define I4600PCMPREC_24			3

#define I4600REG_PCMCROSS		0x56
#define I4600REG_PCMFCROSS		0x7f

#define I4600REG_LDLY			0x57
#define I4600REG_RDLY			0x58
#define I4600REG_CDLY			0x59
#define I4600REG_SUBDLY			0x5a
#define I4600REG_LSDLY			0x5b
#define I4600REG_RSDLY			0x5c
#define I4600REG_DLYUPDATE		0x5d

#define I4600REG_IEC958			0x5e

#define I4600IECMD_OFF			0
#define I4600IECMD_MUTED		1
#define I4600IECMD_PCM			2
#define I4600IECMD_ENCODED		3

#define I4600REG_IEC958CAT		0x5f
#define I4600REG_IEC958CONF1	0x60

#define I4600IDX_IECDIV			0, 5
#define I4600IDX_IECIDLESTATE	5
#define I4600IDX_IECSYNCMT		6

#define I4600REG_IEC958CONF2	0x61

#define I4600IDX_IEC44KHZ		3
#define I4600IDX_IECPREEMPH	2
#define I4600IDX_IECCOPY		1
#define I4600IDX_IECCOMPRESS	0

#define I4600REG_IEC958LEN0	0x75

#define I4600REG_PDEC			0x62
#define I4600REG_PLMOD			0x63

#define I4600PLMOD_NORMAL		0
#define I4600PLMOD_HALL			1
#define I4600PLMOD_MATRIXED	2

#define I4600REG_PLAB			0x64
#define I4600REG_PLDWNX			0x65

#define I4600PLDWNX_DISABLED	0
#define I4600PLDWNX_3_0			3
#define I4600PLDWNX_2_1			4
#define I4600PLDWNX_3_1			5
#define I4600PLDWNX_2_2			6
#define I4600PLDWNX_3_2			7

#define I4600REG_OCFG			0x66

#define I4600OCFG_QUANTRNDPCM	0
#define I4600OCFG_SUMLFREQ		1
#define I4600OCFG_LPONC			2
#define I4600OCFG_NOHP			3
#define I4600OCFG_SUMCHNL		4
#define I4600OCFG_NOPROCESS	5
#define I4600OCFG_QUANTRND		6

#define I4600REG_BAL_LR			0x4e
#define I4600REG_BAL_SUR		0x63
#define I4600REG_PCMSCALE		0x67

#define I4600REG_LFE				0x68
#define I4600REG_COMPMOD		0x69

#define I4600CMPMOD_LINEOUT	0
#define I4600CMPMOD_RFMODE		1
#define I4600CMPMOD_CUSTOM1	2
#define I4600CMPMOD_CUSTOM2	3

#define I4600REG_HDR				0x6a
#define I4600REG_LDR				0x6b
#define I4600REG_RPC				0x6c
#define I4600REG_KARAMODE		0x6d

#define I4600KARAMD_AWARE		0
#define I4600KARAMD_MLTCHANL	3
#define I4600KARAMD_DOWNMIX	7
#define I4600KARAMD_REPRDC_1	5
#define I4600KARAMD_REPRDC_2	6
#define I4600KARAMD_REPRDC_12	4

#define I4600REG_DUALMODE		0x6e

#define I4600DLMD_STEREO		0
#define I4600DLMD_CHANNEL1		1
#define I4600DLMD_CHANNEL2		2
#define I4600DLMD_MIX			3

#define I4600REG_DOWNMIX		0x6f

#define I4600DWNMX_2_0_COMP	0
#define I4600DWNMX_1_0			1
#define I4600DWNMX_2_0			2
#define I4600DWNMX_3_0			3
#define I4600DWNMX_2_1			4
#define I4600DWNMX_3_1			5
#define I4600DWNMX_2_2			6
#define I4600DWNMX_3_2			7

#define I4600REG_AC3STAT0		0x76

#define I4600IDX_FSCOD			5, 2
#define I4600IDX_FRMSZCOD		0, 5

#define I4600REG_AC3STAT1		0x77

#define I4600IDX_LFEPRESENT	3
#define I4600IDX_ACMOD			0, 3

#define I4600REG_AC3STAT2		0x78

#define I4600IDX_BSMOD			5, 3
#define I4600IDX_BSID			0, 5

#define I4600REG_AC3STAT3		0x79

#define I4600IDX_CMIXLEVEL		2, 2
#define I4600IDX_SURMIXLEVEL	0, 2

#define I4600REG_AC3STAT4		0x7a

#define I4600IDX_DSURMOD		3, 2
#define I4600IDX_COPYRIGHT		2
#define I4600IDX_ORIGBS			1
#define I4600IDX_LANGCODE		0

#define I4600REG_LANGCODE		0x7b
#define I4600REG_DIALNORM		0x7c
#define I4600REG_AC3STAT7		0x7d

#define I4600IDX_ROOMTYPE		6, 2
#define I4600IDX_MIXLEVEL		1, 5
#define I4600IDX_AUDPRODIE		0

#define I4600REG_SKIPFRAME		0x73
#define I4600REG_REPEATFRAME	0x74
#define I4600REG_DWSMODE		0x70
#define I4600REG_SOFTVER		0x71


#endif
