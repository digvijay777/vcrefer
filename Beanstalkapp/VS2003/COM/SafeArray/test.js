var objCom = new ActiveXObject("SafeArray.ErrTest2");
try
{
	objCom.ErrTest();
}
catch(e)
{
	WScript.Echo(e.description);
}