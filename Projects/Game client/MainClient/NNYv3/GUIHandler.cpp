#include "GUIHandler.h"
#include "MainClient.h"

CGUIHandler::CGUIHandler(void)
{
}

CGUIHandler::~CGUIHandler(void)
{
}

bool CGUIHandler::QuitBtnClick(const CEGUI::EventArgs &e)
{
	CMainClient::getSingleton().SendNotify(CMainClient::Message_Quit);
	return true;
}