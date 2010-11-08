function WMIReboot()
{
	var wbemServices 		= GetObject("winmgmts:\\\\.\\root\\cimv2");
	var wbemObjectSet		= wbemServices.ExecQuery ("Select * from Win32_OperatingSystem");
	
	wbemServices.ImpersonationLevel = wbemImpersonationLevelImpersonate;
	var objE = new Enumerator(wbemObjectSet);
	for( ; !objE.atEnd(); objE.moveNext())
	{
		var it = objE.item();
		it.Reboot();
	}
}

WMIReboot();