/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This source file defines the util functions used in the progrm.
* 
* Primary Author: 
*	Mariah Tahirah (mariahtahirah.b@digipen.edu) -> MatrixCalc(), SquareMesh(), BoudningBox(), boss input handle
* 
* Secondary Authors:
*	Shayne Gloria (m.shayne@digipen.edu) -> Platform of Structures, Split Screen
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu) -> Input handling, jump logic, collision intersection, collision flag, draw_texture(), rand_int()
*   Yeo Hui Shan (huishan.y@digipen.edu) -> Timer
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
Textures	bgBoss, bgWin, bgTut, winBoss, winRacing;
Audio		jump, collect, win, lose, mainmenu_bgm, puzzle_bgm, racing_bgm, boss_bgm, collectKey, walk, shoot;
Timer		puzzleTime, racingTime;
Player		player1, player2;

//racing mode
RacingBoosts racing_boostsA[MAX_NUM_ITEMS], racing_boostsB[MAX_NUM_ITEMS];
Platform	platformA[MAX_NUM_PLATFORMS], platformB[MAX_NUM_PLATFORMS];
Line		splitscreen, startingline;





//input handle
const float		PLAYER_MOVE{ 8.0f };
const float		PUZZLE_PLAYER_MOVE{ 0.3F }; // cannot go more than 0.3f 
const float		GRAVITY{ -8.0f };
const float		PLAYER_JUMP{ 8.0f };

const float		JUMP_HEIGHT_MAX{ 100.0f };
const float		PLAYER_MOVE_MAX{ 100.0f };

bool			playerWalking = { false };





void draw_texture(Textures texture)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransform(texture.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(texture.texture, 0, 0);
	AEGfxMeshDraw(texture.mesh, AE_GFX_MDM_TRIANGLES);
}





void BoundingBoxUpdate(AABB & boundingbox, AEVec2 const &coords, f32 const length, f32 const height)
{
	boundingbox.min.x = coords.x - length / 2.0f;
	boundingbox.min.y = coords.y - height / 2.0f;
	boundingbox.max.x = coords.x + length / 2.0f;
	boundingbox.max.y = coords.y + height / 2.0f;
}





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
	AEVec2 Atop{}, Abottom{};

	Atop.x = (aabb1.min.x + aabb1.max.x) / 2.0f;
	Atop.y = aabb1.max.y;

	Abottom.x = (aabb1.min.x + aabb1.max.x) / 2.0f;
	Abottom.y = aabb1.min.y;

	//platform hotspots
	AEVec2 Btop{}, Bbottom{};

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
	switch (curr_state)
	{
	case (RACING):
		/*----------------------------------------------------------------------------------
		 player 1 movement controls

		 W -> jump
		 A -> move left
		 D -> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_W) && player1.coord.y <= AEGfxGetWinMaxY() - player1.size) {
			if (player1.onSurface && !player1.jumping) {
				player1.jumping = true;
				player1.onSurface = false;
				AEAudioPlay(jump.audio, jump.aGroup, 0.75, 1, 0);
			}
		}

		//jumping mechanism
		if (player1.jumping)
		{
			player1.velocity.y = PLAYER_JUMP;
		}

		else if (!player1.jumping && !player1.onSurface)
		{
			player1.velocity.y = GRAVITY;
		}

		else
		{
			player1.velocity.y = 0;
		}

		//adding jump limits
		if (player1.coord.y <= player1.ground) {//lower limit
			player1.coord.y = player1.ground;
			player1.onSurface = true;
		}


		if (player1.coord.y >= player1.currGround + player1.jumpHeightMax) {//upper limit
			player1.jumping = false;
			player1.onSurface = false;
		}


		if (AEInputCheckCurr(AEVK_A) && ((player1.coord.x - player1.size / 2.0f)) >= AEGfxGetWinMinX()) { //left limit = MinX
			player1.velocity.x = -PLAYER_MOVE;
		}

		else if (AEInputCheckCurr(AEVK_D) && (player1.coord.x + player1.size / 2.0f) <= 0) { //right limit = 0 - size
			player1.velocity.x = PLAYER_MOVE;
		}

		else
		{
			player1.velocity.x = 0.0f;
		}


		/*----------------------------------------------------------------------------------
		 player 2 movement controls

		 up -> jump
		 left -> move left
		 right -> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_UP) && player2.coord.y <= AEGfxGetWinMaxY() - player2.size) {
			if (player2.onSurface && !player2.jumping) {
				player2.jumping = true;
				player2.onSurface = false;
				AEAudioPlay(jump.audio, jump.aGroup, 0.75, 1, 0);
			}
		}

		//jumping mechanism
		if (player2.jumping)
		{
			player2.velocity.y = PLAYER_JUMP;
		}

		else if (!player2.jumping && !player2.onSurface)
		{
			player2.velocity.y = GRAVITY;
		}

		else
		{
			player2.velocity.y = 0;
		}

		//adding jump limits
		if (player2.coord.y <= player2.ground) {//lower limit
			player2.coord.y = player2.ground;
			player2.onSurface = true;
		}


		if (player2.coord.y >= player2.currGround + player2.jumpHeightMax) {//upper limit
			player2.jumping = false;
			player2.onSurface = false;
		}

		if (AEInputCheckCurr(AEVK_LEFT) && ((player2.coord.x - player2.size / 2.0f)) >= 0) {//left limit = MinX
			player2.velocity.x = -PLAYER_MOVE;
		}

		else if (AEInputCheckCurr(AEVK_RIGHT) && (player2.coord.x + player2.size / 2.0f) <= AEGfxGetWinMaxX()) {//right limit = 0 - size
			player2.velocity.x = PLAYER_MOVE;
		}

		else {
			player2.velocity.x = 0.0f;
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
		
		if (AEInputCheckCurr(AEVK_W))
		{
			player1.velocity.y += player1.acceleration * PLAYER_MOVE * g_dt;
			player1.velocity.y *= (f32)0.99; //simulate friction
		}

		if (AEInputCheckCurr(AEVK_S) && player1.coord.y >= AEGfxGetWinMinY())
		{
			player1.velocity.y += player1.acceleration * -PLAYER_MOVE * g_dt;
			player1.velocity.y *= (f32)0.99; //simulate friction
		}

		if (AEInputCheckCurr(AEVK_A) && player1.coord.x >= AEGfxGetWinMinX())
		{
			player1.velocity.x += player1.acceleration * -PLAYER_MOVE * g_dt;
			player1.velocity.x *= (f32)0.99; //simulate friction
		}

		else if (AEInputCheckCurr(AEVK_D) && player1.coord.x <= AEGfxGetWinMaxX() - player1.size)
		{
			player1.velocity.x += player1.acceleration * PLAYER_MOVE * g_dt;
			player1.velocity.x *= (f32)0.99; //simulate friction
		}
		// wrappers 
		player1.coord.x = AEWrap(player1.coord.x, AEGfxGetWinMinX() - player1.size, AEGfxGetWinMaxX() + player1.size);
		player1.coord.y = AEWrap(player1.coord.y, AEGfxGetWinMinY() - player1.size, AEGfxGetWinMaxY() + player1.size);

		//speed limiters
		if (player1.velocity.x >= PLAYER_MOVE_MAX)
		{
			player1.velocity.x = PLAYER_MOVE_MAX;
		}
		if (player1.velocity.y >= PLAYER_MOVE_MAX)
		{
			player1.velocity.y = PLAYER_MOVE_MAX;

		}
		if (player1.velocity.x <= -PLAYER_MOVE_MAX)
		{
			player1.velocity.x = -PLAYER_MOVE_MAX;
		}
		if (player1.velocity.y <= -PLAYER_MOVE_MAX)
		{
			player1.velocity.y = -PLAYER_MOVE_MAX;

		}
		

		/*----------------------------------------------------------------------------------
		 player 2 movement controls

		 up		-> move up
		 down	-> move down
		 left	-> move left
		 right	-> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_UP))
		{
			player2.velocity.y += player2.acceleration * PLAYER_MOVE * g_dt;
			player2.velocity.y *= (f32)0.99; //simulate friction
		}

		if (AEInputCheckCurr(AEVK_DOWN) && player2.coord.y >= AEGfxGetWinMinY())
		{
			player2.velocity.y += player2.acceleration * -PLAYER_MOVE * g_dt;
			player2.velocity.y *= (f32)0.99; //simulate friction
		}

		if (AEInputCheckCurr(AEVK_LEFT) && player2.coord.x >= AEGfxGetWinMinX())
		{
			player2.velocity.x += player2.acceleration * -PLAYER_MOVE * g_dt;
			player2.velocity.x *= (f32)0.99; //simulate friction
		}

		else if (AEInputCheckCurr(AEVK_RIGHT) && player2.coord.x <= AEGfxGetWinMaxX() - player2.size)
		{
			player2.velocity.x += player2.acceleration * PLAYER_MOVE * g_dt;
			player2.velocity.x *= (f32)0.99; //simulate friction
		}
		// wrappers 
		player2.coord.x = AEWrap(player2.coord.x, AEGfxGetWinMinX() - player2.size, AEGfxGetWinMaxX() + player2.size);
		player2.coord.y = AEWrap(player2.coord.y, AEGfxGetWinMinY() - player2.size, AEGfxGetWinMaxY() + player2.size);

		//speed limiters
		if (player2.velocity.x >= PLAYER_MOVE_MAX)
		{
			player2.velocity.x = PLAYER_MOVE_MAX;
		}
		if (player2.velocity.y >= PLAYER_MOVE_MAX)
		{
			player2.velocity.y = PLAYER_MOVE_MAX;

		}
		if (player2.velocity.x <= -PLAYER_MOVE_MAX)
		{
			player2.velocity.x = -PLAYER_MOVE_MAX;
		}
		if (player2.velocity.y <= -PLAYER_MOVE_MAX)
		{
			player2.velocity.y = -PLAYER_MOVE_MAX;

		}
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
		if (AEInputCheckCurr(AEVK_W) && player1.coord.y <= AEGfxGetWinMaxY() - player1.size)
			player1.velocity.y = PUZZLE_PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_S) && player1.coord.y >= AEGfxGetWinMinY())
			player1.velocity.y = -PUZZLE_PLAYER_MOVE;

		else
			player1.velocity.y = 0;

		if (AEInputCheckCurr(AEVK_A) && player1.coord.x >= AEGfxGetWinMinX())
			player1.velocity.x = -PUZZLE_PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_D) && player1.coord.x <= AEGfxGetWinMaxX() - player1.size)
			player1.velocity.x = PUZZLE_PLAYER_MOVE;

		else
			player1.velocity.x = 0;

		/*----------------------------------------------------------------------------------
		 player 2 movement controls

		 up		-> move up
		 down	-> move down
		 left	-> move left
		 right	-> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_UP) && player2.coord.y <= AEGfxGetWinMaxY() - player2.size)
			player2.velocity.y = PUZZLE_PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_DOWN) && player2.coord.y >= AEGfxGetWinMinY())
			player2.velocity.y = -PUZZLE_PLAYER_MOVE;

		else
			player2.velocity.y = 0;

		if (AEInputCheckCurr(AEVK_LEFT) && player2.coord.x >= AEGfxGetWinMinX())
			player2.velocity.x = -PUZZLE_PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_RIGHT) && player2.coord.x <= AEGfxGetWinMaxX() - player2.size)
			player2.velocity.x = PUZZLE_PLAYER_MOVE;

		else
			player2.velocity.x = 0;

		/*
		if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_D) ||
			AEInputCheckCurr(AEVK_UP) || AEInputCheckCurr(AEVK_DOWN) || AEInputCheckCurr(AEVK_LEFT) || AEInputCheckCurr(AEVK_RIGHT))
		{
			AEAudioPlay(walk.audio, walk.aGroup, 1, 1, 0);
		}*/

		/*------------------------------------------------------------
		END OF PUZZLE
		------------------------------------------------------------*/
		break;
	case TUT:
		/*----------------------------------------------------------------------------------
		 player 1 movement controls

		 W -> move up
		 S -> move down
		 A -> move left
		 D -> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_W) && player1.coord.y <= AEGfxGetWinMaxY() - player1.size / 2.f)
			player1.velocity.y = PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_S) && player1.coord.y >= AEGfxGetWinMinY() + player1.size / 2.f)
			player1.velocity.y = -PLAYER_MOVE;

		else
			player1.velocity.y = 0;

		if (AEInputCheckCurr(AEVK_A) && player1.coord.x >= AEGfxGetWinMinX() + player1.size / 2.f)
			player1.velocity.x = -PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_D) && player1.coord.x <= AEGfxGetWinMaxX() - player1.size/ 2.f)
			player1.velocity.x = PLAYER_MOVE;

		else
			player1.velocity.x = 0;

		/*----------------------------------------------------------------------------------
		 player 2 movement controls

		 up		-> move up
		 down	-> move down
		 left	-> move left
		 right	-> move right
		-----------------------------------------------------------------------------------*/
		if (AEInputCheckCurr(AEVK_UP) && player2.coord.y <= AEGfxGetWinMaxY() - player2.size / 2.f)
			player2.velocity.y = PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_DOWN) && player2.coord.y >= AEGfxGetWinMinY() + player2.size / 2.f)
			player2.velocity.y = -PLAYER_MOVE;

		else
			player2.velocity.y = 0;

		if (AEInputCheckCurr(AEVK_LEFT) && player2.coord.x >= AEGfxGetWinMinX() + player2.size / 2.f)
			player2.velocity.x = -PLAYER_MOVE;

		else if (AEInputCheckCurr(AEVK_RIGHT) && player2.coord.x <= AEGfxGetWinMaxX() - player2.size / 2.f)
			player2.velocity.x = PLAYER_MOVE;

		else
			player2.velocity.x = 0;

		/*------------------------------------------------------------
		END OF TUTORIAL 
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
	float r = (float)rand() / (float)RAND_MAX;
	return (int)(min + r * (max - min));
}

f32 rand_num(f32 min, f32 max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return (f32)(min + r * (max - min));
}
