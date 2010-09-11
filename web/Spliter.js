/*
 * ��̬�ָ���
 *
 * Author: XiaXiangJun
 * Date: 2010-09-11
 */
var 	gObjSpliter		= null;
var 	gPosX			= 0;
var 	gPosY			= 0;

/*
 * ���ö�̬�ָ���
 *
 * ����: obj: ��̬�ָ�������    	objMove: ��̬�ָ����ƶ���Ҫ�ƶ��Ķ���
 *       bLR: �Ƿ�Ϊ���ҷָ� 		cssNormal: �ָ�����ʽ
 *       cssHover: �ָ����õ�������ʽ
 *       minV: objMove����С(��/��) maxV: objMove�����(��/��)
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
		// �ƶ��ָ�
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
			// ���ҷָ�
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
			// ���·ָ�
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