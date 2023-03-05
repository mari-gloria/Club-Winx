/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Mariah Tahirah (mariahtahirah.b@digipen.edu)
* Secondary Authors:
*	Shayne Gloria (m.shayne@digipen.edu) -> Platforms & Split Screen (Load, Init, Draw, Unload)
*   Yeo Hui Shan (huishan.y@digipen.edu) -> Collision 
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "General.h"

// ---------------------------------------------------------------------------


/*------------------------------------------------------------
defines
------------------------------------------------------------*/

// Camera Movement Variables
static f32			CamX{ 0.0f },
CamY{ 0.0f };	// Camera's X & Y Positions

//// 
static f32			maxHeight		{ 0 };
static int			count			{ 1 };
static f32			maxHeight_copy	{ 0 };
static const f32	H				{ 220.0f };
static bool			firstround_over{ false };

///
static int			if_win{ 0 } ; // 0: None, 1: Player_one, 2: Player_two



/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void racing_load()
{
	std::cout << "racing:Load\n";

	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	bgRacing.bgTex = AEGfxTextureLoad("Assets/Racing_BG.png");		// BG Texture
	SquareMesh(&bgRacing.bgMesh, 0);							// BG Mesh
	bgRacing.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgRacing.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();



	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/

	// Informing the library that we're about to start adding triangles

	// player 1 mesh 
	SquareMesh(&player1.pMesh,  0xFFB62891);

	// player 2 mesh
	SquareMesh(&player2.pMesh,0xFFFF00FF);

	// score board mesh
	SquareMesh(&score_board.sMesh, 0xFF000000);

	// loading in platform meshes in map
	racing_map_load();

	// loading in line
	splitscreen_load();

	// loading win texture
	SquareMesh(&winRacing.bgMesh, 0xFFFF00FF);


	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/
	player1.pTex = AEGfxTextureLoad("Assets/Player1.png");
	player2.pTex = AEGfxTextureLoad("Assets/Player2.png");
	winRacing.bgTex = AEGfxTextureLoad("Assets/Racing_Winner.png");

	/*------------------------------------------------------------
	CREATING FONTS
	------------------------------------------------------------*/


	return;
}

void racing_init()
{
	std::cout << "racing:Initialize\n";

	/*------------------------------------------------------------
	// INIT PLAYERS
	------------------------------------------------------------*/
	player1.pGround = AEGfxGetWinMinY() + player1.size / 2.f;
	player1.pCurrGround = player1.pGround;
	player2.pGround = AEGfxGetWinMinY() + player2.size / 2.f;
	player2.pCurrGround = player2.pGround;

	player1.pCoord = { AEGfxGetWinMinX() / 2, player1.pGround }; //spawn at left half of screen
	player2.pCoord = { AEGfxGetWinMaxX() / 2, player2.pGround }; //spawn at right half of screen



	/*------------------------------------------------------------
	// INIT SCOREBOARD
	------------------------------------------------------------*/
	score_board.sCoord.x = 0.0f;
	score_board.sCoord.y = 150.0f;



	/*------------------------------------------------------------
	// INIT PLATFORM - MAP
	------------------------------------------------------------*/
	// declaring values for start and end values for player1 & 2 - which is needed for racing map
	player1.startX	= AEGfxGetWinMinX();
	player1.endX	= - 0.0f;

	//player2.start = (AEGfxGetWinMinX() + AEGfxGetWinMaxX()) / 2;
	player2.startX	= 0.0f;
	player2.endX	= AEGfxGetWinMaxX();

	// calling racing map initialisation fucntion, using values from previous lines
	racing_map_init(player1.startX, player1.endX, 1);
	racing_map_init(player2.startX, player2.endX, 2);



	/*------------------------------------------------------------
	// INIT SPLITSCREEN
	------------------------------------------------------------*/
	splitscreen_init();



	/*------------------------------------------------------------
	// INIT - Camera Movement
	------------------------------------------------------------*/
	CamX = 0.0f, CamY = 0.0f;
	maxHeight = H;
	maxHeight_copy = H;
	count = 1;
	firstround_over = false;


	/*------------------------------------------------------------
	// INIT - Racing Win Texture
	------------------------------------------------------------*/
	// winRacing.bgCoord = {player}
	winRacing.length = 120.0f;
	winRacing.height = 70.0f;
	if_win = 0;

	return;
}

void racing_update()
{
	std::cout << "racing:Update\n";

	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	if (AEInputCheckCurr(AEVK_1)) {
		next_state = PUZZLE;
	}
	if (AEInputCheckCurr(AEVK_3)) {
		next_state = BOSS;
	}
	if (AEInputCheckCurr(AEVK_Q)) {
		next_state = QUIT;
	}
	if (AEInputCheckCurr(AEVK_BACK)) {
		next_state = RESTART;
	}
	if (AEInputCheckCurr(AEVK_ESCAPE)) {
		next_state = MENU;
	}


	/*------------------------------------------------------------
	// PLAYER MOVEMENT
	------------------------------------------------------------*/
	input_handle();


	//checking for collision
	for (int i = 0; i < MAX_NUM_PLATFORMS; i++)
	{
		//player 1
		if (CollisionIntersection_RectRect(player1.pCoord, player1.size, player1.size,
			platformA[i].platVect, platformA[i].length, platformA[i].height) == true) {

			player1.pPrevGround = player1.pCurrGround;
			player1.pCurrGround = platformA[i].platVect.y + platformA[i].height / 2.0f + player1.size / 2.0f;
			player1.maxCurrHeight = platformA[i + 1].platVect.y - platformA[i + 1].height / 2.0f - player1.size / 2.0f + 10.0f;

			player1.pCoord.y = player1.pCurrGround;

			player1.pOnGround = true;

			// if collision on last platform and other player hasn't won
			if ((if_win != 2) && i == (MAX_NUM_PLATFORMS - 1))
			{
				if_win = 1;
				Racing_Win(true, 1);
				MatrixCalc(winRacing.transform, winRacing.length, winRacing.height, 0.f, winRacing.bgCoord);
			}

		}


		//player 2
		if (CollisionIntersection_RectRect(player2.pCoord, player2.size, player2.size,
			platformB[i].platVect, platformB[i].length, platformB[i].height) == true) {

			player2.pPrevGround = player2.pCurrGround;
			player2.pCurrGround = platformB[i].platVect.y + platformB[i].height / 2.0f + player2.size / 2.0f;
			//player2.maxCurrHeight = platformB[i].platVect.y - platformB[i].height / 2.0f - player2.size / 2.0f;

			player2.pCoord.y = player2.pCurrGround;

			player2.pOnGround = true;

			// if collision on last platform, and other player hasn't won
			if ((if_win != 1) && i == (MAX_NUM_PLATFORMS - 1))
			{
				if_win = 2;
				Racing_Win(true, 2);
				MatrixCalc(winRacing.transform, winRacing.length, winRacing.height, 0.f, winRacing.bgCoord);
			}

		}
	}


	/*for (int i = 0; i < platform_max; i++)
	{
		if (CollisionIntersection_RectRect(player1.pCoord, player1.size, player1.size, 
											platformA[i].platVect, platformA[i].length, platformA[i].height) == true) {

			//std::cout << "test test test" << std::endl;
			platformA[i].stepped = true;
			//player1.stepping = true;

		}
		else
		{
			platformA[i].stepped = false;
			//player1.stepping = false;
		}

		if (platformA[i].stepped)
		{
			player1.pCoord.y	= platformA[i].platVect.y + platformA[i].height / 2.f + player1.size / 2.f;
			player1.pCurrGround = platformA[i].platVect.y + platformA[i].height / 2.f + player1.size / 2.f;

			player1.stepping= true;
		}
		else
		{
			//player1.stepping = false;
			//player1.pCurrGround = player1.pGround;
		}

		/// PLAYER 2 PLATFORM COLLSIION 
		if (CollisionIntersection_RectRect(player2.pCoord, player2.size, player2.size, 
											platformB[i].platVect, platformB[i].length, platformB[i].height) == true) {

			//std::cout << "test test test" << std::endl;
			platformB[i].stepped = true;
			//player1.stepping = true;

		}
		else
		{
			platformB[i].stepped = false;
			//player1.stepping = false;
		}

		if (platformB[i].stepped)
		{
			player2.pCoord.y	= platformB[i].platVect.y + platformB[i].height / 2.f + player2.size / 2.f;
			player2.pCurrGround = platformB[i].platVect.y + platformB[i].height / 2.f + player2.size / 2.f;

			player2.stepping = true;
		}
		else
		{
			//player1.stepping = false;
			//player1.pCurrGround = player1.pGround;
		}

	}*/



	/*------------------------------------------------------------
	MATRIX CALCULATION
	------------------------------------------------------------*/
	// for background
	MatrixCalc(bgRacing.transform, bgRacing.length, bgRacing.height, 0.0f, bgRacing.bgCoord);

	// for players 
	MatrixCalc(player1.transform, player1.size, player1.size, 0.f, player1.pCoord);
	MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.pCoord);

	//for scoreboard
	MatrixCalc(score_board.transform, score_board.length, score_board.height, 0.0f, score_board.sCoord);

	//for platforms 
	for (int i = 0; i < MAX_NUM_PLATFORMS; i++) {
		MatrixCalc(platformA[i].transform, platformA[i].length, platformA[i].height, 0.f, platformA[i].platVect);
		MatrixCalc(platformB[i].transform, platformB[i].length, platformB[i].height, 0.f, platformB[i].platVect);
	}

	//for items 
	for (int i = 0; i < MAX_NUM_ITEMS; i++) {
		MatrixCalc(racing_items[i].transform, racing_items[i].size, racing_items[i].size, 0.f, racing_items[i].pCoord);
	}
	
	//for splitscreen
	MatrixCalc(splitscreen.transform, splitscreen.length, splitscreen.height, 0.f, splitscreen.lVect);

	// for winning texture
	//switch (if_win)
	//{
	//	case 1:
	//		break;
	//	case 2:
	//		break;

	//}



	///*------------------------------------------------------------
	// UPDATE - Camera Movement
	// if the player.y + size is over or at maxHeight, CamY will increase
	//------------------------------------------------------------*/
	//CamY = (player1.pCoord.y + player2.pCoord.y) / 2 + winHEIGHT / 4;
	std::cout << "maxHeight: " << maxHeight << "\nPlayer 1: " << (player1.pCoord.y + player1.size) << std::endl;
	if ((player1.pCoord.y + player1.size) >= maxHeight || (player2.pCoord.y + player2.size) >= maxHeight)
	{

		CamY+=maxHeight_copy; // Calculate the target camera position to gradually move towards

		// maxHeight Incrementing
		maxHeight = (maxHeight_copy * ++count);
		if (count == 4) firstround_over = true;
	}
	
	// if both player falls off, restart the whole game
	if (firstround_over == true && (player1.pCoord.y + player1.size < (maxHeight_copy * (count - 3))) && (player2.pCoord.y + player2.size < (maxHeight_copy * (count - 3))))
	{
		next_state = RESTART;
	}



}

void racing_draw()
{
	std::cout << "racing:Draw\n";

	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransform(bgRacing.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(bgRacing.bgTex, 0.f, 0.f);
	AEGfxMeshDraw(bgRacing.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	// DRAWING PLATFORMS - MAP
	------------------------------------------------------------*/
	racing_map_draw();



	/*------------------------------------------------------------
	// DRAWING PLAYERS
	------------------------------------------------------------*/
	// Drawing player 1
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransform(player1.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(player1.pTex, 0, 0);
	AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);

	// drawing player 2
	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(player2.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(player2.pTex, 0, 0);
	AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);



	/*------------------------------------------------------------
	// DRAWING SPLITSCREEN
	------------------------------------------------------------*/
	splitscreen_draw();



	/*------------------------------------------------------------
	// DRAWING SCORE BOARD
	------------------------------------------------------------*/
	/*AEGfxSetTransform(score_board.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	// No texture for scoreboard
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(score_board.sMesh, AE_GFX_MDM_TRIANGLES);*/


	/*------------------------------------------------------------
	// DRAWING Winner Texture
	------------------------------------------------------------*/
		AEGfxSetTransform(winRacing.transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(winRacing.bgTex, 0, 0);
		AEGfxMeshDraw(winRacing.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	 DRAWING - Camera Movement
	------------------------------------------------------------*/
	AEGfxSetCamPosition(CamX, CamY); // Set Camera's Position to values of CamX & CamY


}

void racing_free()
{
	std::cout << "racing:Free\n";
	CamX = 0.0f; 
	CamY = 0.f;
	AEGfxSetCamPosition(CamX, CamY);
}

void racing_unload()
{
	std::cout << "racing:Unload\n";

	AEGfxMeshFree(bgRacing.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgRacing.bgTex); // Unload Texture

	/*------------------------------------------------------------
	// Unload Player Meshes
	------------------------------------------------------------*/
	AEGfxMeshFree(player1.pMesh);
	AEGfxMeshFree(player2.pMesh);

	AEGfxTextureUnload(player1.pTex);
	AEGfxTextureUnload(player2.pTex);


	/*------------------------------------------------------------
	// Unload Platform Meshes
	------------------------------------------------------------*/
	racing_map_unload();


	/*------------------------------------------------------------
	// Unload Split Screen Meshes
	------------------------------------------------------------*/
	splitscreen_unload();

	/*------------------------------------------------------------
	// Unload Player Meshes
	------------------------------------------------------------*/
	AEGfxMeshFree(winRacing.bgMesh);
	AEGfxTextureUnload(winRacing.bgTex);

}
