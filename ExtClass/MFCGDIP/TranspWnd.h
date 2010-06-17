#pragma once

template <class T>
class TTranspWnd : public T
{
 	DECLARE_DYNAMIC(TTranspWnd)
public:
	TTranspWnd();
	virtual ~TTranspWnd();

// protected:
// 	DECLARE_MESSAGE_MAP()
};
// 
// template <class T>
// IMPLEMENT_DYNAMIC_T(TTranspWnd, T, T)


#ifdef _AFXDLL
// #define T_BEGIN_MESSAGE_MAP(theClass) \
// 	template <class T> \
// 	const AFX_MSGMAP* PASCAL theClass<T>::GetThisMessageMap() \
// { return &theClass<T>::messageMap; } \
// 	template <class T> \
// 	const AFX_MSGMAP* theClass<T>::GetMessageMap() const \
// { return &theClass<T>::messageMap; } \
// 	template <class T> \
// 	AFX_COMDAT const AFX_MSGMAP theClass<T>::messageMap = \
// { &T::GetThisMessageMap, &theClass<T>::_messageEntries[0] }; \
// 	AFX_COMDAT const AFX_MSGMAP_ENTRY theClass<T>::_messageEntries[] = \
// { \

// #else
// #define T_BEGIN_MESSAGE_MAP(theClass, T1) \
// 	const AFX_MSGMAP* theClass::GetMessageMap() const \
// 	{ return &theClass::messageMap; } \
// 	template <class T> \
// 	AFX_COMDAT const AFX_MSGMAP theClass::messageMap = \
// 	{ &T1::messageMap, &theClass::_messageEntries[0] }; \
// 	template <class T> \
// 	AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
// 	{ \

#endif

// T_BEGIN_MESSAGE_MAP(TTranspWnd)
// template <class T> 
// const AFX_MSGMAP* TTranspWnd<T>::GetMessageMap() const 
// { return &TTranspWnd<T>::messageMap; } 
// template <class T> 
// AFX_COMDAT const AFX_MSGMAP TTranspWnd<T>::messageMap = 
// { &T::messageMap, &TTranspWnd<T>::_messageEntries[0] }; 
// template <class T> \
// AFX_COMDAT const AFX_MSGMAP_ENTRY TTranspWnd<T>::_messageEntries[] = 
// { 
// template <class T>
// T_BEGIN_MESSAGE_MAP(TTranspWnd<T>, T)
// 
// END_MESSAGE_MAP()
