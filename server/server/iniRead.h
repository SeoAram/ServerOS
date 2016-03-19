#pragma once
#include "stdafx.h"

class IniData{
private:
	IniData(){
		FILE* iniFile;
		if (fopen_s(&iniFile, "../../data.dat", "r")){
			cout << "Ini File Load Fail!!" << endl;
			//m_umIniData.insert((string)"MAP_HEIGHT", 1024);
			//m_umIniData.insert((string)"MAP_WIDTH", 1024);
		}
		else{
			cout << "Ini File Load Success!!!" << endl;
			fclose(iniFile);
		}
	}
public:
	~IniData();
	static IniData* getInstance(){
		static IniData instance;
		return &instance;
	}
	static unordered_multimap<string, int> m_umIniData;
};