// ---------------------------------------------------------------------------
// includes
#include "AEEngine.h"
#include "AEGameStateMgr.h"

#include "gsm.h"
#include "gamestatelist.h"
#include "racing.h"
#include "general.h"
#include "racing_map.h"

#include <iostream>
// ---------------------------------------------------------------------------


/*------------------------------------------------------------
/ FUNCTIONS - PLATFORMS
------------------------------------------------------------*/

// Purpose: loads the platform meshes in the map in racing_load function
void racing_map_load()
{
	// ====  Explanation ==== //
	// 17 platforms in a map - thus need to create mesh for 17*2 (player1 = A, player2 = B)
	// as all length, height and colour is the same - so just use the main platform struct for that
	// create another struct for all the specific details of platform, so as to load in the meshes for those
	

	// for every element, load in the mesh for both platformA & platformB, with the specific Mesh Pointer and values from main_pointer struct
	for (int i = 0; i < platform_max; i++) {
		SquareMesh(&platformA[i].platMesh, main_platform.length, main_platform.height, main_platform.colour);
		SquareMesh(&platformB[i].platMesh, main_platform.length, main_platform.height, main_platform.colour);
	}

	// ==== revert back to these if not working ==== // 
	//// platform for player 1
	//SquareMesh(&platform1.PMesh, platform1.length, platform1.height, platform1.colour);

	//// platform for player 2
	//SquareMesh(&platform2.PMesh, platform2.length, platform2.height, platform2.colour);

	return;
}


// Purpose: initialise the struct values for the platforms : such as the x & y values of the platform
void racing_map_init(f32 start, f32 end, int player)
{
	// ====  Explanation ==== //
	//	- we need to identify if need to call loop for platform A or for platform B, therefore create an if condition
	//	- if start == AEGfxGetWinMinX(), call for platform A - Player 1
	//	- if end == AEGfxGetWinMaxX(), call for platform B - Player 2
	//	for loop to determine y coords, and the overall xy coords (vector) of platform, conditions: 
	//	- y value between 2 platforms should not exceed 100
	//	- L of platform : 150, H of platform : 27
	//	- x value should be in between min_limit & max_limit
	//	- for loop, start from the min_limit, add main_platform.length / 4 * 3 to platX every loop, and stop when the back(platX + main_platform.length) is more than max_limit. 
	//	- if back is > max_limit, minus main_platform.length / 4 * 3 to platX every loop to platX every loop till platX is lesser than min_limit. 
	//	- if it is lesser, change from minus to plus.use the help of boolean is_reverse to help with the extra conditions
	//	- for loop, increment y value by 50 every loop // can add variation through this later on - but for now make it simple
	//	- before the for loop breaks to go to the next i value, initialise the vector(x & y values of the platform)
	
	// Variables
	f32 min_limit = start + 10.0f;
	f32 max_limit = end - 10.0f;
	f32 backX, prevX, prevY;
	bool is_reverse = false;

	// Create a variable to hold the previous i, in order for it to be saved for the next i

	/*------------------------------------------------------------
	// INIT PLATFORM - MAP
	------------------------------------------------------------*/
	if (player == 1)
	{
		// Initialise First Platforms
		platformA[0].platX = { min_limit };
		platformA[0].platY = { player1.pGround + 60.0f };
		platformA[0].platVect = { platformA[0].platX , platformA[0].platY };
		prevX = platformA[0].platX;
		prevY = platformA[0].platY;
		for (int i = 1; i < platform_max; i++)
		{

			if (is_reverse == false)
			{
				platformA[i].platX = { prevX + (main_platform.length / 4 * 3) };
				backX = platformA[i].platX + main_platform.length;
				platformA[i].platY = { prevY + 65.0f };
				prevX = platformA[i].platX; 
				prevY = platformA[i].platY;
				platformA[i].platVect = { platformA[i].platX , platformA[i].platY };
				if (backX > max_limit) { is_reverse = true; }
	
			}

			else if (is_reverse == true)
			{
				platformA[i].platX = { prevX - (main_platform.length / 4 * 3) };
				platformA[i].platY = { prevY + 65.0f };
				prevX = platformA[i].platX;
				prevY = platformA[i].platY;
				platformA[i].platVect = { platformA[i].platX , platformA[i].platY };
				if (platformA[i].platX < min_limit) { is_reverse = false; }
			}

		}

	}
	else if (player == 2)
	{
		// Initialise First Platforms
		platformB[0].platX = { min_limit };
		platformB[0].platY = { player2.pGround + 60.0f };
		platformB[0].platVect = { platformB[0].platX , platformB[0].platY };
		prevX = platformB[0].platX;
		prevY = platformB[0].platY;
		for (int i = 1; i < platform_max; i++)
		{
			if (is_reverse == false)
			{
				platformB[i].platX = { prevX + (main_platform.length / 4 * 3) };
				backX = platformB[i].platX + main_platform.length;
				platformB[i].platY = { prevY + 65.0f };
				platformB[i].platVect = { platformB[i].platX , platformB[i].platY };
				prevX = platformB[i].platX;
				prevY = platformB[i].platY;
				if (backX > max_limit) { is_reverse = true; }
			}

			else if (is_reverse == true)
			{
				platformB[i].platX = { prevX - (main_platform.length / 4 * 3) };
				platformB[i].platY = { prevY + 65.0f };
				platformB[i].platVect = { platformB[i].platX , platformB[i].platY };
				prevX = platformB[i].platX;
				prevY = platformB[i].platY;
				if (platformB[i].platX < min_limit) { is_reverse = false; }
			}

		}
	}
	
	// ==== revert back to these if not working ==== // 
	//platform1.Pspawn = player1.pGround + 60.0f;
	//platform2.Pspawn = player2.pGround + 60.0f;

	//platform1.PCoord = { (AEGfxGetWinMinX() / 2) - 20.0f , platform1.Pspawn };
	//platform2.PCoord = { (AEGfxGetWinMaxX() / 2) - 20.0f , platform2.Pspawn };

	return;
}

// Purpose: function that will draw the map using the initialised values from the initialise functions 
void racing_map_draw()
{

	// ====  Explanation ==== //
	// This function just draws out everything that was initialised in the previous function, 
	// therefore no need to have an if condition to verify if it's for Player 1 or Player 2
	// For each platform, set the position, texture and draw the mesh.
	// Create 2 For loops to draw both platform A and B.

	/*------------------------------------------------------------
	DRAWING PLATFORMS
	------------------------------------------------------------*/
	for (int i = 0; i < platform_max; i++) {
		AEGfxSetPosition(platformA[i].platVect.x, platformA[i].platVect.y);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxMeshDraw(platformA[i].platMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxSetPosition(platformB[i].platVect.x, platformB[i].platVect.y);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxMeshDraw(platformB[i].platMesh, AE_GFX_MDM_TRIANGLES);
	}



	// ==== revert back to these if not working ==== //
	//// Drawing platform 1
	//AEGfxSetPosition(platform1.PCoord.x, platform1.PCoord.y);
	//// No texture
	//AEGfxTextureSet(NULL, 0, 0);
	//// Drawing the mesh (list of triangles)
	//AEGfxMeshDraw(platform1.PMesh, AE_GFX_MDM_TRIANGLES);

	//// Drawing platform 2
	//AEGfxSetPosition(platform2.PCoord.x, platform2.PCoord.y);
	//// No texture
	//AEGfxTextureSet(NULL, 0, 0);
	//// Drawing the mesh (list of triangles)
	//AEGfxMeshDraw(platform2.PMesh, AE_GFX_MDM_TRIANGLES);

	return;
}

void racing_map_unload()
{
	// Unload all the platforms, using the same for loop as in load function
	for (int i = 0; i < platform_max; i++) {
		AEGfxMeshFree(platformA[i].platMesh);
		AEGfxMeshFree(platformB[i].platMesh);
	}

	return;
}

/*------------------------------------------------------------
/ FUNCTIONS - SPLIT SCREEN
------------------------------------------------------------*/

// Purppose: load splitscreen
void splitscreen_load()
{
	splitscreen.height = (AEGfxGetWinMaxY() - AEGfxGetWinMinY());
	SquareMesh(&splitscreen.lMesh, splitscreen.length, splitscreen.height, splitscreen.colour);
	return;
}

// Purpose: initialise splitscreen position trhough vector
void splitscreen_init()
{
	f32 Y = AEGfxGetWinMinY();
	f32 X = - 9.0f;
	/*f32 Y = 10.0f;
	f32 X = - 9.0f;*/
	splitscreen.lVect = { X, Y };
	return;
}

// Purpose: draw out splitscreen
void splitscreen_draw()
{
	// Drawing object 1
	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetPosition(splitscreen.lVect.x, splitscreen.lVect.y);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxMeshDraw(splitscreen.lMesh, AE_GFX_MDM_TRIANGLES);
	return;
}

// Purpose: unload split screen
void splitscreen_unload()
{
	AEGfxMeshFree(splitscreen.lMesh);
	return;
}