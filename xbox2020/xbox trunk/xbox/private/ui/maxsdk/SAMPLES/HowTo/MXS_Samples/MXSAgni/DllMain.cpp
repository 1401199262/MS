#include "MAXScrpt.h"
 
extern void MXSAgni_init();

HMODULE hInstance = NULL;

BOOL APIENTRY
DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	static BOOL controlsInit = FALSE;
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			// Hang on to this DLL's instance handle.
			hInstance = hModule;
			if ( !controlsInit )
			{
				controlsInit = TRUE;				
				// Initialize Win95 controls
 				InitCommonControls();
				InitCustomControls(hInstance);
			}
			break;
	}
		
	return(TRUE);
}

__declspec( dllexport ) void
LibInit() { 
	// do any setup here
	MXSAgni_init();
}


__declspec( dllexport ) const TCHAR *
LibDescription() { return _T("Sample for MAXscript DLLs"); }

__declspec( dllexport ) ULONG
LibVersion() {  return VERSION_3DSMAX; }

TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}

