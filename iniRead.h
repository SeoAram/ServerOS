#pragma once
#include <unordered_map>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

pair<string, int> INIT_DATA;

class IniData{
private:
	IniData(){

		ifstream infile;
		infile.open("../../data.dat");

		if (infile.is_open()){

			cout << "Ini File Load Success!!!" << endl;
			while (!infile.eof()){
				string key;
				int value;
				infile >> key >> value;

				cout << key << " :: " << value << endl;
				m_umIniData.insert({ key, value });
			}
			infile.close();

		}
		else{
			cout << "Ini File Load Fail!!" << endl;
			m_umIniData.insert({ (string)"MAP_HEIGHT", 2048 });
			m_umIniData.insert({ (string)"MAP_WIDTH", 2048 });
		}
		cout << m_umIniData.size() << endl;
	}
	unordered_multimap<string, int> m_umIniData;
	unordered_multimap<string, int>::iterator m_umIter;
public:
	~IniData(){}
	static IniData* getInstance(){
		static IniData instance;
		return &instance;
	}

	int getData(string key){
		m_umIter = m_umIniData.find(key);
		if (m_umIter == m_umIniData.end())
			return -1;
		else
			return m_umIniData.find(key)->second;
	}
};