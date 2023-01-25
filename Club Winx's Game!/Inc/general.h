#pragma once
// ---------------------------------------------------------------------------
//includes
#include "AEVec2.h"
#include "AEGraphics.h"
// ---------------------------------------------------------------------------



struct Player {
	AEVec2 pCoord; // player x y 
	AEGfxVertexList* pMesh; // mesh 
	AEGfxTexture* pTex; // texture
	//f32 size; //player size 
};


extern Player player1, player2;

/******* CREATE SQUARE MESH  (DURING LOAD) ************\

\param (&pMesh, length(float), height(float), colour(0xAARRGGBB))

add "&" infront of pMesh pointer
length and height in FLOAT ( XX.Xf )
colour in HEX Alpha, Red, Green, Blue

///// REMEMBER TO FREE/UNLOAD POINTER TO MESH \\\\\\\*/
void SquareMesh(AEGfxVertexList** pMesh, f32 length, f32 height, u32 colour);



