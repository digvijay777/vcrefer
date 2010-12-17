

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Dec 16 15:42:01 2010
 */
/* Compiler settings for .\SafeArray.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


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

MIDL_DEFINE_GUID(IID, IID_IArrayTest,0xE131B8EE,0xA6BE,0x4045,0xB5,0xCB,0x8A,0x86,0xE9,0xE0,0xD2,0x3E);


MIDL_DEFINE_GUID(IID, IID_ISTAtest,0x4C4C3A14,0x975A,0x44D1,0xA9,0x90,0x81,0xC4,0xD0,0x2B,0xDE,0x9C);


MIDL_DEFINE_GUID(IID, LIBID_SafeArrayLib,0xEE7C5E7A,0x3721,0x48B2,0x99,0x0F,0xC8,0xD2,0x71,0x01,0x36,0x0B);


MIDL_DEFINE_GUID(CLSID, CLSID_ArrayTest,0xD731B46C,0x2575,0x4014,0xAF,0x65,0x68,0x7D,0xA0,0x53,0x4B,0x43);


MIDL_DEFINE_GUID(CLSID, CLSID_STAtest,0x10820A32,0xD879,0x4A48,0xAD,0x53,0x3A,0xEF,0x68,0xCE,0xF0,0x4B);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

