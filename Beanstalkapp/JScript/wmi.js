function ClearGateways()
{
	var wbemLocator = new ActiveXObject("WbemScripting.SWbemLocator");
	var wbemServices = wbemLocator.ConnectServer("192.168.2.22", "root\\cimv2", "webroot", "www.snswall.com/webroot");
	var wbemObjectSet = wbemServices.ExecQuery("Select * from Win32_NetworkAdapterConfiguration where IPEnabled = true");
	var objE = new Enumerator(wbemObjectSet);
	for(var i = 0; i < 20 && !objE.atEnd(); objE.moveNext(), i++)
	{
		var it = objE.item();
		it.SetGateways(Array());
	}
}

ClearGateways();