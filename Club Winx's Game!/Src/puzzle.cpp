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

//#include "General.h"
#include "PuzzleUtil.h"

int** mapdata;
// ---------------------------------------------------------------------------

// ----- TEXTURE DECLARATIONS ----- //
// Textures for Puzzle Mode
AEGfxTexture* Key;
AEGfxTexture* Spiderweb;

int counter = 0;
bool turntransparent = { FALSE };
bool itemdie = { FALSE };
int puzzleMinute = 0;

/* ------------------------------------------------------------
CONSTANTS
------------------------------------------------------------*/
const f64 lightRadius = 100.0; // minimum 70.0
const f32 shadowAlpha = 0.97f; ////CHANGE TRANSPARENCY OF SHADOW HERE


/* ------------------------------------------------------------
GLOABLS
------------------------------------------------------------*/
struct Map {
	AEGfxVertexList* pMesh0{ nullptr }; // mesh of empty
	AEGfxVertexList* pMesh1{ nullptr }; // mesh of wall
	AEGfxTexture* pTex{ nullptr };			// texture
	AEMtx33				MapTransform{};		// transform mtx 
};
Map map;

struct Item {

	AEGfxVertexList* pMesh2{ nullptr }; // mesh of key
	AABB				boundingBox; //collision
	AABB				boundingBox2; //collision
	f32					size{ 1.0f };
	AEVec2				vector{ 2.5,3.5 };
	AEVec2				vector2{ 10.5,9.5 };
	AEVec2				Velocity{ 0.0f, 0.0f }; //velocity for the item 
	AEMtx33				transform{};
	AEMtx33				transform2{};
	AEMtx33				transform3{};
	AEVec2               posCurr;
	AEVec2              velCurr;
	float               scale;
	int					pFlag;



};
Item item, item2;

BG puzzleLight;

static AEMtx33 flipTransform1, flipTransform2;
static AEVec2 numberofkeys;

void puzzle_load()
{
	//std::cout << "puzzle:Load\n";

	AEGfxSetBackgroundColor(255.0f, 0.0f, 0.0f);
	bgPuzzle.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgPuzzle.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/

	// Textures for Puzzle
	bgPuzzle.bgTex = AEGfxTextureLoad("Assets/PUZZLE.png");
	AE_ASSERT_MESG(bgPuzzle.bgTex, "Failed to create bgPuzzle.bgTex!!");

	Key = AEGfxTextureLoad("Assets/KEY.png");
	AE_ASSERT_MESG(Key, "Failed to create Key!!");

	Spiderweb = AEGfxTextureLoad("Assets/SPIDERWEB.png");
	AE_ASSERT_MESG(Spiderweb, "Failed to create Spiderweb!!");

	puzzleLight.bgTex = AEGfxTextureLoad("Assets/puzzleLight.png");
	AE_ASSERT_MESG(puzzleLight.bgTex, "Failed to create puzzle light!!");

	player1.pTex = AEGfxTextureLoad("Assets/Player1.png");
	player2.pTex = AEGfxTextureLoad("Assets/Player2.png");
	
	/*------------------------------------------------------------
	// LOAD SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	puzzle_bgm.audio = AEAudioLoadSound("Assets/Audio/puzzleMusic.wav");
	puzzle_bgm.aGroup = AEAudioCreateGroup();

	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/
	// BG mesh
	SquareMesh(&bgPuzzle.bgMesh, 0);

	// player 1 mesh 
	SquareMesh(&player1.pMesh, 0xFFB62891);

	// player 2 mesh
	SquareMesh(&player2.pMesh, 0xFFFF00FF);

	//Creating Keys
	SquareMesh(&item.pMesh2, 0x00FF0000);

	//Creating the empty
	SquareMesh(&map.pMesh0, 0xFFA4C4DB);

	//Creating  walls
	SquareMesh(&map.pMesh1, 0xFF7B94AB);

	//creating puzzle light 
	SquareMesh(&puzzleLight.bgMesh, 0);

	//creating keys
	SquareMesh(&item.pMesh2, 0x00FF0000);
	SquareMesh(&item2.pMesh2, 0x00FF0000);

	//load pause screen
	pause_load();

	//Compute the matrix of the binary map
	AEMtx33 scale, trans;// , flip;
	//AEMtx33Scale(&flip, 1.0f, -1.0f);
	AEMtx33Trans(&trans, (-(float)GRID_COLS / 2), (-(float)GRID_ROWS / 2));
	AEMtx33Scale(&scale, (float)AEGetWindowWidth() / (float)GRID_COLS, (float)AEGetWindowHeight() / (float)GRID_ROWS);
	AEMtx33Concat(&map.MapTransform, &scale, &trans);
	//AEMtx33Concat(&map.MapTransform, &flip, &map.MapTransform);

}

void puzzle_init()
{
	//std::cout << "puzzle:Initialize\n";
	puzzleTime.minute = 180.0f; //3 minutes
	puzzleTime.second = 60.0f;

	/*------------------------------------------------------------
	// FONT POSITIONS
	------------------------------------------------------------*/
	//puzzle.timeleft.x = -0.9f;
	//puzzle.timeleft.y = 0.8f;
	numberofkeys.x = -0.9f;
	numberofkeys.y = 0.7f;

	/*------------------------------------------------------------
	INIT PLAYERS
	------------------------------------------------------------*/
	player1.pCoord = { 1.5,1.5 };
	player2.pCoord = { player1.pCoord.x + 1, player1.pCoord.y };
	player1.size = 1.0F;
	player2.size = 1.0F;
	/*------------------------------------------------------------
	INIT MAP DATA
	------------------------------------------------------------*/
	if (!InitMapData("Assets/Data/puzzlemap.txt"))
	{
		//std::cout << "unable to open map data";
		next_state = QUIT;
	}

	puzzleLight.length = puzzleLight.height = lightRadius;
	puzzleLight.bgCoord = player2.pCoord;

	AEMtx33Scale(&flipTransform1, 1.0f, 1.0f);
	AEMtx33Scale(&flipTransform2, 1.0f, 1.0f);

	/*------------------------------------------------------------
	// PLAY SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	AEAudioPlay(puzzle_bgm.audio, puzzle_bgm.aGroup, 0.75, 1, -1);

	/*------------------------------------------------------------
	// INIT KEYS
	------------------------------------------------------------*/
	item.vector = { 2.5,3.5 };
	item2.vector2 = { 7.5,9.5 };

	//init pause screen
	pause_init();
}

void puzzle_update()
{
	if (game_paused)
	{
		pause_update();
	}

	else
	{
		/*------------------------------------------------------------
		// CHANGE STATE CONDITIONS
		------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_2)) {
			next_state = RACING;
		}
		if (AEInputCheckCurr(AEVK_3)) {
			next_state = BOSS;
		}
		if (AEInputCheckCurr(AEVK_Q)) {
			next_state = QUIT;
		}
		if (AEInputCheckCurr(AEVK_ESCAPE)) {
			next_state = MENU;
		}

		/*------------------------------------------------------------
		// TIMER
		------------------------------------------------------------*/
		puzzleTime.minute -= AEFrameRateControllerGetFrameTime();
		puzzleTime.second -= AEFrameRateControllerGetFrameTime();
		puzzleMinute = puzzleTime.minute / 60;
		//std::cout << puzzleTime.minute << ":" << puzzleMinute << ":" << puzzleTime.second << std::endl;

		if (puzzleTime.second < 0.0f)
		{
			puzzleTime.second = 60.0f;

			if (puzzleTime.minute < 0.0f)
			{
				//game stops
				next_state = LOSE_BOTHPLAYERS;
			}
		}

		/*------------------------------------------------------------
		PLAYER MOVEMENT
		------------------------------------------------------------*/
		input_handle();

		if (AEInputCheckTriggered(AEVK_A)) {
			AEMtx33Scale(&flipTransform1, -1.0f, 1.0f); // player 1 flip 

		}
		if (AEInputCheckTriggered(AEVK_D)) {
			AEMtx33Scale(&flipTransform1, 1.0f, 1.0f); // player 1 normal 
		}
		if (AEInputCheckTriggered(AEVK_LEFT)) {
			AEMtx33Scale(&flipTransform2, -1.0f, 1.0f); // player 2 flip 

		}
		if (AEInputCheckTriggered(AEVK_RIGHT)) {
			AEMtx33Scale(&flipTransform2, 1.0f, 1.0f); // player 1 normal
		}

		/*------------------------------------------------------------
		UPDATE PLAYER BOUNDING BOX
		------------------------------------------------------------*/

		//update player bounding box
		player1.boundingBox.min.x = player1.pCoord.x - player1.size / 2.0f;
		player1.boundingBox.min.y = player1.pCoord.y - player1.size / 2.0f;
		player1.boundingBox.max.x = player1.pCoord.x + player1.size / 2.0f;
		player1.boundingBox.max.y = player1.pCoord.y + player1.size / 2.0f;

		player2.boundingBox.min.x = player2.pCoord.x - player2.size / 2.0f;
		player2.boundingBox.min.y = player2.pCoord.y - player2.size / 2.0f;
		player2.boundingBox.max.x = player2.pCoord.x + player2.size / 2.0f;
		player2.boundingBox.max.y = player2.pCoord.y + player2.size / 2.0f;

		//item bounding box
		item.boundingBox.min.x = item.vector.x - item.size / 2.0f;
		item.boundingBox.min.y = item.vector.y - item.size / 2.0f;
		item.boundingBox.max.x = item.vector.x + item.size / 2.0f;
		item.boundingBox.max.y = item.vector.y + item.size / 2.0f;

		item2.boundingBox2.min.x = item2.vector2.x - item2.size / 2.0f;
		item2.boundingBox2.min.y = item2.vector2.y - item2.size / 2.0f;
		item2.boundingBox2.max.x = item2.vector2.x + item2.size / 2.0f;
		item2.boundingBox2.max.y = item2.vector2.y + item2.size / 2.0f;

		/*------------------------------------------------------------
		UPDATE PLAYER POSITIONS
		------------------------------------------------------------*/
		player1.pCoord.x += player1.pVel.x * player1.pAcceleration * g_dt;
		player1.pCoord.y += player1.pVel.y * player1.pAcceleration * g_dt;

		player2.pCoord.x += player2.pVel.x * player2.pAcceleration * g_dt;
		player2.pCoord.y += player2.pVel.y * player2.pAcceleration * g_dt;

		// update light position 
		puzzleLight.bgCoord = player2.pCoord;
		/*------------------------------------------------------------
		COLLISION CHECKS
		------------------------------------------------------------*/
		Map_Player_CollisionUpdate(&player1);
		Map_Player_CollisionUpdate(&player2);
		//std::cout << player1.pFlag << '\n';
		/*------------------------------------------------------------
		MATRIX CALCULATION
		------------------------------------------------------------*/
		// for background
		MatrixCalc(bgPuzzle.transform, bgPuzzle.length, bgPuzzle.height, 0.0f, bgPuzzle.bgCoord);

		MatrixCalc(player1.transform, player1.size, player1.size, 0.f, player1.pCoord);
		MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.pCoord);

		MatrixCalc(puzzle.transform, puzzle.length, puzzle.height, 0.f, puzzle.IVector);

		MatrixCalc(puzzleLight.transform, puzzleLight.length, puzzleLight.height, 0.0f, puzzleLight.bgCoord);

		MatrixCalc(item.transform, item.size, item.size, 0.f, item.vector);
		MatrixCalc(item2.transform2, item2.size, item2.size, 0.f, item2.vector2);
		/*if (CollisionIntersection_Item(player2.pCoord, player2.size, player2.size,
			puzzle.IVector, puzzle.length, puzzle.height) == true)
		{
			//std::cout << "COLLIDE  " << std::endl;



			turntransparent = TRUE;
			itemdie = TRUE;

		}
		if (CollisionIntersection_Item(player1.pCoord, player1.size, player1.size,
			puzzle.IVector, puzzle.length, puzzle.height) == true)
		{
			std::cout << "CHECK CHECK" << std::endl;


			turntransparent = TRUE;
			itemdie = TRUE;

		}*/

		if (CollisionIntersection_RectRect(item.boundingBox, item.Velocity, player1.boundingBox, player1.pVel)) {
			std::cout << "COLLIDEEEE!!!!!!" << std::endl;
			item.vector = { -1000,1000 };

		}

		if (CollisionIntersection_RectRect(item2.boundingBox2, item2.vector2, player1.boundingBox, player1.pVel))
		{
			std::cout << "COLLIDEEEE!!!!!!" << std::endl;
			item2.vector2 = { -1000,1000 };

		}
	}
}

void puzzle_draw()
{
	if (game_paused)
	{
		pause_draw();
	}

	else
	{
		//std::cout << "puzzle:Draw\n";

		/*------------------------------------------------------------
		DRAWING BACKGROUND
		------------------------------------------------------------*/
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransform(bgPuzzle.transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);
		AEGfxTextureSet(bgPuzzle.bgTex, 0.0f, 0.0f);
		//AEGfxMeshDraw(bgPuzzle.bgMesh, AE_GFX_MDM_TRIANGLES);

		/*------------------------------------------------------------
		DRAWING PLAYERS
		------------------------------------------------------------*/

		//FONTS
		char strBuf[1000];
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		memset(strBuf, 0, 1000 * sizeof(char));
		sprintf_s(strBuf, "No.of keys: %d", counter);
		AEGfxPrint(fontID, strBuf, -0.95f, 0.5f, 0.8f, 1.0f, 1.0f, 1.0f);

		/*------------------------------------------------------------
		DRAWING TILE MAP
		------------------------------------------------------------*/
		AEMtx33 trans, final, flip;
		AEMtx33Scale(&flip, 1.0f, -1.0f); // create a matrix to flip vertically
		for (int i = 0; i < GRID_ROWS; i++)
		{
			for (int j = 0; j < GRID_COLS; j++)
			{
				AEMtx33Trans(&trans, (float)j + 0.5f, (float)i + 0.5f);
				AEMtx33Concat(&final, &map.MapTransform, &trans);
				AEMtx33Concat(&final, &flip, &final);
				AEGfxSetTransform(final.m);

				if (mapdata[i][j] == WALL)
				{

					AEGfxMeshDraw(map.pMesh1, AE_GFX_MDM_TRIANGLES);

				}
				if (mapdata[i][j] == EMPTY)
				{

					AEGfxMeshDraw(map.pMesh0, AE_GFX_MDM_TRIANGLES);

				}


			}
		}

		/*------------------------------------------------------------
			// DRAWING PLAYERS
		------------------------------------------------------------*/
		// Drawing player 1
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEMtx33Concat(&player1.transform, &player1.transform, &flipTransform1);
		AEMtx33Concat(&player1.transform, &map.MapTransform, &player1.transform);
		AEGfxSetTransform(player1.transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(player1.pTex, 0, 0);
		AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);

		// drawing player 2
		//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEMtx33Concat(&player2.transform, &player2.transform, &flipTransform2);
		AEMtx33Concat(&player2.transform, &map.MapTransform, &player2.transform);
		AEGfxSetTransform(player2.transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(player2.pTex, 0, 0);
		AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);

		//RENDER KEYS
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEMtx33Concat(&item.transform, &map.MapTransform, &item.transform);
		AEGfxSetTransform(item.transform.m);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxMeshDraw(item.pMesh2, AE_GFX_MDM_TRIANGLES);


		AEMtx33Concat(&item2.transform2, &map.MapTransform, &item2.transform2);
		AEGfxSetTransform(item2.transform2.m);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxMeshDraw(item2.pMesh2, AE_GFX_MDM_TRIANGLES);

		// RENDER LIGHT AND SHADOW
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEMtx33Concat(&puzzleLight.transform, &map.MapTransform, &puzzleLight.transform);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransparency(shadowAlpha); 
		AEGfxSetTransform(puzzleLight.transform.m);
		AEGfxTextureSet(puzzleLight.bgTex, 0, 0);
		AEGfxMeshDraw(puzzleLight.bgMesh, AE_GFX_MDM_TRIANGLES);
		
		//FONTS

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		memset(strBuf, 0, 1000 * sizeof(char));
		sprintf_s(strBuf, "Time Left: %d:%.0f", puzzleMinute, puzzleTime.second);
		AEGfxPrint(text, strBuf, puzzleTime.timeleft.x, puzzleTime.timeleft.y, 0.7f, 1.0f, 1.0f, 1.0f);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		memset(strBuf, 0, 1000 * sizeof(char));
		sprintf_s(strBuf, "No. of keys: %d", counter);
		AEGfxPrint(text, strBuf, numberofkeys.x, numberofkeys.y, 0.7f, 1.0f, 1.0f, 1.0f);



	}
}



void puzzle_free()
{
	//std::cout << "puzzle:Free\n";

	AEGfxMeshFree(player1.pMesh);
	AEGfxMeshFree(player2.pMesh);
	AEGfxMeshFree(bgPuzzle.bgMesh);

	AEGfxMeshFree(map.pMesh0);
	AEGfxMeshFree(map.pMesh1);
	AEGfxMeshFree(puzzleLight.bgMesh);
	AEGfxMeshFree(item.pMesh2);
	for (int i = 0; i < GRID_ROWS; i++)
	{
		delete[] mapdata[i];
	}

	//free pause screen
	pause_free();
}

void puzzle_unload()
{
	//std::cout << "puzzle:Unload\n";

	AEGfxTextureUnload(bgPuzzle.bgTex);
	AEGfxTextureUnload(Key);
	AEGfxTextureUnload(Spiderweb);
	AEGfxTextureUnload(player1.pTex);
	AEGfxTextureUnload(player2.pTex);
	AEGfxTextureUnload(puzzleLight.bgTex);

	/*------------------------------------------------------------
	// Exit Audio
	------------------------------------------------------------*/
	AEAudioStopGroup(puzzle_bgm.aGroup);

	//unload pause
	pause_unload();
}

