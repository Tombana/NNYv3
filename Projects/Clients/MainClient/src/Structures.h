#pragma once

#include <string>

struct WORLDSERVER{
	std::string IP;
	unsigned short Port;
	std::string Name;
	bool Online;
};

//General info on looks of an entity. Both for players and monsters
//TODO: Better name than 'LookInfo'.
struct LookInfo{
	int	ModelId; //Or LookId. This is a reference to an xml file!
};

struct CharacterInfo{
	unsigned char	Slot;
	std::string		Name;
	int				Level;
	bool			Gender;	//0=male;1=female
	bool			Online;
	LookInfo		Looks;
	//Lots of info is coming here
	//stats
	//class
	//looks (face, hair etc)
	//clothes and weapons and so on
};