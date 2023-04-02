/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This source gile defines the util functions for the puzzle game mode.
* 
* Primary Author: Mariah Tahirah (mariahtahirah.b@digipen.edu)
==================================================================================*/

// ---------------------------------------------------------------------------
// includes
#include "PuzzleUtil.h"

// ---------------------------------------------------------------------------





int InitMapData(const char* FileName)
{
	std::ifstream ifs{ FileName, std::ios_base::in }; //open file for reading
	
	//if cnnot open
	if (!ifs.is_open())
	{
		return 0;
	}
	
	mapdata = new int* [GRID_ROWS];
	for (int i = 0; i < GRID_ROWS; i++)
	{
		mapdata[i] = new int[GRID_COLS];
	}

	//extract and assign
	for (int i = 0; i < GRID_ROWS; i++)
	{
		for (int j = 0; j < GRID_COLS; j++)
		{
			int temp = 0;
			ifs >> temp;
			mapdata[i][j] = temp;
		}
	}

	ifs.close();
	return 1;
}



void Map_Player_CollisionUpdate(Player * player)
{
	player->flag = Map_Object_Collision(player->coord.x, player->coord.y, player->size, player->size);
	if ((player->flag & COLLISION_BOTTOM)) //if collide bottom 
	{
		SnapToCell(&player->coord.y);
		player->velocity.y = 0.0f;
	}

	if ((player->flag & COLLISION_TOP)) //if collide top 
	{
		SnapToCell(&player->coord.y);
		player->velocity.y = 0.0f;
	}

	if ((player->flag & COLLISION_LEFT)) //if collide LEFT 
	{
		SnapToCell(&player->coord.x);
		player->velocity.x = 0;
	}

	if ((player->flag & COLLISION_RIGHT)) //if collide RIGHT 
	{
		SnapToCell(&player->coord.x);
		player->velocity.x = 0;
	}
}

/******************************************************************************/
/*!
	This function converts coordinates into center of grid cells
*/
/******************************************************************************/
void SnapToCell(float* Coordinate)
{
	*Coordinate = (float)(static_cast<int>(*Coordinate)); // cast to int 
	*Coordinate += 0.5f;  // snap to center
}



int GetCellValue(int X, int Y)
{
	if (X >= 0 && X < GRID_COLS && Y >= 0 && Y < GRID_ROWS) // if in bounds
	{
		int flipped_Y = GRID_ROWS - 1 - Y ; // get the vertically flipped Y coordinate
		return mapdata[flipped_Y][X]; // return the value at the flipped coordinate
	}
	return 0;
	
}



int Map_Object_Collision(float PosX, float PosY, float scaleX, float scaleY)
{
	int flag = 0;
	/************* RIGHT SIDE *****************/
	int rightx1, righty1, rightx2, righty2;
	// HOTSPOT 1 
	rightx1 = (int)(PosX + scaleX / 2.0f);	//To reach the right side
	righty1 = (int)(PosY + scaleY / 4.0f);	//To go up 1 / 4 of the height

	//HOTSPOT 2
	rightx2 = (int)(PosX + scaleX / 2.0f); //To reach right side 
	righty2 = (int)(PosY - scaleY / 4.f); // to go down 1/4 of height 
	if (GetCellValue(rightx1, righty1) == WALL || GetCellValue(rightx2, righty2) == WALL) // if hotspot value > 0
	{	
		//std::cout << "right\n";
		flag |= COLLISION_RIGHT;
	}
	/***************************************/

	/************* LEFT SIDE *****************/
	int leftx1, lefty1, leftx2, lefty2;
	// HOTSPOT 1 
	leftx1 = (int)(PosX  - scaleX / 2.f);	//To reach the left side
	lefty1 = (int)(PosY  + scaleY / 4.f);	//To go up 1 / 4 of the height

	//HOTSPOT 2
	leftx2 = (int)(PosX  - scaleX / 2.f); //To reach left side 
	lefty2 = (int)(PosY  - scaleY / 4.f); // to go down 1/4 of height 
	if (GetCellValue(leftx1, lefty1) == WALL || GetCellValue(leftx2, lefty2) == WALL) // if hotspot value > 0
	{
		//std::cout << "left\n";
		flag |= COLLISION_LEFT;
	}
	/****************************************/

	/************* TOP SIDE *****************/
	int topx1, topy1, topx2, topy2;
	// HOTSPOT 1 
	topx1 = (int)(PosX + scaleX / 4.f);	//To go 1/4 towards right
	topy1 = (int)(PosY + scaleY / 2.f);	//To reach top side

	//HOTSPOT 2
	topx2 = (int)(PosX - scaleX / 4.f); //To go 1/4 towards left
	topy2 = (int)(PosY + scaleY / 2.f); // to reach top
	if (GetCellValue(topx1, topy1) == WALL || GetCellValue(topx2, topy2) == WALL) // if hotspot value > 0
	{
		//std::cout << "TOP\n";
		flag |= COLLISION_TOP;
	}
	/****************************************/

	/************* BOTTOM SIDE *****************/
	int bottomx1, bottomy1, bottomx2, bottomy2;
	// HOTSPOT 1 
	bottomx1 = (int)(PosX  + scaleX / 4.f);	//To go 1/4 towards right
	bottomy1 = (int)(PosY  - scaleY / 2.f);	//To reach bottom side
	//HOTSPOT 2
	bottomx2 = (int)(PosX  - scaleX / 4.f); //To go 1/4 towards left
	bottomy2 = (int)(PosY  - scaleY / 2.f); // to reach bottom
	if (GetCellValue(bottomx1, bottomy1) == WALL || GetCellValue(bottomx2, bottomy2) == WALL) // if hotspot value > 0
	{
		//std::cout << "bottom\n";
		flag |= COLLISION_BOTTOM;
	}
	/****************************************/

	return flag;
}