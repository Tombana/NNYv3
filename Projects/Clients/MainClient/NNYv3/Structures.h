#pragma once

#include <string>

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