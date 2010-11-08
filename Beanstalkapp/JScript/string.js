var str = "hello \" 'hi' is hello \"";

str = str.replace(/([\'\"])/g, "\\$1");

WScript.Echo(str);