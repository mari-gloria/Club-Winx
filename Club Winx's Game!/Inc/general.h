#pragma once
// ---------------------------------------------------------------------------
//includes
#include "AEVec2.h"
#include "AEGraphics.h"
// ---------------------------------------------------------------------------

// Global constant for array for platforms
#define platform_max 17

// ---------------------------------------------------------------------------
// INITALISATION FOR PLAYERS
// ---------------------------------------------------------------------------
struct Player { // initialise in each game mode before use 
	AEVec2 pCoord { 0.0f, 0.0f }; // player x y 
	AEGfxVertexList* pMesh { nullptr}; // mesh 
	AEGfxTexture* pTex {nullptr}; // texture
	f32 size = 50.0f; //player size 
	f32 pGround{ 0.0f }; //y-coord of the ground
	f32 start = 0.0f , end = 0.0f;
};
extern Player player1, player2;

// ---------------------------------------------------------------------------
// PLATFORM STRUCTURES AND DECLARATION OF VARIABLES
// ---------------------------------------------------------------------------

// generic platform details such as length, height, colour
struct Platform_init {
	f32 length = 110.0f; // length of platform - cons
	f32 height = 27.0f; // height of platform - cons
	u32 colour = 0xFFFFFF00; // colour of platform
}; 
extern Platform_init main_platform;

struct Platform_details {
	AEVec2 platVect{ 0.0f, 0.0f }; // vector -> initialise platforms x & y coords, which will then be used for randomisation
	f32 platY{ 0.0f }; // y-coord of platform spawn
	f32 platX{ 0.0f }; // x-coord of platform spawn
	AEGfxVertexList* platMesh{ nullptr }; // mesh 
	AEGfxTexture* platTex{ nullptr }; // texture
};
extern Platform_details platformA[platform_max], platformB[platform_max];


// ---------------------------------------------------------------------------
// SPLIT SCREEN
// ---------------------------------------------------------------------------
struct Line {
	AEVec2 lVect{ 0.0f, 0.0f }; // X & Y points of bottom left
	AEGfxVertexList* lMesh{ nullptr }; // mesh 
	AEGfxTexture* lTex{ nullptr }; // texture
	f32 length = 18.0f; // length of line - cons
	f32 height{ 0.0f }; // height of line - cons
	//f32 height = 100.0f;
	u32 colour = 0xFF000000; // black
}; extern Line splitscreen;


// HANDLES PLAYER INPUT FOR CURRENT GAME STATE ( DURING UPDATE ) //
void input_handle();


/******* CREATE SQUARE MESH  (DURING LOAD) ************\

\param (&pMesh, length(float), height(float), colour(0xAARRGGBB))

add "&" infront of pMesh pointer
length and height in FLOAT ( XX.Xf )
colour in HEX Alpha, Red, Green, Blue

///// REMEMBER TO FREE/UNLOAD POINTER TO MESH \\\\\\\*/
void SquareMesh(AEGfxVertexList** pMesh, f32 length, f32 height, u32 colour);



