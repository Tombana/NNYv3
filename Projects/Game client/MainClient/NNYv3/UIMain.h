#pragma once

class CUIMain
{
public:
	CUIMain(void);
	~CUIMain(void);

	//Called form main thread: To launch the whole GUI
	int StartThread(void);
	//Called from main thread: When the main thread wants to notify the gui thread
	int SendNotify(int MessageID);

	//Message constants
	static const int	Message_Quit		=	100;
	static const int	Message_DoneLoading	=	101;
private:

};
