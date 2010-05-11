#pragma once

//=================================
// Callback function system
//This idea and technique is mainly taken from CEGUI.
//It is used to pass a class-member-function as a parameter (for callbacks).
//The CEGUI version can use any type of function, not only
//member functions. But since this whole project uses
//classes I only took that one which resulted in great simplification.
//Usage example:
//If you want to pass CUIMain::SomeCallback as a callback
//then pass this complete thing as parameter: (assuming m_uimain is a pointer to the class)
// new MemberCallbackFunction(&CUIMain::SomeCallback, m_uimain)
//Or from within the CUIMain class it would be
// new MemberCallbackFunction(&CUIMain::SomeCallback, this)
//=================================

//Abstract base class
class CallbackFunction{
public:
	virtual bool CallCallback(void* Param) = 0;
};

//This class has a different version for every class, since 'T' is the class.
template<typename T>
class MemberCallbackFunction : public CallbackFunction
{
public:
	typedef bool (T::*CallBackFunc)(void* Param);

	MemberCallbackFunction(CallBackFunc function, T* obj) : m_function(function), m_object(obj) {};
	~MemberCallbackFunction(void) {};

	virtual bool CallCallback(void* Param)
	{
		return (m_objet->*m_function)(Param);
	}

private:
	CallBackFunc	m_function;
	T*				m_object;
};
//=================================