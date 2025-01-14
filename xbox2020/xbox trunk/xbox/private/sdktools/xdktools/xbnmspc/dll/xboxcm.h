/*****************************************************************************
 *    xboxcm.h
 *****************************************************************************/

#ifndef _XBOXCONTEXTMENU_H
#define _XBOXCONTEXTMENU_H

#include "xboxdrop.h"


typedef HRESULT (*INVOKEPROC)(CXboxMenu * pfcm, LPCMINVOKECOMMANDINFO pici, LPCTSTR pszCmd, LPCITEMIDLIST pidl);

typedef struct {            /* enum invoke info */
    CXboxMenu * pfcm;
    LPCMINVOKECOMMANDINFO pici;
    HRESULT hres;
    INVOKEPROC pfn;
    LPCTSTR ptszCmd;
} EII, *PEII;




/*****************************************************************************
 *
 *    CXboxMenu
 *
 *
 *****************************************************************************/

class CXboxMenu          : public IContextMenu
                        , public CObjectWithSite
{
public:
    //////////////////////////////////////////////////////
    // Public Interfaces
    //////////////////////////////////////////////////////
    
    // *** IUnknown ***
    virtual STDMETHODIMP_(ULONG) AddRef(void);
    virtual STDMETHODIMP_(ULONG) Release(void);
    virtual STDMETHODIMP QueryInterface(REFIID riid, LPVOID * ppvObj);
    
    // *** IContextMenu ***
    virtual STDMETHODIMP QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    virtual STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO lpici);
    virtual STDMETHODIMP GetCommandString(UINT_PTR idCmd, UINT uType, UINT * pwReserved, LPSTR pszName, UINT cchMax);

public:
    CXboxMenu();
    ~CXboxMenu(void);

    // Public Member Variables
    DWORD                   m_sfgao;        // Shell attributes of objects
    CXboxPidlList *          m_pflHfpl;      // Where I live             // BUGBUG: Come back and see if we want to change this.
    CXboxFolder *            m_pff;          // CXboxFolder in which our pidls live
    CXboxDir *               m_pfd;          // XboxDir in which our pidls live
    HWND                    m_hwnd;         // Owner window for UI
    POINT                   m_ptNewItem;    // Points where the context menu was invoked.  Used to later position a new folder.
    DWORD                   m_uFlags;       // Flags used in ::QueryContextMenu() to determine if the menu was invoked by the file menu.
    BOOL                    m_fBackground;  // Is this a forground or background context menu?

    // Friend Functions
    friend HRESULT CXboxMenu_Create(CXboxFolder * pff, CXboxPidlList * pflHfpl, HWND hwnd, REFIID riid, LPVOID * ppvObj);
    friend HRESULT CXboxMenu_Create(CXboxFolder * pff, CXboxPidlList * pflHfpl, HWND hwnd, CXboxMenu ** ppfm);

    static DWORD CALLBACK _DownloadDialogThreadProc(LPVOID pvThis) { return ((CXboxMenu *) pvThis)->_DownloadDialogThreadProc(); };

protected:
    int                     m_cRef;

    LPTSTR                  m_pszDownloadDir;         // Download Directory
    DWORD                   m_dwDownloadType;         // Download Directory

    // Private Member Functions
    int _RemoveContextMenuItems(HMENU hmenu, UINT idCmdFirst, DWORD sfgao);
    int _InvokeOne(LPCITEMIDLIST pidl, PEII peii);
    HRESULT _InvokeDelete(LPCMINVOKECOMMANDINFO pici);
    HRESULT _InvokeRename(LPCMINVOKECOMMANDINFO pici);
    HRESULT _InvokeCutCopy(UINT_PTR id, LPCMINVOKECOMMANDINFO pici);
    HRESULT _DoDrop(IDropTarget * pdt, IDataObject * pdto);
    HRESULT _InvokePaste(LPCMINVOKECOMMANDINFO pici);
    HRESULT _InvokeDownloadVerb(LPCMINVOKECOMMANDINFO pici);
    HRESULT _InvokeDeleteVerb(LPCMINVOKECOMMANDINFO pici);
    HRESULT _InvokeLoginAsVerb(LPCMINVOKECOMMANDINFO pici);
    HRESULT _InvokeNewFolderVerb(LPCMINVOKECOMMANDINFO pici);
    HRESULT _AddToRecentDocs(LPCITEMIDLIST pidl);
    CStatusBar * _GetStatusBar(void);
    DWORD _DownloadDialogThreadProc(void);
    DWORD _DownloadThreadProc(void);
    BOOL _AreAllFolders(CXboxPidlList * pflHfpl);
    LPITEMIDLIST GetSoftLinkDestination(LPCITEMIDLIST pidlToSoftLink);

    static HRESULT _SoftLinkDestCB(HINTERNET hint, HINTPROCINFO * phpi, LPVOID pvsldcbs, BOOL * pfReleaseHint);
    static int _InvokeOneCB(LPVOID pvPidl, LPVOID pv);
    static HRESULT _ApplyOne(CXboxMenu * pfcm, LPCMINVOKECOMMANDINFO pici, LPCTSTR ptszCmd, LPCITEMIDLIST pidl);
    static DWORD CALLBACK DownloadThreadProc(LPVOID pvThis) { return ((CXboxMenu *) pvThis)->_DownloadThreadProc(); };
    STDMETHODIMP _EnumInvoke(LPCMINVOKECOMMANDINFO pici, INVOKEPROC pfn, LPCTSTR ptszCmd);

private:
    BOOL _ContainsForgroundItems(void);
    BOOL _IsCallerCaptionBar(UINT indexMenu, UINT uFlags);
};

#endif // _XBOXCONTEXTMENU_H
