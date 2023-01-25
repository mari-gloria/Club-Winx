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
};


extern Player player1, player2;





