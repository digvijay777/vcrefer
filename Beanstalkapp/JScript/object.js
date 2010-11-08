var obj = new Object();

obj.ss = 2;
obj.tt = 3;

var str = obj.toString();
WScript.Echo(str);
obj["xx"] = 2;
obj["jj"] = 3;