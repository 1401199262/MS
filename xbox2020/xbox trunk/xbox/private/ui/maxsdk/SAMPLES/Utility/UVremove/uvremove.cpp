/*----------------------------------------------------------------------*
 |
 |	FILE: UVStrip.cpp
 | 
 |	DESC: UVW Coord remover plugin
 |
 |	AUTH: Harry Denholm, Kinetix
 |		  Copyright (c) 1998, All Rights Reserved.
 |
 |	HISTORY: 27.2.98
 |
 *----------------------------------------------------------------------*/

#include "uvremove.h"

#define UV_CLASS_ID	Class_ID(0x653e99d6, 0x68e7731e)

HINSTANCE hInstance;
int controlsInit = FALSE;


BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	hInstance = hinstDLL;

	if (!controlsInit) {
		controlsInit = TRUE;
		InitCustomControls(hInstance);
		InitCommonControls();
	}
			
	return (TRUE);
}

__declspec( dllexport ) const TCHAR* LibDescription()
{
	return GetString(IDS_LIBDESCRIPTION);
}

//TODO: Must change this number when adding a new class
__declspec( dllexport ) int LibNumberClasses()
{
	return 1;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i) {
		case 0: return GetUVStripDesc();
		default: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}

class UVStrip : public UtilityObj {
	public:

		IUtil *iu;
		Interface *ip;
		HWND hPanel;
		
		//Constructor/Destructor
		UVStrip();
		~UVStrip();

		void BeginEditParams(Interface *ip,IUtil *iu);
		void EndEditParams(Interface *ip,IUtil *iu);
		void DeleteThis() {}

		void Init(HWND hWnd);
		void Destroy(HWND hWnd);
		void SelectionSetChanged(Interface *ip,IUtil *iu);
};



static UVStrip theUVStrip;

class UVStripClassDesc:public ClassDesc {
	public:
	int 			IsPublic() {return 1;}
	void *			Create(BOOL loading = FALSE) {return &theUVStrip;}
	const TCHAR *	ClassName() {return GetString(IDS_UV_NAME);}
	SClass_ID		SuperClassID() {return UTILITY_CLASS_ID;}
	Class_ID		ClassID() {return UV_CLASS_ID;}
	const TCHAR* 	Category() {return _T("");}
};

static UVStripClassDesc UVStripDesc;
ClassDesc* GetUVStripDesc() {return &UVStripDesc;}


static BOOL CALLBACK UVStripDlgProc(
		HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_INITDIALOG:
			theUVStrip.Init(hWnd);
			char s[50]; sprintf(s,"%i Object(s) Selected",theUVStrip.ip->GetSelNodeCount());
			SetWindowText(GetDlgItem(hWnd,IDC_SEL),s);
			break;

		case WM_DESTROY:
			theUVStrip.Destroy(hWnd);
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			
			case IDC_R1:{
				int cn = theUVStrip.ip->GetSelNodeCount();
				Interval valid=FOREVER;
				if(cn>0){
					for(int x=0;x<cn;x++){
				      ObjectState os = theUVStrip.ip->GetSelNode(x)->EvalWorldState(theUVStrip.ip->GetTime());
					  if (os.obj->SuperClassID()==GEOMOBJECT_CLASS_ID && os.obj->IsRenderable() && os.obj->IsDeformable()) {
						  TriObject *T1 = (TriObject *)os.obj;
						  T1->GetMesh().setNumTVerts(0);
						  T1->GetMesh().setNumTVFaces(0);
						  //T1->UpdateValidity(TEXMAP_CHAN_NUM,valid);
						  //T1->mesh.InvalidateGeomCache();
						  //T1->mesh.InvalidateTopologyCache();
					  }
					  theUVStrip.ip->ForceCompleteRedraw();
					}
				}
				break;}

			case IDC_R2:{
				int cn = theUVStrip.ip->GetSelNodeCount();
				if(cn>0){
					for(int x=0;x<cn;x++){
						INode *tmp=theUVStrip.ip->GetSelNode(x);
						tmp->SetMtl(NULL);
						if(GetCheckBox(hWnd, IDC_BLANK)) tmp->SetWireColor(RGB(160,160,160));
					}
					theUVStrip.ip->ForceCompleteRedraw();
				}
				break;}

			}
			break;

		default:
			return FALSE;
	}
	return TRUE;
}



//--- UVStrip -------------------------------------------------------
UVStrip::UVStrip()
{
	iu = NULL;
	ip = NULL;	
	hPanel = NULL;
}

UVStrip::~UVStrip()
{

}


void UVStrip::SelectionSetChanged(Interface *ip,IUtil *iu)
{
	char s[50]; sprintf(s,"%i Object(s) Selected",ip->GetSelNodeCount());
	SetWindowText(GetDlgItem(hPanel,IDC_SEL),s);
}

void UVStrip::BeginEditParams(Interface *ip,IUtil *iu) 
{
	this->iu = iu;
	this->ip = ip;
	hPanel = ip->AddRollupPage(
		hInstance,
		MAKEINTRESOURCE(IDD_UV_PANEL),
		UVStripDlgProc,
		GetString(IDS_PARAMS),
		0);
}
	
void UVStrip::EndEditParams(Interface *ip,IUtil *iu) 
{
	this->iu = NULL;
	this->ip = NULL;
	ip->DeleteRollupPage(hPanel);
	hPanel = NULL;
}

void UVStrip::Init(HWND hWnd)
{

}

void UVStrip::Destroy(HWND hWnd)
{

}

