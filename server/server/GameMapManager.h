#pragma once
class GameMapManager
{
private:
	char m_cIsFileLoad;
	GameMap* m_pgMap;
	GameMapManager();
public:
	~GameMapManager();
	static GameMapManager* getInstance(){
		static GameMapManager instance;
		return &instance;
	}
};

