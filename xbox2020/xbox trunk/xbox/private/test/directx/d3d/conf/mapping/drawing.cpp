//#define D3D_OVERLOADS
#include "d3dlocus.h"
#include "cd3dtest.h"
#include "Mapping.h"

//************************************************************************
// Internal API:    DrawGrid
//
// Purpose:         
//
// Return:          None.
//************************************************************************

void CMappingTest::DrawGrid(int nTest, int nTexFormats)
{
	if (nTest <= 256)
	{
		int nTL, nTR, nBL, nBR;

		// Decide which UV coordinates to use
		nTL = (m_uLastTestNum / (64 * nTexFormats)) % 4;
		nTR = (m_uLastTestNum / (16 * nTexFormats)) % 4;
		nBL = (m_uLastTestNum / (4 * nTexFormats)) % 4;
		nBR = (m_uLastTestNum / nTexFormats) % 4;

		sprintf(msgString, "%sMap:(%.0f,%.0f)(%.0f,%.0f)(%.0f,%.0f)(%.0f,%.0f), ",msgString,
					TopLeft[nTL].u, TopLeft[nTL].v,
					TopRight[nTR].u, TopRight[nTR].v,
					BottomLeft[nBL].u, BottomLeft[nBL].v,
					BottomRight[nBR].u, BottomRight[nBR].v);

		// Setup the Mesh
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X +   9.5f,   9.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, TopLeft[nTL].u,     TopLeft[nTL].v);
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X + 265.5f,   9.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, TopRight[nTR].u,    TopRight[nTR].v);
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X + 265.5f, 265.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, BottomRight[nBR].u, BottomRight[nBR].v);
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X +   9.5f,   9.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, TopLeft[nTL].u,     TopLeft[nTL].v);
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X + 265.5f, 265.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, BottomRight[nBR].u, BottomRight[nBR].v);
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X +   9.5f, 265.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, BottomLeft[nBL].u,  BottomLeft[nBL].v);
	}
	else
	{
		int nRot;
		float fTile;

		// Decide which rotation to use
		nRot = (nTest - 257) / 15;

		// Decide how much tiling to use
		if (nTest <= (nRot * 15 + 264))
			fTile = (nTest - (nRot * 15 + 256.0f)) / 8.0f;
		else
			fTile = nTest - (nRot * 15 + 263.0f);

		sprintf(msgString, "%sMap:",msgString);

		if (((TopLeft[nRot].u == 0) || (fTile*TopLeft[nRot].u > 0.9f)) &&
			((TopLeft[nRot].v == 0) || (fTile*TopLeft[nRot].v > 0.9f)))
		{
			sprintf(msgString,"%s(%.0f,%.0f)",msgString,fTile*TopLeft[nRot].u,fTile*TopLeft[nRot].v);
		}
		else
		{
			sprintf(msgString,"%s(%.1f,%.1f)",msgString,fTile*TopLeft[nRot].u,fTile*TopLeft[nRot].v);
		}

		if (((TopRight[nRot].u == 0) || (fTile*TopRight[nRot].u > 0.9f)) &&
			((TopRight[nRot].v == 0) || (fTile*TopRight[nRot].v > 0.9f)))
		{
			sprintf(msgString,"%s(%.0f,%.0f)",msgString,fTile*TopRight[nRot].u,fTile*TopRight[nRot].v);
		}
		else
		{
			sprintf(msgString,"%s(%.1f,%.1f)",msgString,fTile*TopRight[nRot].u,fTile*TopRight[nRot].v);
		}

		if (((BottomLeft[nRot].u == 0) || (fTile*BottomLeft[nRot].u > 0.9f)) &&
			((BottomLeft[nRot].v == 0) || (fTile*BottomLeft[nRot].v > 0.9f)))
		{
			sprintf(msgString,"%s(%.0f,%.0f)",msgString,fTile*BottomLeft[nRot].u,fTile*BottomLeft[nRot].v);
		}
		else
		{
			sprintf(msgString,"%s(%.1f,%.1f)",msgString,fTile*BottomLeft[nRot].u,fTile*BottomLeft[nRot].v);
		}

		if (((BottomRight[nRot].u == 0) || (fTile*BottomRight[nRot].u > 0.9f)) &&
			((BottomRight[nRot].v == 0) || (fTile*BottomRight[nRot].v > 0.9f)))
		{
			sprintf(msgString,"%s(%.0f,%.0f), ",msgString,fTile*BottomRight[nRot].u,fTile*BottomRight[nRot].v);
		}
		else
		{
			sprintf(msgString,"%s(%.1f,%.1f), ",msgString,fTile*BottomRight[nRot].u,fTile*BottomRight[nRot].v);
		}

		// Setup the Mesh
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X +   9.5f,   9.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, fTile*TopLeft[nRot].u,     fTile*TopLeft[nRot].v);
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X + 265.5f, 265.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, fTile*BottomRight[nRot].u, fTile*BottomRight[nRot].v);
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X +   9.5f, 265.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, fTile*BottomLeft[nRot].u,  fTile*BottomLeft[nRot].v);
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X +   9.5f,   9.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, fTile*TopLeft[nRot].u,     fTile*TopLeft[nRot].v);
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X + 265.5f,   9.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, fTile*TopRight[nRot].u,    fTile*TopRight[nRot].v);
		VertexList[dwVertexCount++] = cD3DTLVERTEX(cD3DVECTOR((float)m_vpTest.X + 265.5f, 265.5f + (float)m_vpTest.Y, 0.9f),(1.0f / 0.9f), RGBA_MAKE(255,255,255,255),0, fTile*BottomRight[nRot].u, fTile*BottomRight[nRot].v);
	}
}
