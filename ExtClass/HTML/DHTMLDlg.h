/*
* CDHtmlDialog常用扩展函数
*
* Version:V1.0
* Author:夏祥均（字元杰）
* 适用对象:CDHtmlDialog类
*
* 使用方法:
* 在DHTMLDialog的构造函数里设置之后就可能调用函数了DHTML::g_pDHTMLDialog = this;
* 之后就可能直接用DHTML::Function(...)来调用函数了
*/
#pragma once

//////////////////////////////////////////////////////////////////////////
//防止重复包函
#ifndef _FILE_DHTMLDlg_		
#define _FILE_DHTMLDlg_
// 操作DHTML里元素的一个类
namespace DHTML
{
	//////////////////////////////////////////////////////////////////////////
	// 设置元素属性
	HRESULT		SetElementProperty(LPCTSTR lpID, DISPID dispid, VARIANT *pVar);	
	// 得到元素属性
	VARIANT		GetElementProperty(LPCTSTR lpID, DISPID dispid);	
	// 设置元素的InnerHTML
	HRESULT		SetHTMLInnerHTML(LPCTSTR lpID, LPCTSTR szHtml);	
	// 得到元素的InnerHTML
	CString		GetHTMLInnerHTML(LPCTSTR lpID);
	// 设置元素的InnerText
	HRESULT		SetHTMLInnerText(LPCTSTR lpID, LPCTSTR szText);
	// 得到元素的InnerText
	CString		GetHTMLInnerText(LPCTSTR lpID);
	// 得到ActiveX控件的接口
	HRESULT		GetHTMLElementInterface(LPCTSTR lpID, REFIID riid, void** ppvObject);	
	template <class Q>
	HRESULT		GetHTMLElementInterface(LPCTSTR lpID, Q** ppvObject)
	{
		return SkyHtml::GetHTMLElementInterface(lpID, __uuidof(Q), (void **)ppvObject);
	}
	//////////////////////////////////////////////////////////////////////////
	// 设置Input元素的可用性
	HRESULT		SetHTMLInputDisabled(LPCTSTR lpID, BOOL bDisbled);	
	// 得到Input元素的可用性
	BOOL		GetHTMLInputDisabled(LPCTSTR lpID);	
	// 设置Input元素的先择状态
	HRESULT		SetHTMLInputChecked(LPCTSTR lpID, BOOL bChecked);	
	// 得到Input元素的选择状态
	BOOL		GetHTMLInputChecked(LPCTSTR lpID);	
	// 得到Input的Value
	HRESULT		SetHTMLInputValue(LPCTSTR lpID, LPCTSTR szValue);	
	// 得到Input元素的Value
	CString		GetHTMLInputValue(LPCTSTR lpID);	
	// 设置HTML的Onclick事件
	HRESULT		SetHTMLOnClicked(LPCTSTR lpID, VARIANT* pEvent);	
	// 得到HTML元素的OnClick事件的		
	VARIANT		GetHTMLOnClicked(LPCTSTR lpID);
	// 拷贝HTML元素的事件
	HRESULT		CopyHTMLOnClicked(LPCTSTR lpDesID, LPCTSTR lpSrcID);
	//////////////////////////////////////////////////////////////////////////
	// 设置选择框Select的值
	HRESULT		SetHTMLSelectValue(LPCTSTR lpID, LPCTSTR szValue);
	// 得到选择框Select的值
	CString		GetHTMLSelectValue(LPCTSTR lpID);
	// 添加新项
	HRESULT		HTMLSelectOptionAdd(LPCTSTR lpID, LPCTSTR szText, LPCTSTR szValue);
	// 删除现有项
	HRESULT		HTMLSelectOptionDelete(LPCTSTR lpID, LPCTSTR szValue);
	//////////////////////////////////////////////////////////////////////////
	// 设置HTML元素的Style接口
	HRESULT		SetHTMLElementStyle(LPCTSTR lpID, LPCTSTR sname, VARIANT *vt);
	// 得到HTML元素的Style接口
	HRESULT		GetHTMLElementStyle(LPCTSTR lpID, IHTMLStyle** ppStyle);
	// 设置HTML的可见性
	HRESULT		SetHTMLElementVisible(LPCTSTR lpID, BOOL bShow);
	// 设置HTML的可见性
	BOOL		GetHTMLElementVisible(LPCTSTR lpID);
	//////////////////////////////////////////////////////////////////////////
	// 模拟单击某一元素
	HRESULT		OnClickHTMLElement(LPCTSTR lpID);
	//////////////////////////////////////////////////////////////////////////
	// 检查指针的有效性
	BOOL		CheckDHTMLDialogPointer( CDHtmlDialog* const p);
	//////////////////////////////////////////////////////////////////////////
	extern CDHtmlDialog*	g_pDHTMLDialog;
}
#endif