#pragma once

const CHAR  JScript_GetIIS[] = 
"function GetIIS()"
"{ "
	"var strRet = \"Hello. JScript\"; "
	"Alert('����JScript����C++����');"
	"return strRet; "
"}";

const CHAR JScript_Alert[] =
"Alert('Hello. this is JScript.')";
/*
"var IIsObject; "
"//IIsObject = GetObject(\"IIS://localhost/w3svc\"); "
*/
/*
"//var strRet = IIsObject.Get(AnonymousUserName); "
*/
