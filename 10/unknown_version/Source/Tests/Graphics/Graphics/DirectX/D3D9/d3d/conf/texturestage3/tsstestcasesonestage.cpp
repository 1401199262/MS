///////////////////////////////////////////////////////////
// FILE:        TSStestCasesOneStage.cpp
// DESC:        TextureStageTest 3rd generation.
// AUTHOR:      Bobby Paulose 
//				Bill Kristiansen
// Created:		June 11, 2002.
///////////////////////////////////////////////////////////


#include "TextureStage3.h"




TSStestCaseOneStage g_TestCasesOneStages[] =
{

#ifdef CRAZY
#endif CRAZY

/////////////////////////////////////////////////////////////////////////////////
	{
		_T("NULL texture in ARG1 not referenced"),
		TRI_LIST_DST2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0x0FF00FF0,

		//TSS one
		D3DTOP_SELECTARG2, D3DTA_TEXTURE, D3DTA_TFACTOR, D3DTA_DIFFUSE,
		D3DTOP_SELECTARG2, D3DTA_TFACTOR, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_TEXT3, CYC_DONT_CARE,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 



	},


//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_SELECTARG1 and 2"),
		TRI_LIST_DST2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0x00000000,

		//TSS one
		D3DTOP_SELECTARG1, D3DTA_DIFFUSE, D3DTA_TFACTOR, D3DTA_DIFFUSE,
		D3DTOP_SELECTARG2, D3DTA_TFACTOR, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_NULL, CYC_DONT_CARE,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 



	},



//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_MODULATE and D3DTA_TFACTOR"),
		TRI_STRIP_DST2T2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xDD000000,

		//TSS one
		D3DTOP_MODULATE, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTOP_MODULATE, D3DTA_TFACTOR, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_TEXT3, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},




//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_SELECTARG1/2 and D3DTOP_MODULATE2X"),
		TRI_STRIP_DST2T2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_MODULATE2X, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTOP_MODULATE2X, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_DECOR, CYC_RGB,
		1,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},



//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_MODULATE4X"),
		TRI_STRIP_DST2T2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_MODULATE4X, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTOP_MODULATE4X, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_DECOR2, CYC_RGB,
		1,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},



//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_ADDSIGNED"),
		TRI_STRIP_DST2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		D3DTOP_ADDSIGNED, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTOP_ADDSIGNED, D3DTA_CURRENT, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_NEWTS, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},

//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_ADD color & D3DTOP_SUBTRACT alpha"),
		TRI_STRIP_DST2_DiffuseAlphaGradient,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_ADD, D3DTA_TEXTURE, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTOP_SUBTRACT, D3DTA_TEXTURE, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_DX_LOGO2, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},




//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_ADD"),
		TRI_STRIP_DST2_DiffuseAlphaGradient,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_ADD, D3DTA_TEXTURE, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTOP_ADD, D3DTA_SPECULAR, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_DX_LOGO2, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},





//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_ADDSMOOTH"),
		TRI_STRIP_DST2T2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_ADDSMOOTH, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTOP_ADDSMOOTH, D3DTA_TFACTOR, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_STRIPES, CYC_RGB,
		1,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},




//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_BLENDDIFFUSEALPHA"),
		TRI_STRIP_DST2_DiffuseAlphaGradient,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xDD0000FF,

		//TSS one
		D3DTOP_BLENDDIFFUSEALPHA, D3DTA_TEXTURE, D3DTA_TFACTOR, D3DTA_CURRENT,
		D3DTOP_BLENDDIFFUSEALPHA, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_COLORS_BLEND, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},




//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_BLENDTEXTUREALPHA"),
		TRI_STRIP_DST2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0x55000000,


		//TSS two
		D3DTOP_BLENDTEXTUREALPHA, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTOP_BLENDTEXTUREALPHA, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_GRADIENT_FIRE_WHITE, CYC_ALPHA,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},


//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_BLENDFACTORALPHA"),
		TRI_STRIP_DST2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS two
		D3DTOP_BLENDFACTORALPHA, D3DTA_CURRENT, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTOP_BLENDFACTORALPHA, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_WIN_LOGO2, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},



//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_BLENDTEXTUREALPHAPM"),
		TRI_STRIP_DST2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_BLENDTEXTUREALPHAPM, D3DTA_CURRENT, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTOP_BLENDTEXTUREALPHAPM, D3DTA_CURRENT, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_GRADIENT_ALPHA2, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},


//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_BLENDCURRENTALPHA"),
		TRI_STRIP_DST2_DiffuseAlphaGradient,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS two
		D3DTOP_BLENDCURRENTALPHA, D3DTA_CURRENT, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTOP_BLENDCURRENTALPHA, D3DTA_CURRENT, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_TEXT1, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},

//////////////////////////////////////////////////////////////////////////
	
	// D3DTOP_PREMODULATE test does not make sense with just 
	//	one texture stage.



//////////////////////////////////////////////////////////////////////////







	{
		_T("D3DTOP_MODULATEALPHA_ADDCOLOR COLOROP only"),
		TRI_FAN_DST2T2half,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)FALSE, D3DRS_SRCBLEND, D3DBLEND_ONE, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_MODULATEALPHA_ADDCOLOR, D3DTA_SPECULAR, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_DX_LOGO, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},



//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_MODULATECOLOR_ADDALPHA COLOROP only"),
		TRI_LIST_DST2T2half,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)FALSE, D3DRS_SRCBLEND, D3DBLEND_ONE, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_MODULATECOLOR_ADDALPHA, D3DTA_SPECULAR, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_DECOR2, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},





//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_MODULATEINVALPHA_ADDCOLOR COLOROP only"),
		TRI_LIST_DST2T2half,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)FALSE, D3DRS_SRCBLEND, D3DBLEND_ONE, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_MODULATEINVALPHA_ADDCOLOR, D3DTA_SPECULAR, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_TEXT2, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 






	},




//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_MODULATEINVCOLOR_ADDALPHA COLOROP only"),
		TRI_STRIP_DST2_DiffuseAlphaGradient,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)FALSE, D3DRS_SRCBLEND, D3DBLEND_ONE, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_MODULATEINVCOLOR_ADDALPHA, D3DTA_DIFFUSE, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_CHECKER_HIGHFREQ, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 

	},


//////////////////////////////////////////////////////////////////////////

	// D3DTOP_BUMPENVMAP test does not make sense with just 
	//	one texture stage.

//////////////////////////////////////////////////////////////////////////

	// D3DTOP_BUMPENVMAPLUMINANCE test does not make sense with just 
	//	one texture stage.






//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_DOTPRODUCT3 colorop only"),
		TRI_STRIP_DST2T2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0x00FFFFFF,

		//TSS one
		D3DTOP_DOTPRODUCT3, D3DTA_TEXTURE, D3DTA_TFACTOR, D3DTA_CURRENT,
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_DX_LOGO2, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},


//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_MULTIPLYADD Triadic op"),
		TRI_STRIP_DST2_DiffuseAlphaGradient,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0x88DDDDDD,

		//TSS one
		D3DTOP_MULTIPLYADD, D3DTA_TEXTURE, D3DTA_TFACTOR, D3DTA_DIFFUSE,
		D3DTOP_MULTIPLYADD, D3DTA_TEXTURE, D3DTA_TFACTOR, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_DX_LOGO3, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},








//////////////////////////////////////////////////////////////////////////
	{
		_T("D3DTOP_LERP Triadic op"),
		TRI_STRIP_DST2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xFF000000,

		//TSS one
		D3DTOP_LERP, D3DTA_DIFFUSE, D3DTA_TFACTOR, D3DTA_TEXTURE,
		D3DTOP_LERP, D3DTA_TEXTURE, D3DTA_TFACTOR, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_DX_LOGO3, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 

	},



//////////////////////////////////////////////////////////////////////////
	{
		_T("Per-stage constant D3DTA_CONSTANT"),
		TRI_STRIP_DST2T2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_SELECTARG1, D3DTA_CONSTANT, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTOP_SELECTARG1, D3DTA_CONSTANT, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_NULL, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_CONSTANT, (DWORD)D3DRGBA(0.333,0.5,0.667,0.9), 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},


//////////////////////////////////////////////////////////////////////////
	{
		_T("MaxSimultaneousTextures test"),
		TRI_STRIP_DST2_ZeroDiffuse,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xCC000000,

		//TSS one
		D3DTOP_ADD, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_DIFFUSE,
		D3DTOP_ADD, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_STAGE0, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},



//////////////////////////////////////////////////////////////////////////
	{
		_T("MipMap texture test"),
		TRI_STRIP_DST2T3T3_forMipMap,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0x88000000,

		//TSS one
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_MIPS, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MIPFILTER, (DWORD)D3DTEXF_POINT, 
		D3DTSS_MIPMAPLODBIAS , 254, //this should take us to the centre of 6 mips.
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},





//////////////////////////////////////////////////////////////////////////
	{
		_T("Cube Map texture test"),
		TRI_STRIP_DST2T3T3_forVolCube,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0x88000000,

		//TSS one
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_CUBE, CYC_CUBE,
		2,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MIPFILTER, (DWORD)D3DTEXF_POINT, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},






//////////////////////////////////////////////////////////////////////////
	{
		_T("Volume Map texture test"),
		TRI_STRIP_DST2T3T3_forVolCube,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0x88000000,

		//TSS one
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_DIFFUSE, D3DTA_DIFFUSE,
		D3DTA_CURRENT,
		TEX_VOLUME, CYC_VOLUME,
		1,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MIPFILTER, (DWORD)D3DTEXF_POINT, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},







/////////////////////////////////////////////////////////////////////////////////
	{
		_T("Arg modifier D3DTA_COMPLEMENT."),
		TRI_LIST_DST2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0x22000000,

		//TSS one
		D3DTOP_SELECTARG1, D3DTA_TEXTURE|D3DTA_COMPLEMENT, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTOP_SELECTARG1, D3DTA_TFACTOR|D3DTA_COMPLEMENT, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_DX_LOGO3, CYC_RGB,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},



/////////////////////////////////////////////////////////////////////////////////
	{
		_T("Arg modifier D3DTA_ALPHAREPLICATE."),
		TRI_STRIP_DST2_DiffuseAlphaGradient,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_ZERO,
		D3DRS_TEXTUREFACTOR,(DWORD)0xFF000000,

		//TSS one
		D3DTOP_SELECTARG1, D3DTA_TEXTURE|D3DTA_ALPHAREPLICATE , D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTOP_SELECTARG1, D3DTA_TFACTOR|D3DTA_ALPHAREPLICATE , D3DTA_CURRENT, D3DTA_CURRENT,
		//note: D3DTA_ALPHAREPLICATE in alpha arg should not affect output.
		D3DTA_CURRENT,
		TEX_GRADIENT_ALPHA, CYC_ALPHA,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 


	},

//////////////////////////////////////////////////////////////////////////
	{
		_T("Luminance MODULATE"),
		TRI_LIST_DST2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA,
		D3DRS_TEXTUREFACTOR,(DWORD)0x00000000,

		//TSS one
		D3DTOP_MODULATE, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_CHECKER, CYC_LUMINANCE,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
	},



/////////////////////////////////////////////////////////////////////////////////
	{
		_T("Luminance BLENDTEXTUREALPHA"),
		TRI_LIST_DST2,
		TRUE,

		// Render State
		D3DRS_ALPHABLENDENABLE,(DWORD)TRUE, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA,
		D3DRS_TEXTUREFACTOR,(DWORD)0x00000000,

		//TSS one
		D3DTOP_BLENDTEXTUREALPHA, D3DTA_CURRENT, D3DTA_TEXTURE, D3DTA_CURRENT,
		D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_CURRENT, D3DTA_CURRENT,
		D3DTA_CURRENT,
		TEX_GRADIENT_ALPHA2, CYC_LUMINANCE,
		0,
		D3DTSS_MAGFILTER, (DWORD)D3DTEXF_LINEAR,
		D3DTSS_MINFILTER, (DWORD)D3DTEXF_LINEAR,
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
		(enum _D3DTEXTURESTAGESTATETYPE)0,0, 
	},


};

const UINT NUM_ONE_STAGES_TEST_CASES( sizeof( g_TestCasesOneStages ) / sizeof( TSStestCaseOneStage ) );
