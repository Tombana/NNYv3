#pragma once

#include <string>

struct WORLDSERVER{
	std::string IP;
	unsigned short Port;
	std::string Name;
	bool Online;
};

struct CharacterInfo{
	unsigned char	Slot;
	std::string		Name;
	int				Level;
	bool			Gender;	//0=male;1=female
	bool			Online;
	//Lots of info is coming here
	//stats
	//class
	//looks (face, hair etc)
	//clothes and weapons and so on
};