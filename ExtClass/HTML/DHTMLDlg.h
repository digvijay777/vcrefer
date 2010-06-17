/*
* CDHtmlDialog������չ����
*
* Version:V1.0
* Author:���������Ԫ�ܣ�
* ���ö���:CDHtmlDialog��
*
* ʹ�÷���:
* ��DHTMLDialog�Ĺ��캯��������֮��Ϳ��ܵ��ú�����DHTML::g_pDHTMLDialog = this;
* ֮��Ϳ���ֱ����DHTML::Function(...)�����ú�����
*/
#pragma once

//////////////////////////////////////////////////////////////////////////
//��ֹ�ظ�����
#ifndef _FILE_DHTMLDlg_		
#define _FILE_DHTMLDlg_
// ����DHTML��Ԫ�ص�һ����
namespace DHTML
{
	//////////////////////////////////////////////////////////////////////////
	// ����Ԫ������
	HRESULT		SetElementProperty(LPCTSTR lpID, DISPID dispid, VARIANT *pVar);	
	// �õ�Ԫ������
	VARIANT		GetElementProperty(LPCTSTR lpID, DISPID dispid);	
	// ����Ԫ�ص�InnerHTML
	HRESULT		SetHTMLInnerHTML(LPCTSTR lpID, LPCTSTR szHtml);	
	// �õ�Ԫ�ص�InnerHTML
	CString		GetHTMLInnerHTML(LPCTSTR lpID);
	// ����Ԫ�ص�InnerText
	HRESULT		SetHTMLInnerText(LPCTSTR lpID, LPCTSTR szText);
	// �õ�Ԫ�ص�InnerText
	CString		GetHTMLInnerText(LPCTSTR lpID);
	// �õ�ActiveX�ؼ��Ľӿ�
	HRESULT		GetHTMLElementInterface(LPCTSTR lpID, REFIID riid, void** ppvObject);	
	template <class Q>
	HRESULT		GetHTMLElementInterface(LPCTSTR lpID, Q** ppvObject)
	{
		return SkyHtml::GetHTMLElementInterface(lpID, __uuidof(Q), (void **)ppvObject);
	}
	//////////////////////////////////////////////////////////////////////////
	// ����InputԪ�صĿ�����
	HRESULT		SetHTMLInputDisabled(LPCTSTR lpID, BOOL bDisbled);	
	// �õ�InputԪ�صĿ�����
	BOOL		GetHTMLInputDisabled(LPCTSTR lpID);	
	// ����InputԪ�ص�����״̬
	HRESULT		SetHTMLInputChecked(LPCTSTR lpID, BOOL bChecked);	
	// �õ�InputԪ�ص�ѡ��״̬
	BOOL		GetHTMLInputChecked(LPCTSTR lpID);	
	// �õ�Input��Value
	HRESULT		SetHTMLInputValue(LPCTSTR lpID, LPCTSTR szValue);	
	// �õ�InputԪ�ص�Value
	CString		GetHTMLInputValue(LPCTSTR lpID);	
	// ����HTML��Onclick�¼�
	HRESULT		SetHTMLOnClicked(LPCTSTR lpID, VARIANT* pEvent);	
	// �õ�HTMLԪ�ص�OnClick�¼���		
	VARIANT		GetHTMLOnClicked(LPCTSTR lpID);
	// ����HTMLԪ�ص��¼�
	HRESULT		CopyHTMLOnClicked(LPCTSTR lpDesID, LPCTSTR lpSrcID);
	//////////////////////////////////////////////////////////////////////////
	// ����ѡ���Select��ֵ
	HRESULT		SetHTMLSelectValue(LPCTSTR lpID, LPCTSTR szValue);
	// �õ�ѡ���Select��ֵ
	CString		GetHTMLSelectValue(LPCTSTR lpID);
	// �������
	HRESULT		HTMLSelectOptionAdd(LPCTSTR lpID, LPCTSTR szText, LPCTSTR szValue);
	// ɾ��������
	HRESULT		HTMLSelectOptionDelete(LPCTSTR lpID, LPCTSTR szValue);
	//////////////////////////////////////////////////////////////////////////
	// ����HTMLԪ�ص�Style�ӿ�
	HRESULT		SetHTMLElementStyle(LPCTSTR lpID, LPCTSTR sname, VARIANT *vt);
	// �õ�HTMLԪ�ص�Style�ӿ�
	HRESULT		GetHTMLElementStyle(LPCTSTR lpID, IHTMLStyle** ppStyle);
	// ����HTML�Ŀɼ���
	HRESULT		SetHTMLElementVisible(LPCTSTR lpID, BOOL bShow);
	// ����HTML�Ŀɼ���
	BOOL		GetHTMLElementVisible(LPCTSTR lpID);
	//////////////////////////////////////////////////////////////////////////
	// ģ�ⵥ��ĳһԪ��
	HRESULT		OnClickHTMLElement(LPCTSTR lpID);
	//////////////////////////////////////////////////////////////////////////
	// ���ָ�����Ч��
	BOOL		CheckDHTMLDialogPointer( CDHtmlDialog* const p);
	//////////////////////////////////////////////////////////////////////////
	extern CDHtmlDialog*	g_pDHTMLDialog;
}
#endif