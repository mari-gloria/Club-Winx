
// Include these Header files
#include "General.h"

// Variables
static AEVec2 title, description;


// General Tutorial
void Tutorial_Load()
{
	/*------------------------------------------------------------
	// LOADING BACKGROUND
	------------------------------------------------------------*/	
	SquareMesh(&bgTut.bgMesh, 0xFFFF00FF);
	bgTut.bgTex = AEGfxTextureLoad("Assets/MAIN_MENU_PLAIN_BG.png");
	bgTut.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgTut.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	/*------------------------------------------------------------
	// 
	------------------------------------------------------------*/
	
	return;
}

void Tutorial_Init()
{

	/*------------------------------------------------------------
	// FONT POSITIONS
	------------------------------------------------------------*/

	title.x = -0.2f;
	title.y = 0.75f;

	description.x = -0.8f;
	description.y = 0.7f;
	
	return;
}

void Tutorial_Update()
{
	/*------------------------------------------------------------
	// CHANGE STATES
	------------------------------------------------------------*/
	if (AEInputCheckTriggered(AEVK_LBUTTON) && curr_state == TUT) {
		next_state = prev_state;
	}


	/*------------------------------------------------------------
	// MATRIX CALCULATIONS
	------------------------------------------------------------*/
	MatrixCalc(bgTut.transform, bgTut.length, bgTut.height, 0.f, bgTut.bgCoord);

	return;
}

void Tutorial_Draw()
{
	
	/*------------------------------------------------------------
	// DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransform(bgTut.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(bgTut.bgTex, 0.f, 0.f);
	AEGfxMeshDraw(bgTut.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	// DRAWING TEXT
	------------------------------------------------------------*/
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	char strBuffer[300];

	// general tutorial 
	sprintf_s(strBuffer, "GENERAL");
	AEGfxPrint(text, strBuffer, title.x, title.y, 1.0f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "PLAYER 1: WASD");
	AEGfxPrint(text, strBuffer, description.x, description.y, 0.75f, 1.0f, 1.0f, 1.0f);
	sprintf_s(strBuffer, "PLAYER 2: ARROW KEYS");
	AEGfxPrint(text, strBuffer, description.x, description.y - 0.15f, 0.75f, 1.0f, 1.0f, 1.0f);

	// racing tutorial
	sprintf_s(strBuffer, "RACING");
	AEGfxPrint(text, strBuffer, title.x, title.y - 0.4f, 1.0f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "JUMP FAST TO ESCAPE THE RISING WATER");
	AEGfxPrint(text, strBuffer, description.x, description.y - 0.45f, 0.75f, 1.0f, 1.0f, 1.0f);
	sprintf_s(strBuffer, "FIRST ONE TO THE TOP WINS");
	AEGfxPrint(text, strBuffer, description.x, description.y - 0.55f, 0.75f, 1.0f, 1.0f, 1.0f);

	//puzzle tutorial
	sprintf_s(strBuffer, "PUZZLE");
	AEGfxPrint(text, strBuffer, title.x, title.y - 0.8f, 1.0f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "NAVIGATE THROUGH A MAZE TO ESCAPE");
	AEGfxPrint(text, strBuffer, description.x, description.y - 0.85f, 0.75f, 1.0f, 1.0f, 1.0f);
	sprintf_s(strBuffer, "BE WARY OF OBSTACLES");
	AEGfxPrint(text, strBuffer, description.x, description.y - 0.95f, 0.75f, 1.0f, 1.0f, 1.0f);

	// boss tutorial
	sprintf_s(strBuffer, "BOSS GAME");
	AEGfxPrint(text, strBuffer, title.x, title.y - 1.2f, 1.0f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "DEFEAT THE BOSS TOGETHER");
	AEGfxPrint(text, strBuffer, description.x, description.y - 1.25f, 0.75f, 1.0f, 1.0f, 1.0f);
	sprintf_s(strBuffer, "BE WARY OF OBSTACLES");
	AEGfxPrint(text, strBuffer, description.x, description.y - 1.35f, 0.75f, 1.0f, 1.0f, 1.0f);


	return;
}

void Tutorial_Free()
{
	return;
}

void Tutorial_Unload()
{
	/*------------------------------------------------------------
	// UNLOADING BACKGROUND
	------------------------------------------------------------*/
	AEGfxMeshFree(bgTut.bgMesh);
	AEGfxTextureUnload(bgTut.bgTex);

	return;
}






/**************************************************************
// IN GAME TUTORIAL
**************************************************************/

// Variables
struct game_tut
{
	f32 length{ 425.0f };
	f32 height{ 425.0f };

	AEVec2 coord;
	AEGfxVertexList* mesh{ nullptr };
	AEGfxTexture* texture{ nullptr };
	AEMtx33	transform{};
};

game_tut TUT_game;

static int mouse_x, mouse_y;
bool tut_viewed = false;

// Individual Game Tutorial
void GameTutorial_Load()
{

	// tutorial game mesh
	SquareMesh(&TUT_game.mesh, 0);

	// load tutorial texture according to what state it is & if tut has been viewed or not
	switch (curr_state)
	{
	case PUZZLE:
		TUT_game.texture = AEGfxTextureLoad("Assets/TUT_puzzle.png");
		break;
	case RACING:
		TUT_game.texture = AEGfxTextureLoad("Assets/TUT_racing.png");
		break;
	case BOSS:
		TUT_game.texture = AEGfxTextureLoad("Assets/TUT_boss.png");
		break;
	}


	return;
}

void GameTutorial_Init(f32 CamX, f32 CamY)
{
	if (curr_state == RACING)
	{
		TUT_game.coord.x = CamX;
		TUT_game.coord.y = CamY - 100.0f;
	}
	else {
		TUT_game.coord.x = CamX;
		TUT_game.coord.y = CamY;
		AEGfxSetCamPosition(CamX, CamY);
	}

	return;
}

void GameTutorial_Update()
{
	//update mouse coord
	AEInputGetCursorPosition(&mouse_x, &mouse_y);

	// if player clicks on tutorial page, tutorial continue
	// when you load in a game the tut will 
	if (AEInputCheckReleased(AEVK_SPACE))
	{
		tut_viewed = true;
	}

	//update matrix
	MatrixCalc(TUT_game.transform, TUT_game.length, TUT_game.height, 0.0f, TUT_game.coord);


	return;
}

void GameTutorial_Draw()
{

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransform(TUT_game.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(TUT_game.texture, 0, 0);
	AEGfxMeshDraw(TUT_game.mesh, AE_GFX_MDM_TRIANGLES);

	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));

	sprintf_s(strBuffer, "press SPACE to start");
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	if (curr_state == BOSS) {
		AEGfxPrint(text, strBuffer, -0.315f, -0.675f, 0.75f, 0.0f, 0.0f, 0.0f);
	}
	else 	AEGfxPrint(text, strBuffer, -0.315f, -0.5f, 0.75f, 0.0f, 0.0f, 0.0f);

	return;
}

void GameTutorial_Free()
{
	// free mesh
	AEGfxMeshFree(TUT_game.mesh);
	return;
}

void GameTutorial_Unload()
{
	// free textures
	AEGfxTextureUnload(TUT_game.texture);
	return;
}

