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



//declaring structs: BG, player, scoreboard, racingitems, platform, line, boss, health, puzzle
BG			bgRacing, bgPuzzle, bgBoss, bgWin;
BG			winRacing;

Audio		jump;

Player		player1, player2;
ScoreBoard	score_board;

RacingItems racing_items[MAX_NUM_ITEMS];

Platform	platformA[MAX_NUM_PLATFORMS], platformB[MAX_NUM_PLATFORMS];
Line		splitscreen;

//Boss		boss;
Health	 p1health, p2health;
Puzzle puzzle;



//input handle
const float		PLAYER_MOVE{ 8.0f };

const float		GRAVITY{ -8.0f };
const float		PLAYER_JUMP{ 8.0f };

const float		JUMP_HEIGHT_MAX{ 100.0f };





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





bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2)
{
	// testing for static collision
	if (aabb1.max.x > aabb2.min.x ||
		aabb1.max.y > aabb2.min.y ||
		aabb1.min.x < aabb2.max.x ||
		aabb1.min.y < aabb2.max.y) {

		// calculating relative vel
		AEVec2 relVel = { vel2.x - vel1.x, vel2.y - vel1.y };
		float tFirst = 0;
		float tLast = g_dt;

		/*----------------------------
		 working with x-axis
		----------------------------*/
		if (relVel.x <= 0) {

			//case 1
			if (aabb1.min.x > aabb2.max.x) {
				return 0; //aabb2 is moving away
			}

			// case 4 - revisited
			if (aabb1.max.x < aabb2.min.x) {
				float dFirst = aabb1.max.x - aabb2.min.x;
				tFirst = max(dFirst / relVel.x, tFirst);
			}

		}

		if (relVel.x >= 0) {

			// case 2 - revisited
			if (aabb1.min.x > aabb2.max.x) {
				float dFirst = aabb1.min.x - aabb2.max.x;
				tFirst = max(dFirst / relVel.x, tFirst);
			}

			// case 3
			if (aabb1.max.x < aabb2.min.x) {
				return 0;
			}

		}

		// case 5
		if (tFirst > tLast) {
			return 0;
		}



		/*----------------------------
		 working with y-axis
		----------------------------*/
		if (relVel.y <= 0) {

			//case 1
			if (aabb1.min.y > aabb2.max.y) {
				return 0; //aabb2 is moving away
			}

			// case 4 - revisited
			if (aabb1.max.y < aabb2.min.y) {
				float dFirst = aabb1.max.y - aabb2.min.y;
				tFirst = max(dFirst / relVel.y, tFirst);
			}

		}

		if (relVel.y >= 0) {

			// case 2 - revisited
			if (aabb1.min.y > aabb2.max.y) {
				float dFirst = aabb1.min.y - aabb2.max.y;
				tFirst = max(dFirst / relVel.y, tFirst);
			}

			// case 3
			if (aabb1.max.y < aabb2.min.y) {
				return 0;
			}

		}

		// case 5
		if (tFirst > tLast) {
			return 0;
		}

		return 1;
	}

	return 0;
}





COLLISION get_collision_flag(const AABB& aabb1, const AEVec2& vel1, const AABB& aabb2, const AEVec2& vel2)
{

	//player hotspots
	AEVec2 Atop, Abottom;

	Atop.x = (aabb1.min.x + aabb1.max.x) / 2.0f;
	Atop.y = aabb1.max.y;

	Abottom.x = (aabb1.min.x + aabb1.max.x) / 2.0f;
	Abottom.y = aabb1.min.y;

	//platform hotspots
	AEVec2 Btop, Bbottom;

	Btop.x = (aabb2.min.x + aabb2.max.x) / 2.0f;
	Btop.y = aabb2.max.y;

	Bbottom.x = (aabb2.min.x + aabb2.max.x) / 2.0f;
	Bbottom.y = aabb2.min.y;


	if (Atop.y > Bbottom.y && vel1.y > 0)
	{

		return COLLISION_TOP;
	}

	else//if (Abottom.y < Btop.y && vel1.x < 0)
	{
		return COLLISION_BOTTOM;
	}

	return COLLISION_INVALID;
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
			if (player1.pOnSurface && !player1.pJumping) {
				player1.pJumping = true;
				player1.pOnSurface = false;
				AEAudioPlay(jump.audio, jump.aGroup, 0.75, 1, 0);
			}
		}

		//jumping mechanism
		if (player1.pJumping) {
			player1.pVel.y = PLAYER_JUMP;
		}

		else {
			player1.pVel.y = GRAVITY;
		}

		//adding jump limits
		if (player1.pCoord.y <= player1.pGround) {//lower limit
			player1.pCoord.y = player1.pGround;
			player1.pOnSurface = true;
		}


		if (player1.pCoord.y >= player1.pCurrGround + JUMP_HEIGHT_MAX) {//upper limit
			player1.pJumping = false;
		}


		if (AEInputCheckCurr(AEVK_A) && ((player1.pCoord.x - player1.size / 2.0f)) >= AEGfxGetWinMinX()) { //left limit = MinX
			player1.pVel.x = -PLAYER_MOVE;
		}

		else if (AEInputCheckCurr(AEVK_D) && (player1.pCoord.x + player1.size / 2.0f) <= 0) { //right limit = 0 - size
			player1.pVel.x = PLAYER_MOVE;
		}

		else
		{
			player1.pVel.x = 0.0f;
		}


		/*----------------------------------------------------------------------------------
		 player 2 movement controls

		 up -> jump
		 left -> move left
		 right -> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_UP) && player2.pCoord.y <= AEGfxGetWinMaxY() - player2.size) {
			if (player2.pOnSurface && !player2.pJumping) {
				player2.pJumping = true;
				player2.pOnSurface = false;
				AEAudioPlay(jump.audio, jump.aGroup, 0.75, 1, 0);
			}
		}

		//jumping mechanism
		if (player2.pJumping) {
			player2.pVel.y = PLAYER_JUMP;
		}

		else {
			player2.pVel.y = GRAVITY;
		}

		//adding jump limits
		if (player2.pCoord.y <= player2.pGround) {//lower limit
			player2.pCoord.y = player2.pGround;
			player2.pOnSurface = true;
		}


		if (player2.pCoord.y >= player2.pCurrGround + JUMP_HEIGHT_MAX) {//upper limit
			player2.pJumping = false;
		}

		if (AEInputCheckCurr(AEVK_LEFT) && ((player2.pCoord.x - player2.size / 2.0f)) >= 0) {//left limit = MinX
			player2.pVel.x = -PLAYER_MOVE;
		}

		else if (AEInputCheckCurr(AEVK_RIGHT) && (player2.pCoord.x + player2.size / 2.0f) <= AEGfxGetWinMaxX()) {//right limit = 0 - size
			player2.pVel.x = PLAYER_MOVE;
		}

		else {
			player2.pVel.x = 0.0f;
		}

		/*------------------------------------------------------------
		END OF RACING
		------------------------------------------------------------*/
		break;



	case BOSS:
		/*----------------------------------------------------------------------------------
 player 1 movement controls

 W -> move up
 S -> move down
 A -> move left
 D -> move right
-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_W) && player1.pCoord.y <= AEGfxGetWinMaxY() - player1.size)
			player1.pVel.y = PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_S) && player1.pCoord.y >= AEGfxGetWinMinY())
			player1.pVel.y = -PLAYER_MOVE;

		else
			player1.pVel.y = 0;

		if (AEInputCheckCurr(AEVK_A) && player1.pCoord.x >= AEGfxGetWinMinX())
			player1.pVel.x = -PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_D) && player1.pCoord.x <= AEGfxGetWinMaxX() - player1.size)
			player1.pVel.x = PLAYER_MOVE;

		else
			player1.pVel.x = 0;

		/*----------------------------------------------------------------------------------
		 player 2 movement controls

		 up		-> move up
		 down	-> move down
		 left	-> move left
		 right	-> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_UP) && player2.pCoord.y <= AEGfxGetWinMaxY() - player2.size)
			player2.pVel.y = PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_DOWN) && player2.pCoord.y >= AEGfxGetWinMinY())
			player2.pVel.y = -PLAYER_MOVE;

		else
			player2.pVel.y = 0;

		if (AEInputCheckCurr(AEVK_LEFT) && player2.pCoord.x >= AEGfxGetWinMinX())
			player2.pVel.x = -PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_RIGHT) && player2.pCoord.x <= AEGfxGetWinMaxX() - player2.size)
			player2.pVel.x = PLAYER_MOVE;

		else
			player2.pVel.x = 0;


		/*------------------------------------------------------------
		END OF BOSS
		------------------------------------------------------------*/
		break;

	case PUZZLE:
		/*----------------------------------------------------------------------------------
		 player 1 movement controls

		 W -> move up
		 S -> move down
		 A -> move left
		 D -> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_W) && player1.pCoord.y <= AEGfxGetWinMaxY() - player1.size)
			player1.pVel.y = PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_S) && player1.pCoord.y >= AEGfxGetWinMinY())
			player1.pVel.y = -PLAYER_MOVE;

		else
			player1.pVel.y = 0;

		if (AEInputCheckCurr(AEVK_A) && player1.pCoord.x >= AEGfxGetWinMinX())
			player1.pVel.x = -PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_D) && player1.pCoord.x <= AEGfxGetWinMaxX() - player1.size)
			player1.pVel.x = PLAYER_MOVE;

		else
			player1.pVel.x = 0;

		/*----------------------------------------------------------------------------------
		 player 2 movement controls

		 up		-> move up
		 down	-> move down
		 left	-> move left
		 right	-> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_UP) && player2.pCoord.y <= AEGfxGetWinMaxY() - player2.size)
			player2.pVel.y = PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_DOWN) && player2.pCoord.y >= AEGfxGetWinMinY())
			player2.pVel.y = -PLAYER_MOVE;

		else
			player2.pVel.y = 0;

		if (AEInputCheckCurr(AEVK_LEFT) && player2.pCoord.x >= AEGfxGetWinMinX())
			player2.pVel.x = -PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_RIGHT) && player2.pCoord.x <= AEGfxGetWinMaxX() - player2.size)
			player2.pVel.x = PLAYER_MOVE;

		else
			player2.pVel.x = 0;

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

