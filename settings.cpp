#include "settings.h"

settings::settings()
{
}

settings::~settings()
{
}

void settings::getNameProject(std::vector <std::string> &vectorParam)
{
	//std::ofstream logs;
	//logs.open("./ini/settings2.ini", std::ios_base::app);//���������(�������) ����

	readFile(PATHS, vectorParam);
	//for (size_t i = 0; i < vectorParam.size(); i++)
	//{
	//	logs << vectorParam[i] << std::endl;
	//}
}

void settings::readFile(Parametr _param, std::vector <std::string> &vectorParam)
{
	vectorParam.clear();
	std::vector <std::string> vectorMySettings;
	std::string s;
	std::ifstream file;
	file.open("./ini/settings.ini");

	while (std::getline(file, s))
	{
		vectorMySettings.push_back(s);
	}
	file.close();

	for (size_t i = 0; i < vectorMySettings.size(); i++)
	{
		if (vectorMySettings[i] == ToString(_param))
		{
			while (i < vectorMySettings.size())//���� �� �������� �� �����
			{
				i++;
				if (vectorMySettings[i] != ToString(PATHS)
					&& vectorMySettings[i] != ToString(LANGUAGE)//�� ���������� �� ��������� ��������
					//������ ��������� �� ���������� enum Parametr
					&& !vectorMySettings[i].empty() //������ �� ������
					)
				{
					vectorParam.push_back(vectorMySettings[i]);
				}
				else
				{
					break;//����� �� 2 �����
				}
			};
			break;//����� �� 1 ����� ��� ��������� ���������
		}
	}
}
