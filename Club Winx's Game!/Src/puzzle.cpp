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

#include "General.h"

// ---------------------------------------------------------------------------

// ----- TEXTURE DECLARATIONS ----- //
// Textures for Puzzle Mode
AEGfxTexture* Key;
AEGfxTexture* Spiderweb;

int counter = 0;
bool turntransparent = { FALSE };

bool itemdie = { FALSE };

/* ------------------------------------------------------------
CONSTANTS
------------------------------------------------------------*/


enum MAPOBJ {
	empty = 0,
	wall,
	p1,
	p2
};

#define GRID_COLS 36 
#define GRID_ROWS 20 


const float PLAYERSIZE =1.0;


static int** mapdata;
const int binmap[GRID_ROWS][GRID_COLS]
{

	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

};

/* ------------------------------------------------------------
GLOABLS
------------------------------------------------------------*/
struct Map {
	AEGfxVertexList* pMesh0{ nullptr }; // mesh 
	AEGfxVertexList* pMesh1{ nullptr };
	AEGfxTexture* pTex{ nullptr };			// texture
	AEMtx33				MapTransform{};		// transform mtx 
};
Map map;


void puzzle_load()
{
	std::cout << "puzzle:Load\n";

	AEGfxSetBackgroundColor(255.0f, 0.0f, 0.0f);
	bgPuzzle.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgPuzzle.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	// Textures for Puzzle
	bgPuzzle.bgTex = AEGfxTextureLoad("Assets/PUZZLE.png");
	AE_ASSERT_MESG(bgPuzzle.bgTex, "Failed to create bgPuzzle.bgTex!!");

	Key = AEGfxTextureLoad("Assets/KEY.png");
	AE_ASSERT_MESG(Key, "Failed to create Key!!");

	Spiderweb = AEGfxTextureLoad("Assets/SPIDERWEB.png");
	AE_ASSERT_MESG(Spiderweb, "Failed to create Spiderweb!!");

	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/
	// BG mesh
	SquareMesh(&bgPuzzle.bgMesh, 0);

	// player 1 mesh 
	SquareMesh(&player1.pMesh, 0xFFB62891);

	// player 2 mesh
	SquareMesh(&player2.pMesh, 0xFFFF00FF);

	//Item
	SquareMesh(&puzzle.pMesh, 0x00FFFF00);

	//Creating the empty
	SquareMesh(&map.pMesh0, 0xFFA4C4DB);

	//Creating  walls
	SquareMesh(&map.pMesh1, 0xFF7B94AB);


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
	std::cout << "puzzle:Initialize\n";

	/*------------------------------------------------------------
	INIT PLAYERS
	------------------------------------------------------------*/
	player1.pCoord = { 1,1 };
	//player2.pCoord = { player1.pCord.x + 100.f, player1.pCoord.y };
	
	/*------------------------------------------------------------
	INIT MAP DATA
	------------------------------------------------------------*/
	mapdata = new int* [GRID_ROWS];
	for (int i = 0; i < GRID_ROWS; i++)
	{
		mapdata[i] = new int[GRID_COLS];
	}

	// COPY binmap to mapdata 
	for (int i = 0; i < GRID_ROWS; i++)
	{
		for (int j = 0; j < GRID_COLS; j++)
		{
			mapdata[i][j] = binmap[i][j];
		}
	}


	for (int i = 0; i < GRID_ROWS; i++)
	{
		for (int j = 0; j < GRID_COLS; j++)
		{
			AEVec2 Pos{ (float)i  , (float)j };
			if (mapdata[i][j] == p1)
			{
				//player1.pCoord.x = (int)Pos.x;
				//player1.pCoord.y = (int)Pos.y;
			}
		}
	}
}

void puzzle_update()
{
	std::cout << "puzzle:Update\n";

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
	PLAYER MOVEMENT
	------------------------------------------------------------*/
	input_handle();

	/*------------------------------------------------------------
	COLLISION CHECKS
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

	/*------------------------------------------------------------
	UPDATE PLAYER POSITIONS
	------------------------------------------------------------*/
	player1.pCoord.x += player1.pVel.x * player1.pAcceleration * g_dt;
	player1.pCoord.y += player1.pVel.y * player1.pAcceleration * g_dt;

	player2.pCoord.x += player2.pVel.x * player2.pAcceleration * g_dt;
	player2.pCoord.y += player2.pVel.y * player2.pAcceleration * g_dt;

	/*------------------------------------------------------------
	MATRIX CALCULATION
	------------------------------------------------------------*/
	// for background
	MatrixCalc(bgPuzzle.transform, bgPuzzle.length, bgPuzzle.height, 0.0f, bgPuzzle.bgCoord);

	// for players 
	//AEMtx33 trans{}, rot{}, scale{};
	// // Compute the scaling matrix
	//AEMtx33Scale(&scale, 1, 1);
	// Compute the rotation matrix 
	//AEMtx33Rot(&rot, 0);
	// Compute the translation matrix
	//AEMtx33Trans(&trans, player1.pCoord.x, player1.pCoord.y);
	// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
	//AEMtx33Concat(&player1.transform, &rot, &scale);
	//AEMtx33Concat(&player1.transform, &trans, &player1.transform);
	MatrixCalc(player1.transform, PLAYERSIZE, PLAYERSIZE, 0.f, player1.pCoord);
	MatrixCalc(player2.transform, PLAYERSIZE, PLAYERSIZE, 0.f, player2.pCoord);

	MatrixCalc(puzzle.transform, puzzle.length, puzzle.height, 0.f, puzzle.IVector);

	/*if (CollisionIntersection_Item(player2.pCoord, player2.size, player2.size,
		puzzle.IVector, puzzle.length, puzzle.height) == true)
	{
		std::cout << "COLLIDE  " << std::endl;



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

	std::cout << "player 1 x " << player1.pCoord.x << "\n";

	if (turntransparent == TRUE) {


		++counter;
	}
	turntransparent = FALSE;
}

void puzzle_draw()
{
	std::cout << "puzzle:Draw\n";

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

	//Item Collecteed
	if (itemdie == TRUE) {

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(0.0f);
		AEGfxSetTransform(puzzle.transform.m);
		AEGfxMeshDraw(puzzle.pMesh, AE_GFX_MDM_TRIANGLES);

	}
	else {

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxSetTransform(puzzle.transform.m);
		AEGfxMeshDraw(puzzle.pMesh, AE_GFX_MDM_TRIANGLES);

	}


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
	AEMtx33 trans, final , flip;
	AEMtx33Scale(&flip, 1.0f, -1.0f); // create a matrix to flip vertically
	for (int i = 0; i < GRID_ROWS; i++)
	{
		for (int j = 0; j < GRID_COLS; j++)
		{
			AEMtx33Trans(&trans, (float)j + 0.5f, (float)i + 0.5f);
			AEMtx33Concat(&final, &map.MapTransform, &trans);
			AEMtx33Concat(&final, &flip, &final);
			AEGfxSetTransform(final.m);

			if (binmap[i][j] == wall)
			{

				AEGfxMeshDraw(map.pMesh1, AE_GFX_MDM_TRIANGLES);

			}
			if (binmap[i][j] == empty)
			{

				AEGfxMeshDraw(map.pMesh0, AE_GFX_MDM_TRIANGLES);

			}

		}
	}

	// Drawing player 1
	AEMtx33Concat(&player1.transform, &map.MapTransform, &player1.transform);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTransform(player1.transform.m);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);


	// drawing player 2
	AEMtx33Concat(&player2.transform, &map.MapTransform, &player2.transform);
	AEGfxSetTransform(player2.transform.m);
	//AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);
	
}



void puzzle_free()
{
	std::cout << "puzzle:Free\n";

	AEGfxMeshFree(player1.pMesh);
	AEGfxMeshFree(player2.pMesh);
	AEGfxMeshFree(bgPuzzle.bgMesh);
	AEGfxMeshFree(puzzle.pMesh);

	AEGfxMeshFree(map.pMesh0);
	AEGfxMeshFree(map.pMesh1);

	for (int i = 0; i < GRID_ROWS; i++)
	{
		delete[] mapdata[i];
	}
}

void puzzle_unload()
{
	std::cout << "puzzle:Unload\n";

	AEGfxTextureUnload(bgPuzzle.bgTex);
	AEGfxTextureUnload(Key);
	AEGfxTextureUnload(Spiderweb);

}

/******************************************************************************/
/*!
	This function converts coordinates into center of grid cells
*/
/******************************************************************************/
void SnapToCell(float* Coordinate)
{
	*Coordinate = static_cast<int>(*Coordinate); // cast to int 
	*Coordinate += 0.5f;  // snap to center
}

