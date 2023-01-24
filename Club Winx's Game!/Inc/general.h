#pragma once
#include "AEVec2.h"
#include "AEGraphics.h"

struct Player {
	AEVec2 pcoords; // player x y 
	AEGfxVertexList* pMesh; // mesh 
	AEGfxTexture* pTex; // texture

};
Player playerone, playertwo; 