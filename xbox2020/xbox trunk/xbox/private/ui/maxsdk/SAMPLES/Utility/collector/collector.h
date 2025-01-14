/*----------------------------------------------------------------------*
 |
 |	FILE: Collector.h
 |	AUTH: Harry Denholm, Kinetix
 |		  Copyright (c) 1998, All Rights Reserved.
 |
 *----------------------------------------------------------------------*/

#ifndef __COLLECTOR__H
#define __COLLECTOR__H

#include "Max.h"

#include "resource.h"
#include "utilapi.h"
#include "stdmat.h"
#include "bmmlib.h"

TCHAR *GetString(int id);

extern ClassDesc* GetCollectionDesc();

extern HINSTANCE hInstance;

#endif
