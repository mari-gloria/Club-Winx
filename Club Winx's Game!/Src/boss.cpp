// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "AEGameStateMgr.h"

#include "gsm.h"
#include "gamestatelist.h"
#include "boss.h"
#include "general.h"

#include <iostream>
// ---------------------------------------------------------------------------




/*------------------------------------------------------------
GLOBALS
------------------------------------------------------------*/
int const MAX_BULLETS{ 10 };

AEGfxVertexList* pBullet = 0;

struct Bullet {
	AEVec2 bCoord;
};

Bullet bullets[MAX_BULLETS];




/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void boss_load()
{
	std::cout << "boss:Load\n";

	//setting bg
	AEGfxSetBackgroundColor(0.0f, 0.0f, 255.0f);

	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/
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


	//bullet mesh
	AEGfxMeshStart();

	AEGfxTriAdd(
		-5.0f, -5.0f, 0xFFFFFFFF, 0.0f, 0.0f, // bottom left 
		5.0f, -5.0f, 0xFFFFFFFF, 1.0f, 0.0f, // bottom right
		-5.0f, 2.5f, 0xFFFFFFFF, 0.0f, 1.0f); //top left
	AEGfxTriAdd(
		5.0f, -5.0f, 0xFFFFFFFF, 1.0f, 0.0f, // bottom right
		5.0f, 2.5f, 0xFFFFFFFF, 1.0f, 1.0f, // top right 
		-5.0f, 2.5f, 0xFFFFFFFF, 0.0f, 1.0f); // top left

	pBullet = AEGfxMeshEnd();
	AE_ASSERT_MESG(pBullet, "failed to create bullet!!");

	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/


	/*------------------------------------------------------------
	CREATING FONTS
	------------------------------------------------------------*/


}

void boss_init()
{
	std::cout << "boss:Initialize\n";

	player1.pCoord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 50};
	player2.pCoord = { AEGfxGetWinMinX() + 95, AEGfxGetWinMinY() + 50 };
}

void boss_update()
{
	std::cout << "boss:Update\n";

	/*------------------------------------------------------------
	CHANGE STATE CONDITION
	------------------------------------------------------------*/
	if (AEInputCheckCurr(AEVK_Q)) {
		next_state = QUIT;
	}
	if (AEInputCheckCurr(AEVK_1)) {
		next_state = RACING;
	}

	/*------------------------------------------------------------
	PLAYER MOVEMENT
	------------------------------------------------------------*/
	// player one input
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


	/*------------------------------------------------------------
	BULLET
	------------------------------------------------------------*/
	if (AEInputCheckCurr(AEVK_SPACE)) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			bullets[i].bCoord.x += 10.0f;
		}
	}
}

void boss_draw()
{
	std::cout << "boss:Draw\n";

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
	DRAWING BULLETS
	------------------------------------------------------------*/
	if (AEInputCheckTriggered(AEVK_SPACE)) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			AEGfxSetPosition(player1.pCoord.x, player1.pCoord.y);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxMeshDraw(pBullet, AE_GFX_MDM_TRIANGLES);
		}
	}
	
}

void boss_free()
{
	std::cout << "boss:Free\n";

}

void boss_unload()
{
	std::cout << "boss:Unload\n";

	AEGfxMeshFree(player1.pMesh);
	AEGfxMeshFree(player2.pMesh);

}