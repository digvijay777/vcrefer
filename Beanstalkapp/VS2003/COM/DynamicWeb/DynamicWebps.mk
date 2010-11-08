
DynamicWebps.dll: dlldata.obj DynamicWeb_p.obj DynamicWeb_i.obj
	link /dll /out:DynamicWebps.dll /def:DynamicWebps.def /entry:DllMain dlldata.obj DynamicWeb_p.obj DynamicWeb_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del DynamicWebps.dll
	@del DynamicWebps.lib
	@del DynamicWebps.exp
	@del dlldata.obj
	@del DynamicWeb_p.obj
	@del DynamicWeb_i.obj
