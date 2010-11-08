/*WScript.Echo("hello");*/
if(ChangeUserPassword("Administrator", "123456", "111111"))
{
	WScript.Echo("Change User name Success!");
}
else
{
	WScript.Echo("Change Failed!");
}

function ChangeUserPassword(User, OldPwd, NewPwd)
{
	var	objADs 	= GetObject("WinNT://./" + User + ",user");
	if(null == objADs)
		return false;
	try
	{
		objADs.ChangePassword(OldPwd, NewPwd);
	}
	catch(e)
	{
		return false;
	}

	return true;
}