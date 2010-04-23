#pragma once

class CGUI
{
public:
	CGUI(void);
	~CGUI(void);

	int StartThread(void);
	int SendNotify(void); //Something to send messages between gui thread and main thread?
	int PeekForMessage(void);

	static const int	Message_NoMessages	=	0;
	static const int	Message_Quit		=	100;
	static const int	Message_Previous	=	101;
	static const int	Message_Login		=	1000;
	static const int	Message_RealmSelect	=	1001;
	static const int	Message_CharSelect	=	1002;
};
