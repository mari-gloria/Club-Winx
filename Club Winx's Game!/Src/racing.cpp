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
//AEGfxVertexList* pMesh1 = 0;



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

	playerone.pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(playerone.pMesh, "Failed to create player 1 mesh !!");

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

	playertwo.pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(playertwo.pMesh, "Failed to create player 2 mesh !!");

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
	playerone.pcoords = { 100,100 };
	playertwo.pcoords = { 0 , 0 };
}

void racing_update()
{
	std::cout << "racing:Update\n";

	if (AEInputCheckTriggered(AEVK_1)) {
		next_state = BOSS;
	}

	// player one input 
	if (AEInputCheckCurr(AEVK_W))
		playerone.pcoords.y += 3.0f;
	else if (AEInputCheckCurr(AEVK_S))
		playerone.pcoords.y -= 3.0f;

	if (AEInputCheckCurr(AEVK_A))
		playerone.pcoords.x -= 3.0f;
	else if (AEInputCheckCurr(AEVK_D))
		playerone.pcoords.x += 3.0f;

	//player two input
	if (AEInputCheckCurr(AEVK_UP))
		playertwo.pcoords.y += 3.0f;
	else if (AEInputCheckCurr(AEVK_DOWN))
		playertwo.pcoords.y -= 3.0f;

	if (AEInputCheckCurr(AEVK_LEFT))
		playertwo.pcoords.x -= 3.0f;
	else if (AEInputCheckCurr(AEVK_RIGHT))
		playertwo.pcoords.x += 3.0f;
	
}

void racing_draw()
{
	std::cout << "racing:Draw\n";

	// Drawing object 1
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	// Set position for object 1
	AEGfxSetPosition(playerone.pcoords.x, playerone.pcoords.y);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(playerone.pMesh, AE_GFX_MDM_TRIANGLES);

	// drawing player 2
	AEGfxSetPosition(playertwo.pcoords.x, playertwo.pcoords.y);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(playertwo.pMesh, AE_GFX_MDM_TRIANGLES);
}

void racing_free()
{
	std::cout << "racing:Free\n";
	
}

void racing_unload()
{
	std::cout << "racing:Unload\n";
	AEGfxMeshFree(playerone.pMesh);
	AEGfxMeshFree(playertwo.pMesh);

}