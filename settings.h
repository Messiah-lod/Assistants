#pragma once

#include <iostream>
#include <vector>
#include <string>

//#include <getline>
#include <exception>
#include "Windows.h"
#include <iostream>
#include <any>
#include <fstream>
#include <ctime> //либа дл€вывода текущего времени в лог

class settings
{
public:
	settings();
	~settings();
	//все функции делаем статическими
	static void getNameProject(std::vector <std::string> &vectorParam);
	enum Parametr
	{
		// Ќиже наход€тс€ перечислители
		// Ёто все возможные значени€ этого типа данных
		//  аждый перечислитель раздел€етс€ зап€той (Ќ≈ точкой с зап€той)
		PATHS,
		LANGUAGE, // зап€тую можно ставить, можно нет
	}; // ќднако сам enum должен заканчиватьс€ точкой с зап€той


private:
	static void readFile(Parametr _param, std::vector <std::string> &vectorParam);

//	static const std::vector <std::string> vectorMySettings;

	static inline const char* ToString(Parametr p)
	{
		switch (p)
		{
		case PATHS:   return "[PATHS]";
		case LANGUAGE:   return "[LANGUAGE]";
//		case Windows: return "Windows";
		default:      return "[Unknown Parametr]";
		}
	}

};