/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This header file declares the header files and util functions used in the program.
* 
* Primary Author: 
*	Mariah Tahirah (mariahtahirah.b@digipen.edu) -> MatrixCalc(), SquareMesh(), BoudningBox()
* 
* Secondary Authors: 
*	Shayne Gloria (m.shayne@digipen.edu) -> Platform of Structures, Split Screen
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu) -> Input handling, collision functions, draw_textires, rand_int
*   Yeo Hui Shan (huishan.y@digipen.edu) -> Collision 
*   Amirah Isa (amirah.b@digipen.edu) -> Health, Boss
==================================================================================*/

// ---------------------------------------------------------------------------
//includes
#include "AEEngine.h"
#include "AEVec2.h"
#include "AEGraphics.h"
#include "AEGameStateMgr.h"

#include "Boss.h"
#include "Buttons.h"
#include "GameStateList.h"
#include "GSM.h"
#include "MainMenu.h"
#include "Pause.h"
#include "Puzzle.h"
#include "Racing.h"
#include "RacingMap.h"
#include "Splash_screen.h"
#include "Tutorial.h"
#include "WinLose.h"
#include "WinnerState.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>
// ---------------------------------------------------------------------------




// ============================ externs ============================ //

//from main.cpp
extern int const	winLENGTH, winHEIGHT;
extern float		g_dt;
extern double		g_appTime;
extern s8			font_arial, font_pixel;
extern bool			game_paused;

//from general.cpp
extern const float	JUMP_HEIGHT_MAX;
extern const float	GRAVITY;
extern const float	PLAYER_JUMP;

//for game 
extern bool			arcadeMode;

//Tutorial.cpp
extern bool			tut_viewed; 






// ============================ collsion ============================ //
struct AABB
{
	AEVec2 min;
	AEVec2 max;
};

enum COLLISION
{
	COLLISION_INVALID,
	COLLISION_LEFT = 0x00000001,
	COLLISION_RIGHT = 0x00000002,
	COLLISION_TOP = 0x00000004,
	COLLISION_BOTTOM = 0x00000008
};



// ============================ game textures ============================ //
struct Textures {
	AEVec2				coord		{ 0.f, 0.f };// player x y (min)

	AEGfxVertexList*	mesh		{ nullptr };// mesh
	AEGfxTexture*		texture		{ nullptr };// texture
	AEMtx33				transform	{};			// transform matrix

	f32					length		{0.f};// default length of BG
	f32					height		{0.f};// default height of BG (may need to change for racing)

};

extern Textures bgBoss, bgWin, bgTut, winBoss, winRacing;




// ============================ audio ============================ //
struct Audio {
	AEAudio				audio	{ nullptr };
	AEAudioGroup		aGroup	{ nullptr };
};
extern Audio jump, collect, win, lose, mainmenu_bgm, puzzle_bgm, racing_bgm, boss_bgm, collectKey, walk, shoot;




// ============================ timer ============================ //
struct Timer {
	AEVec2				timeleft{ -0.9f, 0.8f };
	f64					minute{ };
	f64					second{};
};
extern Timer puzzleTime, racingTime;






// ============================ players ============================ //
struct Player { 
	AEVec2				coord{ 0.0f, 0.0f };
	AEGfxVertexList*	mesh{ nullptr };
	AEGfxTexture*		texture{ nullptr };

	f32					size{ 50.0f };
	AEVec2				velocity{ 0.0f, 0.0f };
	f32					acceleration{ 40.0f };
	AABB				boundingBox;
	int					flag;


	f32					ground{ 0.0f };			// y-coord of the ground
	f32					currGround{ 0.0f };		// y-coord of the current ground/platform
	bool				onSurface{ true };			// indicate if player stepping on ground/platform
	bool				jumping{ false };			// indicate if player is jumping
	f32					jumpHeightMax{ 100.0f };	// current max jump height of player

	f32					startX{ 0.0f };				// left x limit
	f32					endX{ 0.0f };				// right X limit

	AEMtx33				transform{};			// transform matrix


	f32					HP{ 100.f };			// player health
	bool				alive{ true };			// player alive/dead
};
extern Player player1, player2;





// ============================ racing mode ============================ //
#define MAX_NUM_PLATFORMS 50
#define MAX_NUM_ITEMS 5

struct RacingBoosts {
	AEVec2				coord{ 0.0f, 0.0f };
	AEGfxVertexList*	mesh{ nullptr };
	AEGfxTexture*		texture{ nullptr };

	f32					size{ 20.0f };
	AABB				boundingBox;
	AEVec2				velocity{ 0.0f, 0.0f };

	bool				collected;

	AEMtx33				transform{};
};
extern RacingBoosts racing_boostsA[MAX_NUM_ITEMS], racing_boostsB[MAX_NUM_ITEMS];



struct Platform {
	AEVec2				coord{ 0.0f, 0.0f };
	
	AEGfxVertexList*	mesh{ nullptr };
	AEGfxTexture*		texture{ nullptr };
	AEVec2				velocity{ 0.0f, 0.0f };
	
	AABB				boundingBox;

	f32					length{ 145.0f };
	f32					height{ 15.0f };

	bool				stepped{ false };
	AEMtx33				transform{};

	f32					acceleration{ 0.0f };
};
extern Platform platformA[MAX_NUM_PLATFORMS], platformB[MAX_NUM_PLATFORMS];



struct Line {
	AEVec2				coord{ 0.0f, 0.0f };

	AEGfxVertexList*	mesh{ nullptr };
	AEGfxTexture*		texture{ nullptr };

	f32					length{ 10.0f };
	f32					height{ 0.0f };

	u32					colour{ 0xFF000000 };

	AEMtx33				transform{};
}; 
extern Line	splitscreen, startingline;





// ============================ functions ============================ //
/*--------------------------------------------------------------------------
draws textures from struct Textures
---------------------------------------------------------------------------*/
void draw_texture(Textures texture);



/*--------------------------------------------------------------------------
Updates bounding box 
---------------------------------------------------------------------------*/
void BoundingBoxUpdate(AABB& boundingbox, AEVec2 const& coords, f32 const length, f32 const height);



/*--------------------------------------------------------------------------
Calculates matrix (under update)
---------------------------------------------------------------------------*/
void MatrixCalc(AEMtx33& transform, const f32 length, const f32 height, const f32 direction, const AEVec2& coords);



/*--------------------------------------------------------------------------
Handles input for current game state (under update)
---------------------------------------------------------------------------*/
void input_handle();



/*--------------------------------------------------------------------------
Creates square mesh

\param (&pMesh, length(float), height(float), colour(0xAARRGGBB))

add "&" infront of pMesh pointer
length and height in FLOAT ( XX.Xf )
colour in HEX Alpha, Red, Green, Blue

///// REMEMBER TO FREE/UNLOAD POINTER TO MESH \\\\\\
---------------------------------------------------------------------------*/
void SquareMesh(AEGfxVertexList** pMesh, u32 colour);



/*--------------------------------------------------------------------------
Check for collision
---------------------------------------------------------------------------*/
bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1, const AABB& aabb2, const AEVec2& vel2);
COLLISION get_collision_flag(const AABB& aabb1, const AEVec2& vel1, const AABB& aabb2, const AEVec2& vel2);



/*--------------------------------------------------------------------------
Generates a random number
---------------------------------------------------------------------------*/
int rand_num(int min, int max);
f32 rand_num(f32 min, f32 max);