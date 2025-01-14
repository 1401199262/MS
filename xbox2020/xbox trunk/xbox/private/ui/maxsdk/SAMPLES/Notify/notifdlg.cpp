// notifdlg.cpp : implementation file
//

#include "stdafx.h"
#include "Notify.h"
#include "notifdlg.h"
#include "helpsys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNotifyApp* theAppPtr;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog {

public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotifyDlg dialog

CNotifyDlg::CNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNotifyDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CNotifyDlg)
	m_CompletionTarget = _T("");
	m_FailureTarget = _T("");
	m_ProgressTarget = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNotifyDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotifyDlg)
	DDX_Text(pDX, IDC_COMPLETION_ED, m_CompletionTarget);
	DDX_Text(pDX, IDC_FAILURE_ED, m_FailureTarget);
	DDX_Text(pDX, IDC_PROGRESS_ED, m_ProgressTarget);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNotifyDlg, CDialog)
	//{{AFX_MSG_MAP(CNotifyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_UPDATE(IDC_FAILURE_ED, OnUpdateFailureEd)
	ON_BN_CLICKED(IDC_BROWSE_FAILURE, OnBrowseFailure)
	ON_BN_CLICKED(IDC_BROWSE_PROGRESS, OnBrowseProgress)
	ON_BN_CLICKED(IDC_BROWSE_COMPLETION, OnBrowseCompletion)
	ON_BN_CLICKED(IDC_PLAY_FAILURE, OnPlayFailure)
	ON_BN_CLICKED(IDC_PLAY_COMPLETION, OnPlayCompletion)
	ON_BN_CLICKED(IDC_PLAY_PROGRESS, OnPlayProgress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotifyDlg message handlers

BOOL CNotifyDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
//	SetIcon(m_hIcon, TRUE);
//	SetIcon(m_hIcon, FALSE);
	SetWindowContextHelpId(idh_notify_main_ui);
			

	SetClassLong(m_hWnd,GCL_HICON,(LONG)m_hIcon);

	return TRUE;
}

void CNotifyDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} else if((nID & 0xfff0) == SC_CONTEXTHELP)
		Help(HELP_CONTEXT,idh_notify_main_ui);
	else
		CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNotifyDlg::OnPaint() {
	if (IsIconic())	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	} else {
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNotifyDlg::OnQueryDragIcon() {
	return (HCURSOR) m_hIcon;
}

void CNotifyDlg::OnUpdateFailureEd() {
#if 0
	CString text;
	CWnd* hDlg = GetDlgItem(IDC_TEST);
	GetDlgItemText(IDC_FAILURE_ED,text);
	if (text.IsEmpty())
		::EnableWindow(hDlg->m_hWnd,FALSE);
	else {
		::EnableWindow(hDlg->m_hWnd,TRUE);
		CString ta;
		GetDlgItemText(IDC_PROGRESS_ED,ta);
		if (ta.IsEmpty())
			SetDlgItemText(IDC_PROGRESS_ED,text);
		GetDlgItemText(IDC_COMPLETION_ED,ta);
		if (ta.IsEmpty())
			SetDlgItemText(IDC_PROGRESS_ED,text);
	}
#endif
}

BOOL CNotifyDlg::Browse(int type, TCHAR *filename) {

	static TCHAR initDir[MAX_PATH];
	
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lpstrFilter	= "Sound Files\0*.wav\0\0";
	ofn.Flags		= OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hwndOwner	= m_hWnd;
	ofn.nMaxFile	= MAX_PATH;
	ofn.lpstrFile	= filename;

	CString title;

	switch (type) {
		case NOTIFY_PROGRESS:	title.LoadString(IDS_PROGRESS_TITLE);	break;
		case NOTIFY_COMPLETION:	title.LoadString(IDS_COMPLETION_TITLE);	break;
		case NOTIFY_FAILURE:	title.LoadString(IDS_FAILURE_TITLE);	break;
	}

	ofn.lpstrTitle	= (char *)(void *)LPCTSTR(title);

	if (!initDir[0]) {
		GetWindowsDirectory(initDir,MAX_PATH);
		_tcscat(initDir,"\\Media");
	}

	ofn.lpstrInitialDir	= initDir;

	if (GetOpenFileName(&ofn)) {
		_tcscpy(filename,ofn.lpstrFile);
		return TRUE;
	}

	return FALSE;

}

void CNotifyDlg::OnBrowseFailure() {
	CString filename;
	GetDlgItemText(IDC_FAILURE_ED,filename);
	if (Browse(NOTIFY_FAILURE,filename.GetBuffer(MAX_PATH)))
		SetDlgItemText(IDC_FAILURE_ED,filename);
}

void CNotifyDlg::OnBrowseProgress() {
	CString filename;
	GetDlgItemText(IDC_PROGRESS_ED,filename);
	if (Browse(NOTIFY_PROGRESS,filename.GetBuffer(MAX_PATH)))
		SetDlgItemText(IDC_PROGRESS_ED,filename);
}

void CNotifyDlg::OnBrowseCompletion() {
	CString filename;
	GetDlgItemText(IDC_COMPLETION_ED,filename);
	if (Browse(NOTIFY_COMPLETION,filename.GetBuffer(MAX_PATH)))
		SetDlgItemText(IDC_COMPLETION_ED,filename);
}

void CNotifyDlg::OnPlayFailure() {
	CString filename;
	GetDlgItemText(IDC_FAILURE_ED,filename);
	if (!filename.IsEmpty())
		sndPlaySound(filename,SND_ASYNC);
}

void CNotifyDlg::OnPlayCompletion() {
	CString filename;
	GetDlgItemText(IDC_COMPLETION_ED,filename);
	if (!filename.IsEmpty())
		sndPlaySound(filename,SND_ASYNC);
}

void CNotifyDlg::OnPlayProgress() {
	CString filename;
	GetDlgItemText(IDC_PROGRESS_ED,filename);
	if (!filename.IsEmpty())
		sndPlaySound(filename,SND_ASYNC);
}
