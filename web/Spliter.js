/*
 * 动态分隔条
 *
 * Author: XiaXiangJun
 * Date: 2010-09-11
 */
var 	gObjSpliter		= null;
var 	gPosX			= 0;
var 	gPosY			= 0;

/*
 * 设置动态分隔条
 *
 * 参数: obj: 动态分隔条对像    	objMove: 动态分隔条移动后要移动的对像
 *       bLR: 是否为左右分隔 		cssNormal: 分隔条样式
 *       cssHover: 分隔条得到焦点样式
 *       minV: objMove的最小(宽/高) maxV: objMove的最大(宽/高)
 */
function SpliterBorder(obj, objMove, bLR, cssNormal, cssHover, minV, maxV)
{
	obj.onmouseover = function(){
		if(null == gObjSpliter)
		{
			this.className = cssHover;
		}
	}
	obj.onmouseout = function(){
		if(null == gObjSpliter)
		{
			this.className = cssNormal;
		}
	}
	obj.onmousedown = function(){
		gObjSpliter = this;
		gPosX = GetPosX();
		gPosY = GetPosY();
		this.setCapture(true)
	}
	obj.onmouseup = function (){
		this.releaseCapture();
		this.className = cssNormal;
		OnWndMouseUp();
	}
	obj.onmousemove = function (){
		if(null == gObjSpliter)
			return;
		var objPre	 = gObjSpliter.objMove;
		if(null == objMove)
			return;
		// 移动分隔
		var x	= GetPosX();
		var y 	= GetPosY();
		var n;
		
		if(true == gObjSpliter.bLR)
		{
			n = objPre.offsetWidth + x - gPosX;
			if(n <= 0)
				n = 1;
			if(0 < minV && n < minV)
			{
				n = minV;
				x = gObjSpliter.offsetLeft + gObjSpliter.offsetWidth / 2;
			}
			else if(0 < maxV && n > maxV)
			{
				n = maxV;
				x = gObjSpliter.offsetLeft + gObjSpliter.offsetWidth / 2;
			}
			// 左右分隔
			objPre.style.width = n + "px";
		}
		else
		{
			n = objPre.offsetHeight + y - gPosY;
			if(n <= 0)
				n = 1;
			if(0 < minV && n < minV)
			{
				n = minV;
				y = gObjSpliter.offsetTop + gObjSpliter.offsetHeigth / 2;
			}
			else if(0 < maxV && n > maxV)
			{
				n = maxV;
				y = gObjSpliter.offsetTop + gObjSpliter.offsetHeigth / 2;
			}
			// 上下分隔
			objPre.style.height = n + "px";
		}
		gPosX = x;
		gPosY = y;
	};
	obj.bLR = bLR;
	obj.objMove = objMove;
}

function OnWndMouseUp()
{
	gObjSpliter = null;
	gPosX = 0;
	gPosY = 0;
}

function GetPosX()
{
	return window.event.clientX + document.body.scrollLeft;
}

function GetPosY()
{
	return window.event.clientY + document.body.scrollTop;
}