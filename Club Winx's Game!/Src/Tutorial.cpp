/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: 
*	Shayne Gloria (m.shayne@digipen.edu) ->in game tutorial 45%
* Secondary Authors:
*	Mariah Tahirah (mariahtahirah.b@digipen.edu) -> Main Tutorial 45%
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu) -> editted design 10%
==================================================================================*/

// ---------------------------------------------------------------------------
//includes
#include "General.h"

// ---------------------------------------------------------------------------



// ============================ structs ============================ //
struct game_tut
{
	f32					length{ 425.0f };
	f32					height{ 425.0f };

	AEVec2				coord{};
	AEGfxVertexList*	mesh{ nullptr };
	AEGfxTexture*		texture{ nullptr };
	AEMtx33				transform{};
};

struct Keycap
{
	AEGfxVertexList*	mesh{};
	AEVec2				coords{};
	AEMtx33				transform{};
	f32					size{ 50.f };
	bool				clicked{ false };
	f32					colourbase{};
	f32					colourtxt{};
	AEVec2				screencoords{};
};
Keycap w,a,s,d,up,down,left,right; 

//for flipped immage
static AEMtx33 flipTransform1, flipTransform2;

Textures back_button_tut;
float buffer = 50.0f;
static int mouse_x, mouse_y;



// ============================ helper functions ============================ //v
void KeycapInit(Keycap& w, f32 xcoord, f32 ycoord);
void KeycapUpdate(Keycap& w);
void KeycapDraw(Keycap& w);





// =========================================================================== //
// ============================ GENERAL TUTORIAL ============================= //
// =========================================================================== //

void Tutorial_Load()
{
	/*------------------------------------------------------------
	// LOADING BACKGROUND
	------------------------------------------------------------*/	
	SquareMesh(&bgTut.mesh, 0xFFFF00FF);
	bgTut.texture = AEGfxTextureLoad("Assets/MAIN_MENU_PLAIN_BG.png");
	bgTut.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgTut.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	/*------------------------------------------------------------
	//  Loading players
	------------------------------------------------------------*/
	// player 1 mesh
	SquareMesh(&player1.mesh, 0xFFB62891);
	// player 2 mesh
	SquareMesh(&player2.mesh, 0xFFFF00FF);

	player1.texture = AEGfxTextureLoad("Assets/PLAYER1.png");
	player2.texture = AEGfxTextureLoad("Assets/PLAYER2.png");

	/*------------------------------------------------------------
	// Creating keycap 
	------------------------------------------------------------*/
	SquareMesh(&w.mesh, 0xFFFFFFFF);
	SquareMesh(&a.mesh, 0xFFFFFFFF);
	SquareMesh(&s.mesh, 0xFFFFFFFF);
	SquareMesh(&d.mesh, 0xFFFFFFFF);
	SquareMesh(&up.mesh, 0xFFFFFFFF);
	SquareMesh(&down.mesh, 0xFFFFFFFF);
	SquareMesh(&left.mesh, 0xFFFFFFFF);
	SquareMesh(&right.mesh, 0xFFFFFFFF);
	
	/*------------------------------------------------------------
	// LOAD BUTTON
	------------------------------------------------------------*/
	SquareMesh(&back_button_tut.mesh, 0xFFFFFFFF);
	back_button_tut.texture = AEGfxTextureLoad("Assets/Buttons/BACK_BUTTON.png");

	return;
}

void Tutorial_Init()
{

	/*------------------------------------------------------------
	// Player 
	------------------------------------------------------------*/
	player1.coord = { -200, -50.f };
	player2.coord = { 200.f , -50.f };
	player1.size = 50.0f;
	player2.size = 50.0f;

	AEMtx33Scale(&flipTransform1, 1.0f, 1.0f);
	AEMtx33Scale(&flipTransform2, 1.0f, 1.0f);

	/*------------------------------------------------------------
	// KEYCAP INIT
	------------------------------------------------------------*/
	KeycapInit(w, -400.f, 0.0f);
	KeycapInit(a, -460.f, w.coords.y - a.size - 10.f );
	KeycapInit(s,w.coords.x , a.coords.y);
	KeycapInit(d, w.coords.x + d.size + 10.f, a.coords.y);
	KeycapInit(up, -w.coords.x,w.coords.y);
	KeycapInit(down, up.coords.x, a.coords.y);
	KeycapInit(left, down.coords.x - left.size - 10.f, down.coords.y);
	KeycapInit(right, down.coords.x + right.size + 10.f, down.coords.y);

	/*------------------------------------------------------------
	// BUTTON INIT
	------------------------------------------------------------*/
	back_button_tut.height = 50.0f;
	back_button_tut.length = 200.0f;
	back_button_tut.coord.x = 0.0f;
	back_button_tut.coord.y = -250.0f;

	return;
}

void KeycapInit(Keycap& w, f32 xcoord, f32 ycoord)
{
	w.coords = { xcoord, ycoord };
	w.screencoords = { (w.coords.x - 8.f) / (AEGetWindowWidth() / 2.f) , (w.coords.y - 8.f) / (AEGetWindowHeight() / 2.f) };
}

void Tutorial_Update()
{
	AEInputShowCursor(true);
	/*------------------------------------------------------------
	// UPDATE MOUSE
	------------------------------------------------------------*/
	AEInputGetCursorPosition(&mouse_x, &mouse_y);


	/*------------------------------------------------------------
	// CHANGE STATE
	------------------------------------------------------------*/
	if (checkHovering(mouse_x, mouse_y, back_button_tut.length, back_button_tut.height, back_button_tut.coord.x, back_button_tut.coord.y) && AEInputCheckTriggered(AEVK_LBUTTON))
	{
		next_state = MENU;
	}

	/*------------------------------------------------------------
		KEYCAP UPDATE
	------------------------------------------------------------*/
	if (AEInputCheckCurr(AEVK_W))
		w.clicked = true;
	else
		w.clicked = false;

	if (AEInputCheckCurr(AEVK_A))
		a.clicked = true;
	else
		a.clicked = false;

	if (AEInputCheckCurr(AEVK_S))
		s.clicked = true;
	else
		s.clicked = false;

	if (AEInputCheckCurr(AEVK_D))
		d.clicked = true;
	else
		d.clicked = false;

	if (AEInputCheckCurr(AEVK_UP))
		up.clicked = true;
	else
		up.clicked = false;

	if (AEInputCheckCurr(AEVK_DOWN))
		down.clicked = true;
	else
		down.clicked = false;

	if (AEInputCheckCurr(AEVK_LEFT))
		left.clicked = true;
	else
		left.clicked = false;

	if (AEInputCheckCurr(AEVK_RIGHT))
		right.clicked = true;
	else
		right.clicked = false;

	KeycapUpdate(w);
	KeycapUpdate(a);
	KeycapUpdate(s);
	KeycapUpdate(d);
	KeycapUpdate(up);
	KeycapUpdate(down);
	KeycapUpdate(left);
	KeycapUpdate(right);
	
	/*------------------------------------------------------------
		PLAYER UPDATE
	------------------------------------------------------------*/
	input_handle();

	player1.coord.x += player1.velocity.x * player1.acceleration * g_dt;
	player1.coord.y += player1.velocity.y * player1.acceleration * g_dt;

	player2.coord.x += player2.velocity.x * player2.acceleration * g_dt;
	player2.coord.y += player2.velocity.y * player2.acceleration * g_dt;


	//texture flipping 
	if (AEInputCheckTriggered(AEVK_A))
	{
		AEMtx33Scale(&flipTransform1, -1.0f, 1.0f); // player 1 flip
	}
	if (AEInputCheckTriggered(AEVK_D))
	{
		AEMtx33Scale(&flipTransform1, 1.0f, 1.0f); // player 1 normal
	}
	if (AEInputCheckTriggered(AEVK_LEFT))
	{
		AEMtx33Scale(&flipTransform2, -1.0f, 1.0f); // player 2 flip
	}
	if (AEInputCheckTriggered(AEVK_RIGHT))
	{
		AEMtx33Scale(&flipTransform2, 1.0f, 1.0f); // player 1 normal
	}


	/*------------------------------------------------------------
	// MATRIX CALCULATIONS
	------------------------------------------------------------*/

	// for players 
	MatrixCalc(player1.transform, player1.size, player1.size, 0.0f, player1.coord);
	MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.coord);

	//for keycaps 
	MatrixCalc(w.transform, w.size, w.size, 0.0f, w.coords);
	MatrixCalc(a.transform, a.size, a.size, 0.0f, a.coords);
	MatrixCalc(s.transform, s.size, s.size, 0.0f, s.coords);
	MatrixCalc(d.transform, d.size, d.size, 0.0f, d.coords);
	MatrixCalc(up.transform, up.size, up.size, 0.0f, up.coords);
	MatrixCalc(down.transform, down.size, down.size, 0.0f, down.coords);
	MatrixCalc(left.transform, left.size, left.size, 0.0f, left.coords);
	MatrixCalc(right.transform, right.size, right.size, 0.0f, right.coords);

	//for background
	MatrixCalc(bgTut.transform, bgTut.length, bgTut.height, 0.f, bgTut.coord);

	//button
	MatrixCalc(back_button_tut.transform, back_button_tut.length, back_button_tut.height, 0.0f, back_button_tut.coord);

	return;
}

void KeycapUpdate(Keycap& w)
{
	if (w.clicked)
	{
		w.colourbase = 0.f;
		w.colourtxt = 1.f;
	}
	else
	{
		w.colourbase = 1.f;
		w.colourtxt = 0.f;
	}
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
	AEGfxTextureSet(bgTut.texture, 0.f, 0.f);
	AEGfxMeshDraw(bgTut.mesh, AE_GFX_MDM_TRIANGLES);


	/*------------------------------------------------------------
	// DRAWING PLAYERS
	------------------------------------------------------------*/
	// Drawing player 1
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEMtx33Concat(&player1.transform, &player1.transform, &flipTransform1);
	AEGfxSetTransform(player1.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(player1.texture, 0, 0);
	AEGfxMeshDraw(player1.mesh, AE_GFX_MDM_TRIANGLES);

	// drawing player 2
	AEMtx33Concat(&player2.transform, &player2.transform, &flipTransform2);
	AEGfxSetTransform(player2.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(player2.texture, 0, 0);
	AEGfxMeshDraw(player2.mesh, AE_GFX_MDM_TRIANGLES);


	/*------------------------------------------------------------
	// DRAWING Keycaps
	------------------------------------------------------------*/
	KeycapDraw(w);
	KeycapDraw(a);
	KeycapDraw(s);
	KeycapDraw(d);
	KeycapDraw(up);
	KeycapDraw(down);
	KeycapDraw(left);
	KeycapDraw(right);
	/*------------------------------------------------------------
	// DRAWING TEXT
	------------------------------------------------------------*/
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	char strBuffer[300];

	sprintf_s(strBuffer, "You and your friend");
	AEGfxPrint(font_pixel, strBuffer, -0.4f, 0.85f, 1.0f, 1.0f, 1.0f, 1.0f);
	
	sprintf_s(strBuffer, "have been kidnapped by aliens");
	AEGfxPrint(font_pixel, strBuffer, -0.6f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f);

	sprintf_s(strBuffer, "Learn how to escape and get home!");
	AEGfxPrint(font_pixel, strBuffer, -0.66f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f);

	sprintf_s(strBuffer, "W");
	AEGfxPrint(font_pixel, strBuffer, w.screencoords.x, w.screencoords.y, 0.75f, w.colourtxt, w.colourtxt, w.colourtxt);
	sprintf_s(strBuffer, "A");
	AEGfxPrint(font_pixel, strBuffer, a.screencoords.x, a.screencoords.y, 0.75f, a.colourtxt, a.colourtxt, a.colourtxt);
	sprintf_s(strBuffer, "S");
	AEGfxPrint(font_pixel, strBuffer, s.screencoords.x, s.screencoords.y, 0.75f, s.colourtxt, s.colourtxt, s.colourtxt);
	sprintf_s(strBuffer, "D");
	AEGfxPrint(font_pixel, strBuffer, d.screencoords.x, d.screencoords.y, 0.75f, d.colourtxt, d.colourtxt, d.colourtxt);
	sprintf_s(strBuffer, "/\\");
	AEGfxPrint(font_pixel, strBuffer, up.screencoords.x, up.screencoords.y, 0.40f, up.colourtxt, up.colourtxt, up.colourtxt);
	sprintf_s(strBuffer, "\\/");
	AEGfxPrint(font_pixel, strBuffer, down.screencoords.x, down.screencoords.y, 0.40f, down.colourtxt, down.colourtxt, down.colourtxt);
	sprintf_s(strBuffer, "<");
	AEGfxPrint(font_pixel, strBuffer, left.screencoords.x, left.screencoords.y, 0.75f, left.colourtxt, left.colourtxt, left.colourtxt);
	sprintf_s(strBuffer, ">");
	AEGfxPrint(font_pixel, strBuffer, right.screencoords.x, right.screencoords.y, 0.75f, right.colourtxt, right.colourtxt, right.colourtxt);
	
	sprintf_s(strBuffer, "PLAYER 1");
	AEGfxPrint(font_pixel, strBuffer, -0.5f, 0.2f, 0.75f, 1.0f, 1.0f, 1.0f);
	sprintf_s(strBuffer, "PLAYER 2");
	AEGfxPrint(font_pixel, strBuffer, 0.20f, 0.2f, 0.75f, 1.0f, 1.0f, 1.0f);

	sprintf_s(strBuffer, "Press the ESC button if you need a break.");
	AEGfxPrint(font_pixel, strBuffer, -0.65f, -0.45f, 0.75f, 1.0f, 1.0f, 1.0f);

	/*------------------------------------------------------------
	// DRAWING MINI TUT
	------------------------------------------------------------*/
	draw_texture(back_button_tut);

	return;
}

void KeycapDraw(Keycap& w) {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(w.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(0.75f);
	AEGfxSetTintColor(w.colourbase, w.colourbase, w.colourbase, 1.0f);
	AEGfxMeshDraw(w.mesh, AE_GFX_MDM_TRIANGLES);
}

void Tutorial_Free()
{
	/*------------------------------------------------------------
	// Free Player Meshes
	------------------------------------------------------------*/
	AEGfxMeshFree(player1.mesh);
	AEGfxMeshFree(player2.mesh);

	/*------------------------------------------------------------
	// Free Keycap Meshes
	------------------------------------------------------------*/
	AEGfxMeshFree(w.mesh);
	AEGfxMeshFree(a.mesh);
	AEGfxMeshFree(s.mesh);
	AEGfxMeshFree(d.mesh);
	AEGfxMeshFree(up.mesh);
	AEGfxMeshFree(down.mesh);
	AEGfxMeshFree(left.mesh);
	AEGfxMeshFree(right.mesh);


	/*------------------------------------------------------------
	// Free button
	------------------------------------------------------------*/
	AEGfxMeshFree(back_button_tut.mesh);

	return;
}

void Tutorial_Unload()
{
	/*------------------------------------------------------------
	// UNLOADING BACKGROUND
	------------------------------------------------------------*/
	AEGfxMeshFree(bgTut.mesh);
	AEGfxTextureUnload(bgTut.texture);

	/*------------------------------------------------------------
	// Unload Player Meshes
	------------------------------------------------------------*/
	AEGfxTextureUnload(player1.texture);
	AEGfxTextureUnload(player2.texture);

	/*------------------------------------------------------------
	// Unload button
	------------------------------------------------------------*/
	AEGfxTextureUnload(back_button_tut.texture);

	return;
}





// =========================================================================== //
// ============================ IN GAME TUTORIAL ============================= //
// =========================================================================== //

game_tut TUT_game;


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
		TUT_game.texture = AEGfxTextureLoad("Assets/TUTORIAL_PUZZLE.png");
		break;
	case RACING:
		TUT_game.texture = AEGfxTextureLoad("Assets/TUTORIAL_RACING.png");
		break;
	case BOSS:
		TUT_game.texture = AEGfxTextureLoad("Assets/TUTORIAL_BOSS.png");
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
		AEGfxPrint(font_pixel, strBuffer, -0.315f, -0.675f, 0.75f, 0.0f, 0.0f, 0.0f);
	}
	else 	AEGfxPrint(font_pixel, strBuffer, -0.315f, -0.5f, 0.75f, 0.0f, 0.0f, 0.0f);

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

