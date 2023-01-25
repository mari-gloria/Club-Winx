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
	player1.pCoord = {0.0f,0.0f};
	player2.pCoord = {100.0f,100.0f};
	
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
}