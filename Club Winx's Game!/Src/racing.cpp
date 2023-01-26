// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "AEGameStateMgr.h"

#include "gsm.h"
#include "gamestatelist.h"
#include "racing.h"
#include "general.h"

#include <iostream>
// ---------------------------------------------------------------------------


/*------------------------------------------------------------
GLOBALS
------------------------------------------------------------*/




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

	// platform for player 1
	SquareMesh(&platform1.PMesh, platform1.length, platform1.height, platform1.colour);

	// platform for player 2
	SquareMesh(&platform2.PMesh, platform2.length, platform2.height, platform2.colour);

	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/


	/*------------------------------------------------------------
	CREATING FONTS
	------------------------------------------------------------*/


}

void racing_init()
{
	std::cout << "racing:Initialize\n";

	// INIT PLAYERS
	//curretly set ground level = AEGfxGetWinMinY() + 50.0f
	//need to update ground to patform levels after jumping! (implement once platforms can be rendered properly)
	player1.pGround = AEGfxGetWinMinY() + 50.0f;
	player2.pGround = AEGfxGetWinMinY() + 50.0f;


	player1.pCoord = {AEGfxGetWinMinX() / 2, player1.pGround}; //spawn at left half of screen
	player2.pCoord = { AEGfxGetWinMaxX() / 2, player2.pGround }; //spawn at right half of screen

	// INIT PLATFORMS :: actually may need to create a function to initialise platforms, in order for randomisation | but for now use this to try it out 
	// set spawning point to be from above ground. 
	platform1.Pspawn = player1.pGround + 60.0f;
	platform2.Pspawn = player2.pGround + 60.0f;


	platform1.PCoord = { (AEGfxGetWinMinX() / 2) - 20.0f , platform1.Pspawn };
	platform2.PCoord = { (AEGfxGetWinMaxX() / 2) - 20.0f , platform2.Pspawn };
	
}

void racing_update()
{
	std::cout << "racing:Update\n";

	/*------------------------------------------------------------
	CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	if (AEInputCheckCurr(AEVK_2)) {
		next_state = BOSS;
	}

	if (AEInputCheckCurr(AEVK_Q)) {
		next_state = QUIT;
	}


	/*------------------------------------------------------------
	PLAYER MOVEMENT
	------------------------------------------------------------*/
	input_handle();

}

void racing_draw()
{
	std::cout << "racing:Draw\n";

	/*------------------------------------------------------------
	DRAWING PLAYERS
	------------------------------------------------------------*/
	// Drawing object 1
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
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
	DRAWING PLATFORMS
	------------------------------------------------------------*/
	// Drawing platform 1
	AEGfxSetPosition(platform1.PCoord.x, platform1.PCoord.y);
	// No texture
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(platform1.PMesh, AE_GFX_MDM_TRIANGLES);

	// Drawing platform 2
	AEGfxSetPosition(platform2.PCoord.x, platform2.PCoord.y);
	// No texture
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(platform2.PMesh, AE_GFX_MDM_TRIANGLES);

}

void racing_free()
{
	std::cout << "racing:Free\n";
	
}

void racing_unload()
{
	std::cout << "racing:Unload\n";
	AEGfxMeshFree(player1.pMesh);
	AEGfxMeshFree(player2.pMesh);

	// unload platform meshes
	AEGfxMeshFree(platform1.PMesh);
	AEGfxMeshFree(platform2.PMesh);
}