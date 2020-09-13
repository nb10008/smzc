

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Thu Jan 20 14:18:57 2011
 */
/* Compiler settings for .\htmlancher.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IHtmLanch,0x8AEF255C,0x5431,0x4D81,0x8C,0x25,0xF7,0xBA,0x25,0xD1,0x74,0x88);


MIDL_DEFINE_GUID(IID, LIBID_htmlancherLib,0x2BB539ED,0x3F8A,0x4C31,0x9B,0xD4,0x6E,0x73,0xBD,0xE5,0x0C,0x86);


MIDL_DEFINE_GUID(IID, DIID__IHtmLanchEvents,0xC0F93300,0x65C0,0x4F10,0xBF,0xFC,0xC1,0xAA,0x3E,0xE2,0x8F,0x93);


MIDL_DEFINE_GUID(CLSID, CLSID_HtmLanch,0xF062BC96,0x8EAF,0x4F38,0xA4,0x3A,0x90,0x53,0x87,0x92,0xE0,0xCE);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



