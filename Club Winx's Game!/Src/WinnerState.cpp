// ================================================================================================================ //
// ======================================== FUNCTIONS FOR DIFFERENT WIN STATE ===================================== //
// ================================================================================================================ //

// Include these Header files
#include "General.h"

// Camera Movement Variables
static f32			CamX{ 0.0f },
CamY{ 0.0f };	// Camera's X & Y Positions

// ============================ PLAYERONE WIN ============================ //

void WIN_PLAYERONE_load()
{

	//AEGfxSetBackgroundColor(255.0f, 255.0f, 248.0f);
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	SquareMesh(&bgWin.bgMesh, 0xFFFF00FF);
	bgWin.bgTex = AEGfxTextureLoad("Assets/player1_bg.png");
	bgWin.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgWin.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();


	return;
}

void WIN_PLAYERONE_init()
{

	return;
}

void WIN_PLAYERONE_update()
{

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	// if press SPACE, go to the next game
	if (AEInputCheckTriggered(AEVK_SPACE) && prev_state == RACING) {
		next_state = BOSS;
	}
	//if (AEInputCheckTriggered(AEVK_SPACE) && prev_state == PUZZLE) {
	//	next_state = BOSS;
	//}

	/*------------------------------------------------------------
	// MATRIX CALCULATIONS
	------------------------------------------------------------*/
	MatrixCalc(bgWin.transform, bgWin.length, bgWin.height, 0.f, bgWin.bgCoord);


	return;
}

void WIN_PLAYERONE_draw()
{

	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransform(bgWin.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(bgWin.bgTex, 0.f, 0.f);
	AEGfxMeshDraw(bgWin.bgMesh, AE_GFX_MDM_TRIANGLES);


	return;
}

void WIN_PLAYERONE_free()
{

	return;
}

void WIN_PLAYERONE_unload()
{

	// Unload Background
	AEGfxMeshFree(bgWin.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgWin.bgTex); // Unload Texture

	return;
}

// ============================ PLAYERTWO WIN ============================ //

void WIN_PLAYERTWO_load()
{

	//AEGfxSetBackgroundColor(255.0f, 255.0f, 248.0f);
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	SquareMesh(&bgWin.bgMesh, 0xFFFF00FF);
	bgWin.bgTex = AEGfxTextureLoad("Assets/player2_bg.png");
	bgWin.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgWin.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();


	return;
}

void WIN_PLAYERTWO_init()
{

	return;
}

void WIN_PLAYERTWO_update()
{

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/

	// if press SPACE, go to the next game
	if (AEInputCheckTriggered(AEVK_SPACE) && prev_state == RACING) {
		next_state = BOSS;
	}
	//if (AEInputCheckTriggered(AEVK_SPACE) && prev_state == PUZZLE) {
	//	next_state = BOSS;
	//}
	if (AEInputCheckTriggered(AEVK_SPACE) && prev_state == BOSS) {
		next_state = ENDGAME;
	}

	/*------------------------------------------------------------
	// MATRIX CALCULATIONS
	------------------------------------------------------------*/
	MatrixCalc(bgWin.transform, bgWin.length, bgWin.height, 0.f, bgWin.bgCoord);


	return;
}

void WIN_PLAYERTWO_draw()
{

	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransform(bgWin.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(bgWin.bgTex, 0.f, 0.f);
	AEGfxMeshDraw(bgWin.bgMesh, AE_GFX_MDM_TRIANGLES);


	return;
}

void WIN_PLAYERTWO_free()
{

	return;
}

void WIN_PLAYERTWO_unload()
{

	// Unload Background
	AEGfxMeshFree(bgWin.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgWin.bgTex); // Unload Texture

	return;
}

// ============================ BOTH PLAYERS WIN ============================ //

void WIN_BOTHPLAYERS_load()
{

	//AEGfxSetBackgroundColor(255.0f, 255.0f, 248.0f);
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	SquareMesh(&bgWin.bgMesh, 0xFFFF00FF);
	bgWin.bgTex = AEGfxTextureLoad("Assets/bothwin_bg.png");
	bgWin.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgWin.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();


	return;
}

void WIN_BOTHPLAYERS_init()
{

	return;
}

void WIN_BOTHPLAYERS_update()
{

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/

	// if press space, go to the next game
	if (AEInputCheckTriggered(AEVK_SPACE)) {
		
		next_state = prev_state == RACING? BOSS : (PUZZLE ? RACING : MENU);
	}

	/*------------------------------------------------------------
	// MATRIX CALCULATIONS
	------------------------------------------------------------*/
	MatrixCalc(bgWin.transform, bgWin.length, bgWin.height, 0.f, bgWin.bgCoord);


	return;
}

void WIN_BOTHPLAYERS_draw()
{

	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransform(bgWin.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(bgWin.bgTex, 0.f, 0.f);
	AEGfxMeshDraw(bgWin.bgMesh, AE_GFX_MDM_TRIANGLES);


	return;
}

void WIN_BOTHPLAYERS_free()
{

	return;
}

void WIN_BOTHPLAYERS_unload()
{

	// Unload Background
	AEGfxMeshFree(bgWin.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgWin.bgTex); // Unload Texture

	return;
}

// ============================ BOTH PLAYERS LOSE ============================ //

void LOSE_BOTHPLAYERS_load()
{

	//AEGfxSetBackgroundColor(255.0f, 255.0f, 248.0f);
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	SquareMesh(&bgWin.bgMesh, 0xFFFF00FF);
	bgWin.bgTex = AEGfxTextureLoad("Assets/bothlose_bg.png");
	bgWin.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgWin.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();


	return;
}

void LOSE_BOTHPLAYERS_init()
{

	return;
}

void LOSE_BOTHPLAYERS_update()
{

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	// if press back, go replay
	std::cout << "prev" << prev_state << std::endl;
	std::cout << "curr" << curr_state << std::endl;
	std::cout << "next" << next_state << std::endl;

	if (AEInputCheckTriggered(AEVK_SPACE)) {
		next_state = prev_state;
	}

	/*------------------------------------------------------------
	// MATRIX CALCULATIONS
	------------------------------------------------------------*/
	MatrixCalc(bgWin.transform, bgWin.length, bgWin.height, 0.f, bgWin.bgCoord);


	return;
}

void LOSE_BOTHPLAYERS_draw()
{

	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransform(bgWin.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(bgWin.bgTex, 0.f, 0.f);
	AEGfxMeshDraw(bgWin.bgMesh, AE_GFX_MDM_TRIANGLES);


	return;
}

void LOSE_BOTHPLAYERS_free()
{

	return;
}

void LOSE_BOTHPLAYERS_unload()
{

	// Unload Background
	AEGfxMeshFree(bgWin.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgWin.bgTex); // Unload Texture

	return;
}

// ============================ ENDGAME WIN ============================ //

void ENDGAME_load()
{

	//AEGfxSetBackgroundColor(255.0f, 255.0f, 248.0f);
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	SquareMesh(&bgWin.bgMesh, 0xFFFF00FF);
	bgWin.bgTex = AEGfxTextureLoad("Assets/win_bg.png");
	bgWin.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgWin.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();


	return;
}

void ENDGAME_init()
{

	return;
}

void ENDGAME_update()
{

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	// if press space, go back to mainmenu
	if (AEInputCheckTriggered(AEVK_SPACE)) {

		next_state = MENU;
	}

	/*------------------------------------------------------------
	// MATRIX CALCULATIONS
	------------------------------------------------------------*/
	MatrixCalc(bgWin.transform, bgWin.length, bgWin.height, 0.f, bgWin.bgCoord);


	return;
}

void ENDGAME_draw()
{

	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransform(bgWin.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(bgWin.bgTex, 0.f, 0.f);
	AEGfxMeshDraw(bgWin.bgMesh, AE_GFX_MDM_TRIANGLES);


	return;
}

void ENDGAME_free()
{

	return;
}

void ENDGAME_unload()
{

	// Unload Background
	AEGfxMeshFree(bgWin.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgWin.bgTex); // Unload Texture

	return;
}
