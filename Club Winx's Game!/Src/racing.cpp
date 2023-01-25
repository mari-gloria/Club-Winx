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
	AEGfxMeshStart();
	AEGfxTriAdd(
		-25.5f, -25.5f, 0xFFFF0000, 0.0f, 0.0f, // bottom left 
		25.5f, -25.5f, 0xFFFF0000, 1.0f, 0.0f, // bottom right
		-25.5f, 25.5f, 0xFFFF0000, 0.0f, 1.0f); //top left
	AEGfxTriAdd(
		25.5f, -25.5f, 0xFFFF0000, 1.0f, 0.0f, // bottom right
		25.5f, 25.5f, 0xFFFF0000, 1.0f, 1.0f, // top right 
		-25.5f, 25.5f, 0xFFFF0000, 0.0f, 1.0f); // top left 

	player1.pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(player1.pMesh, "Failed to create player 1 mesh !!");

	// player 2 mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-25.5f, -25.5f, 0xFFFFFF00, 0.0f, 0.0f, // bottom left 
		25.5f, -25.5f, 0xFFFF0000, 1.0f, 0.0f, // bottom right
		-25.5f, 25.5f, 0xFFFF00FF, 0.0f, 1.0f); //top left
	AEGfxTriAdd(
		25.5f, -25.5f, 0xFF00FF00, 1.0f, 0.0f, // bottom right
		25.5f, 25.5f, 0xFFFF00FF, 1.0f, 1.0f, // top right 
		-25.5f, 25.5f, 0xFFFF0000, 0.0f, 1.0f); // top left 

	player2.pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(player2.pMesh, "Failed to create player 2 mesh !!");


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
	// INIT PLAYER ONE 
	player1.pCoord = {0,0};
	player2.pCoord = {100,100};

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
	// player one input
	if (AEInputCheckCurr(AEVK_W) && player1.pCoord.y <= AEGfxGetWinMaxY() - 25)//25 is half the height of square
		player1.pCoord.y += 3.0f;

	else if (AEInputCheckCurr(AEVK_S) && player1.pCoord.y >= AEGfxGetWinMinY() + 25)
		player1.pCoord.y -= 3.0f;

	if (AEInputCheckCurr(AEVK_A) && player1.pCoord.x >= AEGfxGetWinMinX() + 25)
		player1.pCoord.x -= 3.0f;

	else if (AEInputCheckCurr(AEVK_D) && player1.pCoord.x <= AEGfxGetWinMaxX() - 25)
		player1.pCoord.x += 3.0f;

	//player two input
	if (AEInputCheckCurr(AEVK_UP) && player2.pCoord.y <= AEGfxGetWinMaxY() - 25)
		player2.pCoord.y += 3.0f;

	else if (AEInputCheckCurr(AEVK_DOWN) && player2.pCoord.y >= AEGfxGetWinMinY() + 25)
		player2.pCoord.y -= 3.0f;

	if (AEInputCheckCurr(AEVK_LEFT) && player2.pCoord.x >= AEGfxGetWinMinX() + 25)
		player2.pCoord.x -= 3.0f;

	else if (AEInputCheckCurr(AEVK_RIGHT) && player2.pCoord.x <= AEGfxGetWinMaxX() - 25)
		player2.pCoord.x += 3.0f;
	
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