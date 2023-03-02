/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Mariah Tahirah (mariahtahirah.b@digipen.edu) 
* Secondary Authors:
*	Shayne Gloria (m.shayne@digipen.edu) -> Platform of Structures, Split Screen
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu) -> Input handling, Jump logic
*   Yeo Hui Shan (huishan.y@digipen.edu) -> Collision Intersection
==================================================================================*/

//---------------------------------------------------------------------------
//includes

#include "General.h"
// ---------------------------------------------------------------------------


/*------------------------------------------------------------
Defines
------------------------------------------------------------*/

//delta time, app run time
float	 g_dt;
double	 g_appTime;



//declaring players & platforms
BG			bgRacing, bgPuzzle, bgBoss;
BG			winRacing;

Player		player1, player2;
ScoreBoard	score_board;
RacingItems racing_items[MAX_NUM_ITEMS];

Platform	platformA[MAX_NUM_PLATFORMS], platformB[MAX_NUM_PLATFORMS];
Line		splitscreen;

//Boss		boss;
Health	 p1health, p2health;



//variables for RACING
const float		GRAVITY{ 5.0f };
const int		JUMP_HEIGHT_MAX{ 100 };





void MatrixCalc(AEMtx33 & transform, const f32 length, const f32 height, const f32 direction, const AEVec2& coords)
{
	AEMtx33 trans{}, rot{}, scale{};

	// Compute the scaling matrix
	AEMtx33Scale(&scale, length, height);
	// Compute the rotation matrix 
	AEMtx33Rot(&rot, direction);
	// Compute the translation matrix
	AEMtx33Trans(&trans, coords.x, coords.y);
	// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
	AEMtx33Concat(&transform, &rot, &scale);
	AEMtx33Concat(&transform, &trans, &transform);
}





bool CollisionIntersection_RectRect(const AEVec2& A, f32 Alength, f32 Aheight, const AEVec2& B, f32 Blength, f32 Bheight)
{
	//std::cout << "checkCollision" << std::endl;

	//Player bounding box (min.x = player.x, min.y = player.y)
	AEVec2 Amax, Amin;
	Amax.x = A.x + Alength / 2.f;
	Amax.y = A.y + Aheight / 2.f;

	Amin.x = A.x - Alength / 2.f;
	Amin.y = A.y - Aheight / 2.f;

	//Platform bounding box (minstep.x = platform.x, minstep.y = platform.y)
	AEVec2 Bmax, Bmin;
	Bmax.x = B.x + Blength / 2.f;
	Bmax.y = B.y + Bheight / 2.f;

	Bmin.x = B.x - Blength / 2.f;
	Bmin.y = B.y - Bheight / 2.f;

	bool verticalCollision{ false };
	bool horizontalCollision{ false };

	//Check if player intersect platform vertically
	if (Amin.y >= Bmin.y && Amin.y <= Bmax.y && Amax.x >= Bmin.x && Amin.x <= Bmax.x)
	{
		verticalCollision = true;
		horizontalCollision = true;
	}


	//If player and platform is colliding
	if (verticalCollision == true && horizontalCollision == true)
	{
		//std::cout << "collide true" << std::endl;
		return true;
	}

	return false;
}





void input_handle()
{
	std::cout << "Input:Handle\n";
	switch (curr_state)
	{
	case (RACING):
		/*----------------------------------------------------------------------------------
 player 1 movement controls

 W -> jump
 A -> move left
 D -> move right
-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_W) && player1.pCoord.y <= AEGfxGetWinMaxY() - player1.size) {
			if (player1.pOnGround && !player1.pJumping) {
				player1.pJumping = true;
				player1.pOnGround = false;
			}
		}

		//jumping mechanism
		if (player1.pJumping) {
			player1.pCoord.y += GRAVITY * player1.pAcceleration * g_dt;
		}

		else {
			player1.pCoord.y -= GRAVITY * player1.pAcceleration * g_dt;
		}

		//adding jump limits
		if (player1.pCoord.y <= player1.pGround) {//lower limit
			player1.pCoord.y = player1.pGround;
			player1.pOnGround = true;
		}


		if (player1.pCoord.y >= player1.pPrevGround + JUMP_HEIGHT_MAX) {//upper limit
			player1.pJumping = false;
		}


		if (AEInputCheckCurr(AEVK_A) && ((player1.pCoord.x - player1.size / 2.0f)) >= AEGfxGetWinMinX()) { //left limit = MinX
			player1.pCoord.x -= 3.0f * player1.pAcceleration * g_dt;
		}

		else if (AEInputCheckCurr(AEVK_D) && (player1.pCoord.x + player1.size / 2.0f) <= 0) { //right limit = 0 - size
			player1.pCoord.x += 3.0f * player1.pAcceleration * g_dt;
		}

		/*----------------------------------------------------------------------------------
		 player 2 movement controls

		 up -> jump
		 left -> move left
		 right -> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_UP) && player2.pCoord.y <= AEGfxGetWinMaxY() - player2.size) {
			if (player2.pOnGround && !player2.pJumping) {
				player2.pJumping = true;
				player2.pOnGround = false;
			}
		}

		//jumping mechanism
		if (player2.pJumping) {
			player2.pCoord.y += GRAVITY * player2.pAcceleration * g_dt;
		}

		else {
			player2.pCoord.y -= GRAVITY * player2.pAcceleration * g_dt;
		}

		//adding jump limits
		if (player2.pCoord.y <= player2.pGround) {//lower limit
			player2.pCoord.y = player2.pGround;
			player2.pOnGround = true;
		}


		if (player2.pCoord.y >= player2.pPrevGround + JUMP_HEIGHT_MAX) {//upper limit
			player2.pJumping = false;
		}


		if (AEInputCheckCurr(AEVK_LEFT) && ((player2.pCoord.x - player2.size / 2.0f)) >= 0) //left limit = MinX
			player2.pCoord.x -= 3.0f * player2.pAcceleration * g_dt;

		else if (AEInputCheckCurr(AEVK_RIGHT) && (player2.pCoord.x + player2.size / 2.0f) <= AEGfxGetWinMaxX()) //right limit = 0 - size
			player2.pCoord.x += 3.0f * player2.pAcceleration * g_dt;

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

	case PUZZLE:
		/*----------------------------------------------------------------------------------
		 player 1 movement controls

		 W -> jump
		 A -> move left
		 D -> move right
		-----------------------------------------------------------------------------------*/
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
		END OF PUZZLE
		------------------------------------------------------------*/


	case RESTART:
		break;

	case QUIT:
		break;

	default:
		AE_FATAL_ERROR("Invalid state for input handling!");
	}
}





void SquareMesh(AEGfxVertexList** pMesh, u32 colour)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, colour, 0.0f, 1.0f,
		0.5f, -0.5f, colour, 1.0f, 1.0f,
		-0.5f, 0.5f, colour, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, colour, 1.0f, 1.0f,
		0.5f, 0.5f, colour, 1.0f, 0.0f,
		-0.5f, 0.5f, colour, 0.0f, 0.0f);


	*pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh, "Failed to create square mesh !!");
}





int rand_num(int min, int max)
{
	int r = (int)rand() / (int)RAND_MAX;
	return min + r * (max - min);
}

float rand_num(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

ItemType rand_item() {
	int temp = rand_num(0, 1);
	return temp ? GOOD : BAD;
}