/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Mariah Tahirah (mariahtahirah.b@digipen.edu)
* Secondary Authors:
*	Shayne Gloria (m.shayne@digipen.edu) -> Platforms & Split Screen (Load, Init, Draw, Unload)
*   Yeo Hui Shan (huishan.y@digipen.edu) -> Collision 
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
defines
------------------------------------------------------------*/

// Camera Movement Variables
static f32			maxHeight		{ 0 };
static int			count			{ 1 };
static f32			maxHeight_copy	{ 0 };
static const f32	H				{ 200.0f };

static f32			CamX{ 0.0f }, CamY{ 0.0f }; // Camera's X & Y Positions





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
	SquareMesh(&player1.pMesh,  0xFFB62891);

	// player 2 mesh
	SquareMesh(&player2.pMesh,0xFFFF00FF);

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
	player1.pGround = AEGfxGetWinMinY() + player1.size / 2.f ;
	player2.pGround = AEGfxGetWinMinY() + player2.size / 2.f ;

	player1.pCoord = { AEGfxGetWinMinX() / 2, player1.pGround }; //spawn at left half of screen
	player2.pCoord = { AEGfxGetWinMaxX() / 2, player2.pGround }; //spawn at right half of screen



	/*------------------------------------------------------------
	// INIT PLATFORM - MAP
	------------------------------------------------------------*/
	// declaring values for start and end values for player1 & 2 - which is needed for racing map
	player1.startX	= AEGfxGetWinMinX();
	player1.endX	= - 0.0f;

	//player2.start = (AEGfxGetWinMinX() + AEGfxGetWinMaxX()) / 2;
	player2.startX	= 0.0f;
	player2.endX	= AEGfxGetWinMaxX();

	// calling racing map initialisation fucntion, using values from previous lines
	racing_map_init(player1.startX, player1.endX, 1);
	racing_map_init(player2.startX, player2.endX, 2);



	/*------------------------------------------------------------
	// INIT SPLITSCREEN
	------------------------------------------------------------*/
	splitscreen_init();



	/*------------------------------------------------------------
	// INIT - Camera Movement
	------------------------------------------------------------*/
	maxHeight = H;
	maxHeight_copy = H;

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
	if (AEInputCheckCurr(AEVK_3)) {
		next_state = PUZZLE;
	}



	/*------------------------------------------------------------
	// PLAYER MOVEMENT
	------------------------------------------------------------*/
	input_handle();

	for (int i = 0; i < platform_max; i++)
	{
		if (CollisionIntersection_RectRect(player1.pCoord, player1.size, player1.size, 
											platformA[i].platVect, platformA[i].length, platformA[i].height) == true) {

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
			player1.pCoord.y	= platformA[i].platVect.y + platformA[i].height / 2.f + player1.size / 2.f;
			player1.pCurrGround = platformA[i].platVect.y + platformA[i].height / 2.f + player1.size / 2.f;

			player1.stepping= true;
		}
		else
		{
			//player1.stepping = false;
			//player1.pCurrGround = player1.pGround;
		}

		/// PLAYER 2 PLATFORM COLLSIION 
		if (CollisionIntersection_RectRect(player2.pCoord, player2.size, player2.size, 
											platformB[i].platVect, platformB[i].length, platformB[i].height) == true) {

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
			player2.pCoord.y	= platformB[i].platVect.y + platformB[i].height / 2.f + player2.size / 2.f;
			player2.pCurrGround = platformB[i].platVect.y + platformB[i].height / 2.f + player2.size / 2.f;

			player2.stepping = true;
		}
		else
		{
			//player1.stepping = false;
			//player1.pCurrGround = player1.pGround;
		}

	}



	/*------------------------------------------------------------
	MATRIX CALCULATION
	------------------------------------------------------------*/
	// for players 
	MatrixCalc(player1.transform, player1.size, player1.size, 0.f, player1.pCoord);
	MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.pCoord);

	//for platforms 
	for (int i = 0; i < platform_max; i++) {
		MatrixCalc(platformA[i].transform, platformA[i].length, platformA[i].height, 0.f, platformA[i].platVect);
		MatrixCalc(platformB[i].transform, platformB[i].length, platformB[i].height, 0.f, platformB[i].platVect);
	}
	
	//for splitscreen
	MatrixCalc(splitscreen.transform, splitscreen.length, splitscreen.height, 0.f, splitscreen.lVect);



	///*------------------------------------------------------------
	// UPDATE - Camera Movement
	// if the player.y + size is over or at maxHeight, CamY will increase
	//------------------------------------------------------------*/
	if ((player1.pCoord.y + player1.size) >= maxHeight || (player2.pCoord.y + player2.size) >= maxHeight)
	{
		count++;
		CamY+=maxHeight_copy; // Calculate the target camera position to gradually move towards

		// maxHeight Incrementing
		maxHeight = (maxHeight_copy * count);
	}


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
	AEGfxSetTransform(player1.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);

	// drawing player 2
	AEGfxSetTransform(player2.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);



	/*------------------------------------------------------------
	// DRAWING SPLITSCREEN
	------------------------------------------------------------*/
	splitscreen_draw();



	/*------------------------------------------------------------
	 DRAWING - Camera Movement
	------------------------------------------------------------*/
	AEGfxSetCamPosition(CamX, CamY); // Set Camera's Position to values of CamX & CamY


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
