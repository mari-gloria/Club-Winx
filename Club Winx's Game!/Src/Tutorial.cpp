
// Include these Header files
#include "General.h"

static AEVec2 title, description;

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