#include "d3dlocus.h"
#include "cd3dtest.h"
#include "dxlegacy.h"
#include "3dmath.h"
#include "Clipping.h"


WTFan::WTFan()
{
    m_pInputVertices = 0;
    m_pRenderVertices = 0;

    m_szTestName = TEXT("Triangle fan clipping");
    m_szCommandKey = TEXT("tfan");
    
    m_dwRotations = 140;
}

WTFan::~WTFan()
{
    if (m_pInputVertices)
        delete [] m_pInputVertices;
    if (m_pRenderVertices)
        delete [] m_pRenderVertices;
}

UINT WTFan::TestInitialize()
{
    if (!GenerateTriangleFan())
    {
        WriteToLog("%s: Can't allocate memory for vertices\n", m_szTestName);
        return D3DTESTINIT_ABORT;
    }

    // Calculate total tests
    if (0x0600 == m_dwVersion)
    {
        // Total tests:     
        // -API:            m_dwRPFullDX6APIListSize (14) * frequency (2)
        // -FVF:            2
        // -properties:     7

        m_dwTotalAPIs = m_dwRPFullDX6APIListSize * 2;
        m_dwTotalTests = m_dwTotalAPIs * 2 * 7; // (392)

        SetAPI(m_pRPFullDX6APIList, m_dwRPFullDX6APIListSize, 2);
    }
    else if (0x0700 == m_dwVersion)
    {
        // Total tests:     
        // -API:            m_dwRPFullDX7APIListSize (14) * frequency (2) 
        // -FVF:            2
        // -properties:     7

        m_dwTotalAPIs = m_dwRPFullDX7APIListSize * 2;
        m_dwTotalTests = m_dwTotalAPIs * 2 * 7; // (392)

        SetAPI(m_pRPFullDX7APIList, m_dwRPFullDX7APIListSize, 2);
    }
    else if (0x0800 == m_dwVersion)
    {
        // Total tests:     
        // -API:            m_dwRPFullDX7APIListSize (?) * frequency (2) 
        // -FVF:            2
        // -properties:     7

        m_dwTotalAPIs = m_dwRPFullDX8APIListSize * 2;
        m_dwTotalTests = m_dwTotalAPIs * 2 * 7; // (392)

        SetAPI(m_pRPFullDX8APIList, m_dwRPFullDX8APIListSize, 2);
    }
    else
    {
        WriteToLog("%s: Incorrect DirectX version\n", m_szTestName);
    }

    return WireClip::TestInitialize();
}

bool WTFan::TestTerminate(void)
{
    if (m_pInputVertices)
    {
        delete [] m_pInputVertices;
        m_pInputVertices = NULL;
    }
    if (m_pRenderVertices)
    {
        delete [] m_pRenderVertices;
        m_pRenderVertices = NULL;
    }
    return WireClip::TestTerminate();
}


bool WTFan::GenerateTriangleFan(void) {
	float	fRadius = 0.8f;
	float	fInset = 0.4f;
	int		iPetals = 5;
	int		iSegmentsPerPetal = 10;
	DWORD	dwSegments = (DWORD)(iPetals * iSegmentsPerPetal);
	DWORD	dwCount;
	float	fStartRad = 0.f;
	float	fRadInc = 2.f * pi / (float)dwSegments;
	
	m_dwVertices = dwSegments + 1;
	m_pInputVertices = new D3DVERTEX [ m_dwVertices ];
	if (m_pInputVertices == NULL)
		return false;

	m_pInputVertices[0] = D3DVERTEX(cD3DVECTOR(0.f, 0.f, 0.f), cD3DVECTOR(0.f, 0.f, 1.f), 0.f, 0.f);

	for (dwCount = 1; dwCount < dwSegments; dwCount++) {
		int		iCurrentSegment		= (dwCount-1) % iSegmentsPerPetal;
		float	fRad				= (float)iCurrentSegment * pi / (float)iSegmentsPerPetal;
		float	fTempRad				= fRadius - fInset + fInset*(float)sin(fRad);
		m_pInputVertices[dwCount] = D3DVERTEX(
			cD3DVECTOR(fTempRad*(float)cos(fStartRad), fTempRad*(float)sin(fStartRad), 0.f),
			cD3DVECTOR((float)cos(fStartRad),(float)sin(fStartRad),0.0f),
			0.0f,0.0f);
		fStartRad += fRadInc;
	}
	m_pInputVertices[dwCount] = m_pInputVertices[1];


    m_pRenderVertices = new BYTE [ (3 + 3 + 8*2)*sizeof(float)*m_dwVertices ];
    if (m_pRenderVertices == NULL)
        return false;

	return true;
}

bool WTFan::ResolveTestNumber()
{
    DWORD   num = m_dwCurrentTestNumber - 1;
    m_dwCurrentAPI = num % m_dwTotalAPIs;   num /= m_dwTotalAPIs;
    m_dwCurrentFVF = num % 2;               num /= 2;
    m_dwCurrentProperties = num % 7;        num /= 7;

    // Resolve current FVF
    if (m_dwCurrentFVF == 0)        m_dwCurrentFVF = D3DFVF_XYZ | D3DFVF_NORMAL;
    else                            m_dwCurrentFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
    
    // Add specular to the FVF
    if ((m_dwCurrentFVF & D3DFVF_DIFFUSE) && (m_dwCurrentProperties == CP_SPECULAR || m_dwCurrentProperties == CP_FOG))
        m_dwCurrentFVF |= D3DFVF_SPECULAR;

    // Add texture to the FVF
    if (m_dwCurrentProperties == CP_TEXTURE)    m_dwCurrentFVF |= 1 << D3DFVF_TEXCOUNT_SHIFT;
    if (m_dwCurrentProperties == CP_MULTITEX)   m_dwCurrentFVF |= (m_dwMaxTextures) << D3DFVF_TEXCOUNT_SHIFT;

    // Primitive type
    m_dwCurrentPrimitiveType = D3DPT_TRIANGLEFAN;

    return WireClip::ResolveTestNumber();
}


bool WTFan::SetWorldMatrix(void) {
	D3DMATRIX	Matrix;

    Matrix = MatrixMult(
        TranslateMatrix(
            (float)sin(float(m_dwCurrentRotation)*0.02f)*(float)sin(float(m_dwCurrentRotation)*0.2f), 
            (float)cos(float(m_dwCurrentRotation)*0.02f)*(float)cos(float(m_dwCurrentRotation)*0.2f), 
            0.f),
            RotateZMatrix((float)m_dwCurrentRotation)
    );

    if (m_dwCurrentZClip) {
		D3DMATRIX	ZMatrix;
		ZMatrix = RotateXMatrix(0.12f);
		Matrix = MatrixMult(ZMatrix, Matrix);
	}
    SetTransform(D3DTRANSFORMSTATE_WORLD,&Matrix);

    HRESULT hr = GetLastError();
	if (hr != D3D_OK)
	{
		WriteToLog("%s: SetWorldMatrix() returned an error on test %d\n", m_szTestName, m_dwCurrentTestNumber);
		return false;
	}
    return true;		
}

