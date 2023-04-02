/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This source file defines the functions for the puzzle game mode.
* 
* Primary Author: 
*	Mariah Tahirah (mariahtahirah.b@digipen.edu) -> Binary Map, Map transform, Light mechanism, Player-map collision) 
* Secondary Authors:
*	Yeo Hui Shan (huishan.y@digipen.edu)
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "PuzzleUtil.h"

int** mapdata;
// ---------------------------------------------------------------------------





/* ------------------------------------------------------------
STRUCTS
------------------------------------------------------------*/
// puzzle map
struct Map 
{
	AEGfxVertexList*	meshEmpty{ nullptr }; // mesh of empty
	AEGfxVertexList*	meshWall{ nullptr }; // mesh of wall

	AEGfxTexture*		textureEmpty{ nullptr };// texture
	AEGfxTexture*		textureWall{ nullptr };// texture

	AEMtx33				transform{};		// transform mtx 
};
Map map;



//puzzle items
struct Item 
{
	AEGfxVertexList*	mesh{ nullptr }; // mesh of key
	AEGfxTexture*		texture{ nullptr }; // texture

	AABB				boundingBox; //collision

	f32					size{ 1.2f };

	AEVec2				vector{ 2.5,3.5 };

	AEVec2				velocity{ 0.0f, 0.0f }; //velocity for the item

	AEMtx33				transform{};
};
Item item1, item2, item3;



//puzzle traps
struct Trap 
{
	AEGfxVertexList*	mesh{ nullptr }; // mesh of portal
	AEGfxTexture*		texture{ nullptr }; // texture

	AABB				boundingBox; //collision

	f32					size{ 1.2f };

	AEVec2				vector{ 0,0 };

	AEVec2				velocity{ 0.0f, 0.0f }; //velocity for the item 

	AEMtx33				transform{};

	AEVec2              posCurr;
	AEVec2              velCurr;

	float               scale;
	int					pFlag;
};
Trap trap1, trap2, trap3;



//puzzle gate
struct Gate 
{
	AEGfxVertexList*	mesh{ nullptr }; // mesh of gate
	AEGfxTexture*		texture{ nullptr }; // texture

	AABB				boundingBox; //collision

	f32					size{ 2.0f };

	AEVec2				vector{ 0,0 };
	AEVec2				velocity{ 0.0f, 0.0f }; //velocity for the item

	AEMtx33				transform{};

	AEVec2              posCurr;
	AEVec2              velCurr;

	float               scale;
	int					gFlag;
};
Gate gate;



/* ------------------------------------------------------------
CONSTANTS
------------------------------------------------------------*/
const f64 lightRadius = 100.0; // minimum 70.0
const f32 shadowAlpha = 0.97f; ////CHANGE TRANSPARENCY OF SHADOW HERE



/* ------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/
Textures puzzleLight;

static AEMtx33 flipTransform1, flipTransform2;
static AEVec2 numberofkeys;

int counter = 0; //counter for number of keys collected
int puzzleMinute = 0;

bool turntransparent	= { FALSE };
bool itemdie			= { FALSE };
bool gateIsOpen			= { FALSE }; //true only when all keys are collected
bool player1AtGate		= { FALSE };
bool player2AtGate		= { FALSE };



/* ------------------------------------------------------------
HELPER FUNCTIONS
------------------------------------------------------------*/
template <typename T>
void draw_puzzle_asset(T asset, Map map);





/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void puzzle_load()
{
	AEGfxSetBackgroundColor(255.0f, 0.0f, 0.0f);

	/*------------------------------------------------------------
	LOADING TEXTURES (IMAGES)
	------------------------------------------------------------*/
	// Textures for Puzzle
	item1.texture = AEGfxTextureLoad("Assets/PuzzleAssets/KEY.png");
	AE_ASSERT_MESG(item1.texture, "Failed to create Key1!!");

	item2.texture = AEGfxTextureLoad("Assets/PuzzleAssets/KEY.png");
	AE_ASSERT_MESG(item2.texture, "Failed to create Key2!!");

	item3.texture = AEGfxTextureLoad("Assets/PuzzleAssets/KEY.png");
	AE_ASSERT_MESG(item3.texture, "Failed to create Key3!!");

	trap1.texture = AEGfxTextureLoad("Assets/PuzzleAssets/PORTAL.png");
	AE_ASSERT_MESG(trap1.texture, "Failed to create Portal1!!");

	trap2.texture = AEGfxTextureLoad("Assets/PuzzleAssets/PORTAL.png");
	AE_ASSERT_MESG(trap2.texture, "Failed to create Portal2!!");

	trap3.texture = AEGfxTextureLoad("Assets/PuzzleAssets/PORTAL.png");
	AE_ASSERT_MESG(trap3.texture, "Failed to create Portal3!!");

	puzzleLight.texture = AEGfxTextureLoad("Assets/PuzzleAssets/PUZZLE_LIGHT.png");
	AE_ASSERT_MESG(puzzleLight.texture, "Failed to create puzzle light!!");

	gate.texture = AEGfxTextureLoad("Assets/PuzzleAssets/GATE.png");
	AE_ASSERT_MESG(gate.texture, "Failed to create Gate!!");

	player1.texture = AEGfxTextureLoad("Assets/PLAYER1.png");
	player2.texture = AEGfxTextureLoad("Assets/PLAYER2.png");
	
	map.textureEmpty = AEGfxTextureLoad("Assets/PuzzleAssets/EMPTY.png");
	map.textureWall = AEGfxTextureLoad("Assets/PuzzleAssets/WALL.png");


	/*------------------------------------------------------------
	// LOAD SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	puzzle_bgm.audio = AEAudioLoadSound("Assets/Audio/puzzleMusic.wav");
	puzzle_bgm.aGroup = AEAudioCreateGroup();
	
	collectKey.audio = AEAudioLoadSound("Assets/Audio/collectKey.wav");
	collectKey.aGroup = AEAudioCreateGroup();
	
	walk.audio = AEAudioLoadSound("Assets/Audio/walk.wav");
	walk.aGroup = AEAudioCreateGroup();


	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/
	// player 1 mesh 
	SquareMesh(&player1.mesh, 0xFFB62891);

	// player 2 mesh
	SquareMesh(&player2.mesh, 0xFFFF00FF);

	//Creating the empty
	SquareMesh(&map.meshEmpty, 0xFFA4C4DB);

	//Creating  walls
	SquareMesh(&map.meshWall, 0xFF7B94AB);

	//creating puzzle light 
	SquareMesh(&puzzleLight.mesh, 0x00FF0000);

	//creating keys
	SquareMesh(&item1.mesh, 0x00FF0000);
	SquareMesh(&item2.mesh, 0x00FF0000);
	SquareMesh(&item3.mesh, 0x00FF0000);

	//creating traps
	SquareMesh(&trap1.mesh, 0x003300FF);
	SquareMesh(&trap2.mesh, 0x003300FF);
	SquareMesh(&trap3.mesh, 0x003300FF);
	
	//Creating gate
	SquareMesh(&gate.mesh, 0x003300FF);

	//load pause screen
	pause_load();

	// load tut screen
	GameTutorial_Load();

	//Compute the matrix of the binary map
	AEMtx33 scale, trans;
	AEMtx33Trans(&trans, (-(float)GRID_COLS / 2), (-(float)GRID_ROWS / 2));
	AEMtx33Scale(&scale, (float)AEGetWindowWidth() / (float)GRID_COLS, (float)AEGetWindowHeight() / (float)GRID_ROWS);
	AEMtx33Concat(&map.transform, &scale, &trans);
	
}




void puzzle_init()
{
	/*------------------------------------------------------------
	// SET TIMER
	------------------------------------------------------------*/
	puzzleTime.minute = 180.0f; //3 minutes
	puzzleTime.second = 60.0f;

	/*------------------------------------------------------------
	// FONT POSITIONS
	------------------------------------------------------------*/
	numberofkeys.x = -0.9f;
	numberofkeys.y = 0.7f;

	/*------------------------------------------------------------
	INIT PLAYERS
	------------------------------------------------------------*/
	player1.coord = { 1.5,1.5 };
	player2.coord = { player1.coord.x + 1, player1.coord.y };
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
	puzzleLight.coord = player2.coord;

	counter = 0;

	AEMtx33Scale(&flipTransform1, 1.0f, 1.0f);
	AEMtx33Scale(&flipTransform2, 1.0f, 1.0f);

	/*------------------------------------------------------------
	// PLAY SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	AEAudioPlay(puzzle_bgm.audio, puzzle_bgm.aGroup, 0.75, 1, -1);

	/*------------------------------------------------------------
	// INIT KEYS
	------------------------------------------------------------*/
	item1.vector = { 2.5,18 };
	item2.vector = { 14,5.5 };
	item3.vector = { 34,14 };

	trap1.vector = { 2,11 };
	trap2.vector = { 33,1.5 };
	trap3.vector = { 33,18 };

	/*------------------------------------------------------------
	// INIT GATE
	------------------------------------------------------------*/
	gate.vector = { 18,2 };

	//init pause screen
	pause_init();

	/*------------------------------------------------------------
	// INIT IN GAME TUTORIAL
	------------------------------------------------------------*/
	tut_viewed = false;
	GameTutorial_Init(0.0f, 0.0f);
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
		// IN GAME TUTORIAL
		------------------------------------------------------------*/
		if (arcadeMode)
		{
			tut_viewed = true;
		}
		// Game Tutorial
		if (tut_viewed == false)
		{
			GameTutorial_Update();
			AEAudioPauseGroup(puzzle_bgm.aGroup);
		}
		else
			AEAudioResumeGroup(puzzle_bgm.aGroup);
		
		/*------------------------------------------------------------
		// TIMER
		------------------------------------------------------------*/
		if (tut_viewed == true) {
			puzzleTime.minute -= g_dt;
			puzzleTime.second -= g_dt;
			puzzleMinute = (int)puzzleTime.minute / 60;
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
		BoundingBoxUpdate(player1.boundingBox, player1.coord, player1.size, player1.size);
		BoundingBoxUpdate(player2.boundingBox, player2.coord, player2.size, player2.size);

		//item bounding box
		BoundingBoxUpdate(item1.boundingBox, item1.vector, item1.size, item1.size);
		BoundingBoxUpdate(item2.boundingBox, item2.vector, item2.size, item2.size);
		BoundingBoxUpdate(item3.boundingBox, item3.vector, item3.size, item3.size);

		//Trap Bounding box
		BoundingBoxUpdate(trap1.boundingBox, trap1.vector, trap1.size, trap1.size);
		BoundingBoxUpdate(trap2.boundingBox, trap2.vector, trap2.size, trap2.size);
		BoundingBoxUpdate(trap3.boundingBox, trap3.vector, trap3.size, trap3.size);

		//Gate Bounding box
		BoundingBoxUpdate(gate.boundingBox, gate.vector, gate.size, gate.size);

		/*------------------------------------------------------------
		UPDATE PLAYER POSITIONS
		------------------------------------------------------------*/
		player1.coord.x += player1.velocity.x * player1.acceleration * g_dt;
		player1.coord.y += player1.velocity.y * player1.acceleration * g_dt;

		player2.coord.x += player2.velocity.x * player2.acceleration * g_dt;
		player2.coord.y += player2.velocity.y * player2.acceleration * g_dt;

		// update light position 
		puzzleLight.coord = player2.coord;

		/*------------------------------------------------------------
		COLLISION CHECKS
		------------------------------------------------------------*/
		Map_Player_CollisionUpdate(&player1);
		Map_Player_CollisionUpdate(&player2);

		/*------------------------------------------------------------
		MATRIX CALCULATION
		------------------------------------------------------------*/
		MatrixCalc(player1.transform, player1.size, player1.size, 0.f, player1.coord);
		MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.coord);

		MatrixCalc(puzzleLight.transform, puzzleLight.length, puzzleLight.height, 0.0f, puzzleLight.coord);

		MatrixCalc(item1.transform, item1.size, item1.size, 0.f, item1.vector);
		MatrixCalc(item2.transform, item2.size, item2.size, 0.f, item2.vector);
		MatrixCalc(item3.transform, item3.size, item3.size, 0.f, item3.vector);

		MatrixCalc(trap1.transform, trap1.size, trap1.size, 0.f, trap1.vector);
		MatrixCalc(trap2.transform, trap2.size, trap2.size, 0.f, trap2.vector);
		MatrixCalc(trap3.transform, trap3.size, trap3.size, 0.f, trap3.vector);
		
		MatrixCalc(gate.transform, gate.size, gate.size, 0.f, gate.vector);

		/*------------------------------------------------------------
		COLLISION BETWEEN KEYS AND PLAYER
		------------------------------------------------------------*/
		if (CollisionIntersection_RectRect(item1.boundingBox, item1.velocity, player1.boundingBox, player1.velocity)) 
		{
			item1.vector = { -1000,1000 };
			counter = counter + 1;
			AEAudioPlay(collectKey.audio, collectKey.aGroup, 0.85f, 1.0f, 0);
		}

		if (CollisionIntersection_RectRect(item2.boundingBox, item2.velocity, player1.boundingBox, player1.velocity))
		{
			item2.vector = { -1000,1000 };
			counter = counter + 1;
			AEAudioPlay(collectKey.audio, collectKey.aGroup, 0.85f, 1.0f, 0);
		}

		if (CollisionIntersection_RectRect(item3.boundingBox, item3.velocity, player1.boundingBox, player1.velocity))
		{
			item3.vector = { -1000,1000 };
			counter = counter + 1;
			AEAudioPlay(collectKey.audio, collectKey.aGroup, 0.85f, 1.0f, 0);
		}

		/*------------------------------------------------------------
		COLLISION BETWEEN TRAPS AND PLAYER
		//Player that touches the trap will be sent back to original position
		------------------------------------------------------------*/
		
		//TRAP 1
		if (CollisionIntersection_RectRect(trap1.boundingBox, trap1.velocity, player1.boundingBox, player1.velocity)) {
			player1.coord = { 1.5,1.5 };
		}

		if (CollisionIntersection_RectRect(trap1.boundingBox, trap1.velocity, player2.boundingBox, player2.velocity)) {
			player2.coord = { 1.5,1.5 };
		}
		
		//TRAP 2
		if (CollisionIntersection_RectRect(trap2.boundingBox, trap2.velocity, player1.boundingBox, player1.velocity)) {
			player1.coord = { 1.5,1.5 };
		}

		if (CollisionIntersection_RectRect(trap2.boundingBox, trap2.velocity, player2.boundingBox, player2.velocity)) {
			player2.coord = { 1.5,1.5 }; 
		}

		//TRAP 3
		if (CollisionIntersection_RectRect(trap3.boundingBox, trap3.velocity, player1.boundingBox, player1.velocity)) {
			player1.coord = { 1.5,1.5 };
		}

		if (CollisionIntersection_RectRect(trap3.boundingBox, trap3.velocity, player2.boundingBox, player2.velocity)) {
			player2.coord = { 1.5,1.5 };
		}


		/*------------------------------------------------------------
		COLLISION BETWEEN GATE AND PLAYER
		------------------------------------------------------------*/
		if (CollisionIntersection_RectRect(gate.boundingBox, gate.velocity, player1.boundingBox, player1.velocity)) 
		{
			player1AtGate = true;
		}
		else
		{
			player1AtGate = false;
		}

		if (CollisionIntersection_RectRect(gate.boundingBox, gate.velocity, player2.boundingBox, player2.velocity)) 
		{
			player2AtGate = true;
		}
		else 
		{
			player2AtGate = false;
		}

		/*------------------------------------------------------------
		WIN/LOSE CONDITION
		------------------------------------------------------------*/
		//Collected all 3 keys
		if (counter == 3)
		{
			gateIsOpen = true;
			//std::cout << "gate open" << std::endl;
			
			if (player1AtGate == true && player2AtGate == true)
			{
				next_state = WIN_BOTHPLAYERS;
			}
		}
		else
		{
			gateIsOpen = false;
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
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

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
				AEMtx33Concat(&final, &map.transform, &trans);
				AEMtx33Concat(&final, &flip, &final);
				AEGfxSetTransform(final.m);

				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTransparency(1.0f);

				if (mapdata[i][j] == WALL)
				{
					AEGfxTextureSet(map.textureWall, 0, 0);
					AEGfxMeshDraw(map.meshWall, AE_GFX_MDM_TRIANGLES);
				}

				if (mapdata[i][j] == EMPTY)
				{
					AEGfxTextureSet(map.textureEmpty, 0, 0);
					AEGfxMeshDraw(map.meshEmpty, AE_GFX_MDM_TRIANGLES);
				}
			}
		}

		/*------------------------------------------------------------
			// DRAWING PLAYERS
		------------------------------------------------------------*/
		// Drawing player 1
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEMtx33Concat(&player1.transform, &player1.transform, &flipTransform1);
		AEMtx33Concat(&player1.transform, &map.transform, &player1.transform);
		AEGfxSetTransform(player1.transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(player1.texture, 0, 0);
		AEGfxMeshDraw(player1.mesh, AE_GFX_MDM_TRIANGLES);

		// drawing player 2
		AEMtx33Concat(&player2.transform, &player2.transform, &flipTransform2);
		AEMtx33Concat(&player2.transform, &map.transform, &player2.transform);
		AEGfxSetTransform(player2.transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(player2.texture, 0, 0);
		AEGfxMeshDraw(player2.mesh, AE_GFX_MDM_TRIANGLES);

		/*------------------------------------------------------------
		 DRAWING KEYS
		------------------------------------------------------------*/
		draw_puzzle_asset(item1, map);
		draw_puzzle_asset(item2, map);
		draw_puzzle_asset(item3, map);

		/*------------------------------------------------------------
		 DRAWING TRAPS
		------------------------------------------------------------*/
		draw_puzzle_asset(trap1, map);
		draw_puzzle_asset(trap2, map);
		draw_puzzle_asset(trap3, map);

		/*------------------------------------------------------------
		 DRAWING GATE
		------------------------------------------------------------*/
		draw_puzzle_asset(gate, map);

		/*------------------------------------------------------------
		 RENDERING LIGHT AND SHADOW
		------------------------------------------------------------*/
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEMtx33Concat(&puzzleLight.transform, &map.transform, &puzzleLight.transform);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransparency(shadowAlpha); //transparency
		AEGfxSetTransform(puzzleLight.transform.m);
		AEGfxTextureSet(puzzleLight.texture, 0, 0);
		AEGfxMeshDraw(puzzleLight.mesh, AE_GFX_MDM_TRIANGLES);
		
		/*------------------------------------------------------------
		 DRAWING WORDS
		------------------------------------------------------------*/
		char strBuf[1000];

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		memset(strBuf, 0, 1000 * sizeof(char));
		sprintf_s(strBuf, "Time Left: %d:%.0f", puzzleMinute, puzzleTime.second);
		AEGfxPrint(font_pixel, strBuf, puzzleTime.timeleft.x, puzzleTime.timeleft.y, 0.7f, 1.0f, 1.0f, 1.0f);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		memset(strBuf, 0, 1000 * sizeof(char));
		sprintf_s(strBuf, "No. of keys: %d/3", counter);
		AEGfxPrint(font_pixel, strBuf, numberofkeys.x, numberofkeys.y, 0.7f, 1.0f, 1.0f, 1.0f);

		/*------------------------------------------------------------
		// DRAWING TUTORIAL
		------------------------------------------------------------*/
		if (tut_viewed == false)
			GameTutorial_Draw();
	}
}



void puzzle_free()
{
	/*------------------------------------------------------------
	// free mesh
	------------------------------------------------------------*/
	AEGfxMeshFree(player1.mesh);
	AEGfxMeshFree(player2.mesh);

	AEGfxMeshFree(map.meshEmpty);
	AEGfxMeshFree(map.meshWall);

	AEGfxMeshFree(puzzleLight.mesh);

	AEGfxMeshFree(item1.mesh);
	AEGfxMeshFree(item2.mesh);
	AEGfxMeshFree(item3.mesh);

	AEGfxMeshFree(trap1.mesh);
	AEGfxMeshFree(trap2.mesh);
	AEGfxMeshFree(trap3.mesh);
	
	AEGfxMeshFree(gate.mesh);


	/*------------------------------------------------------------
	// free map
	------------------------------------------------------------*/
	for (int i = 0; i < GRID_ROWS; i++)
	{
		delete[] mapdata[i];
	}

	/*------------------------------------------------------------
	// free pause 
	------------------------------------------------------------*/
	pause_free();

	/*------------------------------------------------------------
	// free tutorial
	------------------------------------------------------------*/
	GameTutorial_Free();

}



void puzzle_unload()
{
	/*------------------------------------------------------------
	// unload textures
	------------------------------------------------------------*/
	AEGfxTextureUnload(item1.texture);
	AEGfxTextureUnload(item2.texture);
	AEGfxTextureUnload(item3.texture);

	AEGfxTextureUnload(trap1.texture);
	AEGfxTextureUnload(trap2.texture);
	AEGfxTextureUnload(trap3.texture);

	AEGfxTextureUnload(player1.texture);
	AEGfxTextureUnload(player2.texture);

	AEGfxTextureUnload(puzzleLight.texture);

	AEGfxTextureUnload(gate.texture);

	AEGfxTextureUnload(map.textureEmpty);
	AEGfxTextureUnload(map.textureWall);

	/*------------------------------------------------------------
	// exit audio
	------------------------------------------------------------*/
	AEAudioStopGroup(puzzle_bgm.aGroup);
	AEAudioStopGroup(collectKey.aGroup);
	AEAudioStopGroup(walk.aGroup);

	/*------------------------------------------------------------
	// unload pause
	------------------------------------------------------------*/
	pause_unload();

	/*------------------------------------------------------------
	// unload tutorial
	------------------------------------------------------------*/
	GameTutorial_Unload();
}




/* ------------------------------------------------------------
HELPER FUNCTIONS
------------------------------------------------------------*/
template<typename T>
void draw_puzzle_asset(T asset, Map map)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEMtx33Concat(&asset.transform, &map.transform, &asset.transform);
	AEGfxSetTransform(asset.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(asset.texture, 0, 0);
	AEGfxMeshDraw(asset.mesh, AE_GFX_MDM_TRIANGLES);
}