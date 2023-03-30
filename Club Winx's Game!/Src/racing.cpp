/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Mariah Tahirah (mariahtahirah.b@digipen.edu)
* Secondary Authors:
*	Shayne Gloria (m.shayne@digipen.edu) -> Platforms & Split Screen (Load, Init, Draw, Unload)
*   Yeo Hui Shan (huishan.y@digipen.edu) -> Collision 
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "General.h"

// ---------------------------------------------------------------------------


/*------------------------------------------------------------
defines
------------------------------------------------------------*/

// Camera Movement Variables
static f32			CamX{ 0.0f },
CamY{ 0.0f };	// Camera's X & Y Positions

//// 
static AABB			Waves_boundingBox;
static AEVec2		Waves_vel{ 0.f, 0.f };
static bool			section_done{ false };

///
static int			if_win{ 0 } ; // 0: None, 1: Player_one, 2: Player_two

//players variables
int player1_current_platform;
int player2_current_platform;

bool player1_boosted{ false };
bool player2_boosted{ false };

int player1_boost_counter{ 0 };
int player1_use_boost_counter{ 0 };

int player2_boost_counter{ 0 };
int player2_use_boost_counter{ 0 };

float const BOOST_JUMP_HEIGHT = 200.0f;
float const BOOST_JUMP_VEL = 16.0f;

float bgspeed; 

AEMtx33 flipTransform1, flipTransform2;
//AEMtx33Scale(&flipTransform, -1.0f, 1.0f); // horizontal scale
//(&player1.transform, &player1.transform, &flipTransform);

/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void racing_load()
{
	std::cout << "racing:LOAD\n";
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	bgRacing.bgTex = AEGfxTextureLoad("Assets/Racing_BG.png");		// BG Texture
	SquareMesh(&bgRacing.bgMesh, 0);							// BG Mesh
	bgRacing.length = winWIDTH;
	bgRacing.height = winHEIGHT;



	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/

	// Informing the library that we're about to start adding triangles

	// player 1 mesh 
	SquareMesh(&player1.pMesh,  0xFFB62891);

	// player 2 mesh
	SquareMesh(&player2.pMesh,0xFFFF00FF);

	// loading in platform meshes in map
	racing_map_load();

	// loading in line
	splitscreen_load();

	// loading win texture
	SquareMesh(&winRacing.bgMesh, 0xFFFF00FF);

	// loagin Waves texture
	SquareMesh(&bgWaves.bgMesh, 0);

	//loading ground texture
	SquareMesh(&bgRacingGround.bgMesh, 0);


	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/
	player1.pTex = AEGfxTextureLoad("Assets/Player1.png");
	player2.pTex = AEGfxTextureLoad("Assets/Player2.png");

	winRacing.bgTex = AEGfxTextureLoad("Assets/Racing_Winner.png");
	bgWaves.bgTex = AEGfxTextureLoad("Assets/Waves.png"); // Waves Texture for rising water..
	bgRacingGround.bgTex = AEGfxTextureLoad("Assets/RacingGround.png"); //texture for ground

	/*------------------------------------------------------------
	LOAD SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	jump.audio = AEAudioLoadSound("Assets/Audio/jump.wav");
	jump.aGroup = AEAudioCreateGroup();

	return;
}

void racing_init()
{
	std::cout << "racing:Initialize\n";
	/*------------------------------------------------------------
	// INIT PLAYERS
	------------------------------------------------------------*/
	player1.pGround = AEGfxGetWinMinY() + player1.size / 2.f;
	player1.pCurrGround = player1.pGround;
	player2.pGround = AEGfxGetWinMinY() + player2.size / 2.f;
	player2.pCurrGround = player2.pGround;

	player1.pCoord = { AEGfxGetWinMinX() / 2, player1.pGround }; //spawn at left half of screen
	player2.pCoord = { AEGfxGetWinMaxX() / 2, player2.pGround }; //spawn at right half of screen

	player1.size = 50.0f;
	player2.size = 50.0f;
	AEMtx33Scale(&flipTransform1, 1.0f, 1.0f);
	AEMtx33Scale(&flipTransform2, 1.0f, 1.0f);

	/*------------------------------------------------------------
	// INIT PLATFORM - MAP
	------------------------------------------------------------*/
	// declaring values for start and end values for player1 & 2 - which is needed for racing map
	player1.startX	= AEGfxGetWinMinX();
	player1.endX	= - 0.0f;

	//player2.start = (AEGfxGetWinMinX() + AEGfxGetWinMaxX()) / 2;
	player2.startX	= 0.0f;
	player2.endX	= AEGfxGetWinMaxX();

	// calling racing map initialisation fucntion, using values from previous lines
	racing_map_init(player1.startX, player1.endX, 1);
	racing_map_init(player2.startX, player2.endX, 2);



	/*------------------------------------------------------------
	// INIT SPLITSCREEN
	------------------------------------------------------------*/
	splitscreen_init();



	/*------------------------------------------------------------
	// INIT - Camera Movement
	------------------------------------------------------------*/
	CamX = 0.0f;
	CamY = (player1.pCoord.y + player2.pCoord.y) / 2 + winHEIGHT / 6;



	/*------------------------------------------------------------
	// INIT - Racing Win Texture
	------------------------------------------------------------*/
	winRacing.length = 190.0f;
	winRacing.height = 100.0f;
	if_win = 0;

	/*------------------------------------------------------------
	// INIT WAVES
	------------------------------------------------------------*/
	bgWaves.height = bgRacing.height / 5.0f;
	bgWaves.length = bgRacing.length;
	bgWaves.bgCoord.y = AEGfxGetWinMinY() - 2*bgWaves.height;

	/*------------------------------------------------------------
	// INIT GROUND
	------------------------------------------------------------*/
	bgRacingGround.height = winHEIGHT / 3.f;
	bgRacingGround.length = bgRacing.length;
	bgRacingGround.bgCoord.y = AEGfxGetWinMinY() - winHEIGHT / 6.f;


	bgspeed = 0.05f;
	return;
}

void racing_update()
{
	std::cout << "racing:Update\n";
	/*------------------------------------------------------------
	// CHANGE STATE CONDITIONS
	------------------------------------------------------------*/
	// for winning texture
	switch (if_win)
	{
	case 1:
		Racing_Win(1);
		break;
	case 2:
		Racing_Win(2);
		break;
	}


	
	//for testing
	if (AEInputCheckCurr(AEVK_1)) {
		next_state = PUZZLE;
	}
	if (AEInputCheckCurr(AEVK_3)) {
		next_state = BOSS;
	}
	if (AEInputCheckCurr(AEVK_Q)) {
		next_state = QUIT;
	}
	if (AEInputCheckCurr(AEVK_BACK)) {
		next_state = RESTART;
	}
	if (AEInputCheckCurr(AEVK_ESCAPE)) {
		next_state = MENU;
	}
	

	/*------------------------------------------------------------
	// INPUT HANDLING
	------------------------------------------------------------*/
	input_handle();
	AEAudioUpdate();

	if (AEInputCheckTriggered(AEVK_A)) {
		AEMtx33Scale(&flipTransform1, -1.0f, 1.0f); // player 1 flip 

	}
	if (AEInputCheckTriggered(AEVK_D)) {
		AEMtx33Scale(&flipTransform1, 1.0f, 1.0f); // player 1 normal 
	}
	if (AEInputCheckTriggered(AEVK_LEFT)) {
		AEMtx33Scale(&flipTransform2, -1.0f, 1.0f); // player 2 flip 

	}
	if (AEInputCheckTriggered(AEVK_RIGHT)) {
		AEMtx33Scale(&flipTransform2, 1.0f, 1.0f); // player 1 normal
	}


	/*------------------------------------------------------------
	// CHECK PLAYER-PLATFORM COLLISON
	------------------------------------------------------------*/
	//update player bounding box
	player1.boundingBox.min.x = player1.pCoord.x - player1.size / 2.0f;
	player1.boundingBox.min.y = player1.pCoord.y - player1.size / 2.0f;
	player1.boundingBox.max.x = player1.pCoord.x + player1.size / 2.0f;
	player1.boundingBox.max.y = player1.pCoord.y + player1.size / 2.0f;

	player2.boundingBox.min.x = player2.pCoord.x - player2.size / 2.0f;
	player2.boundingBox.min.y = player2.pCoord.y - player2.size / 2.0f;
	player2.boundingBox.max.x = player2.pCoord.x + player2.size / 2.0f;
	player2.boundingBox.max.y = player2.pCoord.y + player2.size / 2.0f;


	//checking for player-platform collision
	for (int i = 0; i < MAX_NUM_PLATFORMS; i++)
	{
		//update platform bounding box
		platformA[i].platBoundingBox.min.x = platformA[i].platVect.x - platformA[i].length / 2.0f;
		platformA[i].platBoundingBox.min.y = platformA[i].platVect.y - platformA[i].height / 2.0f;
		platformA[i].platBoundingBox.max.x = platformA[i].platVect.x + platformA[i].length / 2.0f;
		platformA[i].platBoundingBox.max.y = platformA[i].platVect.y + platformA[i].height / 2.0f;

		platformB[i].platBoundingBox.min.x = platformB[i].platVect.x - platformB[i].length / 2.0f;
		platformB[i].platBoundingBox.min.y = platformB[i].platVect.y - platformB[i].height / 2.0f;
		platformB[i].platBoundingBox.max.x = platformB[i].platVect.x + platformB[i].length / 2.0f;
		platformB[i].platBoundingBox.max.y = platformB[i].platVect.y + platformB[i].height / 2.0f;


		bool player1_collide = CollisionIntersection_RectRect(player1.boundingBox, player1.pVel, platformA[i].platBoundingBox, platformA[i].platVel);
		bool player2_collide = CollisionIntersection_RectRect(player2.boundingBox, player2.pVel, platformB[i].platBoundingBox, platformB[i].platVel);

		/*******************
			player 1
		*******************/
		if (player1_collide)
		{
			player1.pFlag = get_collision_flag(player1.boundingBox, player1.pVel, platformA[i].platBoundingBox, platformA[i].platVel);

			if (player1.pFlag == COLLISION_BOTTOM)
			{
				player1.pCurrGround = platformA[i].platVect.y + platformA[i].height / 2.0f + player1.size / 2.0f;
				player1.pCoord.y = player1.pCurrGround;

				player1.pJumping = false;
				player1.pOnSurface = true;

				player1_current_platform = i;
				player1.pVel.y = 0;
			}
		}


		//update pOnSurface
		if (player1.pCoord.y == player1.pGround)//player on the ground
		{
			player1.pCurrGround = player1.pGround;
			player1.pOnSurface = true;
		}

		else
		{
			if (player1.boundingBox.min.x > platformA[player1_current_platform].platVect.x + platformA[player1_current_platform].length / 2.0f ||
				player1.boundingBox.max.x < platformA[player1_current_platform].platVect.x - platformA[player1_current_platform].length / 2.0f)
			{
				player1.pOnSurface = false;
			}
		}

		// if collision on last platform and other player hasn't won
		float platformA_last_y = platformA[MAX_NUM_PLATFORMS - 1].platVect.y + platformA[MAX_NUM_PLATFORMS - 1].height / 2.0f + player1.size / 2.0f;
		if ((if_win == 0) && player1.pCurrGround == platformA_last_y)
		{
			if_win = 1;
			Racing_Win(1);
			MatrixCalc(winRacing.transform, winRacing.length, winRacing.height, 0.f, winRacing.bgCoord);
		}


		/*******************
			player 2
		*******************/
		if (player2_collide)
		{
			player2.pFlag = get_collision_flag(player2.boundingBox, player2.pVel, platformB[i].platBoundingBox, platformB[i].platVel);

			if (player2.pFlag == COLLISION_BOTTOM)
			{
				player2.pCurrGround = platformB[i].platVect.y + platformB[i].height / 2.0f + player2.size / 2.0f;
				player2.pCoord.y = player2.pCurrGround;

				player2.pJumping = false;
				player2.pOnSurface = true;

				player2_current_platform = i;
				player2.pVel.y = 0;
			}
		}

		//update pOnSurface
		if (player2.pCoord.y == player2.pGround)//player on the ground
		{
			player2.pCurrGround = player2.pGround;
			player2.pOnSurface = true;
		}

		else
		{
			if (player2.boundingBox.min.x > platformB[player2_current_platform].platVect.x + platformB[player2_current_platform].length / 2.0f ||
				player2.boundingBox.max.x < platformB[player2_current_platform].platVect.x - platformB[player2_current_platform].length / 2.0f)
			{
				player2.pOnSurface = false;
			}
		}

		// if collision on last platform and other player hasn't won
		float platformB_last_y = platformB[MAX_NUM_PLATFORMS - 1].platVect.y + platformB[MAX_NUM_PLATFORMS - 1].height / 2.0f + player2.size / 2.0f;
		if ((if_win == 0) && player2.pCurrGround == platformB_last_y)
		{
			if_win = 2;
			Racing_Win(2);
			MatrixCalc(winRacing.transform, winRacing.length, winRacing.height, 0.f, winRacing.bgCoord);
		}
	}


	/*------------------------------------------------------------
	// CHECK FOR PLAYER-BOOST COLLISION
	------------------------------------------------------------*/
	for (int i = 0; i < MAX_NUM_ITEMS; i++)
	{
		racing_boostsA[i].boundingBox.min.x = racing_boostsA[i].pCoord.x - racing_boostsA[i].size / 2.0f;
		racing_boostsA[i].boundingBox.min.y = racing_boostsA[i].pCoord.y - racing_boostsA[i].size / 2.0f;
		racing_boostsA[i].boundingBox.max.x = racing_boostsA[i].pCoord.x + racing_boostsA[i].size / 2.0f;
		racing_boostsA[i].boundingBox.max.y = racing_boostsA[i].pCoord.y + racing_boostsA[i].size / 2.0f;

		racing_boostsB[i].boundingBox.min.x = racing_boostsB[i].pCoord.x - racing_boostsB[i].size / 2.0f;
		racing_boostsB[i].boundingBox.min.y = racing_boostsB[i].pCoord.y - racing_boostsB[i].size / 2.0f;
		racing_boostsB[i].boundingBox.max.x = racing_boostsB[i].pCoord.x + racing_boostsB[i].size / 2.0f;
		racing_boostsB[i].boundingBox.max.y = racing_boostsB[i].pCoord.y + racing_boostsB[i].size / 2.0f;

	}


	//checking for player-boost collision
	for (int i = 0; i < MAX_NUM_ITEMS; i++)
	{
		bool player1_collected_boost = CollisionIntersection_RectRect(player1.boundingBox, player1.pVel, racing_boostsA[i].boundingBox, racing_boostsA[i].pVel);
		bool player2_collected_boost = CollisionIntersection_RectRect(player2.boundingBox, player2.pVel, racing_boostsB[i].boundingBox, racing_boostsB[i].pVel);


		/*******************
			player 1
		*******************/
		if (player1_collected_boost && !racing_boostsA[i].collected)
		{
			racing_boostsA[i].collected = true;

			//updated boost bool
			player1_boosted = true;

			//update boost counter
			player1_boost_counter++;
		}

		//boosted jumping mechanism
		if (player1.pJumping)
		{
			//boost > used = boost unused
			if (player1_boosted)
			{
				//set new jump height
				player1.pJumpHeightMax = BOOST_JUMP_HEIGHT;

				//set new vel
				player1.pVel.y = BOOST_JUMP_VEL;

				player1_use_boost_counter = player1_boost_counter;
			}

			else
			{
				//reset player jump height max
				player1.pJumpHeightMax = JUMP_HEIGHT_MAX;

				//reset player vel
				player1.pVel.y = PLAYER_JUMP;
			}
		}

		if (player1_boost_counter == player1_use_boost_counter && player1_boosted && player1.pOnSurface)
		{
			player1_boosted = false;
		}



		/*******************
			player 2
		*******************/
		if (player2_collected_boost && !racing_boostsB[i].collected)
		{
				racing_boostsB[i].collected = true;

				//updated boost bool
				player2_boosted = true;

				//update boost counter
				player2_boost_counter++;
		}

		//boosted jumping mechanism
		if (player2.pJumping)
		{
			//boost > used = boost unused
			if (player2_boosted)
			{
				//set new jump height
				player2.pJumpHeightMax = BOOST_JUMP_HEIGHT;

				//set new vel
				player2.pVel.y = BOOST_JUMP_VEL;

				player2_use_boost_counter = player2_boost_counter;
			}

			else
			{
				//reset player jump height max
				player2.pJumpHeightMax = JUMP_HEIGHT_MAX;

				//reset player vel
				player2.pVel.y = PLAYER_JUMP;
			}
		}

		if (player2_boost_counter == player2_use_boost_counter && player2_boosted && player2.pOnSurface)
		{
			player2_boosted = false;
		}
	}

	/*------------------------------------------------------------
	UPDATE PLAYER POSITIONS
	------------------------------------------------------------*/
	player1.pCoord.x += player1.pVel.x * player1.pAcceleration * g_dt;
	player1.pCoord.y += player1.pVel.y * player1.pAcceleration * g_dt;

	player2.pCoord.x += player2.pVel.x * player2.pAcceleration * g_dt;
	player2.pCoord.y += player2.pVel.y * player2.pAcceleration * g_dt;

	/*------------------------------------------------------------
	UPDATE WAVES MOVEMENT
	//------------------------------------------------------------*/
	Waves_boundingBox.min.x = bgWaves.bgCoord.x - bgWaves.length / 3.0f;
	Waves_boundingBox.min.y = bgWaves.bgCoord.y - bgWaves.height / 3.0f;
	Waves_boundingBox.max.x = bgWaves.bgCoord.x + bgWaves.length / 3.0f - 8.0f;
	Waves_boundingBox.max.y = bgWaves.bgCoord.y + bgWaves.height / 3.0f - 8.0f;

	bool player1_lose = CollisionIntersection_RectRect(player1.boundingBox, player1.pVel, Waves_boundingBox, Waves_vel);
	bool player2_lose = CollisionIntersection_RectRect(player2.boundingBox, player2.pVel, Waves_boundingBox, Waves_vel);

	int section = int(MAX_NUM_PLATFORMS / 10); // divides the platforms to 10 sections


	// if both players pass the 1st section
	if (player1.boundingBox.min.y > platformA[section].platBoundingBox.max.y && player2.boundingBox.min.y > platformB[section].platBoundingBox.max.y)
	{
			if (section_done == false)	bgWaves.bgCoord.y = CamY - 2.6f * bgWaves.height;

		// if players passes the halfway mark
		if (player1.boundingBox.min.y > platformA[section * 5].platBoundingBox.max.y && player2.boundingBox.min.y > platformB[section * 5].platBoundingBox.max.y)
		{
			section_done = true;
			if (section_done == true)	bgWaves.bgCoord.y = CamY - 2.3f * bgWaves.height;
		}
	}

	// players lose mechanics

	if (player1_lose || player2_lose) {

		COLLISION lose1_flag = get_collision_flag(player1.boundingBox, player1.pVel, Waves_boundingBox, Waves_vel);
		COLLISION lose2_flag = get_collision_flag(player2.boundingBox, player2.pVel, Waves_boundingBox, Waves_vel);

		if (lose1_flag == COLLISION_BOTTOM || lose2_flag == COLLISION_BOTTOM)
		{
			next_state = LOSE_BOTHPLAYERS;
		}
	}


	/*------------------------------------------------------------
	UPDATE CAMERA MOVEMENT
	//------------------------------------------------------------*/
	CamY = (player1.pCoord.y + player2.pCoord.y) / 2 + winHEIGHT / 6;
	

	/*------------------------------------------------------------
	MATRIX CALCULATION
	------------------------------------------------------------*/
	// for background
	MatrixCalc(bgRacing.transform, bgRacing.length, bgRacing.height, 0.0f, bgRacing.bgCoord);

	// for players 
	MatrixCalc(player1.transform, player1.size, player1.size, 0.f, player1.pCoord);
	MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.pCoord);


	//for platforms 
	for (int i = 0; i < MAX_NUM_PLATFORMS; i++) {
		MatrixCalc(platformA[i].transform, platformA[i].length, platformA[i].height, 0.f, platformA[i].platVect);
		MatrixCalc(platformB[i].transform, platformB[i].length, platformB[i].height, 0.f, platformB[i].platVect);
	}

	//for boosts
	for (int i = 0; i < MAX_NUM_ITEMS; i++) {
		MatrixCalc(racing_boostsA[i].transform, racing_boostsA[i].size, racing_boostsA[i].size, 0.f, racing_boostsA[i].pCoord);
		MatrixCalc(racing_boostsB[i].transform, racing_boostsB[i].size, racing_boostsB[i].size, 0.f, racing_boostsB[i].pCoord);
	}
	
	//for splitscreen
	MatrixCalc(splitscreen.transform, splitscreen.length, splitscreen.height, 0.f, splitscreen.lVect);

	//for Waves
	MatrixCalc(bgWaves.transform, bgWaves.length, bgWaves.height, 0.f, bgWaves.bgCoord);

	//for ground

	MatrixCalc(bgRacingGround.transform, bgRacingGround.length, bgRacingGround.height, 0.f, bgRacingGround.bgCoord);


	///*------------------------------------------------------------
	// UPDATE - Background Y
	//------------------------------------------------------------*/
	bgRacing.bgCoord.y = CamY;
}

static float v = 0;
static float w = 0;
void racing_draw()
{
	std::cout << "racing:Draw\n";
	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransform(bgRacing.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	v += bgspeed * g_dt;
	if (v >= 1 || if_win)
	{
		v = 0;
	}
	AEGfxTextureSet(bgRacing.bgTex, 0.f, v);
	AEGfxMeshDraw(bgRacing.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	// DRAWING PLATFORMS - MAP
	------------------------------------------------------------*/
	racing_map_draw();


	/*------------------------------------------------------------
	// DRAWING PLAYERS
	------------------------------------------------------------*/
	// Drawing player 1
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEMtx33Concat(&player1.transform, &player1.transform, &flipTransform1);
	AEGfxSetTransform(player1.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(player1.pTex, 0, 0);
	AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);

	// drawing player 2
	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEMtx33Concat(&player2.transform, &player2.transform, &flipTransform2);
	AEGfxSetTransform(player2.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(player2.pTex, 0, 0);
	AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	// DRAWING GROUND
	------------------------------------------------------------*/
	AEGfxSetTransform(bgRacingGround.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(bgRacingGround.bgTex, 0, 0);
	AEGfxMeshDraw(bgRacingGround.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	// DRAWING SPLITSCREEN
	------------------------------------------------------------*/
	splitscreen_draw();


	/*------------------------------------------------------------
	// DRAWING Winner Texture
	------------------------------------------------------------*/
	AEGfxSetTransform(winRacing.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(winRacing.bgTex, 0, 0);
	AEGfxMeshDraw(winRacing.bgMesh, AE_GFX_MDM_TRIANGLES);

	/*------------------------------------------------------------
	// DRAWING Waves BG
	------------------------------------------------------------*/
	AEGfxSetTransform(bgWaves.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	w += bgspeed * g_dt;
	if (w >= 1)
	{
		w = 0;
	}
	AEGfxTextureSet(bgWaves.bgTex, w, 0);
	AEGfxMeshDraw(bgWaves.bgMesh, AE_GFX_MDM_TRIANGLES);

	

	/*------------------------------------------------------------
	 DRAWING - Camera Movement
	------------------------------------------------------------*/
	AEGfxSetCamPosition(CamX, CamY); // Set Camera's Position to values of CamX & CamY


}

void racing_free()
{
	std::cout << "racing:Free\n";
	/*------------------------------------------------------------
	// Resetting Camera
	------------------------------------------------------------*/
	CamX = 0.0f;
	CamY = 0.f;
	AEGfxSetCamPosition(CamX, CamY);

}

void racing_unload()
{
	std::cout << "racing:Unload\n";
	/*------------------------------------------------------------
	// Unload Background Meshes & Texture
	------------------------------------------------------------*/
	AEGfxMeshFree(bgRacing.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgRacing.bgTex); // Unload Texture

	AEGfxMeshFree(bgWaves.bgMesh);
	AEGfxTextureUnload(bgWaves.bgTex);

	AEGfxMeshFree(bgRacingGround.bgMesh);
	AEGfxTextureUnload(bgRacingGround.bgTex);

	/*------------------------------------------------------------
	// Unload Player Meshes
	------------------------------------------------------------*/
	AEGfxMeshFree(player1.pMesh);
	AEGfxMeshFree(player2.pMesh);

	AEGfxTextureUnload(player1.pTex);
	AEGfxTextureUnload(player2.pTex);

	/*------------------------------------------------------------
	// Unload Platform Meshes
	------------------------------------------------------------*/
	racing_map_unload();

	/*------------------------------------------------------------
	// Unload Split Screen Meshes
	------------------------------------------------------------*/
	splitscreen_unload();

	/*------------------------------------------------------------
	// Unload Player Meshes
	------------------------------------------------------------*/
	AEGfxMeshFree(winRacing.bgMesh);
	AEGfxTextureUnload(winRacing.bgTex);

	/*------------------------------------------------------------
	// Exit Audio
	------------------------------------------------------------*/
	AEAudioStopGroup(jump.aGroup);
}
