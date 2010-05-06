#pragma once

#include <CEGUI/CEGUI.h>

class CGUIHandler
{
public:
	CGUIHandler(void);
	~CGUIHandler(void);

	bool QuitBtnClick(const CEGUI::EventArgs &e);
};
