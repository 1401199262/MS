#if !defined(AFX_DLLDATAX_H__B002CD32_269B_47AA_8B8E_00D9B3C5396F__INCLUDED_)
#define AFX_DLLDATAX_H__B002CD32_269B_47AA_8B8E_00D9B3C5396F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MERGE_PROXYSTUB

extern "C" 
{
BOOL WINAPI PrxDllMain(HINSTANCE hInstance, DWORD dwReason, 
	LPVOID lpReserved);
STDAPI PrxDllCanUnloadNow(void);
STDAPI PrxDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
STDAPI PrxDllRegisterServer(void);
STDAPI PrxDllUnregisterServer(void);
}

#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLDATAX_H__B002CD32_269B_47AA_8B8E_00D9B3C5396F__INCLUDED_)
