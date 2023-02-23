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
BCKGROUND TEXTURE
---------------------------------------------------------------------------*/
struct BG {
	//AEVec2				bgCoord		{ AEGfxGetWinMinX(), AEGfxGetWinMinY() };	// player x y (min)
	AEVec2				bgCoord		{ 0.f, 0.f };	// player x y (min)
	AEGfxVertexList*	bgMesh		{ nullptr };	// mesh
	AEGfxTexture*		bgTex		{ nullptr };	// texture
	AEMtx33				transform	{};				// transform matrix
	//f32					length		{AEGfxGetWinMaxX() - AEGfxGetWinMinX()}; // default length of BG
	//f32					height		{AEGfxGetWinMaxY() - AEGfxGetWinMinY()}; // default height of BG (may need to change for racing)
	f32					length		{0.f}; // default length of BG
	f32					height		{0.f}; // default height of BG (may need to change for racing)

};
extern BG bgRacing, bgPuzzle, bgBoss;





/*--------------------------------------------------------------------------
PLAYERS
---------------------------------------------------------------------------*/

//struct for players 
struct Player { // initialise in each game mode before use 
	AEVec2				pCoord		{ 0.0f, 0.0f };	// player x y (min)
	AEGfxVertexList*	pMesh		{ nullptr };	// mesh 
	AEGfxTexture*		pTex		{ nullptr };	// texture
	f32					size		{50.0f};		// player size 
	f32					pGround		{ 0.0f };		// y-coord of the ground
	f32					pCurrGround	{ 0.0f };		// y-coord of the current ground player is on
	f32					startX		{ 0.0f };		// left x limit
	f32					endX		{ 0.0f };		// right X limit
	bool				stepping	{ true };		// is player stepping on smth 
	AEMtx33				transform	{};				// transform matrix
};
extern Player player1, player2;





/*--------------------------------------------------------------------------
PLATFORM STRUCTURES AND DECLARATION OF VARIABLES
---------------------------------------------------------------------------*/

// Global constant for array for platforms
#define platform_max 17

// generic platform details such as length, height, colour
struct Platform {
	AEVec2				platVect	{ 0.0f, 0.0f };	// vector -> initialise platforms x & y coords, which will then be used for randomisation
	AEGfxVertexList*	platMesh	{ nullptr };	// mesh 
	AEGfxTexture*		platTex		{ nullptr };	// texture
	bool				stepped		{ false };
	AEMtx33				transform	{};				// transform matrix
	f32					length		{ 100.0f };
	f32					height		{ 30.0f };
};
extern Platform platformA[platform_max], platformB[platform_max];





/*--------------------------------------------------------------------------
SPLIT SCREEN
---------------------------------------------------------------------------*/
struct Line {
	AEVec2				lVect		{ 0.0f, 0.0f };	// X & Y points of bottom left
	AEGfxVertexList*	lMesh		{ nullptr };	// mesh 
	AEGfxTexture*		lTex		{ nullptr };	// texture
	f32					length		{ 18.0f };		// length of line - cons
	f32					height		{ 0.0f };		// height of line - cons
	//f32 height = 100.0f;
	u32					colour		{ 0xFF000000 }; // black
	AEMtx33				transform	{};				// transform mtx 
}; extern Line			splitscreen;





/*--------------------------------------------------------------------------
BOSS
---------------------------------------------------------------------------*/
struct Boss { // initialise in each game mode before use 

	AEGfxVertexList*	pMesh1		{ nullptr };			// mesh 
	AEGfxVertexList*	pMesh2		{ nullptr };			// mesh 
	AEGfxTexture*		pTex		{ nullptr };			// texture
	AEMtx33				transform	{};						// transform mtx 
	AEVec2				Bcoord		{ 380.0f, -30.0f };	// position of boss
	f32 length = 250.0f; //boss length 
	f32 height = 450.f; // boss height
};
extern Boss boss;

struct Health { // initialise in each game mode before use 

	AEGfxVertexList*	pMesh		{ nullptr };			// mesh    
	AEGfxTexture*		pTex		{ nullptr };			// texture
	AEMtx33				transform	{};
	AEVec2				Hcoord		{ -35.0f, 300.0f  };
	AEVec2				Hcoord2		{};
	f32					plength		{};
	f32					pheight		{ 10.0f };
	f32					length		{ 1200 }; 
	f32					height		{ 30.0f };
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
Generates a random float
---------------------------------------------------------------------------*/
float rand_float(float min, float max);