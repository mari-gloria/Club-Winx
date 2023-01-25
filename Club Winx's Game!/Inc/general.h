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
};
extern Player player1, player2;


// HANDLES PLAYER INPUT FOR CURRENT GAME STATE ( DURING UPDATE ) //
void input_handle();


/******* CREATE SQUARE MESH  (DURING LOAD) ************\

\param (&pMesh, length(float), height(float), colour(0xAARRGGBB))

add "&" infront of pMesh pointer
length and height in FLOAT ( XX.Xf )
colour in HEX Alpha, Red, Green, Blue

///// REMEMBER TO FREE/UNLOAD POINTER TO MESH \\\\\\\*/
void SquareMesh(AEGfxVertexList** pMesh, f32 length, f32 height, u32 colour);



