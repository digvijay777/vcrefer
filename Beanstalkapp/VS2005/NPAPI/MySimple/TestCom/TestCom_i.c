

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sat Mar 05 22:04:01 2011
 */
/* Compiler settings for .\TestCom.idl:
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

MIDL_DEFINE_GUID(IID, IID_ISimple,0x5485B63C,0xB773,0x49BD,0xB1,0x4A,0x28,0x91,0x84,0x61,0x5D,0x70);


MIDL_DEFINE_GUID(IID, IID_ISimple2,0x85EB30BA,0x1820,0x43D9,0x8D,0x2D,0xC7,0x64,0x55,0x21,0xC6,0x8B);


MIDL_DEFINE_GUID(IID, IID_IATLControl,0xCCE220DF,0x2F87,0x4B13,0x95,0xBD,0x5B,0xDD,0x16,0x1E,0x51,0xBF);


MIDL_DEFINE_GUID(IID, IID_ITestWindow,0x29D30587,0x8E31,0x4595,0x8D,0xF5,0xBA,0x82,0x59,0xB6,0x64,0x70);


MIDL_DEFINE_GUID(IID, LIBID_TestComLib,0x53429F72,0x7F20,0x4687,0xB1,0xFE,0xA3,0x0C,0xCE,0xDF,0x7B,0x8A);


MIDL_DEFINE_GUID(CLSID, CLSID_Simple,0xF3D5C9B3,0x6C35,0x4066,0x8A,0xDC,0x63,0xD7,0x29,0xC3,0x5F,0x10);


MIDL_DEFINE_GUID(CLSID, CLSID_Simple2,0x9082122F,0x3E3E,0x492E,0x8F,0x8B,0x8E,0x61,0x84,0x49,0xE4,0x88);


MIDL_DEFINE_GUID(CLSID, CLSID_ATLControl,0x23AFB164,0xE01F,0x4AD1,0xAA,0xC1,0x3E,0x8E,0xBE,0x9B,0x16,0x8F);


MIDL_DEFINE_GUID(CLSID, CLSID_TestWindow,0xD867829A,0x60F8,0x446E,0xA9,0x49,0xE0,0xD2,0x40,0x62,0xD9,0x7C);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



