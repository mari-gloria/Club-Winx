/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Shayne Gloria (m.shayne@digipen.edu)
* Secondary Authors:
*
*
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "General.h"
// ---------------------------------------------------------------------------

/*--------------------------------------------------------------------------
Items for racing - boost/disadvantage for players
---------------------------------------------------------------------------*/

//variables for items
int rand_nums[MAX_NUM_ITEMS]; //list of random platform numbers


/*------------------------------------------------------------
/ FUNCTIONS - PLATFORMS
------------------------------------------------------------*/

// Purpose: loads the platform meshes in the map in racing_load function
void racing_map_load()
{
	for (int i = 0; i < MAX_NUM_ITEMS; i++)
	{
		//creating circle shape for collectables
		AEGfxMeshStart();
		int Parts = 12;
		for (float i = 0; i < Parts; ++i)
		{
			AEGfxTriAdd(
				0.0f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
				cosf(i * 2 * PI / Parts) * 0.5f, sinf(i * 2 * PI / Parts) * 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
				cosf((i + 1) * 2 * PI / Parts) * 0.5f, sinf((i + 1) * 2 * PI / Parts) * 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		}

		racing_items[i].pMesh = AEGfxMeshEnd();
		AE_ASSERT_MESG(racing_items[i].pMesh, "fail to create object!!");
	}
	
	

	// ====  Explanation ==== //
	// 17 platforms in a map - thus need to create mesh for 17*2 (player1 = A, player2 = B)
	// as all length, height and colour is the same - so just use the main platform struct for that
	// create another struct for all the specific details of platform, so as to load in the meshes for those


	// for every element, load in the mesh for both platformA & platformB, with the specific Mesh Pointer and values from main_pointer struct
	for (int i = 0; i < MAX_NUM_PLATFORMS; i++) {
		//SquareMesh(&platformA[i].platMesh, main_platform.length, main_platform.height, main_platform.colour);
		//SquareMesh(&platformB[i].platMesh, main_platform.length, main_platform.height, main_platform.colour);
		platformA[i].platTex = AEGfxTextureLoad("Assets/platform.jpg");
		platformB[i].platTex = AEGfxTextureLoad("Assets/platform.jpg");
		SquareMesh(&platformA[i].platMesh, 0xFFFFFF00);
		SquareMesh(&platformB[i].platMesh, 0xFFFFFF00);
	}

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
	f32 min_limit = start + platformA[0].length / 2 + 50.0f; //50.0f buffer
	f32 max_limit = end - platformA[0].length / 2 - 50.0f; //50.0f buffer
	//f32 backX, prevX, prevY;
	//bool is_reverse = false;

	// Create a variable to hold the previous i, in order for it to be saved for the next i

	/*------------------------------------------------------------
	// INIT PLATFORM - MAP
	------------------------------------------------------------*/
	switch (player) {

	case 1: //draw map for player 1
		platformA[0].platVect.x = player1.pCoord.x;
		platformA[0].platVect.y = player1.pCoord.y + 75.0f;

		//in progress -kristy
		//generate a list of random platform numbers 
		for (int i = 0; i < MAX_NUM_ITEMS; i ++) {
			rand_nums[i] = 0 + i * (rand_num(1, 3));
		}


		for (int i = 1; i < MAX_NUM_PLATFORMS; i++) {
			//x-coord: random x coord is generated within min and max limit
			//y-coord: increases by 100.0f from prev platform
			platformA[i].platVect.x = rand_num(min_limit, max_limit);
			platformA[i].platVect.y = platformA[i - 1].platVect.y + 100.0f;

			// END POINT: if last platform, let x be the center of the platform area
			if (i == (MAX_NUM_PLATFORMS - 1))
			{
				platformA[i].platVect.x = (min_limit + max_limit) / 2;
				platformA[i].platVect.y = platformA[i - 1].platVect.y + 100.0f;
				platformA[i].length = 350.0f; // make end platform longer
			}
		}

		//update position of items
		for (int i = 0; i < MAX_NUM_ITEMS; i++)
		{
			int platform_num = rand_nums[i];
			racing_items[i].pCoord = { platformA[platform_num].platVect.x,  platformA[platform_num].platVect.y + racing_items[i].size };

			//update collected
			racing_items[i].collected = false;
		}

		break;

	case 2: //draw map for player 2
		platformB[0].platVect.x = player2.pCoord.x;
		platformB[0].platVect.y = player2.pCoord.y + 75.0f;

		for (int i = 1; i < MAX_NUM_PLATFORMS; i++) {
			//x-coord: random x coord is generated within min and max limit
			//y-coord: increases by 100.0f from prev platform
			platformB[i].platVect.x = rand_num(min_limit, max_limit);
			platformB[i].platVect.y = platformB[i - 1].platVect.y + 100.0f;

			// END POINT: if last platform, let x be the center of the platform area, and init the length here
			if (i == (MAX_NUM_PLATFORMS - 1))
			{
				platformB[i].platVect.x = (min_limit + max_limit) / 2;
				platformB[i].platVect.y = platformA[i - 1].platVect.y + 100.0f;
				platformB[i].length = 350.0f; // make end platform longer
			}
		}
		break;
	}

	//old code!
	/*
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
	*/

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
	for (int i = 0; i < MAX_NUM_PLATFORMS; i++) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//AEGfxSetTextureMode(AE_GFX_TM_PRECISE);
		AEGfxSetTransform(platformA[i].transform.m);
		AEGfxTextureSet(platformA[i].platTex, 0, 0);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(platformA[i].platMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxSetTransform(platformB[i].transform.m);
		AEGfxTextureSet(platformB[i].platTex, 0, 0);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(platformB[i].platMesh, AE_GFX_MDM_TRIANGLES);

	}

	/*------------------------------------------------------------
	DRAWING ITEMS
	------------------------------------------------------------*/
	for (int i = 0; i < MAX_NUM_ITEMS; i++)
	{
		if (!racing_items[i].collected)
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxSetBlendMode(AE_GFX_BM_NONE);
			AEGfxSetTransform(racing_items[i].transform.m);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxMeshDraw(racing_items[i].pMesh, AE_GFX_MDM_TRIANGLES);
		}
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
	for (int i = 0; i < MAX_NUM_PLATFORMS; i++) {
		AEGfxMeshFree(platformA[i].platMesh);
		AEGfxMeshFree(platformB[i].platMesh);
		AEGfxTextureUnload(platformA[i].platTex); // Unload Texture
		AEGfxTextureUnload(platformB[i].platTex); // Unload Texture
	}

	return;
}





/*------------------------------------------------------------
/ FUNCTIONS - SPLIT SCREEN
------------------------------------------------------------*/

// Purppose: load splitscreen
void splitscreen_load()
{
	splitscreen.height = AEGfxGetWinMaxY() * AEGfxGetWinMaxY();
	//SquareMesh(&splitscreen.lMesh, splitscreen.length, splitscreen.height, splitscreen.colour);
	SquareMesh(&splitscreen.lMesh, splitscreen.colour);
	return;
}





// Purpose: initialise splitscreen position trhough vector
void splitscreen_init()
{
	//f32 Y = AEGfxGetWinMinY();
	//f32 X = - 9.0f;
	f32 Y = 0.0f ;
	f32 X = 0.0f ;
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
	//AEGfxSetPosition(splitscreen.lVect.x, splitscreen.lVect.y);
	AEGfxSetTransform(splitscreen.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
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