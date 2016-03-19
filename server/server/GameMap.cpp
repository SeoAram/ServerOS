#include "stdafx.h"
#include "GameMap.h"


GameMap::GameMap()
{
	FILE* mapFile;
	if (fopen_s(&mapFile, "MapPattern.dat", "r")){
		cout << "Map Pattern File Load Fail!!" << endl;
		//for (int i = 0; i < MAX_MAP_LIST; ++i){
			//pattern[0]->list[i] = i;
		//}
	}
	else{
		for (int i = 0; i < 100; ++i){
			//fscanf_s(mapFile, "%d", &(pattern[i]->id));
			//for (int j = 0; j < MAX_MAP_LIST; ++j){
				//fread_s((char*)&tmp, 1, 1, 1, mapFile);
			//}
			cout << endl;
		}

		cout << "Map Pattern File Load Success!!!" << endl;
		fclose(mapFile);
	}
}


GameMap::~GameMap()
{
}
