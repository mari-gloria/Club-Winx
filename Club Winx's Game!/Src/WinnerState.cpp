/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This source file defines the functions for the win/lose screens after each game.
* 
* Primary Author: Shayne Gloria (m.shayne@digipen.edu)
==================================================================================*/

// ---------------------------------------------------------------------------
//includes
#include "General.h"

// ---------------------------------------------------------------------------





// ============================ variables ============================ //

// Camera Movement Variables
static f32			CamX{ 0.0f }, CamY{ 0.0f };

// Background movement
static float v = 0, w = 0, bgspeed = 0.05f;

// Text positions
static AEVec2 state = { -0.2f, 0.2f }, instruction = { -0.5f, -0.2f };
static AEVec2 state_end = { -0.275f, 0.2f }, instruction_end = { -0.5f, -0.2f };





// =========================================================================== //
// ============================ BOTH PLAYERS WIN ============================= //
// =========================================================================== //

void WIN_BOTHPLAYERS_load()
{
	//load BG
	SquareMesh(&bgWin.mesh, 0xFFFF00FF);
	bgWin.texture = AEGfxTextureLoad("Assets/WINLOSE_BG.png");
	

	//load audio
	win.audio = AEAudioLoadSound("Assets/Audio/win.wav");
	win.aGroup = AEAudioCreateGroup();

	return;
}


void WIN_BOTHPLAYERS_init()
{
	//init BG
	bgWin.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgWin.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	//play audio
	AEAudioPlay(win.audio, win.aGroup, 0.75f, 0.9f, 1);

	return;
}


void WIN_BOTHPLAYERS_update()
{
	//change state consitions
	// if press space, go to the next game
	if (AEInputCheckReleased(AEVK_SPACE)) {
		if (arcadeMode)
		{
			next_state = prev_state;
		}
		else {
			next_state = prev_state == RACING ? BOSS : (PUZZLE ? RACING : MENU);
		}

	}

	//matrix calculations
	MatrixCalc(bgWin.transform, bgWin.length, bgWin.height, 0.f, bgWin.coord);

	return;
}

void WIN_BOTHPLAYERS_draw()
{
	//draw BG
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransform(bgWin.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	v += bgspeed * g_dt;
	if (v >= 1)
	{
		v = 0;
	}

	AEGfxTextureSet(bgWin.texture, v, 0.f);
	AEGfxMeshDraw(bgWin.mesh, AE_GFX_MDM_TRIANGLES);

	//draw text
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	char strBuffer[300];

	sprintf_s(strBuffer, "You Win!");
	AEGfxPrint(font_pixel, strBuffer, state.x, state.y, 1.25f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "Press space to continue");
	AEGfxPrint(font_pixel, strBuffer, instruction.x, instruction.y, 1.0f, 1.0f, 1.0f, 1.0f);

	return;
}

void WIN_BOTHPLAYERS_free()
{
	//free mesh
	AEGfxMeshFree(bgWin.mesh); 
	return;
}

void WIN_BOTHPLAYERS_unload()
{	
	//unload textures
	AEGfxTextureUnload(bgWin.texture);

	//exit audio
	AEAudioStopGroup(win.aGroup);

	return;
}


// =========================================================================== //
// ============================ BOTH PLAYERS LOSE ============================ //
// =========================================================================== //

void LOSE_BOTHPLAYERS_load()
{
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	SquareMesh(&bgWin.mesh, 0xFFFF00FF);
	bgWin.texture = AEGfxTextureLoad("Assets/WINLOSE_BG.png");
	

	/*------------------------------------------------------------
	LOAD SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	lose.audio = AEAudioLoadSound("Assets/Audio/lose.wav");
	lose.aGroup = AEAudioCreateGroup();

	return;
}

void LOSE_BOTHPLAYERS_init()
{
	/*------------------------------------------------------------
	INIT BACKGROUND
	------------------------------------------------------------*/
	bgWin.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgWin.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	/*------------------------------------------------------------
	// PLAY AUDIO
	------------------------------------------------------------*/
	AEAudioPlay(lose.audio, lose.aGroup, 0.75f, 0.9f, 1);

	return;
}

void LOSE_BOTHPLAYERS_update()
{
	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	// if press back, go replay
	if (AEInputCheckReleased(AEVK_SPACE)) {
		next_state = prev_state;
	}

	/*------------------------------------------------------------
	// MATRIX CALCULATIONS
	------------------------------------------------------------*/
	MatrixCalc(bgWin.transform, bgWin.length, bgWin.height, 0.f, bgWin.coord);

	return;
}

void LOSE_BOTHPLAYERS_draw()
{
	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransform(bgWin.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	v += bgspeed * g_dt;
	if (v >= 1)
	{
		v = 0;
	}

	AEGfxTextureSet(bgWin.texture, v, 0.f);
	AEGfxMeshDraw(bgWin.mesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	DRAWING TEXT
	------------------------------------------------------------*/
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	char strBuffer[300];

	// general tutorial 
	sprintf_s(strBuffer, "You Lose!");
	AEGfxPrint(font_pixel, strBuffer, state.x, state.y, 1.25f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "Press space to continue");
	AEGfxPrint(font_pixel, strBuffer, instruction.x, instruction.y, 1.0f, 1.0f, 1.0f, 1.0f);

	return;
}

void LOSE_BOTHPLAYERS_free()
{
	// Free Mesh
	AEGfxMeshFree(bgWin.mesh);

	return;
}

void LOSE_BOTHPLAYERS_unload()
{

	// Unload Texture
	AEGfxTextureUnload(bgWin.texture);

	// Exit Audio
	AEAudioStopGroup(lose.aGroup);


	return;
}

// ============================ ENDGAME WIN ============================ //

void ENDGAME_load()
{
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	SquareMesh(&bgWin.mesh, 0xFFFF00FF);
	bgWin.texture = AEGfxTextureLoad("Assets/WINLOSE_BG.png");

	/*------------------------------------------------------------
	LOAD SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	win.audio = AEAudioLoadSound("Assets/Audio/win.wav");
	win.aGroup = AEAudioCreateGroup();

	return;
}

void ENDGAME_init()
{
	/*------------------------------------------------------------
	// INIT BACKGROUND
	------------------------------------------------------------*/
	bgWin.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgWin.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	/*------------------------------------------------------------
	// PLAY AUDIO
	------------------------------------------------------------*/
	AEAudioPlay(win.audio, win.aGroup, 0.75f, 0.9f, 1);

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
	MatrixCalc(bgWin.transform, bgWin.length, bgWin.height, 0.f, bgWin.coord);


	return;
}

void ENDGAME_draw()
{
	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransform(bgWin.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	v += bgspeed * g_dt;
	if (v >= 1)
	{
		v = 0;
	}

	AEGfxTextureSet(bgWin.texture, v, 0.f);
	AEGfxMeshDraw(bgWin.mesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	DRAWING TEXT
	------------------------------------------------------------*/
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	char strBuffer[300];

	// general tutorial 
	sprintf_s(strBuffer, "You Escaped!");
	AEGfxPrint(font_pixel, strBuffer, state_end.x, state_end.y, 1.25f, 0.5f, 0.5f, 1.0f);
	sprintf_s(strBuffer, "Press space to go to menu");
	AEGfxPrint(font_pixel, strBuffer, instruction_end.x, instruction_end.y, 1.0f, 1.0f, 1.0f, 1.0f);


	return;
}

void ENDGAME_free()
{
	//free mesh
	AEGfxMeshFree(bgWin.mesh);
	return;
}

void ENDGAME_unload()
{
	// Unload texture	
	AEGfxTextureUnload(bgWin.texture);

	// Exit Audio
	AEAudioStopGroup(win.aGroup);

	return;
}
