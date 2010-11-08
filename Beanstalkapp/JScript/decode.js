//var str1 = DecodeString("hefdgghihgfhhifliahahggeibhbijhbhnilhhhoibgmioilgpiahmjcikjbifgnhdhghihghcimikhlhnifjhjfkcjojijojiinkkkdkhkbjokkkikpkljhlemhmnmenanjngielnlgihlinlnjnbnboaobninpnpndnpkplilomklcmmkmldlemalflhlhnhleldleljljmckilelkmdlmlnmjodpdaeaaagalpgacadndabaoapamnhmnmonbfkfefbffflfofffjfhfmgeflkdjbfoglge");
//var str2 = "S1CTQ1Q3WFK8TCZALYDJL6WS6FAVMSF-2452-FC34;LIUPING;WORKGROUP;Windows XP Professional;CHS:S288C787V20043; +0800;Tcsnswall;http://192.168.1.29/vc/;3";
var str1 = DecodeString("phoiobmopkoe");
var str2 = "”√ªß“ª";
if(str1 != str2)
{
	alert("error");
}

function DecodeString(str)
{	
	var strRet			= "";
	var temp;
	var len				= str.length / 2;
	
	if(str.length % 2 != 0)
		return str;
		
	for(var i = 0; i < len; i++)
	{
		temp = ((((str.charCodeAt(i*2) - 0x61) << 4)
				+ str.charCodeAt(i*2+1) + 0x9F)
				- ModValue(i, len) % 256) % 256;
		if (temp < 32 || temp > 255)
		{
			return str;
		}
		strRet += "%" + temp.toString(16).toUpperCase();
	}
	
	return decodeURIComponent(strRet);
}

function ModValue(nIdx, nLen)
{
	return (nLen * nLen) % 256 + nIdx % 128;
}