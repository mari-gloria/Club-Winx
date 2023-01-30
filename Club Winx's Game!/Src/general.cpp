/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: NIL
* Secondary Authors:
*	Shayne Gloria (m.shayne@digipen.edu) -> Platform of Structures, Split Screen
*
==================================================================================*/

//---------------------------------------------------------------------------
//includes

#include "AEEngine.h"
#include "AEVec2.h"
#include "AEGraphics.h"

#include "general.h"
#include <iostream>
#include "gsm.h"
#include "gamestatelist.h"
// ---------------------------------------------------------------------------


/*------------------------------------------------------------
GLOBALS
------------------------------------------------------------*/

//declaring players & platforms
Player player1, player2;
Platform_init main_platform;
Platform_details platformA[platform_max], platformB[platform_max];
Line splitscreen;
Boss boss;
Health health;

//variables for RACING
const float GRAVITY{ 5.0f };
const int JUMP_HEIGHT_MAX{ 70 };
bool p1_jumping = false;
bool p1_on_ground = true;
bool p2_jumping = false;
bool p2_on_ground = true;

bool CollisionIntersection_RectRect(const AEVec2& A, f32 Alength, f32 Aheight, const AEVec2& B, f32 Blength, f32 Bheight)
{
	//std::cout << "checkCollision" << std::endl;

	//Player bounding box (min.x = player.x, min.y = player.y)
	AEVec2 Amax;
	Amax.x = A.x + Alength;
	Amax.y = A.y + Aheight;

	//Platform bounding box (minstep.x = platform.x, minstep.y = platform.y)
	AEVec2 Bmax;
	Bmax.x = B.x + Blength;
	Bmax.y = B.y + Bheight;

	bool verticalCollision{ false };
	bool horizontalCollision{ false };

	//Check if player intersect platform vertically
	if (A.y >= B.y && A.y <= Bmax.y && Amax.x >= B.x && A.x <= Bmax.x)
	{
		verticalCollision = true;
		horizontalCollision = true;
	}
	/*if (player.y >= platform.y) {
		verticalCollision = true;
		std::cout << "vertical true" << std::endl;
	}
	else {
		verticalCollision = false;
		//std::cout << "vertical false" << std::endl;
	}

	if (verticalCollision == true) {
		if (player.y > maxstep.y) {
			toLand = true;
		}
	}

	//Check if player intersect platform horizontally
	if (max.x < platform.x || player.x > maxstep.x) {
		horizontalCollision = false;
		std::cout << "horizontal false" << std::endl;
	}
	else {
		horizontalCollision = true;
	}

	//If player and platform is colliding*/
	if (verticalCollision == true && horizontalCollision == true)
	{
		//return true;
		//std::cout << "collide true" << std::endl;
		return true;
	}

	return false;
	//std::cout << "collide false" << std::endl;
}

void input_handle()
{
	std::cout << "Input:Handle\n";
	switch (curr_state)
	{
	case (RACING):
		// ----------------------------------------------------------------------------------
		// player one movement controls
		//
		// W -> jump
		// A -> move left
		// D -> move right
		//-----------------------------------------------------------------------------------
		if (AEInputCheckCurr(AEVK_W) && player1.pCoord.y <= AEGfxGetWinMaxY() - player1.size) {
			//if (p1_on_ground && !p1_jumping) {
			if (player1.stepping && !p1_jumping) {
				p1_jumping = true;
				//p1_on_ground = false;
				player1.stepping = false;
			}
		}

		//jumping mechanism
		if (p1_jumping) {
			player1.pCoord.y += GRAVITY;
		}

		else {
			player1.pCoord.y -= GRAVITY;
		}

		//adding jump limits
		if (player1.pCoord.y <= player1.pGround) {//lower limit
			player1.pCoord.y = player1.pGround;
			//p1_on_ground = true;
			player1.stepping = true;
		}

		if (player1.pCoord.y >= player1.pCurrGround + JUMP_HEIGHT_MAX) {//upper limit
			p1_jumping = false;
		}


		if (AEInputCheckCurr(AEVK_A) && player1.pCoord.x >= AEGfxGetWinMinX()) //left limit = MinX
			player1.pCoord.x -= 3.0f;

		else if (AEInputCheckCurr(AEVK_D) && player1.pCoord.x <= -player1.size) //right limit = 0 - size
			player1.pCoord.x += 3.0f;


		// ----------------------------------------------------------------------------------
		// player one movement controls
		//
		// up -> jump
		// left -> move left
		// right -> move right
		//-----------------------------------------------------------------------------------
		if (AEInputCheckCurr(AEVK_UP) && player2.pCoord.y <= AEGfxGetWinMaxY() - player2.size) {
			//if (p2_on_ground && !p2_jumping) {
			if (player2.stepping && !p2_jumping) {
				p2_jumping = true;
				//p2_on_ground = false;
				player2.stepping = false;
			}
		}

		//jumping mechanism
		if (p2_jumping) {
			player2.pCoord.y += GRAVITY;
		}

		else {
			player2.pCoord.y -= GRAVITY;
		}

		//adding jump limits
		if (player2.pCoord.y <= player2.pGround) {//lower limit
			player2.pCoord.y = player2.pGround;
			//p2_on_ground = true;
			player2.stepping = true;
		}

		if (player2.pCoord.y >= player2.pCurrGround + JUMP_HEIGHT_MAX) {//upper limit
			p2_jumping = false;
		}


		if (AEInputCheckCurr(AEVK_LEFT) && player2.pCoord.x >= 0) //left limit = 0
			player2.pCoord.x -= 3.0f;

		else if (AEInputCheckCurr(AEVK_RIGHT) && player2.pCoord.x <= AEGfxGetWinMaxX() - player2.size) //right limit is = MaxX - size
			player2.pCoord.x += 3.0f;

		/*------------------------------------------------------------
		END OF RACING
		------------------------------------------------------------*/
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


		/*------------------------------------------------------------
		END OF BOSS
		------------------------------------------------------------*/
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