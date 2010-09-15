/*
 * ��̬�ָ���
 *
 * Author: XiaXiangJun
 * Date: 2010-09-11
 */

/*
 * ���ö�̬�ָ���
 *
 * ����: obj: ��̬�ָ�������    	objMove: ��̬�ָ����ƶ���Ҫ�ƶ��Ķ���
 *       bLR: �Ƿ�Ϊ���ҷָ� 		cssNormal: �ָ�����ʽ
 *       cssHover: �ָ����õ�������ʽ
 *       minV: objMove����С(��/��) maxV: objMove�����(��/��)
 */
function SpliterBorder(obj, objMove, bLR, cssNormal, cssHover, cssMove, minV, maxV)
{
	obj.bLR = bLR;
	obj.objMove = objMove;
	obj.bMoving = false;
	
	// �������
	obj.onmouseover = function(){
		if(false == obj.bMoving){
			this.className = cssHover;
		}
	}
	// ����ƿ�
	obj.onmouseout = function(){
		if(false == obj.bMoving){
			this.className = cssNormal;
		}
	}
	// ��갴��
	obj.onmousedown = function(el){
		obj.bMoving = true;
		obj.className = 'Spliter_LR_Move';

		// ����Capture
		if(this.setCapture)
			this.setCapture();
		else if(window.captureEvents)
			window.captureEvents(Event.MOUSEMOVE|Event.MOUSEUP);
		
		// �϶�����
		if(!el) el = window.event;
		
		var doc 		= document;
		var posX		= objMove.offsetLeft + ((false != bLR)?objMove.offsetWidth:0);
		var posY		= objMove.offsetTop + ((false != bLR)?0:objMove.offsetHeight);
		var startPosX	= el.layerX?el.layerX:el.clientX;
		var startPosY	= el.layerY?el.layerY:el.clientY;
		var minPos		= ((false != bLR)?objMove.offsetLeft:objMove.offsetTop) + minV;
		var maxPos		= ((false != bLR)?objMove.offsetLeft:objMove.offsetTop) + maxV;
		
		obj.style.left 	= 0;
		// ����ƶ���Ϣ
		doc.onmousemove = function(el)
		{
			if(!el) el = window.event;
			if(!el.pageX) el.pageX = el.clientX;
			if(!el.pageY) el.pageY = el.clientY;
			
			if(bLR){
				obj.style.left = Math.max(minPos, Math.min(el.pageX, maxPos)) - posX;// + posX;// + posX;// - startPosX;
			} else {
				obj.style.top = Math.max(minPos, Math.min(el.pageY, maxPos)) - posY;
			}//obj.style.height = height + "px";
		}
		doc.onmouseup = function(el)
		{
			if(obj.releaseCapture)  
				obj.releaseCapture();  
			else if(window.captureEvents)  
				window.captureEvents(Event.MOUSEMOVE|Event.MOUSEUP);  

			if(bLR)
				objMove.style.width = obj.offsetLeft - objMove.offsetLeft;
			else
				objMove.style.height = obj.offsetTop - objMove.offsetTop;
			obj.className = cssNormal;
			doc.onmousemove = null;
			doc.onmouseup = null;
			obj.bMoving = false;
		}
	}
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