#include "UIMain.h"
#include <iostream>

CUIMain::CUIMain(void) :
	Started(false), m_uithread()
{
}

CUIMain::~CUIMain(void)
{
}

int CUIMain::StartUI(void)
{
	if( Started ) return 1;
	Started = true;

	//Setup Ogre

	//Setup sound

	//Create the ui thread
	if( pthread_create(&m_uithread, NULL, UIThreadStarter, this) ){
		std::cout << "[ERROR] pthread: pthread_create() failed!" << std::endl;
		return 0;
	}

	return 1;
}

int CUIMain::SendNotify(int MessageID)
{
	return 0;
}


void* UIThreadStarter(void* class_pointer){
	return ((CUIMain*)class_pointer)->UIThread();
}

void* CUIMain::UIThread(void)
{

	return 0;
}