/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Mariah Tahirah (mariahtahirah.b@digipen.edu)
* Secondary Authors:
*	Yeo Hui Shan (huishan.y@digipen.edu)
*
==================================================================================*/

// ---------------------------------------------------------------------------
// includes
#include "AEEngine.h"
#include "AEGameStateMgr.h"

#include "gsm.h"
#include "gamestatelist.h"
#include "general.h"
#include <iostream>
// ---------------------------------------------------------------------------

//#define PUZZLE_MAP_W 840
//#define PUZZLE_MAP_H 520

// ----- TEXTURE DECLARATIONS ----- //
// Textures for Puzzle Mode
AEGfxTexture* Key;
AEGfxTexture* Spiderweb;

void puzzle_load()
{
	std::cout << "puzzle:Load\n";

	AEGfxSetBackgroundColor(255.0f, 0.0f, 0.0f);
	bgPuzzle.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgPuzzle.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	// Textures for Puzzle
	bgPuzzle.bgTex = AEGfxTextureLoad("Assets/PUZZLE_2.png");
	AE_ASSERT_MESG(bgPuzzle.bgTex, "Failed to create bgPuzzle.bgTex!!");

	Key = AEGfxTextureLoad("Assets/KEY.png");
	AE_ASSERT_MESG(Key, "Failed to create Key!!");

	Spiderweb = AEGfxTextureLoad("Assets/SPIDERWEB.png");
	AE_ASSERT_MESG(Spiderweb, "Failed to create Spiderweb!!");

	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/
	// BG mesh
	SquareMesh(&bgPuzzle.bgMesh, 0);

	// player 1 mesh 
	SquareMesh(&player1.pMesh, 0xFFB62891);

	// player 2 mesh
	SquareMesh(&player2.pMesh, 0xFFFF00FF);
}

void puzzle_init()
{
	std::cout << "puzzle:Initialize\n";

	/*------------------------------------------------------------
	INIT PLAYERS
	------------------------------------------------------------*/
	player1.pCoord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 50 };
	player2.pCoord = { player1.pCoord.x + 100.f, player1.pCoord.y };
}

void puzzle_update()
{
	std::cout << "puzzle:Update\n";

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	if (AEInputCheckCurr(AEVK_2)) {
		next_state = BOSS;
	}

	if (AEInputCheckCurr(AEVK_Q)) {
		next_state = QUIT;
	}

	if (AEInputCheckCurr(AEVK_1)) {
		next_state = RACING;
	}

	/*------------------------------------------------------------
	PLAYER MOVEMENT
	------------------------------------------------------------*/
	input_handle();

	/*------------------------------------------------------------
	MATRIX CALCULATION
	------------------------------------------------------------*/
	// for background
	MatrixCalc(bgPuzzle.transform, bgPuzzle.length, bgPuzzle.height, 0.0f, bgPuzzle.bgCoord);

	// for players 
	MatrixCalc(player1.transform, player1.size, player1.size, 0.f, player1.pCoord);
	MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.pCoord);
}

void puzzle_draw()
{
	std::cout << "puzzle:Draw\n";

	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransform(bgPuzzle.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);
	AEGfxTextureSet(bgPuzzle.bgTex, 0.0f, 0.0f);
	AEGfxMeshDraw(bgPuzzle.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	DRAWING PLAYERS
	------------------------------------------------------------*/
	// Drawing player 1
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTransform(player1.transform.m);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);

	// drawing player 2
	AEGfxSetTransform(player2.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);

}

void puzzle_free()
{
	std::cout << "puzzle:Free\n";

	AEGfxMeshFree(player1.pMesh);
	AEGfxMeshFree(player2.pMesh);
	AEGfxMeshFree(bgPuzzle.bgMesh);
}

void puzzle_unload()
{
	std::cout << "puzzle:Unload\n";

	AEGfxTextureUnload(bgPuzzle.bgTex);
	AEGfxTextureUnload(Key);
	AEGfxTextureUnload(Spiderweb);

}