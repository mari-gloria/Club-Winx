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
#include "../Main.h"

// ---------------------------------------------------------------------------

//#define PUZZLE_MAP_W 840
//#define PUZZLE_MAP_H 520

// ----- TEXTURE DECLARATIONS ----- //
// Textures for Puzzle Mode
AEGfxTexture* Key;
AEGfxTexture* Spiderweb;

int counter = 0;
bool turntransparent = { FALSE };

bool itemdie = { FALSE };

/*
//Binary map data
static int** MapData;
static int				BINARY_MAP_WIDTH;
static int				BINARY_MAP_HEIGHT;
static GameObjInst* pBlackInstance;
static GameObjInst* pWhiteInstance;
static AEMtx33			MapTransform;
int					MAX_MAP_WIDTH = 20;
int					MAX_MAP_HEIGHT = 20;

int						ImportMapDataFromFile(const char* FileName);
int						GetCellValue(int X, int Y);
void					FreeMapData(void);
static GameObj* sGameObjList;
static GameObjInst* sGameObjInstList;
static unsigned int		sGameObjNum;
const unsigned int	GAME_OBJ_INST_NUM_MAX = 3000;

//Flags
const unsigned int	FLAG_ACTIVE = 0x00000001;
const unsigned int	FLAG_VISIBLE = 0x00000002;
const unsigned int	FLAG_NON_COLLIDABLE = 0x00000004;

enum TYPE_OBJECT
{
	TYPE_OBJECT_EMPTY,			//0
	TYPE_OBJECT_COLLISION,		//1

};
//State of objects
enum STATE
{
	STATE_NONE

};

// function to create/destroy a game object instance
static GameObjInst* gameObjInstCreate(unsigned int type, float scale, AEVec2* pPos, AEVec2* pVel, float dir, enum STATE startState);
static void				gameObjInstDestroy(GameObjInst* pInst);

*/

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

	//Creating the black walls 
	/*
	GameObj* pObj;
	SquareMesh(&puzzle.pMeshwall, 0x0099FFFF);
	pObj->pMeshwall;

	//Creating White walls
	SquareMesh(&puzzle.pMeshwall2,0x000099CC);
	pObj->pMeshwall2;
	*/


	/*
	if (AEInputCheckCurr(AEVK_3)) {
		next_state = PUZZLE;
		(!ImportMapDataFromFile("..Assets/Exported.txt"));


	}
	*/
	/*
	//Compute the matrix of the binary map
	AEMtx33 scale, trans;
	AEMtx33Trans(&trans, (-(float)BINARY_MAP_WIDTH / 2), (-(float)BINARY_MAP_HEIGHT / 2));
	AEMtx33Scale(&scale, (float)AEGetWindowWidth() / MAX_MAP_WIDTH, (float)AEGetWindowHeight() / MAX_MAP_HEIGHT);
	AEMtx33Concat(&MapTransform, &scale, &trans);
	*/
}

void puzzle_init()
{
	std::cout << "puzzle:Initialize\n";

	/*------------------------------------------------------------
	INIT PLAYERS
	------------------------------------------------------------*/
	player1.pCoord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 50 };
	player2.pCoord = { player1.pCoord.x + 100.f, player1.pCoord.y };


	/*
	//Binary Map
	plightblue = 0;
	pdarkblue = 0;

	//Create an object instance representing the light blue cell.

	plightblue = gameObjInstCreate(TYPE_OBJECT_EMPTY, 1.0f, 0, 0, 0.0f, STATE_NONE);
	plightblue->flag ^= FLAG_VISIBLE;
	plightblue->flag |= FLAG_NON_COLLIDABLE;

	//Create an object instance representing dark blue cell.

	pdarkblue = gameObjInstCreate(TYPE_OBJECT_COLLISION, 1.0f, 0, 0, 0.0f, STATE_NONE);
	pdarkblue->flag ^= FLAG_VISIBLE;
	pdarkblue->flag |= FLAG_NON_COLLIDABLE;


	int i, j;

	for (i = 0; i < BINARY_MAP_WIDTH; ++i)
		for (j = 0; j < BINARY_MAP_HEIGHT; ++j)
		{


			if (MapData[i][j] == TYPE_OBJECT_COLLISION || MapData[i][j] == TYPE_OBJECT_EMPTY)
			{
				continue;
			}



		}
		*/


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
	MATRIX CALCULATION
	------------------------------------------------------------*/
	// for background
	MatrixCalc(bgPuzzle.transform, bgPuzzle.length, bgPuzzle.height, 0.0f, bgPuzzle.bgCoord);

	// for players 
	MatrixCalc(player1.transform, player1.size, player1.size, 0.f, player1.pCoord);
	MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.pCoord);

	MatrixCalc(puzzle.transform, puzzle.length, puzzle.height, 0.f, puzzle.IVector);

	/*
	//for walls
	GameObj* pObj;

	//Creating the black object
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_OBJECT_EMPTY;
	MatrixCalc(puzzle.transform, puzzle.length , puzzle.length, 0.f, puzzle.wallposition);
	//Creating the enemey1 object
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_OBJECT_COLLISION;
	MatrixCalc(puzzle.transform, puzzle.length, puzzle.length, 0.f, puzzle.wallposition);




*/

	if (CollisionIntersection_Item(player2.pCoord, player2.size, player2.size,
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

	}


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
	AEGfxMeshDraw(bgPuzzle.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	DRAWING PLAYERS
	------------------------------------------------------------*/
	// Drawing player 1
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTransform(player1.transform.m);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);



	// drawing player 2
	AEGfxSetTransform(player2.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);

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

	//Drawing the tile map (the grid)
	//int i, j;
	//AEMtx33 cellTranslation, cellFinalTransformation;
	//for (i = 0; i < BINARY_MAP_WIDTH; ++i)
	//	for (j = 0; j < BINARY_MAP_HEIGHT; ++j)
		//{
		//	AEMtx33Trans(&cellTranslation, i + 0.5f, j + 0.5f);
		//	AEMtx33Concat(&cellFinalTransformation, &MapTransform, &cellTranslation);
		//	AEGfxSetTransform(cellFinalTransformation.m);


			//if (GetCellValue(i, j) == TYPE_OBJECT_EMPTY)
			//{
			//	AEGfxMeshDraw(pBlackInstance->pObject->pMeshwall, AE_GFX_MDM_TRIANGLES);
			//}
			//else if (GetCellValue(i, j) == TYPE_OBJECT_COLLISION)
			//{
			//	AEGfxMeshDraw(pWhiteInstance->pObject->pMeshwall2, AE_GFX_MDM_TRIANGLES);
			//}
		//}

}

void puzzle_free()
{
	std::cout << "puzzle:Free\n";

	AEGfxMeshFree(player1.pMesh);
	AEGfxMeshFree(player2.pMesh);
	AEGfxMeshFree(bgPuzzle.bgMesh);
	AEGfxMeshFree(puzzle.pMesh);
}

void puzzle_unload()
{
	std::cout << "puzzle:Unload\n";

	AEGfxTextureUnload(bgPuzzle.bgTex);
	AEGfxTextureUnload(Key);
	AEGfxTextureUnload(Spiderweb);

}

//Puzzle Mode Binary
/*
int GetCellValue(int X, int Y)
{
	//check if out of bound
	if (X < 0 || Y >= BINARY_MAP_HEIGHT || X >= BINARY_MAP_WIDTH || Y < 0)
	{
		return 0;
	}


	//return 0;
}

int ImportMapDataFromFile(const char* FileName)
{

	std::string line, val; // Strings to store values

	// Make sure there is no garbage values
	BINARY_MAP_HEIGHT = 0;
	BINARY_MAP_WIDTH = 0;
	std::ifstream file(FileName); // Used for Opening File
	if (file.is_open())
	{
		int y = 0; // Y-Coordinate
		while (std::getline(file, line))
		{
			size_t found = line.find_first_of("0123456789"); // to find position of numerical value in the string
			val = line.substr(found);
			if (BINARY_MAP_WIDTH == 0)
			{
				BINARY_MAP_WIDTH = std::stoi(val);
				continue;
			}
			else if (BINARY_MAP_HEIGHT == 0)
			{
				BINARY_MAP_HEIGHT = std::stoi(val);
				continue;
			}

			//check if height and width not equal to 0
			if (BINARY_MAP_HEIGHT > 0 && BINARY_MAP_WIDTH > 0)
			{
				if (!MapData)
				{
					MapData = new int* [BINARY_MAP_WIDTH];
					for (int i{ 0 }; i < BINARY_MAP_WIDTH; ++i)
					{
						MapData[i] = new int[BINARY_MAP_HEIGHT];
					}
				}




				// to store and set MapData
				// BinaryCollisionArray is also set according to the corresponding collision (In this case, 1 in the mapdata)
				int x = 0;
				for (int i = 0; i < line.length(); i++)
				{
					if (line[i] >= '0' && line[i] <= '9')// to make sure is only numerical value before storing
					{
						if (x >= 0 && x < BINARY_MAP_WIDTH && y >= 0 && y < BINARY_MAP_HEIGHT) // make sure is within map
						{
							MapData[x][y] = line[i] - '0';

							x++;
						}
					}
					else
					{
						continue;
					}
				}
				y++;
			}


		}
		file.close();

		return 1; // file exist

	}
	else
	{
		BINARY_MAP_HEIGHT = 0;
		BINARY_MAP_WIDTH = 0;
		return 0; // file doesn't exist

	}
}

GameObjInst* gameObjInstCreate(unsigned int type, float scale,
	AEVec2* pPos, AEVec2* pVel,
	float dir, enum STATE startState)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	AE_ASSERT_PARM(type < sGameObjNum);

	// loop through the object instance list to find a non-used object instance
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sGameObjList + type;
			pInst->flag = FLAG_ACTIVE | FLAG_VISIBLE;


			// return the newly created instance
			return pInst;
		}
	}

	return 0;
}


void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}

void FreeMapData(void)
{


	if (MapData)
	{
		for (int i{ 0 }; i < BINARY_MAP_HEIGHT; ++i)
		{
			delete[] MapData[i];
		}
		delete[] MapData;
	}
}
*/
