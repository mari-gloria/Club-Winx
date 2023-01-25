// ---------------------------------------------------------------------------
//includes

#include "AEEngine.h"
#include "AEVec2.h"
#include "AEGraphics.h"

#include "general.h"
// ---------------------------------------------------------------------------


Player player1, player2;


void SquareMesh(AEGfxVertexList** pMesh, f32 length, f32 height, u32 colour)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		0.0f, height, colour, 0.0f, 0.0f, // bottom left 
		length, height, colour, 1.0f, 0.0f, // bottom right
		0.0f, 0.0f, colour, 0.0f, 1.0f); //top left
	AEGfxTriAdd(
		length, height, colour, 1.0f, 0.0f, // bottom right
		length, 0.0f, colour, 1.0f, 1.0f, // top right 
		0.0f, 0.0f, colour, 0.0f, 1.0f); // top left 

	*pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh, "Failed to create square mesh !!");
}

