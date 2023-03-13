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
	std::cout << "WIN_PLAYERONE:Load\n";

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
	std::cout << "WIN_PLAYERONE:Initialize\n";

	return;
}

void WIN_PLAYERONE_update()
{
	std::cout << "WIN_PLAYERONE:Update\n";

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
	std::cout << "WIN_PLAYERONE:Draw\n";

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
	std::cout << "WIN_PLAYERONE:Free\n";

	return;
}

void WIN_PLAYERONE_unload()
{
	std::cout << "WIN_PLAYERONE:Unload\n";

	// Unload Background
	AEGfxMeshFree(bgWin.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgWin.bgTex); // Unload Texture

	return;
}

// ============================ PLAYERTWO WIN ============================ //

void WIN_PLAYERTWO_load()
{
	std::cout << "WIN_PLAYERTWO:Load\n";

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
	std::cout << "WIN_PLAYERTWO:Initialize\n";

	return;
}

void WIN_PLAYERTWO_update()
{
	std::cout << "WIN_PLAYERTWO:Update\n";

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
	std::cout << "WIN_PLAYERTWO:Draw\n";

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
	std::cout << "WIN_PLAYERTWO:Free\n";

	return;
}

void WIN_PLAYERTWO_unload()
{
	std::cout << "WIN_PLAYERTWO:Unload\n";

	// Unload Background
	AEGfxMeshFree(bgWin.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgWin.bgTex); // Unload Texture

	return;
}

// ============================ BOTH PLAYERS WIN ============================ //

void WIN_BOTHPLAYERS_load()
{
	std::cout << "WIN_BOTHPLAYERS:Load\n";

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
	std::cout << "WIN_BOTHPLAYERS:Initialize\n";

	return;
}

void WIN_BOTHPLAYERS_update()
{
	std::cout << "WIN_BOTHPLAYERS:Update\n";

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/

	// if press space, go to the next game
	if (AEInputCheckTriggered(AEVK_RETURN)) {
		next_state = BOSS;
	}

	/*------------------------------------------------------------
	// MATRIX CALCULATIONS
	------------------------------------------------------------*/
	MatrixCalc(bgWin.transform, bgWin.length, bgWin.height, 0.f, bgWin.bgCoord);


	return;
}

void WIN_BOTHPLAYERS_draw()
{
	std::cout << "WIN_BOTHPLAYERS:Draw\n";

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
	std::cout << "WIN_BOTHPLAYERS:Free\n";

	return;
}

void WIN_BOTHPLAYERS_unload()
{
	std::cout << "WIN_BOTHPLAYERS:Unload\n";

	// Unload Background
	AEGfxMeshFree(bgWin.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgWin.bgTex); // Unload Texture

	return;
}

// ============================ BOTH PLAYERS LOSE ============================ //

void LOSE_BOTHPLAYERS_load()
{
	std::cout << "LOSE_BOTHPLAYERS:Load\n";

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
	std::cout << "LOSE_BOTHPLAYERS:Initialize\n";

	return;
}

void LOSE_BOTHPLAYERS_update()
{
	std::cout << "LOSE_BOTHPLAYERS:Update\n";

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	// if press back, go replay
	if (AEInputCheckCurr(AEVK_SPACE)) {
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
	std::cout << "LOSE_BOTHPLAYERS:Draw\n";

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
	std::cout << "LOSE_BOTHPLAYERS:Free\n";

	return;
}

void LOSE_BOTHPLAYERS_unload()
{
	std::cout << "LOSE_BOTHPLAYERS:Unload\n";

	// Unload Background
	AEGfxMeshFree(bgWin.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgWin.bgTex); // Unload Texture

	return;
}

// ============================ ENDGAME WIN ============================ //

void ENDGAME_load()
{
	std::cout << "ENDGAME:Load\n";

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
	std::cout << "ENDGAME:Initialize\n";

	return;
}

void ENDGAME_update()
{
	std::cout << "ENDGAME:Update\n";

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	// if press space, go back to mainmenu
	if (AEInputCheckTriggered(AEVK_SPACE)) {
		next_state = MAINMENU;
	}

	/*------------------------------------------------------------
	// MATRIX CALCULATIONS
	------------------------------------------------------------*/
	MatrixCalc(bgWin.transform, bgWin.length, bgWin.height, 0.f, bgWin.bgCoord);


	return;
}

void ENDGAME_draw()
{
	std::cout << "ENDGAME:Draw\n";

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
	std::cout << "ENDGAME:Free\n";

	return;
}

void ENDGAME_unload()
{
	std::cout << "ENDGAME:Unload\n";

	// Unload Background
	AEGfxMeshFree(bgWin.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgWin.bgTex); // Unload Texture

	return;
}
