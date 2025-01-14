#define STRICT
#define BURN_IN_HELL
//#define D3D_OVERLOADS
#include "d3dlocus.h"
#include "cd3dtest.h"
#include "3dmath.h"
#include "WBuffer.h"

#define FVF_LVERTEX_ ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1 )

//extern bool Unique(DWORD dwVersion, PBUFFER *pBuffers, LPPIXELFORMAT pFormat);

// Global Test and App instance
//CD3DWindowFramework	App;

//******************************************************************************
//
// Function:
//
//     ValidateDisplay
//
// Description:
//
//     Evaluate the given display information in order to determine whether or
//     not the display is capable of rendering the scene.  If not, the given 
//     display will not be included in the display list.
//
// Arguments:
//
//     CDirect3D8* pd3d                 - Pointer to the Direct3D object
//
//     D3DCAPS8* pd3dcaps               - Capabilities of the device
//
//     D3DDISPLAYMODE*                  - Display mode into which the device
//                                        will be placed
//
// Return Value:
//
//     TRUE if the scene can be rendered using the given display, FALSE if
//     it cannot.
//
//******************************************************************************
extern "C" BOOL PREPEND_MODULE(_ValidateDisplay)(CDirect3D8* pd3d, D3DCAPS8* pd3dcaps, D3DDISPLAYMODE* pd3ddm) {

    return TRUE;
}

//******************************************************************************
//
// Function:
//
//     ExhibitScene
//
// Description:
//
//     Create the scene, pump messages, process user input,
//     update the scene, render the scene, and release the scene when finished.
//
// Arguments:
//
//     CDisplay* pDisplay           - Pointer to the Display object
//
//     int* pnExitCode              - Optional pointer to an integer that will
//                                    be set to the exit value contained in the 
//                                    wParam parameter of the WM_QUIT message 
//                                    (if received)
//
// Return Value:
//
//     TRUE if the display remains functional on exit, FALSE otherwise.
//
//******************************************************************************
extern "C" BOOL PREPEND_MODULE(_ExhibitScene)(CDisplay* pDisplay, int* pnExitCode) {

    CWBufferTest*   WBuffer;
    BOOL            bRet;

    // Verify that the display has been initialized
    if (!pDisplay) {
        return FALSE;
    }

    WBuffer = new CWBufferTest();
    if (!WBuffer) {
        return FALSE;
    }

    // Initialize the scene
    if (!WBuffer->Create(pDisplay)) {
        WBuffer->Release();
        return FALSE;
    }

    bRet = WBuffer->Exhibit(pnExitCode);

    // Clean up the scene
    WBuffer->Release();

    return bRet;
}

CWBufferTest::CWBufferTest()
{
    D3DFORMAT fmtd[] = {
#ifndef UNDER_XBOX
        D3DFMT_D16,
        D3DFMT_D16,
        D3DFMT_D24S8,
        D3DFMT_D24S8
#else
        D3DFMT_LIN_D16,
        D3DFMT_LIN_D24S8,
        D3DFMT_LIN_F16,
#endif // UNDER_XBOX
    };

    m_uZBufferFormats = countof(fmtd);
    memcpy(m_fmtd, fmtd, m_uZBufferFormats * sizeof(D3DFORMAT));

	m_szTestName = TEXT("W Buffer");
	m_szCommandKey = TEXT("WBuffer");

    // Disable textures
    m_ModeOptions.fTextures = false;

    m_pd3dsOriginalZ = NULL;
}

CWBufferTest::~CWBufferTest()
{
    // Restore the original depth buffer
    if (m_pd3dsOriginalZ) {
        m_pDevice->SetRenderTarget(NULL, m_pd3dsOriginalZ);
    }
}
/*
bool CWBufferTest::AddModeList(PMODELIST pMode, PVOID pData, DWORD dwDataSize)
{
    PBUFFER pBuffers = NULL;

	// Test all of the common ZBuffer Formats
    if (IsConformance())
    {
        for (int i=0; i < m_uZBufferFormats; i++)
		{
            if (Unique(m_dwVersion, &pBuffers, &m_pCommonZBufferFormats[i].ddpfPixelFormat))
            {
				pMode->D3DMode.nZBufferFormat = i;

				CD3DTest::AddModeList(pMode,pData,dwDataSize);
			}
        }
    }
    else
    {
		// Let the framework handle stress mode
		CD3DTest::AddModeList(pMode,pData,dwDataSize);
	}

    // Cleanup any memory that we alloc'd
    if (pBuffers)
    {
        PBUFFER pBuffer, pTemp;

        for (pBuffer = pBuffers; pBuffer; pBuffer = pTemp)
        {
            pTemp = pBuffer->pNext;
            free(pBuffer);
        }
    }

    return true;
}
*/
bool CWBufferTest::SetDefaultRenderStates(void)
{
	// Turn off Lighting for DX7
	if (m_dwVersion >= 0x0700)
		SetRenderState(D3DRENDERSTATE_LIGHTING, (DWORD)FALSE);

	// Make sure that we are using the W Buffer
//	SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, (DWORD)TRUE);
	SetRenderState(D3DRENDERSTATE_ZENABLE, (DWORD)D3DZB_USEW);
	return true;
}

bool CWBufferTest::SetDefaultMatrices(void)
{
    D3DMATRIX	Matrix;

	// Setup a Projection Matrix for W buffering
	Matrix = ProjectionMatrix(0.01f, 100.0f, pi/3);

    if (!SetTransform(D3DTRANSFORMSTATE_PROJECTION,&Matrix))
	{
		WriteToLog("SetTransform(ProjectionMatrix) failed with HResult = %s.\n",GetLastError());
        return false;
	}

	Matrix = IdentityMatrix();

    if (!SetTransform(D3DTRANSFORMSTATE_VIEW,&Matrix))
	{
		WriteToLog("SetTransform(ViewMatrix) failed with HResult = %s.\n",GetLastError());
        return false;
	}

    return true;
}

UINT CWBufferTest::TestInitialize(void)
{
//    UINT n = m_pMode->nSrcDevice;

	// Setup the Test range
	SetTestRange(1,MAX_TESTS * m_uZBufferFormats);

	// Check the caps we are interested in
	DWORD dwRasterCaps = m_d3dcaps.RasterCaps;
 
	// Make sure the driver supports W Buffering.
	if (!(dwRasterCaps & D3DPRASTERCAPS_WBUFFER))
	{
		WriteToLog("Capability not supported: W Buffer.\n");
		return D3DTESTINIT_SKIPALL;
	}

    // Save the original depth buffer
    m_pDevice->GetDepthStencilSurface(&m_pd3dsOriginalZ);

	// Initialize the Plane
	PlaneList[0] = LVERTEX_(cD3DVECTOR(-4.0f, -4.0f, +1.0f), RGBA_MAKE(0,0,255,255),0, 0.0f,0.0f);
	PlaneList[1] = LVERTEX_(cD3DVECTOR(-4.0f, +4.0f, +1.0f), RGBA_MAKE(0,0,255,255),0, 0.0f,0.0f);
	PlaneList[2] = LVERTEX_(cD3DVECTOR(+4.0f, +4.0f, +1.0f), RGBA_MAKE(0,0,255,255),0, 0.0f,0.0f);
	PlaneList[3] = LVERTEX_(cD3DVECTOR(+4.0f, -4.0f, +1.0f), RGBA_MAKE(0,0,255,255),0, 0.0f,0.0f);

	// Initialize the Waffle
	for (int i=0; i < 40; i+=8)
	{
		WaffleList[0][i]   = LVERTEX_(cD3DVECTOR(-5.0f + (0.25f * i), -5.0f, +1.0f), RGBA_MAKE(255,0,0,255),0, 0.0f,0.0f);
		WaffleList[0][i+1] = LVERTEX_(cD3DVECTOR(-5.0f + (0.25f * i), +5.0f, +1.0f), RGBA_MAKE(255,0,0,255),0, 0.0f,0.0f);
		WaffleList[0][i+2] = LVERTEX_(cD3DVECTOR(-4.5f + (0.25f * i), -5.0f,  0.0f), RGBA_MAKE(255,0,0,255),0, 0.0f,0.0f);
		WaffleList[0][i+3] = LVERTEX_(cD3DVECTOR(-4.5f + (0.25f * i), +5.0f,  0.0f), RGBA_MAKE(255,0,0,255),0, 0.0f,0.0f);
		WaffleList[0][i+4] = LVERTEX_(cD3DVECTOR(-4.0f + (0.25f * i), -5.0f, -1.0f), RGBA_MAKE(255,0,0,255),0, 0.0f,0.0f);
		WaffleList[0][i+5] = LVERTEX_(cD3DVECTOR(-4.0f + (0.25f * i), +5.0f, -1.0f), RGBA_MAKE(255,0,0,255),0, 0.0f,0.0f);
		WaffleList[0][i+6] = LVERTEX_(cD3DVECTOR(-3.5f + (0.25f * i), -5.0f,  0.0f), RGBA_MAKE(255,0,0,255),0, 0.0f,0.0f);
		WaffleList[0][i+7] = LVERTEX_(cD3DVECTOR(-3.5f + (0.25f * i), +5.0f,  0.0f), RGBA_MAKE(255,0,0,255),0, 0.0f,0.0f);

		WaffleList[1][i]   = LVERTEX_(cD3DVECTOR(-5.0f + (0.25f * i), -5.0f, -1.0f), RGBA_MAKE(0,255,0,255),0, 0.0f,0.0f);
		WaffleList[1][i+1] = LVERTEX_(cD3DVECTOR(-5.0f + (0.25f * i), +5.0f, -1.0f), RGBA_MAKE(0,255,0,255),0, 0.0f,0.0f);
		WaffleList[1][i+2] = LVERTEX_(cD3DVECTOR(-4.5f + (0.25f * i), -5.0f,  0.0f), RGBA_MAKE(0,255,0,255),0, 0.0f,0.0f);
		WaffleList[1][i+3] = LVERTEX_(cD3DVECTOR(-4.5f + (0.25f * i), +5.0f,  0.0f), RGBA_MAKE(0,255,0,255),0, 0.0f,0.0f);
		WaffleList[1][i+4] = LVERTEX_(cD3DVECTOR(-4.0f + (0.25f * i), -5.0f, +1.0f), RGBA_MAKE(0,255,0,255),0, 0.0f,0.0f);
		WaffleList[1][i+5] = LVERTEX_(cD3DVECTOR(-4.0f + (0.25f * i), +5.0f, +1.0f), RGBA_MAKE(0,255,0,255),0, 0.0f,0.0f);
		WaffleList[1][i+6] = LVERTEX_(cD3DVECTOR(-3.5f + (0.25f * i), -5.0f,  0.0f), RGBA_MAKE(0,255,0,255),0, 0.0f,0.0f);
		WaffleList[1][i+7] = LVERTEX_(cD3DVECTOR(-3.5f + (0.25f * i), +5.0f,  0.0f), RGBA_MAKE(0,255,0,255),0, 0.0f,0.0f);
	}

	WaffleList[0][i]   = LVERTEX_(cD3DVECTOR(-5.0f + (0.25f * i), -5.0f, +1.0f), RGBA_MAKE(255,0,0,255),0, 0.0f,0.0f);
	WaffleList[0][i+1] = LVERTEX_(cD3DVECTOR(-5.0f + (0.25f * i), +5.0f, +1.0f), RGBA_MAKE(255,0,0,255),0, 0.0f,0.0f);
	WaffleList[1][i]   = LVERTEX_(cD3DVECTOR(-5.0f + (0.25f * i), -5.0f, -1.0f), RGBA_MAKE(0,255,0,255),0, 0.0f,0.0f);
	WaffleList[1][i+1] = LVERTEX_(cD3DVECTOR(-5.0f + (0.25f * i), +5.0f, -1.0f), RGBA_MAKE(0,255,0,255),0, 0.0f,0.0f);

    // Tell RenderPrimitive disable API cycling
    // m_dwPrimitives = 1; should be changed to something like 
    // SetAPI(RP_DP);

	// Init the msgString to empty
//	msgString[0] = '\0';

	// Log the current W Buffer format
//    if (m_dwVersion <= 0x0700)
//    {
//        PIXELFORMAT Format;
//        memcpy(&Format, &m_pCommonZBufferFormats[m_pMode->nZBufferFormat].ddpfPixelFormat, sizeof(Format));

//	    sprintf(msgString, "%sW Buffer Format, Z: %d-bit (0x%x), Stencil: %d-bit (0x%x).\n",msgString,Format.dwZBufferBitDepth - Format.dwStencilBitDepth,Format.dwZBitMask,Format.dwStencilBitDepth,Format.dwStencilBitMask);
//    }
//    else
//	    sprintf(msgString, "%sW Buffer Format: %s.\n",msgString,D3DFmtToString(m_fmtd[nZBufferFormat]));

//    WriteToLog(msgString);

	return D3DTESTINIT_RUN;
}

bool CWBufferTest::ClearFrame(void)
{
	return Clear(RGBA_MAKE(0,0,0,255));
}

bool CWBufferTest::ExecuteTest(UINT uTestNum)
{
	float		fRotate;
	D3DMATRIX	Matrix;
	char 		szBuffer[80];
    UINT        uTest = (uTestNum - 1) % MAX_TESTS + 1;

    if (uTest == 1) {
        CSurface8* pd3ds;
        CSurface8* pd3dsRT;
        D3DSURFACE_DESC d3dsd;
        int nZBufferFormat = (uTestNum - 1) / MAX_TESTS;
        if (FAILED(m_pDevice->GetRenderTarget(&pd3dsRT))) {
            Log(LOG_FAIL, TEXT("IDirect3DDevice8::GetRenderTarget failed"));
            SkipTests(MAX_TESTS);
            return false;
        }
        pd3dsRT->GetDesc(&d3dsd);
        pd3dsRT->Release();
        if (FAILED(m_pDevice->CreateDepthStencilSurface(d3dsd.Width, d3dsd.Height, m_fmtd[nZBufferFormat], D3DMULTISAMPLE_NONE, &pd3ds))) {
            Log(LOG_FAIL, TEXT("IDirect3DDevice8::CreateDepthStencilSurface failed on format %s"), D3DFmtToString(m_fmtd[nZBufferFormat]));
            SkipTests(MAX_TESTS);
            return false;
        }
    	msgString[0] = '\0';
	    sprintf(msgString, "%sW Buffer Format: %s.\n",msgString,D3DFmtToString(m_fmtd[nZBufferFormat]));
        WriteToLog(msgString);
        if (FAILED(m_pDevice->SetRenderTarget(NULL, pd3ds))) {
            Log(LOG_FAIL, TEXT("IDirect3DDevice8::SetRenderTarget failed on depth format %s"), D3DFmtToString(m_fmtd[nZBufferFormat]));
            SkipTests(MAX_TESTS);
            pd3ds->Release();
            return false;
        }
        pd3ds->Release();
    }

	// Initialize some variables
	msgString[0] = 0;
	sprintf(msgString, "");

	// Start with the identity matrix
	Matrix = IdentityMatrix();
 	Matrix = ScaleMatrix(0.5f, 0.5f, 1.0f);

	fRotate = 30.0f + (uTest - 1);

	if (fRotate > 360.0f ) 
		fRotate -= 360.0f;

	// Convert to radians and rotate around Z
	fRotate = (fRotate * 2 * pi) / 360.0f;
	Matrix = MatrixMult(Matrix, RotateZMatrix(fRotate));

	// Update the logfile and status window
	sprintf(msgString,"Depth: %0.2f, Rotation: %0.2f",(float)uTest, fRotate);

	szBuffer[0] = 0;
	sprintf(szBuffer,"$y%0.2f [0.01 - 100.00]",(float)uTest);
	WriteStatus("$gDepth [near - far]",szBuffer);

	szBuffer[0] = 0;
	sprintf(szBuffer,"$y%0.2f",fRotate);
	WriteStatus("$gRotation (radians)",szBuffer);

	// Finialy translate and set the world
	Matrix = MatrixMult(Matrix, TranslateMatrix(0.0f, 0.0f, ((float)uTest + 0.01f)));

    if (!SetTransform(D3DTRANSFORMSTATE_WORLD,&Matrix))
	{
		WriteToLog("SetTransform(WorldMatrix) failed with HResult = %s.\n",GetLastError());
		TestFinish(D3DTESTFINISH_ABORT);
        return false;
	}

	// Tell the log that we are starting
	BeginTestCase(msgString);

    return true;
}

void CWBufferTest::SceneRefresh(void)
{
 	if (BeginScene())
	{
		// Render the data.
		RenderPrimitive(D3DPT_TRIANGLESTRIP,FVF_LVERTEX_,WaffleList[0],42,NULL,0,0);
		RenderPrimitive(D3DPT_TRIANGLESTRIP,FVF_LVERTEX_,WaffleList[1],42,NULL,0,0);
		RenderPrimitive(D3DPT_TRIANGLEFAN,FVF_LVERTEX_,PlaneList,4,NULL,0,0);

		EndScene();
	}
}

bool CWBufferTest::ProcessFrame(void)
{
	char	szBuffer[80];
	static	int nPass = 0;
	static  int nFail = 0;
    bool	bResult;

     // Use the standard 15%
    bResult = GetCompareResult(0.15f, 0.78f, 0);

	// Tell the logfile how the compare went
    if (bResult)
    {
		(m_fIncrementForward) ? nPass++ : nPass--;
		sprintf(szBuffer,"$yPass: %d, Fail: %d", nPass, nFail);
        Pass();
    }
	else
	{
		(m_fIncrementForward) ? nFail++ : nFail--;
		sprintf(szBuffer,"$yPass: %d, Fail: %d", nPass, nFail);
        Fail();
	}

	WriteStatus("$gOverall Results",szBuffer);

    return bResult;
}

