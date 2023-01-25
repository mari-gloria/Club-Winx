// ---------------------------------------------------------------------------
//includes

#include "AEEngine.h"
#include "AEVec2.h"
#include "AEGraphics.h"

#include "general.h"
#include <iostream>
#include "gsm.h"
#include "gamestatelist.h"
// ---------------------------------------------------------------------------


Player player1, player2;

void input_handle()
{
	std::cout << "Input:Handle\n";
	switch (curr_state)
	{
	case (RACING):
		///// IMPLEMENT JUMP MECHANISM HERE \\\\
		// player one input
		if (AEInputCheckCurr(AEVK_W) && player1.pCoord.y <= AEGfxGetWinMaxY() - player1.size)
			player1.pCoord.y += 3.0f;

		else if (AEInputCheckCurr(AEVK_S) && player1.pCoord.y >= AEGfxGetWinMinY())
			player1.pCoord.y -= 3.0f;

		if (AEInputCheckCurr(AEVK_A) && player1.pCoord.x >= AEGfxGetWinMinX())
			player1.pCoord.x -= 3.0f;

		else if (AEInputCheckCurr(AEVK_D) && player1.pCoord.x <= AEGfxGetWinMaxX() - player1.size)
			player1.pCoord.x += 3.0f;

		//player two input
		if (AEInputCheckCurr(AEVK_UP) && player2.pCoord.y <= AEGfxGetWinMaxY() - player2.size)
			player2.pCoord.y += 3.0f;

		else if (AEInputCheckCurr(AEVK_DOWN) && player2.pCoord.y >= AEGfxGetWinMinY())
			player2.pCoord.y -= 3.0f;

		if (AEInputCheckCurr(AEVK_LEFT) && player2.pCoord.x >= AEGfxGetWinMinX())
			player2.pCoord.x -= 3.0f;

		else if (AEInputCheckCurr(AEVK_RIGHT) && player2.pCoord.x <= AEGfxGetWinMaxX() - player2.size)
			player2.pCoord.x += 3.0f;
		break;
	case BOSS:
		// player one input
		if (AEInputCheckCurr(AEVK_W) && player1.pCoord.y <= AEGfxGetWinMaxY() - player1.size)
			player1.pCoord.y += 3.0f;

		else if (AEInputCheckCurr(AEVK_S) && player1.pCoord.y >= AEGfxGetWinMinY())
			player1.pCoord.y -= 3.0f;

		if (AEInputCheckCurr(AEVK_A) && player1.pCoord.x >= AEGfxGetWinMinX())
			player1.pCoord.x -= 3.0f;

		else if (AEInputCheckCurr(AEVK_D) && player1.pCoord.x <= AEGfxGetWinMaxX() - player1.size)
			player1.pCoord.x += 3.0f;

		//player two input
		if (AEInputCheckCurr(AEVK_UP) && player2.pCoord.y <= AEGfxGetWinMaxY() - player2.size)
			player2.pCoord.y += 3.0f;

		else if (AEInputCheckCurr(AEVK_DOWN) && player2.pCoord.y >= AEGfxGetWinMinY())
			player2.pCoord.y -= 3.0f;

		if (AEInputCheckCurr(AEVK_LEFT) && player2.pCoord.x >= AEGfxGetWinMinX())
			player2.pCoord.x -= 3.0f;

		else if (AEInputCheckCurr(AEVK_RIGHT) && player2.pCoord.x <= AEGfxGetWinMaxX() - player2.size)
			player2.pCoord.x += 3.0f;
		break;
	case RESTART:
		break;
	case QUIT:
		break;
	default:
		AE_FATAL_ERROR("Invalid state for input handling!");
	
	}
}

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

