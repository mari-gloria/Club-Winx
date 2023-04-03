/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
*
* Brief: This header file declares the functions for the racing game mode layout.
* 
* Primary Author: 
*	Shayne Gloria (m.shayne@digipen.edu) -> platforms, split screen
* 
* Secondary Authors:
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu) -> platform randomization, racing boosts
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "RacingMap.h"
// ---------------------------------------------------------------------------





/*--------------------------------------------------------------------------
variables
---------------------------------------------------------------------------*/
//racing mode
RacingBoosts racing_boostsA[MAX_NUM_ITEMS], racing_boostsB[MAX_NUM_ITEMS];
Platform	platformA[MAX_NUM_PLATFORMS], platformB[MAX_NUM_PLATFORMS];
Line		splitscreen, startingline;

int rand_nums_listA[MAX_NUM_ITEMS]; //list of random platform numbers for player 1
int rand_nums_listB[MAX_NUM_ITEMS]; //list of random platform numbers for player 2





/*--------------------------------------------------------------------------
declare helper functions
---------------------------------------------------------------------------*/
template <typename T>
void draw_shape(T item);





// =========================================================================== //
// ================================ PLATFORMS ================================ //
// =========================================================================== //

// Purpose: loads the platform meshes in the map in racing_load function
void racing_map_load()
{
	//boosts for player1
	for (int i = 0; i < MAX_NUM_ITEMS; i++)
	{
		//creating circle shape for collectables
		AEGfxMeshStart();
		int Parts = 20;
		for (float i = 0; i < Parts; ++i)
		{
			AEGfxTriAdd(
				0.0f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
				cosf(i * 2 * PI / Parts) * 0.5f, sinf(i * 2 * PI / Parts) * 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
				cosf((i + 1) * 2 * PI / Parts) * 0.5f, sinf((i + 1) * 2 * PI / Parts) * 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		}

		racing_boostsA[i].mesh = AEGfxMeshEnd();

		AE_ASSERT_MESG(racing_boostsA[i].mesh, "fail to create object!!");
	}

	//boosts for player2
	for (int i = 0; i < MAX_NUM_ITEMS; i++)
	{
		//creating circle shape for collectables
		AEGfxMeshStart();
		int Parts = 20;
		for (float i = 0; i < Parts; ++i)
		{
			AEGfxTriAdd(
				0.0f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
				cosf(i * 2 * PI / Parts) * 0.5f, sinf(i * 2 * PI / Parts) * 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
				cosf((i + 1) * 2 * PI / Parts) * 0.5f, sinf((i + 1) * 2 * PI / Parts) * 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
		}

		racing_boostsB[i].mesh = AEGfxMeshEnd();

		AE_ASSERT_MESG(racing_boostsB[i].mesh, "fail to create object!!");
	}
	
	

	// ====  Explanation ==== //
	// 17 platforms in a map - thus need to create mesh for 17*2 (player1 = A, player2 = B)
	// as all length, height and colour is the same - so just use the main platform struct for that
	// create another struct for all the specific details of platform, so as to load in the meshes for those


	// for every element, load in the mesh for both platformA & platformB, with the specific Mesh Pointer and values from main_pointer struct
	for (int i = 0; i < MAX_NUM_PLATFORMS; i++) 
	{
		platformA[i].texture = AEGfxTextureLoad("Assets/RacingAssets/RACING_PLATFORM.jpg");
		platformB[i].texture = AEGfxTextureLoad("Assets/RacingAssets/RACING_PLATFORM.jpg");
		SquareMesh(&platformA[i].mesh, 0xFFFFFF00);
		SquareMesh(&platformB[i].mesh, 0xFFFFFF00);
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
	float buffer = 50.0f;
	f32 min_limit = start + platformA[0].length / 2 + buffer; 
	f32 max_limit = end - platformA[0].length / 2 - buffer;


	// Create a variable to hold the previous i, in order for it to be saved for the next i

	/*------------------------------------------------------------
	// INIT PLATFORM - MAP
	------------------------------------------------------------*/
	switch (player) {

		/****************************
			draw map for player 1
		****************************/
	case 1:
		platformA[0].coord.x = player1.coord.x;
		platformA[0].coord.y = player1.coord.y + 50.0f;


		for (int i = 1; i < MAX_NUM_PLATFORMS; i++) {
			//x-coord: random x coord is generated within min and max limit
			//y-coord: increases by 100.0f from prev platform
			platformA[i].coord.x = rand_num(min_limit, max_limit);
			platformA[i].coord.y = platformA[i - 1].coord.y + 100.0f;

			// END POINT: if last platform, let x be the center of the platform area
			if (i == (MAX_NUM_PLATFORMS - 1))
			{
				platformA[i].coord.x = (min_limit + max_limit) / 2;
				platformA[i].coord.y = platformA[i - 1].coord.y + 100.0f;
				platformA[i].length = 350.0f; // make end platform longer
			}
		}

		//init boost
		//generate a list of random platform numbers 
		for (int i = 0; i < MAX_NUM_ITEMS; i++) {
			rand_nums_listA[i] = rand_num(0, MAX_NUM_PLATFORMS - MAX_NUM_ITEMS);
		}

		//sort random number list
		std::sort(rand_nums_listA, rand_nums_listA + MAX_NUM_ITEMS);

		//remove repeated numbers (if any)
		for (int i = 0; i < MAX_NUM_ITEMS; i++)
		{
			if (rand_nums_listA[i] == rand_nums_listA[i + 1])
			{
				rand_nums_listA[i] += rand_num(1, MAX_NUM_ITEMS);
			}
		}

		//sort list again
		std::sort(rand_nums_listA, rand_nums_listA + MAX_NUM_ITEMS);

		//update position of boosts
		for (int i = 0; i < MAX_NUM_ITEMS; i++)
		{
			int platform_num = rand_nums_listA[i];
			racing_boostsA[i].coord = { platformA[platform_num].coord.x,  platformA[platform_num].coord.y + racing_boostsA[i].size };

			//update collected
			racing_boostsA[i].collected = false;
		}

		break;

	/****************************
	draw map for player 2
	****************************/
	case 2:
		platformB[0].coord.x = player2.coord.x;
		platformB[0].coord.y = player2.coord.y + 50.0f;

		for (int i = 1; i < MAX_NUM_PLATFORMS; i++) {
			//x-coord: random x coord is generated within min and max limit
			//y-coord: increases by 100.0f from prev platform
			platformB[i].coord.x = rand_num(min_limit, max_limit);
			platformB[i].coord.y = platformB[i - 1].coord.y + 100.0f;

			// END POINT: if last platform, let x be the center of the platform area, and init the length here
			if (i == (MAX_NUM_PLATFORMS - 1))
			{
				platformB[i].coord.x = (min_limit + max_limit) / 2;
				platformB[i].coord.y = platformB[i - 1].coord.y + 100.0f;
				platformB[i].length = 350.0f; // make end platform longer
			}
		}

		//init boost
		//generate a list of random platform numbers 
		for (int i = 0; i < MAX_NUM_ITEMS; i++) {
			rand_nums_listB[i] = rand_num(0, MAX_NUM_PLATFORMS - MAX_NUM_ITEMS);
		}

		//sort random number list
		std::sort(rand_nums_listB, rand_nums_listB + MAX_NUM_ITEMS);

		//remove repeated numbers (if any)
		for (int i = 0; i < MAX_NUM_ITEMS; i++)
		{
			if (rand_nums_listB[i] == rand_nums_listB[i + 1])
			{
				rand_nums_listB[i] += rand_num(1, MAX_NUM_ITEMS);
			}
		}

		//sort list again
		std::sort(rand_nums_listB, rand_nums_listB + MAX_NUM_ITEMS);

		//update position of boosts
		for (int i = 0; i < MAX_NUM_ITEMS; i++)
		{
			int platform_num = rand_nums_listB[i];
			racing_boostsB[i].coord = { platformB[platform_num].coord.x,  platformB[platform_num].coord.y + racing_boostsB[i].size };

			//update collected
			racing_boostsB[i].collected = false;
		}

		break;
	}

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
		AEGfxSetTransform(platformA[i].transform.m);
		AEGfxTextureSet(platformA[i].texture, 0, 0);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(platformA[i].mesh, AE_GFX_MDM_TRIANGLES);

		AEGfxSetTransform(platformB[i].transform.m);
		AEGfxTextureSet(platformB[i].texture, 0, 0);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(platformB[i].mesh, AE_GFX_MDM_TRIANGLES);

	}

	/*------------------------------------------------------------
	DRAWING BOOSTS
	------------------------------------------------------------*/
	for (int i = 0; i < MAX_NUM_ITEMS; i++)
	{
		if (!racing_boostsA[i].collected)
		{
			draw_shape(racing_boostsA[i]);
		}

		if (!racing_boostsB[i].collected)
		{
			draw_shape(racing_boostsB[i]);
		}
	}

	return;
}




void racing_map_free()
{
	// Unload all the platforms, using the same for loop as in load function
	for (int i = 0; i < MAX_NUM_PLATFORMS; i++) {
		AEGfxMeshFree(platformA[i].mesh);
		AEGfxMeshFree(platformB[i].mesh);
	}

	//free item mesh
	for (int i = 0; i < MAX_NUM_ITEMS; i++)
	{
		AEGfxMeshFree(racing_boostsA[i].mesh);
		AEGfxMeshFree(racing_boostsB[i].mesh);
	}

	return;
}




void racing_map_unload()
{
	// Unload all the platforms, using the same for loop as in load function
	for (int i = 0; i < MAX_NUM_PLATFORMS; i++) {
		AEGfxTextureUnload(platformA[i].texture); // Unload Texture
		AEGfxTextureUnload(platformB[i].texture); // Unload Texture
	}

	return;
}






// =========================================================================== //
// ============================== SPLIT SCREEN =============================== //
// =========================================================================== //

// Purppose: load splitscreen
void splitscreen_load()
{
	splitscreen.height = AEGfxGetWinMaxY() * AEGfxGetWinMaxY();
	//SquareMesh(&splitscreen.lMesh, splitscreen.length, splitscreen.height, splitscreen.colour);
	SquareMesh(&splitscreen.mesh, splitscreen.colour);
	return;
}



// Purpose: initialise splitscreen position trhough vector
void splitscreen_init()
{
	f32 Y = 0.0f ;
	f32 X = 0.0f ;
	splitscreen.coord = { X, Y };
	return;
}



// Purpose: draw out splitscreen
void splitscreen_draw()
{
	AEGfxSetTransform(splitscreen.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxMeshDraw(splitscreen.mesh, AE_GFX_MDM_TRIANGLES);
	return;
}



// Purpose: free split screen
void splitscreen_free()
{
	AEGfxMeshFree(splitscreen.mesh);
	return;
}





/*--------------------------------------------------------------------------
helper functions
---------------------------------------------------------------------------*/
template <typename T>
void draw_shape(T item)
{
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTransform(item.transform.m);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxMeshDraw(item.mesh, AE_GFX_MDM_TRIANGLES);
}