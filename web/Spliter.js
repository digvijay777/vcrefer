/*
 * 动态分隔条
 *
 * Author: XiaXiangJun
 * Date: 2010-09-11
 */

/*
 * 设置动态分隔条
 *
 * 参数: obj: 动态分隔条对像    	objMove: 动态分隔条移动后要移动的对像
 *       bLR: 是否为左右分隔 		cssNormal: 分隔条样式
 *       cssHover: 分隔条得到焦点样式
 *       minV: objMove的最小(宽/高) maxV: objMove的最大(宽/高)
 */
function SpliterBorder(obj, objMove, bLR, cssNormal, cssHover, cssMove, minV, maxV)
{
	obj.bLR = bLR;
	obj.objMove = objMove;
	obj.bMoving = false;
	
	// 鼠标移上
	obj.onmouseover = function(){
		if(false == obj.bMoving){
			this.className = cssHover;
		}
	}
	// 鼠标移开
	obj.onmouseout = function(){
		if(false == obj.bMoving){
			this.className = cssNormal;
		}
	}
	// 鼠标按下
	obj.onmousedown = function(el){
		obj.bMoving = true;
		obj.className = 'Spliter_LR_Move';

		// 设置Capture
		if(this.setCapture)
			this.setCapture();
		else if(window.captureEvents)
			window.captureEvents(Event.MOUSEMOVE|Event.MOUSEUP);
		
		// 拖动处理
		if(!el) el = window.event;
		
		var doc 		= document;
		var posX		= objMove.offsetLeft + ((false != bLR)?objMove.offsetWidth:0);
		var posY		= objMove.offsetTop + ((false != bLR)?0:objMove.offsetHeight);
		var startPosX	= el.layerX?el.layerX:el.clientX;
		var startPosY	= el.layerY?el.layerY:el.clientY;
		var minPos		= ((false != bLR)?objMove.offsetLeft:objMove.offsetTop) + minV;
		var maxPos		= ((false != bLR)?objMove.offsetLeft:objMove.offsetTop) + maxV;
		
		obj.style.left 	= 0;
		// 鼠标移动消息
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