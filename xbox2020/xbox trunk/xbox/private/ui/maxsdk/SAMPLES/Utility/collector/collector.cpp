/*----------------------------------------------------------------------*
 |
 |	FILE: Collector.cpp
 | 
 |	DESC: Resource Collection plugin
 |
 |	AUTH: Harry Denholm, Kinetix
 |		  Copyright (c) 1998, All Rights Reserved.
 |
 |	HISTORY: 27.2.98
 |
 *----------------------------------------------------------------------*/


#include "collector.h"

#define CL_CLASSID	Class_ID(0x57ab2b60, 0x590572f5)
// two handy macros to set cursors for busy or normal operation
#define UI_MAKEBUSY			SetCursor(LoadCursor(NULL, IDC_WAIT));
#define UI_MAKEFREE			SetCursor(LoadCursor(NULL, IDC_ARROW));

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
		case 0: return GetCollectionDesc();
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

class Collection : public UtilityObj {
	public:
		IUtil *iu;
		Interface *ip;
		HWND hPanel;
		
		char WZPath[255];
		BOOL WZisHere;

		Collection();
		~Collection();

		NameTab mapPaths;
		
		void BeginEditParams(Interface *ip,IUtil *iu);
		void EndEditParams(Interface *ip,IUtil *iu);
		void DeleteThis() {}

		void Init(HWND hWnd);
		void Destroy(HWND hWnd);

};

static Collection theCollection;


class CollectionClassDesc:public ClassDesc {
	public:
	int 			IsPublic() {return 1;}
	void *			Create(BOOL loading = FALSE) {return &theCollection;}
	const TCHAR *	ClassName() {return _T("Resource Collector");}
	SClass_ID		SuperClassID() {return UTILITY_CLASS_ID;}
	Class_ID		ClassID() {return CL_CLASSID;}
	const TCHAR* 	Category() {return _T("");}
};

static CollectionClassDesc CollectionDesc;
ClassDesc* GetCollectionDesc() {return &CollectionDesc;}


class CMtlEnum {
	public:

		virtual void  proc(MtlBase *m){
			Interface *ip = theCollection.ip;

			// Check for bitmaptex, and cast
			if(m->ClassID()==Class_ID(BMTEX_CLASS_ID,0)) {
				Texmap *bt = (Texmap*)m;
				BitmapTex *b = (BitmapTex*)bt;

				if(_stricmp(b->GetMapName(),"")!=0){
					
					// Use the very useful BMMGetFull.. to scan for existing files
					BitmapInfo bi(b->GetMapName());
					BMMGetFullFilename(&bi);

					char Bname[MAX_PATH];
					strcpy(Bname,bi.Name());

					// If our found file exists, add it to the list
					if(BMMIsFile(Bname)){
						char s[256];
						sprintf(s,"%s",Bname);

						// check for update, and do so if needed
						BOOL upd = GetCheckBox(theCollection.hPanel,IDC_UPMAP);
						if (upd) {
							TCHAR tfile[MAX_PATH];
							TCHAR textn[MAX_PATH];
							char to_path[255],fnl[MAX_PATH];
							GetWindowText(GetDlgItem(theCollection.hPanel,IDC_PATH),to_path,254);

							BMMSplitFilename(Bname, NULL, tfile, textn);
							BMMAppendSlash(to_path);
							sprintf(fnl,"%s%s%s",to_path,tfile,textn);

							b->SetMapName(fnl);
						}

						if(theCollection.mapPaths.FindName(Bname)==-1){
							theCollection.mapPaths.AddName(s);
						}
					}
				}
			}
		}
};


CMtlEnum CEnym;

// do the material enumeration
void CEnumMtlTree(MtlBase *mb, CMtlEnum &tenum) {
	tenum.proc(mb);
	for (int i=0; i<mb->NumSubTexmaps(); i++) {
		Texmap *st = mb->GetSubTexmap(i); 
		if (st) 
			CEnumMtlTree(st,tenum);
		}
	if (IsMtl(mb)) {
		Mtl *m = (Mtl *)mb;
		for (i=0; i<m->NumSubMtls(); i++) {
			Mtl *sm = m->GetSubMtl(i);
			if (sm) 
				CEnumMtlTree(sm,tenum);
			}
		}
	}


// Enumerate the scene
void NodeEnum(INode *root)
{
	for (int k=0; k<root->NumberOfChildren(); k++)
	{
		INode *node = root->GetChildNode(k);
		MtlBase* mat = (MtlBase*)node->GetMtl();
		if (mat)
			CEnumMtlTree(mat,CEnym);
		if(node->NumberOfChildren()>0) NodeEnum(node);
	}
}



static BOOL CALLBACK CollectionDlgProc(
		HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Interface *ip;
	ip = theCollection.ip;


	switch (msg) {
		case WM_INITDIALOG:
			theCollection.Init(hWnd);
			break;

		case WM_DESTROY:
			theCollection.Destroy(hWnd);
			break;

		case WM_COMMAND:

			switch(HIWORD(wParam)){
				case EN_SETFOCUS :
					DisableAccelerators();
					break;
				case EN_KILLFOCUS :
					EnableAccelerators();
					break;
			}

			switch (LOWORD(wParam)) {

			case IDC_BROWSE:
					TCHAR dir[256];
					TCHAR desc[256];
					_tcscpy(desc, _T("Target Directory"));
					ip->ChooseDirectory(hWnd, _T("Choose Output"), dir, desc);
					SetWindowText(GetDlgItem(hWnd,IDC_PATH),dir);
					break;

			case IDC_GO:
				{
				UI_MAKEBUSY
				theCollection.mapPaths.ZeroCount();

				// Collect bitmaps
				BOOL doMAPS = GetCheckBox(hWnd,IDC_PACKMAPS);

				if(doMAPS) 
				{
					INode *root = ip->GetRootNode();
					NodeEnum(root);
				}

				// Check and store MAX File
				BOOL doMAX = GetCheckBox(hWnd,IDC_PACKMAX);
				TCHAR winZipName[MAX_PATH];

				if(stricmp(ip->GetCurFilePath(),"")==0) 
				{
					MessageBox(NULL,"No valid MAX File Path","WARNING",MB_OK);

					// No file name. Assign a default.
					strcpy(winZipName,"UntitledScene.ZIP");
				}
				else
				{
					if (doMAX) theCollection.mapPaths.AddName(ip->GetCurFilePath()); 

					// Figure out a Winzip ZIP file name from the filepath
					TCHAR tfile[MAX_PATH];
						
					BMMSplitFilename(ip->GetCurFilePath(), NULL, tfile, NULL);
					sprintf(winZipName,"%s.ZIP",tfile);
				}
				


				// Do the copy/move options
				char to_path[255];
				char fnl[MAX_PATH];
				GetWindowText(GetDlgItem(hWnd,IDC_PATH),to_path,254);
				BOOL type;

				type = GetCheckBox(hWnd,IDC_TYPE);

				NameTab zipList; zipList.ZeroCount();

				for (int q=0; q<theCollection.mapPaths.Count(); q++)
				{
					TCHAR tfile[MAX_PATH];
					TCHAR textn[MAX_PATH];
					
					BMMSplitFilename(theCollection.mapPaths[q], NULL, tfile, textn);
					BMMAppendSlash(to_path);

					sprintf(fnl,"%s%s%s",to_path,tfile,textn);
					if(type)CopyFile(theCollection.mapPaths[q],fnl,FALSE);
					if(!type)MoveFile(theCollection.mapPaths[q],fnl);
					zipList.AddName(fnl);
				}

				BOOL zipup = GetCheckBox(hWnd,IDC_PACKUP);
				FILE *stream;

				// Create file list, and WINZIP the files together
				if(zipup&&theCollection.WZisHere)
				{
					char zpn[MAX_PATH];
					sprintf(zpn,"%sLIST$PCK.TXT",to_path);
					stream = fopen(zpn,"w+t");
					for (int u=0;u<zipList.Count();u++)
					{
					 fprintf(stream,"%s\n",zipList[u]);
					}
					fclose(stream);

					TCHAR tpath[MAX_PATH];
					TCHAR tfile[MAX_PATH];
					TCHAR textn[MAX_PATH];
					TCHAR exec[MAX_PATH];
					BMMSplitFilename(theCollection.WZPath, tpath, tfile, textn);

					// <winzip path> -a <zip filename> @LIST$PCK.TXT
					sprintf(exec,"%s%s -a \"%s\" @LIST$PCK.TXT",tpath,tfile,winZipName);

					_flushall();
					system(exec);

					DeleteFile(zpn);
				}

				UI_MAKEFREE
				ip->RedrawViews(ip->GetTime());
				} //mloop
				break;
			}
			break;

		default:
			return FALSE;
	}
	return TRUE;
}	

Collection::Collection()
{
	iu = NULL;
	ip = NULL;	
	hPanel = NULL;
}

Collection::~Collection()
{
	
}

static BOOL CheckWinZip()
{
	HKEY	hKey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\classes\\WinZip", 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return FALSE;
	else
		return TRUE;
}
static void FindWinZipDir(TCHAR *Path)
{
	HKEY	hKey;
	long	kSize;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\classes\\WinZip\\shell\\open\\command", 0, KEY_ALL_ACCESS, &hKey);
	RegQueryValue(hKey,NULL,Path,&kSize);
}

void Collection::BeginEditParams(Interface *ip,IUtil *iu) 
{
	this->iu = iu;
	this->ip = ip;
	hPanel = ip->AddRollupPage(
		hInstance,
		MAKEINTRESOURCE(IDD_CL_PANEL),
		CollectionDlgProc,
		"Parameters",
		0);

	mapPaths.ZeroCount();

	// Do a simple registry check for WinZip
	WZisHere = CheckWinZip();
	if(WZisHere) FindWinZipDir(WZPath);
		EnableWindow(GetDlgItem(hPanel,IDC_PACKUP),WZisHere);
		EnableWindow(GetDlgItem(hPanel,IDC_STATICPACK),WZisHere);
}
	
void Collection::EndEditParams(Interface *ip,IUtil *iu) 
{
	this->iu = NULL;
	ip->DeleteRollupPage(hPanel);
	hPanel = NULL;
}

void Collection::Init(HWND hWnd)
{
	// Set our defaults
	SetCheckBox(hWnd,IDC_UPMAP,FALSE);
	SetCheckBox(hWnd,IDC_PACKMAX,FALSE);
	SetCheckBox(hWnd,IDC_PACKUP,FALSE);
	SetCheckBox(hWnd,IDC_TYPE2,FALSE);

	SetCheckBox(hWnd,IDC_PACKMAPS,TRUE);
	SetCheckBox(hWnd,IDC_TYPE,TRUE);

	SetWindowText(GetDlgItem(hWnd,IDC_PATH),ip->GetDir(APP_AUTOBACK_DIR));
}

void Collection::Destroy(HWND hWnd)
{

}
