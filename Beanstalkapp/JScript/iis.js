var objADs = GetObject("IIS://localhost/W3SVC");
var a = 1;
WScript.Echo(objADs.UploadReadAheadSize);