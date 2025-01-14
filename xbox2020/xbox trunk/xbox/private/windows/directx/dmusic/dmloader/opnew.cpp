// Copyright (c) 1999 Microsoft Corporation
// OpNew.cpp
//
// Override operator new[] so that we ignore the new_handler mechanism.
//
//

#ifdef XBOX
#include <xtl.h>
#else // XBOX
#include <windows.h>
#endif // XBOX

#include <stdio.h>
#include <stdlib.h>

#include "Debug.h"

LPVOID __cdecl operator new(size_t cbBuffer)
{
    LPVOID p;

    p = malloc(cbBuffer ? cbBuffer : 1);
    return p;
}

void __cdecl operator delete(LPVOID p)
{
    free(p);
}

