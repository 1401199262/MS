//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//
//  Copyright (c) 1998-1998 Microsoft Corporation
//
//  File:       aariff.h
//
//--------------------------------------------------------------------------

//
// aariff.h
//

#ifndef __AARIFF__
#define __AARIFF__

#ifdef XBOX
#include <xtl.h>
#else // XBOX
#include <windows.h>
#endif // XBOX

#include <mmsystem.h>
#include <objbase.h>
#define FixBytes(a1,a2)

// {0D5057E1-8889-11CF-B9DA-00AA00C08146}
DEFINE_GUID( IID_IAARIFFStream, 0xd5057e1, 0x8889, 0x11cf, 0xb9, 0xda, 0x0, 0xaa, 0x0, 0xc0, 0x81, 0x46 );
#undef INTERFACE
#define INTERFACE IAARIFFStream
DECLARE_INTERFACE_(IAARIFFStream, IUnknown)
{
    // IUnknown members
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // IMKRIFFStream members
    STDMETHOD_(UINT, Descend)(LPMMCKINFO lpck, LPMMCKINFO lpckParent, UINT wFlags) PURE;
    STDMETHOD_(UINT, Ascend)(LPMMCKINFO lpck, UINT wFlags) PURE;
    STDMETHOD_(UINT, CreateChunk)(LPMMCKINFO lpck, UINT wFlags) PURE;
    STDMETHOD(SetStream)(LPSTREAM pStream) PURE;
    STDMETHOD_(LPSTREAM, GetStream)() PURE;
};





// seeks to a 32-bit position in a stream.
HRESULT __inline StreamSeek( LPSTREAM pStream, long lSeekTo, DWORD dwOrigin )
{
	LARGE_INTEGER li;

	if( lSeekTo < 0 )
	{
		li.HighPart = -1;
	}
	else
	{
        li.HighPart = 0;
	}
	li.LowPart = lSeekTo;
	return pStream->Seek( li, dwOrigin, NULL );
}

// returns the current 32-bit position in a stream.
DWORD __inline StreamTell( LPSTREAM pStream )
{
	LARGE_INTEGER li;
    ULARGE_INTEGER ul;
#ifdef DBG
    HRESULT hr;
#endif

    li.HighPart = 0;
    li.LowPart = 0;
#ifdef DBG
    hr = pStream->Seek( li, STREAM_SEEK_CUR, &ul );
    if( FAILED( hr ) )
#else
    if( FAILED( pStream->Seek( li, STREAM_SEEK_CUR, &ul ) ) )
#endif
    {
        return 0;
    }
    return ul.LowPart;
}

// this function gets a long that is formatted the correct way
// i.e. the motorola way as opposed to the intel way
BOOL __inline GetMLong( LPSTREAM pStream, DWORD& dw )
{
    union uLong
	{
		unsigned char buf[4];
        DWORD dw;
	} u;
    unsigned char ch;

    if( FAILED( pStream->Read( u.buf, 4, NULL ) ) )
    {
        return FALSE;
    }

#ifndef _MAC
    // swap bytes
    ch = u.buf[0];
    u.buf[0] = u.buf[3];
    u.buf[3] = ch;

    ch = u.buf[1];
    u.buf[1] = u.buf[2];
    u.buf[2] = ch;
#endif

    dw = u.dw;
    return TRUE;
}

BOOL __inline IsGUIDZero( REFGUID guid )
{
    GUID g;

    memset( &g, 0, sizeof( g ) );
    return IsEqualGUID( g, guid );
}

// misc function prototypes

STDAPI AllocRIFFStream( IStream* pStream, IAARIFFStream** ppRiff );

#endif  // __AARIFF_H__
