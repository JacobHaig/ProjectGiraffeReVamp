#include "stdafx.h"
#include "Person.h"
#include "Maps.h"
#include "Utilities.h"
#include "Draw.h"
#include "Movement.h"
#include "Fight.h"
#include "SaveLoadGame.h"

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <vector>
#include <fstream>
#include <thread>

using namespace std;


//main function
void main() {

	Draw::drawMainMenu();

	// ADD new Game prompt

	string yourName = "Gary 25";

	vector<Person*> curEnt = Maps::Entities1(), ent1 = Maps::Entities1();
	vector<Person*> ent2 = Maps::Entities2();
	vector<Person*> ent3 = Maps::Entities3();
	vector<Person*> ent4 = Maps::Entities4();

	vector<string> curMap = Maps::Map1();
	vector<string> lastMap = curMap;

	string statsFilename = "TheStats.txt", npcInfoFile = "npcs.txt";
	vector<int> playerStats = SaveLoad::playerStatsLoadMain();

	Draw::drawMap(Maps::playerInfo());
	Draw::drawMap(curMap);
	Draw::drawEntities(curEnt, curMap);
	Draw::drawYourStats(ent1, ent2, ent3, ent4);


	while (true) {
		Draw::drawEntities(curEnt, curMap);           // draws all units only if they are on a blank spot on the map
		curEnt[Utilities::vectorObjIndex(" ", curEnt)]->setPos(curEnt[Utilities::vectorObjIndex("@", curEnt)]->getPos());
		Movement::move(Movement::getInput(), curEnt, curMap);

		curMap = Utilities::checkMap(curEnt, curMap); // checks if your char is on maps border, if so than push if back to previous pos.

		if (curMap != lastMap) {					  // if the map is changed. change the entities in use and move your char to the correct portal.
													  // Saves current entities to current entities' map
			{
				if (lastMap == Maps::Map1())ent1 = curEnt;// saves entities if moving to differant map 
				if (lastMap == Maps::Map2())ent2 = curEnt;
				if (lastMap == Maps::Map3())ent3 = curEnt;
				if (lastMap == Maps::Map4())ent4 = curEnt;
			}

			curEnt = Utilities::movePlayerOnMapChange(curMap, lastMap, curEnt, ent1, ent2, ent3, ent4);
			lastMap = curMap;                         // set lastMap to current map 
		}

		// Saves current entities to current entities' map
		{
			if (curMap == Maps::Map1())ent1 = curEnt;// saves entities if moving to differant map 
			if (curMap == Maps::Map2())ent2 = curEnt;
			if (curMap == Maps::Map3())ent3 = curEnt;
			if (curMap == Maps::Map4())ent4 = curEnt;
		}

		Draw::drawYourStats(ent1, ent2, ent3, ent4);
		if (Utilities::checkPos(curEnt).compare("Nope")) {// if one of units coords is identical to another than do stuff
			system("CLS");
			Fight::fight(curEnt, curEnt[Utilities::vectorObjIndex(Utilities::checkPos(curEnt), curEnt)]->RealName, yourName, playerStats, statsFilename);
			curEnt.erase(curEnt.begin() + Utilities::vectorObjIndex(Utilities::checkPos(curEnt), curEnt));
			//Draw::drawCredits(); only to test the credits
		}

	}
	Draw::drawCredits();
	_getch();
}



