/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Mariah Tahirah (mariahtahirah.b@digipen.edu)
* Secondary Authors:
*	
*
==================================================================================*/

#include "General.h"


enum MAPOBJ {
	EMPTY = 0,
	WALL,
	P1,
	P2
};

#define GRID_COLS 36 
#define GRID_ROWS 20 


//static int** mapdata;
extern int ** mapdata;

void SnapToCell(float* Coordinate);
int Map_Object_Collision(float x, float y, float scalex, float scaley);
int GetCellValue(int x, int y);
void Map_Player_CollisionUpdate(Player &player);
int InitMapData(const char* FileName);