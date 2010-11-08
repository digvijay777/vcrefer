/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Jun 04 21:17:09 2009
 */
/* Compiler settings for C:\test\com\DynamicWeb\DynamicWeb.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


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

const IID IID_IWebForm = {0x460A70A9,0x2429,0x4D89,{0xB5,0x88,0x6E,0x99,0xBA,0x2A,0x1A,0xF7}};


const IID LIBID_DYNAMICWEBLib = {0x9D379A8B,0x4067,0x40D8,{0x81,0x5C,0x50,0x64,0x6C,0x2D,0x7A,0x3E}};


const CLSID CLSID_WebForm = {0x7386AC8E,0xF309,0x4600,{0xA3,0xD0,0x59,0x1F,0x40,0x19,0x27,0x21}};


#ifdef __cplusplus
}
#endif

