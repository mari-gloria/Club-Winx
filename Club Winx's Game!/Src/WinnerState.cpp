// ================================================================================================================ //
// ======================================== FUNCTIONS FOR DIFFERENT WIN STATE ===================================== //
// ================================================================================================================ //

// Include these Header files
#include "General.h"

// Camera Movement Variables
static f32			CamX{ 0.0f },
CamY{ 0.0f };	// Camera's X & Y Positions

// Background movement
static float v = 0, w = 0, bgspeed = 0.05;

// Text
static AEVec2 state = {-0.25f, 0.2f}, instruction = {-0.45f, -0.2f};



// ============================ PLAYERONE WIN ============================ //

void WIN_PLAYERONE_load()
{

	//AEGfxSetBackgroundColor(255.0f, 255.0f, 248.0f);
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	SquareMesh(&bgWin.bgMesh, 0xFFFF00FF);
	bgWin.bgTex = AEGfxTextureLoad("Assets/win_lose_bg.png");
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
	if (AEInputCheckReleased(AEVK_SPACE) && prev_state == RACING) {
		next_state = BOSS;
	}

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

	v += bgspeed * g_dt;
	if (v >= 1)
	{
		v = 0;
	}

	AEGfxTextureSet(bgWin.bgTex, v, 0.f);
	AEGfxMeshDraw(bgWin.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	DRAWING TEXT
	------------------------------------------------------------*/
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	char strBuffer[300];

	// general tutorial 
	sprintf_s(strBuffer, "Player 1 Win's!");
	AEGfxPrint(text, strBuffer, state.x, state.y, 1.25f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "Press space to continue");
	AEGfxPrint(text, strBuffer, instruction.x, instruction.y, 1.0f, 1.0f, 1.0f, 1.0f);


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
	bgWin.bgTex = AEGfxTextureLoad("Assets/win_lose_bg.png");
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
	if (AEInputCheckReleased(AEVK_SPACE) && prev_state == RACING) {
		next_state = BOSS;
	}
	//if (AEInputCheckTriggered(AEVK_SPACE) && prev_state == PUZZLE) {
	//	next_state = BOSS;
	//}
	if (AEInputCheckReleased(AEVK_SPACE) && prev_state == BOSS) {
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
	v += bgspeed * g_dt;
	if (v >= 1)
	{
		v = 0;
	}

	AEGfxTextureSet(bgWin.bgTex, v, 0.f);
	AEGfxMeshDraw(bgWin.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	DRAWING TEXT
	------------------------------------------------------------*/
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	char strBuffer[300];

	// general tutorial 
	sprintf_s(strBuffer, "Player 2 Win's!");
	AEGfxPrint(text, strBuffer, state.x, state.y, 1.25f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "Press space to continue");
	AEGfxPrint(text, strBuffer, instruction.x, instruction.y, 1.0f, 1.0f, 1.0f, 1.0f);


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
	bgWin.bgTex = AEGfxTextureLoad("Assets/win_lose_bg.png");
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
	if (AEInputCheckReleased(AEVK_SPACE)) {
		
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
	v += bgspeed * g_dt;
	if (v >= 1)
	{
		v = 0;
	}

	AEGfxTextureSet(bgWin.bgTex, v, 0.f);
	AEGfxMeshDraw(bgWin.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	DRAWING TEXT
	------------------------------------------------------------*/
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	char strBuffer[300];

	// general tutorial 
	sprintf_s(strBuffer, "You Win!");
	AEGfxPrint(text, strBuffer, state.x, state.y, 1.25f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "Press space to continue");
	AEGfxPrint(text, strBuffer, instruction.x, instruction.y, 1.0f, 1.0f, 1.0f, 1.0f);



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
	bgWin.bgTex = AEGfxTextureLoad("Assets/win_lose_bg.png");
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

	if (AEInputCheckReleased(AEVK_SPACE)) {
		switch (prev_state) {
		case RACING:
			next_state = RACING;
			break;
		case PUZZLE:
			next_state = PUZZLE;
			break;
		case BOSS:
			next_state = BOSS;
		}

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
	v += bgspeed * g_dt;
	if (v >= 1)
	{
		v = 0;
	}

	AEGfxTextureSet(bgWin.bgTex, v, 0.f);
	AEGfxMeshDraw(bgWin.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	DRAWING TEXT
	------------------------------------------------------------*/
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	char strBuffer[300];
	
	// general tutorial 
	sprintf_s(strBuffer, "You Lose!");
	AEGfxPrint(text, strBuffer, state.x, state.y, 1.25f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "Press space to continue");
	AEGfxPrint(text, strBuffer, instruction.x, instruction.y, 1.0f, 1.0f, 1.0f, 1.0f);

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
	bgWin.bgTex = AEGfxTextureLoad("Assets/win_lose_bg.png");
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
	v += bgspeed * g_dt;
	if (v >= 1)
	{
		v = 0;
	}

	AEGfxTextureSet(bgWin.bgTex, v, 0.f);
	AEGfxMeshDraw(bgWin.bgMesh, AE_GFX_MDM_TRIANGLES);
	
	/*------------------------------------------------------------
	DRAWING TEXT
	------------------------------------------------------------*/
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	char strBuffer[300];

	// general tutorial 
	sprintf_s(strBuffer, "You Escaped!");
	AEGfxPrint(text, strBuffer, state.x, state.y, 1.25f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "Press space to go to menu");
	AEGfxPrint(text, strBuffer, instruction.x, instruction.y, 1.0f, 1.0f, 1.0f, 1.0f);


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
