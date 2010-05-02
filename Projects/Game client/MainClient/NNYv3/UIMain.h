#pragma once

#include "pthread.h"

class CUIMain
{
public:
	CUIMain(void);
	~CUIMain(void);

	//Called form main thread: To launch the whole GUI
	int StartUI(void);
	//Called from main thread: When the main thread wants to notify the gui thread
	int SendNotify(int MessageID);

	//Message constants
	static const int	Message_Quit		=	100;
	static const int	Message_DoneLoading	=	101;
private:
	bool		Started;
	//=============
	// Thread related
	//=============
	pthread_t	m_uithread;
	friend		void* UIThreadStarter(void* class_pointer); //Helper function to give the created thread the right class pointer
	void*		UIThread(void);

	//=============
	// Ogre related
	//=============

};
