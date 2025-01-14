/*===========================================================================*\
 | 
 |  FILE:	GlobalUtility.h
 |			Skeleton project and code for a Global Utility Plugin (GUP)
 |			3D Studio MAX R3.0
 | 
 |  AUTH:   Harry Denholm
 |			Developer Consulting Group
 |			Copyright(c) Discreet 1999
 |
 |  HIST:	Started 21-1-99
 | 
\*===========================================================================*/

#ifndef __GUPSKEL__H
#define __GUPSKEL__H


#include <Max.h>
#include <bmmlib.h>
#include <guplib.h>

#include "resource.h"


// IMPORTANT:
// The ClassID must be changed whenever a new project
// is created using this skeleton
#define	SKGUP_CLASSID		Class_ID(0x52d735b4, 0x4ad1587)


TCHAR *GetString(int id);
extern ClassDesc* GetGUPDesc();


class SkeletonGUP : public GUP {
	public:
      
		SkeletonGUP		( ) {}
		~SkeletonGUP	( ) {}
	
		// GUP Methods
		DWORD	Start			( );
		void	Stop			( );
		DWORD	Control			( DWORD parameter );
};


// ViewWindow subclass for putting a custom window in a MAX viewport
class SkeletonViewWindow : public ViewWindow
{
	SkeletonGUP *gupPtr;

public:

	SkeletonViewWindow( SkeletonGUP *gP ) { gupPtr = gP; }
	~SkeletonViewWindow() { gupPtr = NULL; }

	TCHAR *GetName() { return GetString(IDS_VIEWWIN_NAME); }
	HWND CreateViewWindow(HWND hParent, int x, int y, int w, int h);
	void DestroyViewWindow(HWND hWnd);
};


#endif