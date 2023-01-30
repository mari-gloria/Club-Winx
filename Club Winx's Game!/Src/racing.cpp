/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: NIL
* Secondary Authors:
*	Shayne Gloria (m.shayne@digipen.edu) -> Platforms & Split Screen (Load, Init, Draw, Unload)
*
==================================================================================*/

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
GLOBALS
------------------------------------------------------------*/
const float		BOUNDING_RECT_SIZE = 0.40f;

/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void racing_load()
{
	std::cout << "racing:Load\n";

	AEGfxSetBackgroundColor(0.0f, 255.0f, 0.0f);

	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/

	// Informing the library that we're about to start adding triangles

	// player 1 mesh 
	SquareMesh(&player1.pMesh, player1.size, player1.size, 0xFFB62891);

	// player 2 mesh
	SquareMesh(&player2.pMesh, player2.size, player2.size, 0xFFFF00FF);

	// loading in platform meshes in map
	racing_map_load();

	// loading in line
	splitscreen_load();

	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/


	/*------------------------------------------------------------
	CREATING FONTS
	------------------------------------------------------------*/

	return;
}

void racing_init()
{
	std::cout << "racing:Initialize\n";

	/*------------------------------------------------------------
	// INIT PLAYERS
	------------------------------------------------------------*/
	//curretly set ground level = AEGfxGetWinMinY() + 50.0f
	//need to update ground to patform levels after jumping! (implement once platforms can be rendered properly)
	player1.pGround = AEGfxGetWinMinY() + 50.0f;
	player2.pGround = AEGfxGetWinMinY() + 50.0f;

	player1.pCoord = { AEGfxGetWinMinX() / 2, player1.pGround }; //spawn at left half of screen
	player2.pCoord = { AEGfxGetWinMaxX() / 2, player2.pGround }; //spawn at right half of screen

	/*------------------------------------------------------------
	// INIT PLATFORM - MAP
	------------------------------------------------------------*/
	// declaring values for start and end values for player1 & 2 - which is needed for racing map
	player1.start = AEGfxGetWinMinX();
	player1.end = (AEGfxGetWinMinX() + AEGfxGetWinMaxX()) / 2;

	player2.start = (AEGfxGetWinMinX() + AEGfxGetWinMaxX()) / 2;
	player2.end = AEGfxGetWinMaxX();

	// calling racing map initialisation fucntion, using values from previous lines
	racing_map_init(player1.start, player1.end, 1);
	racing_map_init(player2.start, player2.end, 2);

	/*------------------------------------------------------------
	// INIT SPLITSCREEN
	------------------------------------------------------------*/
	splitscreen_init();

	return;
}

void racing_update()
{
	std::cout << "racing:Update\n";

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	if (AEInputCheckCurr(AEVK_2)) {
		next_state = BOSS;
	}

	if (AEInputCheckCurr(AEVK_Q)) {
		next_state = QUIT;
	}

	/*------------------------------------------------------------
	// PLAYER MOVEMENT
	------------------------------------------------------------*/
	input_handle();

	for (int i = 0; i < platform_max; i++)
	{
		if (CollisionIntersection_RectRect(player1.pCoord, player1.size, player1.size, platformA[i].platVect, main_platform.length, main_platform.height) == true ) {
		
			//std::cout << "test test test" << std::endl;
			platformA[i].stepped = true;
			//player1.stepping = true;
		
		}
		else
		{
			platformA[i].stepped = false;
			//player1.stepping = false;
		}

		if (platformA[i].stepped)
		{
			player1.pCoord.y = platformA[i].platY + main_platform.height;
			player1.pCurrGround = platformA[i].platY + main_platform.height;
			player1.stepping = true;
		}
		else
		{
			//player1.stepping = false;
			//player1.pCurrGround = player1.pGround;
		}

		/// PLAYER 2 PLATFORM COLLSIION 
		if (CollisionIntersection_RectRect(player2.pCoord, player2.size, player2.size, platformB[i].platVect, main_platform.length, main_platform.height) == true) {

			//std::cout << "test test test" << std::endl;
			platformB[i].stepped = true;
			//player1.stepping = true;

		}
		else
		{
			platformB[i].stepped = false;
			//player1.stepping = false;
		}

		if (platformB[i].stepped)
		{
			player2.pCoord.y = platformB[i].platY + main_platform.height;
			player2.pCurrGround = platformB[i].platY + main_platform.height;
			player2.stepping = true;
		}
		else
		{
			//player1.stepping = false;
			//player1.pCurrGround = player1.pGround;
		}
		
	}

	//input_handle();
}

void racing_draw()
{
	std::cout << "racing:Draw\n";

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	/*------------------------------------------------------------
	// DRAWING PLATFORMS - MAP
	------------------------------------------------------------*/
	racing_map_draw();
	/*------------------------------------------------------------
	// DRAWING PLAYERS
	------------------------------------------------------------*/
	// Drawing object 1
	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	// Set position for object 1
	AEGfxSetPosition(player1.pCoord.x, player1.pCoord.y);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);

	// drawing player 2
	AEGfxSetPosition(player2.pCoord.x, player2.pCoord.y);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	// DRAWING PLATFORMS - MAP
	------------------------------------------------------------*/
	//racing_map_draw();

	/*------------------------------------------------------------
	// DRAWING SPLITSCREEN
	------------------------------------------------------------*/
	splitscreen_draw();

}

void racing_free()
{
	std::cout << "racing:Free\n";

}

void racing_unload()
{
	std::cout << "racing:Unload\n";

	/*------------------------------------------------------------
	// Unload Player Meshes
	------------------------------------------------------------*/
	AEGfxMeshFree(player1.pMesh);
	AEGfxMeshFree(player2.pMesh);

	/*------------------------------------------------------------
	// Unload Platform Meshes
	------------------------------------------------------------*/
	racing_map_unload();

	/*------------------------------------------------------------
	// Unload Split Screen Meshes
	------------------------------------------------------------*/
	splitscreen_unload();
}
