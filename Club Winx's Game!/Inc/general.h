#pragma once
// ---------------------------------------------------------------------------
//includes
#include "AEVec2.h"
#include "AEGraphics.h"
// ---------------------------------------------------------------------------


struct Player { // initialise in each game mode before use 
	AEVec2 pCoord { 0.0f, 0.0f }; // player x y 
	AEGfxVertexList* pMesh { nullptr}; // mesh 
	AEGfxTexture* pTex {nullptr}; // texture
	f32 size = 50.0f; //player size 
	f32 pGround{ 0.0f }; //y-coord of the ground
};
extern Player player1, player2;

struct Platform_init {
	AEVec2 PCoord{ 0.0f, 0.0f }; // initialise platforms x & y coords, which will then be used for randomisation
	AEGfxVertexList* PMesh{ nullptr }; // mesh 
	AEGfxTexture* PTex{ nullptr }; // texture
	f32 length = 100.0f; // length of platform - cons
	f32 height = 25.0f; // height of platform - cons
	f32 Pspawn{ 0.75f }; //y-coord of spawn
	u32 colour = 0xFFFFFF00; // colour of platform
}; 
extern Platform_init platform1, platform2;


// HANDLES PLAYER INPUT FOR CURRENT GAME STATE ( DURING UPDATE ) //
void input_handle();


/******* CREATE SQUARE MESH  (DURING LOAD) ************\

\param (&pMesh, length(float), height(float), colour(0xAARRGGBB))

add "&" infront of pMesh pointer
length and height in FLOAT ( XX.Xf )
colour in HEX Alpha, Red, Green, Blue

///// REMEMBER TO FREE/UNLOAD POINTER TO MESH \\\\\\\*/
void SquareMesh(AEGfxVertexList** pMesh, f32 length, f32 height, u32 colour);



