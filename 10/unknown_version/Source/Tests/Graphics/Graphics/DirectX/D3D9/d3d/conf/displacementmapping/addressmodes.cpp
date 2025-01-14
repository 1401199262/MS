#define D3D_OVERLOADS
#include "DisplacementMapping.h"
//#include "d3d9.h"       //for D3DSHADER_VERSION_MAJOR and D3DSHADER_VERSION_MINOR

D3DTEXTUREADDRESS AddressModes[] = 
{
	D3DTADDRESS_WRAP,
	D3DTADDRESS_MIRROR,                             
	D3DTADDRESS_CLAMP, 
	D3DTADDRESS_BORDER,
	D3DTADDRESS_MIRRORONCE 
};
TCHAR *AddressModeStr[] = 
{
	"ADDRESS_WRAP",
	"ADDRESS_MIRROR",                             
	"ADDRESS_CLAMP", 
	"ADDRESS_BORDER",
	"ADDRESS_MIRRORONCE"
};

CAddressModesTest::CAddressModesTest()
{
	m_szTestName = "Address Modes Test";
	m_szCommandKey = "AddressMode";
}

CAddressModesTest::~CAddressModesTest()
{
	RELEASE(m_pVB1);
	RELEASE(m_pVB2);
	RELEASE(m_pDMTexture);
	RELEASE(m_pTexture);
	for(int i = 0; i < m_nShaders; i++)
	{
		RELEASE(m_pShaders[i]);
		RELEASE(m_pDecls[i]);
	}
}

UINT CAddressModesTest::TestInitialize(void)
{
	HRESULT hr;
	FORMAT   Format = m_pCommonTextureFormats[m_pCurrentMode->nTextureFormat];

	m_pTexture = NULL;
	m_pDMTexture = NULL;
	m_pVB1 = NULL;
	m_pVB2 = NULL;
	m_isPreswizzled = false;

	UINT res = DisplacementMapping::TestInitialize(); 
	if ( res != D3DTESTINIT_RUN )
		return res;

	RELEASE(m_pVB1);
	RELEASE(m_pVB2);
	if (!GenerateVertexBuffer(true))
        return D3DTESTINIT_ABORT;
	
	//check for mipmapping support
	CAPS caps;
	m_pSrcDevice->GetDeviceCaps(&caps); 

	if (!(caps.TextureCaps & D3DPTEXTURECAPS_MIPMAP) ||
		 (m_pD3D->GetRuntimeVersion() >= 9.029f && FAILED(m_pD3D->CheckDeviceFormat(m_pSrcDevice->GetAdapterID(), m_pSrcDevice->GetDevType(), 
														  m_pCurrentMode->pDisplay->Format, USAGE_QUERY_WRAPANDMIP, RTYPE_TEXTURE, Format)) ))
		tcCount.MipLevel = 1;
	else
	{
		tcCount.MipLevel = 7;
		if (CheckFilterSupport(D3DTEXF_LINEAR, caps.TextureFilterCaps, 2))
		//if (caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFPOINT)
		{
			SetSamplerState(          0, SAMP_MIPFILTER, (DWORD)D3DTEXF_LINEAR);
			SetSamplerState(DMAPSAMPLER, SAMP_MIPFILTER, (DWORD)D3DTEXF_LINEAR);
		}
		//else if (caps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR)
		else if (CheckFilterSupport(D3DTEXF_POINT, caps.TextureFilterCaps, 2))
		{
			SetSamplerState(          0, SAMP_MIPFILTER, (DWORD)D3DTEXF_POINT);
			SetSamplerState(DMAPSAMPLER, SAMP_MIPFILTER, (DWORD)D3DTEXF_POINT);
		}
		else
		{
			WriteToLog("Missmatch in caps: MipMapping is supported but no MipFilter is supported\n");
			return D3DTESTINIT_ABORT;
		}
	}

	tcCount.View         = m_nViewMatrices + 1; 
	tcCount.LookupType   = sizeof(LookupTypes ) / sizeof(WORD);
	tcCount.AddressMode  = sizeof(AddressModes) / sizeof(D3DTEXTUREADDRESS);

    ADDRESSTESTCASE tc;
	tc.nTest = 0;
	ClearRangeList();
	for( tc.AddressMode = 0; tc.AddressMode < tcCount.AddressMode; tc.AddressMode++ )
	{
		if ( !CheckAddressModeSupport(AddressModes[tc.AddressMode]) )
			continue;
		for(tc.LookupType = 0; tc.LookupType < tcCount.LookupType; tc.LookupType++)
		{
			if((KeySet(_T("noDisMap")) || KeySet(_T("fixed"))) && tc.LookupType != LOOKUP)
				continue;
			if(KeySet(_T("presampled")) && tc.LookupType != PRESAMPLED)
				continue;
			if ( tc.LookupType == PRESAMPLED && !m_Presampled )
				continue;
			if ( tc.LookupType != PRESAMPLED && !m_Lookup ) 
				continue;
			for( tc.MipLevel = 0; tc.MipLevel < tcCount.MipLevel; tc.MipLevel++ )
			{
				tc.View = 0;
				if (tc.LookupType == PRESAMPLED || KeySet(_T("fixed")))
					AddTestRange( tc.nTest + 2, tc.nTest + tcCount.View );
				else
					AddTestRange( tc.nTest + 1, tc.nTest + tcCount.View );
			}
		}
	}
	
	return D3DTESTINIT_RUN;
}

bool CAddressModesTest::ExecuteTest(UINT iTest)
{
	ADDRESSTESTCASE tc;
	tc.nTest = iTest-1;
	if(	tc.AddressMode >= tcCount.AddressMode ||
		tc.LookupType  >= tcCount.LookupType  ||   
		tc.MipLevel    >= tcCount.MipLevel    ||
		tc.View        >= tcCount.View         )
    {
        WriteToLog( "No such test (%d)\n", iTest);
		return false;
    }
	m_isPreswizzled = (BOOL)(tc.LookupType == PRESAMPLED);

	float AdaptiveTess_x = 0;         
	float AdaptiveTess_y = 0;         
	float AdaptiveTess_z = 0;         
	float AdaptiveTess_w = (float)( 1 << tc.MipLevel ); 
	SetRenderState(D3DRS_ADAPTIVETESS_X, *(DWORD*)&AdaptiveTess_x);
	SetRenderState(D3DRS_ADAPTIVETESS_Y, *(DWORD*)&AdaptiveTess_y);
	SetRenderState(D3DRS_ADAPTIVETESS_Z, *(DWORD*)&AdaptiveTess_z);
	SetRenderState(D3DRS_ADAPTIVETESS_W, *(DWORD*)&AdaptiveTess_w);

	if ( tc.LookupType == PRESAMPLED )
	{
		float n = m_MaxTess/7.f;
		m_TessLevel = (float)( (int)( max(1.f, n*(tc.MipLevel+1)) ) );
		SetNPatchMode(m_TessLevel);
	}
	else
	{
		m_TessLevel = (1 << tc.MipLevel) * m_MaxTess;
		SetNPatchMode(m_TessLevel);
		SetRenderState(D3DRS_MAXTESSELLATIONLEVEL, *(DWORD*)&m_TessLevel);
		m_TessLevel = m_MaxTess;
	}

	if ( (tc.LookupType != PRESAMPLED && (KeySet(_T("fixed")) ? tc.View == 1 : tc.View == 0) && tc.MipLevel == 0) ||
		 (tc.LookupType == PRESAMPLED && tc.View == 1 && tc.MipLevel == 0) || 
		 (m_pTexture == NULL || m_pDMTexture == NULL) )
	{
        RELEASE(m_pDMTexture);
		RELEASE(m_pTexture);

		if ( tc.LookupType == PRESAMPLED )
			SetRenderState(D3DRS_ENABLEADAPTIVETESSELLATION, (DWORD)FALSE);
		else
			SetRenderState(D3DRS_ENABLEADAPTIVETESSELLATION, (DWORD)TRUE);
		
		if (!GenerateTextures(false, true))
		{
			TESTCASE tempTC;
			tempTC.nTest = tc.nTest;
			UINT i = tempTC.MipLevel;
			for ( ; i < tcCount.MipLevel; i++)
			{
				tempTC.MipLevel = i;
				if (tempTC.MipLevel != tc.MipLevel)
				{
					tempTC.View = 0;
					SkipTestRange(tempTC.nTest+1, tempTC.nTest+m_nViewMatrices + 1);
				}
				else
					SkipTestRange(tempTC.nTest+1, tempTC.nTest+m_nViewMatrices + 1-tempTC.View);
			}
			return false;
		}
		if(KeySet(_T("noDisMap")) || KeySet(_T("fixed")))
		{
			if(KeySet(_T("dmtex")))
				SetTexture(0, m_pDMTexture);
			else
				SetTexture(0, m_pTexture);
			if(KeySet(_T("fixed")))
				SetVertexShader(FVF_VERTEX);
			else
			{
				SetVertexDeclaration(m_pDecls[3]);
				SetVertexShader(m_pShaders[3]);
			}
			SetSamplerState(0, SAMP_ADDRESSU, AddressModes[tc.AddressMode]);
			SetSamplerState(0, SAMP_ADDRESSV, AddressModes[tc.AddressMode]);
			SetSamplerState(0, SAMP_ADDRESSW, AddressModes[tc.AddressMode]);
		}
		else
		{
			SetTexture(          0, m_pTexture);
			SetTexture(DMAPSAMPLER, m_pDMTexture);
			
			SetSamplerState(0, SAMP_ADDRESSU, AddressModes[tc.AddressMode]);
			SetSamplerState(0, SAMP_ADDRESSV, AddressModes[tc.AddressMode]);
			SetSamplerState(0, SAMP_ADDRESSW, AddressModes[tc.AddressMode]);
			SetSamplerState(DMAPSAMPLER, SAMP_ADDRESSU, AddressModes[tc.AddressMode]);
			SetSamplerState(DMAPSAMPLER, SAMP_ADDRESSV, AddressModes[tc.AddressMode]);
			SetSamplerState(DMAPSAMPLER, SAMP_ADDRESSW, AddressModes[tc.AddressMode]);
			if (tc.View > 1)
			{
				SetStreamSource(0, m_pVB1, sizeof(VERTEX));
				SetSamplerState(0, SAMP_MAXMIPLEVEL, (DWORD)tc.MipLevel);

				if (tc.LookupType == LOOKUP)
				{
					SetVertexDeclaration(m_pDecls[0]);
					SetVertexShader(m_pShaders[0]);
				}
				else if (tc.LookupType == IMPLICIT)
				{
					SetVertexDeclaration(m_pDecls[1]);
					SetVertexShader(m_pShaders[1]);
				}
				else //tc.LokkupType = PRESAMPLED
				{
					DWORD offset = 0;
					float n = m_MaxTess / 7.f;
					for (int i = 0; i < tc.MipLevel; i++)
						offset += ((int)( max(1.f, n*(i+1)) )+1)*((int)( max(1.f, n*(i+1)) )+2);
					SetSamplerState(DMAPSAMPLER, SAMP_DMAPOFFSET, offset);
					SetVertexDeclaration(m_pDecls[2]);
					SetVertexShader(m_pShaders[2]);
				}
			}
		}
	}

	if (tc.View == 0)
	{
		SetStreamSource(0, m_pVB2, sizeof(VERTEX));
		if (KeySet(_T("noDisMap")))
		{
			SetSamplerState(0, SAMP_MAXMIPLEVEL, (DWORD)0);
			
			if(KeySet(_T("fixed")))
				SetVertexShader(FVF_VERTEX);
			else
			{
				SetVertexDeclaration(m_pDecls[6]);
				SetVertexShader(m_pShaders[6]);
			}
		}
		else
		{
			SetSamplerState( 0, SAMP_MAXMIPLEVEL, (DWORD)0);
			
			if (tc.LookupType == LOOKUP)
			{
				SetVertexDeclaration(m_pDecls[4]);
				SetVertexShader(m_pShaders[4]);
			}
			else //tc.LookupType = IMPLICIT
			{
				SetVertexDeclaration(m_pDecls[5]);
				SetVertexShader(m_pShaders[5]);
			}
		}
	}
	else if (tc.View == 1)
	{
        SetStreamSource(0, m_pVB1, sizeof(VERTEX));
		if (KeySet(_T("noDisMap")) || KeySet(_T("fixed")))
		{
			SetSamplerState(0, SAMP_MAXMIPLEVEL, (DWORD)tc.MipLevel);
			
			if(KeySet(_T("fixed")))
				SetVertexShader(FVF_VERTEX);
			else
			{
				SetVertexDeclaration(m_pDecls[3]);
				SetVertexShader(m_pShaders[3]);
			}
		}
		else
		{
			SetSamplerState( 0, SAMP_MAXMIPLEVEL, (DWORD)tc.MipLevel);
			
			if (tc.LookupType == LOOKUP)
			{
				SetVertexDeclaration(m_pDecls[0]);
				SetVertexShader(m_pShaders[0]);
			}
			else if (tc.LookupType == IMPLICIT)
			{
				SetVertexDeclaration(m_pDecls[1]);
				SetVertexShader(m_pShaders[1]);
			}
			else //tc.LokkupType = PRESAMPLED
			{
				DWORD offset = 0;
				float n = m_MaxTess / 7.f;
				for (int i = 0; i < tc.MipLevel; i++)
					offset += ((int)( max(1.f, n*(i+1)) )+1)*((int)( max(1.f, n*(i+1)) )+2);
				SetSamplerState(DMAPSAMPLER, SAMP_DMAPOFFSET, offset);
				SetVertexDeclaration(m_pDecls[2]);
				SetVertexShader(m_pShaders[2]);
			}
		}
	}

	D3DXMATRIX mat;
	if (KeySet(_T("fixed")))
	{
		ASSERT(tc.View > 0);
		ASSERT(tc.View < VIEW_COUNT);
		mat = m_ViewMatrices[tc.View-1];
		if (!SetTransform(D3DTS_VIEW, &mat))
		{
			WriteToLog("Can't set up a veiw matrix.\n");			
			EndScene();
			return false;
		}
	}
	else
	{
		//for tc.View == 0 we don't need to set matrices
		if (tc.View >= 1)
		{
			D3DXMatrixMultiply( &mat, &m_ViewMatrices[tc.View-1], &m_ProjMatrix);
			D3DXMatrixTranspose( &mat, &mat );
			SetVertexShaderConstant(0, &mat, 4);
		}
	}

	msgString[0] = '\0';
	if( (tc.LookupType == LOOKUP && m_DMap && m_Tex) && !KeySet(_T("nodismap")) && !(KeySet(_T("fixed"))))
	{
		SetTextureStageState((DWORD)0, D3DTSS_COLOROP, (DWORD)D3DTOP_MODULATE);
		sprintf(msgString, "%sMODULATE", msgString);
	}
	else if((m_Tex && !m_DMap && tc.LookupType == LOOKUP) || KeySet(_T("nodismap")) || KeySet(_T("fixed")))
	{
		SetTextureStageState((DWORD)0, D3DTSS_COLOROP, (DWORD)D3DTOP_SELECTARG1);
		sprintf(msgString, "%sSELECTARG1", msgString);
	}
	else 
	{
		SetTextureStageState((DWORD)0, D3DTSS_COLOROP, (DWORD)D3DTOP_SELECTARG2);
		sprintf(msgString, "%sSELECTARG2", msgString);
	}
	TCHAR tempStr[80];
	sprintf( tempStr, TEXT("%s, %d mip, %.2f tess, %s address"),
			LookupStr[tc.LookupType], tc.MipLevel, m_TessLevel, AddressModeStr[tc.AddressMode] );
	sprintf(msgString, "%s %s", msgString, tempStr);

	BeginTestCase(msgString, iTest);
    BeginScene();
				
	if (tc.View == 0)
	{
		if (! DrawPrimitive(D3DPT_TRIANGLESTRIP, tc.MipLevel*4, 2, CD3D_BOTH) )
		{
			WriteToLog("DrawPrimitive failed.\n");
			EndScene();
			return false;
		}
	}
	else
	{
		if (! DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2, CD3D_BOTH) )
		{
			WriteToLog("DrawPrimitive failed.\n");
			EndScene();
			return false;
		}
	}
	EndScene();
	return true;
}

bool CAddressModesTest::TestTerminate()
{
	RELEASE(m_pVB1);
	RELEASE(m_pVB2);
	RELEASE(m_pDMTexture);
	RELEASE(m_pTexture);
	for(int i = 0; i < m_nShaders; i++)
	{
		RELEASE(m_pShaders[i]);
		RELEASE(m_pDecls[i]);
	}
	return true;
}

bool CAddressModesTest::CheckAddressModeSupport(D3DTEXTUREADDRESS mode)
{
	CAPS caps;
	m_pSrcDevice->GetDeviceCaps(&caps); 
	
	switch (mode)
	{
		case D3DTADDRESS_WRAP:
			if ( !(caps.TextureAddressCaps & D3DPTADDRESSCAPS_WRAP) )
			{
				WriteToLog("CAddressModesTest: D3DTADDRESS_WRAP is not supported.\n");
				return false;
			}
			break;
		case D3DTADDRESS_MIRROR:
			if ( !(caps.TextureAddressCaps & D3DPTADDRESSCAPS_MIRROR) )
			{
				WriteToLog("CAddressModesTest: D3DTADDRESS_MIRROR is not supported.\n");
				return false;
			}
			break;
		case D3DTADDRESS_CLAMP:
			if ( !(caps.TextureAddressCaps & D3DPTADDRESSCAPS_CLAMP) )
			{
				WriteToLog("CAddressModesTest: D3DTADDRESS_CLAMP is not supported.\n");
				return false;
			}
			break;
		case D3DTADDRESS_BORDER:
			if ( !(caps.TextureAddressCaps & D3DPTADDRESSCAPS_BORDER) )
			{
				WriteToLog("CAddressModesTest: D3DTADDRESS_BORDER is not supported.\n");
				return false;
			}  
			break;
		case D3DTADDRESS_MIRRORONCE:
			if ( !(caps.TextureAddressCaps & D3DPTADDRESSCAPS_MIRRORONCE) )
			{
				WriteToLog("CAddressModesTest: D3DTADDRESS_MIRRORONCE is not supported.\n");
				return false;
			}  
			break;
		default:
		{
			WriteToLog("Unknown address mode %d.\n", mode);
			return false;
		}
	}

	return true;
}




	