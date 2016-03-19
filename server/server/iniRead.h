#pragma once
#include "stdafx.h"

pair<string, int> INIT_DATA;

class IniData{
private:
	IniData(){
		FILE* iniFile;
		if (fopen_s(&iniFile, "../../data.dat", "r")){
			cout << "Ini File Load Fail!!" << endl;
			m_umIniData.insert({ (string)"MAP_HEIGHT", 2048 });
			m_umIniData.insert({ (string)"MAP_WIDTH", 2048 });
			//m_umIniData.insert({ (string)"MAP_HEIGHT", m_umIniData.find("MAP_HEIGHT") });
			//m_umIniData.insert({ (string)"MAP_WIDTH", 2048 * 2 });
		}
		else{
			cout << "Ini File Load Success!!!" << endl;
			m_umIniData.insert({ (string)"MAP_HEIGHT", 2048 });
			m_umIniData.insert({ (string)"MAP_WIDTH", 2048 });
			fclose(iniFile);
		}
	}
public:
	~IniData(){}
	static IniData* getInstance(){
		static IniData instance;
		return &instance;
	}
	static unordered_multimap<string, int> m_umIniData;
};