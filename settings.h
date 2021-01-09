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
#include <ctime> //���� ��������� �������� ������� � ���

class settings
{
public:
	settings();
	~settings();
	//��� ������� ������ ������������
	static void getNameProject(std::vector <std::string> &vectorParam);
	enum Parametr
	{
		// ���� ��������� �������������
		// ��� ��� ��������� �������� ����� ���� ������
		// ������ ������������� ����������� ������� (�� ������ � �������)
		PATHS,
		LANGUAGE, // ������� ����� �������, ����� ���
	}; // ������ ��� enum ������ ������������� ������ � �������


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