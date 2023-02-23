/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Mariah Tahirah (mariahtahirah.b@digipen.edu)
* Secondary Authors: 
*	Shayne Gloria (m.shayne@digipen.edu) -> Platform of Structures, Split Screen
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu) -> Input handling
*   Yeo Hui Shan (huishan.y@digipen.edu) -> Collision 
*   Amirah Isa (amirah.b@digipen.edu) -> Health, Boss
==================================================================================*/

// ---------------------------------------------------------------------------
//includes
#include "AEVec2.h"
#include "AEGraphics.h"
// ---------------------------------------------------------------------------

/*--------------------------------------------------------------------------
Defines
---------------------------------------------------------------------------*/
extern int const winWIDTH, winHEIGHT;
extern float	g_dt;
extern double	g_appTime;
extern const int JUMP_HEIGHT_MAX;





/*--------------------------------------------------------------------------
Players
---------------------------------------------------------------------------*/

//struct for players 
struct Player { // initialise in each game mode before use 
	AEVec2				pCoord			{ 0.0f, 0.0f };	// player x,y coordinates
	AEGfxVertexList*	pMesh			{ nullptr };	// mesh 
	AEGfxTexture*		pTex			{ nullptr };	// texture
	f32					size			{ 50.0f };		// player size
	AEVec2				pVel			{ 0.0f, 0.0f }; // velocity of player
	f32					pAcceleration	{ 80.0f };		// acceleration of player


	f32					pGround			{ 0.0f };		// y-coord of the ground
	f32					pCurrGround		{ 0.0f };		// y-coord of the current ground/platform
	f32					pPrevGround		{ 0.0f };		// y-coord of the previous ground/platform
	bool				pOnGround		{ true };		// indicate if player stepping on ground/platform
	bool				pJumping		{ false };		// indicate if player is jumping
	f32					maxCurrHeight	{ 0.0f };

	f32					startX			{ 0.0f };		// left x limit
	f32					endX			{ 0.0f };		// right X limit

	bool				collectedItem	{ false };		// indicate if player has collected an item
	bool				usedItem		{ false };		// indicate if item has been used

	AEMtx33				transform		{};				// transform matrix
};
extern Player player1, player2;

struct ScoreBoard {
	AEVec2				sCoord		{ 0.0f, 0.0f };	// score x y (min)
	AEGfxVertexList*	sMesh		{ nullptr };	// mesh 
	AEGfxTexture*		sTex		{ nullptr };	// texture

	f32					length		{ (f32)winWIDTH };
	f32					height		{ 50.0f };						

	int					p1_score	{ 0 };			// player1 score
	int					p2_score	{ 0 };			// player2 score

	int					p1_lives	{ 0 };			// player1 lives
	int					p2_lives	{ 0 };			// player2 lives

	AEMtx33				transform	{};				// transform matrix
};
extern ScoreBoard score_board;





/*--------------------------------------------------------------------------
Items for racing - boost/disadvantage for players
---------------------------------------------------------------------------*/
#define MAX_NUM_ITEMS 10

//item types will be randomly generated
enum ItemType {
	BAD = 0,
	GOOD
};

//Generates a random item type - GOOD/BAD
ItemType rand_item();

struct RacingItems {
	AEVec2				pCoord	{ 0.0f, 0.0f };	// item x,y coordinates
	AEGfxVertexList*	pMesh	{ nullptr };	// mesh 
	AEGfxTexture*		pTex	{ nullptr };	// texture
	f32					size	{ 20.0f };		// item size

	ItemType			itemType	{ rand_item() };

	AEMtx33				transform{};				// transform matrix
};
extern RacingItems racing_items[MAX_NUM_ITEMS];





/*--------------------------------------------------------------------------
Platform
---------------------------------------------------------------------------*/

// Global constant for array for platforms
#define MAX_NUM_PLATFORMS 50

// generic platform details such as length, height, colour
struct Platform {
	AEVec2				platVect		{ 0.0f, 0.0f };	// vector -> initialise platforms x & y coords, which will then be used for randomisation
	AEGfxVertexList*	platMesh		{ nullptr };	// mesh 
	AEGfxTexture*		platTex			{ nullptr };	// texture

	f32					length			{ 145.0f };
	f32					height			{ 15.0f };

	bool				stepped			{ false };
	AEMtx33				transform		{};				// transform matrix

	f32					pAcceleration	{ 0.0f };		// acceleration of platform
};
extern Platform platformA[MAX_NUM_PLATFORMS], platformB[MAX_NUM_PLATFORMS];





/*--------------------------------------------------------------------------
Split Screen
---------------------------------------------------------------------------*/
struct Line {
	AEVec2				lVect		{ 0.0f, 0.0f };	// X & Y points of bottom left
	AEGfxVertexList*	lMesh		{ nullptr };	// mesh 
	AEGfxTexture*		lTex		{ nullptr };	// texture

	f32					length		{ 10.0f };		// length of line - cons
	f32					height		{ 0.0f };		// height of line - cons
	u32					colour		{ 0xFF000000 }; // black

	AEMtx33				transform	{};				// transform mtx 
}; extern Line			splitscreen;





/*--------------------------------------------------------------------------
Boss
---------------------------------------------------------------------------*/
struct Boss { // initialise in each game mode before use 

	AEGfxVertexList*	pMesh1		{ nullptr };			// mesh 
	AEGfxVertexList*	pMesh2		{ nullptr };			// mesh 
	AEGfxTexture*		pTex		{ nullptr };			// texture

	AEVec2				Bcoord		{ 250.0f, -100.0f };	// position of boss
	f32					size		{ 350.0f };				// boss size 

	AEMtx33				transform	{};						// transform mtx 

};
extern Boss boss;

struct Health { // initialise in each game mode before use 

	AEGfxVertexList*	pMesh		{ nullptr };			// mesh    
	AEGfxTexture*		pTex		{ nullptr };			// texture

	AEVec2				Hcoord		{ -35.0f, 300.0f  };
	AEVec2				Hcoord2		{};

	f32					plength		{};
	f32					pheight		{ 10.0f };

	f32					length		{ 1200 }; 
	f32					height		{ 30.0f };

	AEMtx33				transform	{};
};
extern Health health, health2, p1health, p2health;





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
bool CollisionIntersection_RectRect(const AEVec2& A, f32 Alength, f32 Aheight, const AEVec2& B, f32 Blength, f32 Bheight);




/*--------------------------------------------------------------------------
Generates a random number
---------------------------------------------------------------------------*/
int rand_num(int min, int max);
float rand_num(float min, float max);