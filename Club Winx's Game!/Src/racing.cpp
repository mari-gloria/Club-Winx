/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This source file defines the functions required for the racing game mode.
* 
*Primary Author: 
*	Mariah Tahirah (mariahtahirah.b@digipen.edu) ->move backgrounds, set camera limit, flipping of characters, arcade 
* 
* Secondary Authors:
*	Shayne Gloria (m.shayne@digipen.edu) -> Platforms & Split Screen (Load, Init, Draw, Unload), win/lose condition
*   Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu) -> Collision checks (player-platform, player-item), boost logic, update players
*	Yeo Hui Shan (huishan.y@digipen.edu) -> Timer, Audio (BGM)
==================================================================================*/

// ---------------------------------------------------------------------------
// includes
#include "RacingMap.h"

// ---------------------------------------------------------------------------






/*------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/

//Camera Movement Variables, camera's X & Y Positions
f32 CamX{0.0f}, CamY{0.0f}; 

// WAVES
static AABB Waves_boundingBox;
static AEVec2 Waves_vel{0.f, 0.f};
static bool section_done{false};

//for winning condition
static int if_win{0}; // 0: None, 1: Player_one, 2: Player_two

// players variables
int player1_current_platform;
int player2_current_platform;

bool player1_boosted{false};
bool player2_boosted{false};

int player1_boost_counter{0};
int player1_use_boost_counter{0};

int player2_boost_counter{0};
int player2_use_boost_counter{0};

float const BOOST_JUMP_HEIGHT = 200.0f;
float const BOOST_JUMP_VEL = 16.0f;

//textures
Textures bgRacing, wavesRacing, groundRacing;

//bg movement
float bgspeed;
static float v = 0;
static float w = 0;

//timer 
int racingMinute = 0;

static AEMtx33 flipTransform1, flipTransform2;
static AEVec2 timeleft;





/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void racing_load()
{
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	bgRacing.texture = AEGfxTextureLoad("Assets/RacingAssets/RACING_BG.png");
	SquareMesh(&bgRacing.mesh, 0);
	bgRacing.length = (f32)winLENGTH;
	bgRacing.height = (f32)winHEIGHT;

	/*------------------------------------------------------------
	CREATE MESH
	------------------------------------------------------------*/
	// player 1 mesH
	SquareMesh(&player1.mesh, 0xFFB62891);

	// player 2 mesh
	SquareMesh(&player2.mesh, 0xFFFF00FF);

	// loading in platform meshes in map
	racing_map_load();

	// loading in line
	splitscreen_load();

	// loading win texture
	SquareMesh(&winRacing.mesh, 0xFFFF00FF);

	// loagin Waves texture
	SquareMesh(&wavesRacing.mesh, 0);

	// loading ground texture
	SquareMesh(&groundRacing.mesh, 0);

	// load pause screen
	pause_load();

	// load tut screen
	GameTutorial_Load();

	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/
	player1.texture = AEGfxTextureLoad("Assets/PLAYER1.png");
	player2.texture = AEGfxTextureLoad("Assets/PLAYER2.png");

	winRacing.texture = AEGfxTextureLoad("Assets/RacingAssets/RACING_WINNER.png");
	wavesRacing.texture = AEGfxTextureLoad("Assets/RacingAssets/RACING_WAVE.png");
	groundRacing.texture = AEGfxTextureLoad("Assets/RacingAssets/RACING_GROUND.png");

	/*------------------------------------------------------------
	// LOAD SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	jump.audio = AEAudioLoadSound("Assets/Audio/jump.wav");
	jump.aGroup = AEAudioCreateGroup();

	racing_bgm.audio = AEAudioLoadSound("Assets/Audio/racingMusic.wav");
	racing_bgm.aGroup = AEAudioCreateGroup();

	return;
}



void racing_init()
{
	/*------------------------------------------------------------
	// SET TIMER
	------------------------------------------------------------*/
	racingTime.minute = 60.0f; // 2 minutes
	racingTime.second = 60.0f;

	/*------------------------------------------------------------
	// INIT PLAYERS
	------------------------------------------------------------*/
	player1.ground = AEGfxGetWinMinY() + player1.size / 2.f;
	player1.currGround = player1.ground;
	player2.ground = AEGfxGetWinMinY() + player2.size / 2.f;
	player2.currGround = player2.ground;

	player1.coord = {AEGfxGetWinMinX() / 2, player1.ground}; // spawn at left half of screen
	player2.coord = {AEGfxGetWinMaxX() / 2, player2.ground}; // spawn at right half of screen

	player1.size = 50.0f;
	player2.size = 50.0f;

	AEMtx33Scale(&flipTransform1, 1.0f, 1.0f);
	AEMtx33Scale(&flipTransform2, 1.0f, 1.0f);

	/*------------------------------------------------------------
	// INIT PLATFORM - MAP
	------------------------------------------------------------*/
	// declaring values for start and end values for player1 & 2 - which is needed for racing map
	player1.startX = AEGfxGetWinMinX();
	player1.endX = -0.0f;

	// player2.start = (AEGfxGetWinMinX() + AEGfxGetWinMaxX()) / 2;
	player2.startX = 0.0f;
	player2.endX = AEGfxGetWinMaxX();

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
	CamY = (player1.coord.y + player2.coord.y) / 2.f + winHEIGHT / 6.f;

	/*------------------------------------------------------------
	// INIT - Racing Win Texture
	------------------------------------------------------------*/
	winRacing.length = 190.0f;
	winRacing.height = 100.0f;
	if_win = 0;
	winRacing.coord = {-600, 600};

	/*------------------------------------------------------------
	// INIT WAVES
	------------------------------------------------------------*/
	wavesRacing.height = bgRacing.height / 5.0f;
	wavesRacing.length = bgRacing.length;
	wavesRacing.coord.y = -AEGetWindowHeight() + 10.f;
	section_done = false;

	/*------------------------------------------------------------
	// INIT GROUND
	------------------------------------------------------------*/
	groundRacing.height = winHEIGHT / 3.f;
	groundRacing.length = bgRacing.length;
	groundRacing.coord.y = player1.coord.y - (groundRacing.height / 2.0f) - (player1.size / 2.0f);

	/*------------------------------------------------------------
	// PLAY SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	AEAudioPlay(racing_bgm.audio, racing_bgm.aGroup, 0.75, 1, -1);

	/*------------------------------------------------------------
	// INIT PAUSE
	------------------------------------------------------------*/
	pause_init();

	/*------------------------------------------------------------
	// INIT TUT_game
	------------------------------------------------------------*/
	tut_viewed = false;
	
	GameTutorial_Init(CamX, CamY);
	
	/*------------------------------------------------------------
	// Others
	------------------------------------------------------------*/
	bgspeed = 0.05f;
	
	return;
}

void racing_update()
{
	if (game_paused)
	{
		pause_update();
		AEInputShowCursor(true);
	}

	else
	{
		/*------------------------------------------------------------
		// IN GAME TUTORIAL
		------------------------------------------------------------*/
		if (arcadeMode)
		{
			tut_viewed = true;
		}

		if (tut_viewed == false)
		{
			GameTutorial_Update();
			AEAudioPauseGroup(racing_bgm.aGroup);
		}
		else
		{
			AEAudioResumeGroup(racing_bgm.aGroup);
			input_handle();
		}
			

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
		case 0:
			Racing_Win(0);
			break;
		}

		/*------------------------------------------------------------
		// TIMER
		------------------------------------------------------------*/
		if (tut_viewed == true)
		{
			racingTime.minute -= g_dt;
			racingTime.second -= g_dt;
			racingMinute = (int)racingTime.minute / 60;
			// std::cout << racingTime.minute << ":" << racingMinute << ":" << racingTime.second << std::endl;

			if (racingTime.second < 0.0f)
			{
				racingTime.second = 60.0f;

				if (racingTime.minute < 0.0f)
				{
					// game stops
					next_state = LOSE_BOTHPLAYERS;
				}
			}
		}

		/*------------------------------------------------------------
		// INPUT HANDLING
		------------------------------------------------------------*/
		AEInputShowCursor(false);

		if (AEInputCheckTriggered(AEVK_A))
		{
			AEMtx33Scale(&flipTransform1, -1.0f, 1.0f); // player 1 flip
		}
		if (AEInputCheckTriggered(AEVK_D))
		{
			AEMtx33Scale(&flipTransform1, 1.0f, 1.0f); // player 1 normal
		}
		if (AEInputCheckTriggered(AEVK_LEFT))
		{
			AEMtx33Scale(&flipTransform2, -1.0f, 1.0f); // player 2 flip
		}
		if (AEInputCheckTriggered(AEVK_RIGHT))
		{
			AEMtx33Scale(&flipTransform2, 1.0f, 1.0f); // player 1 normal
		}

		/*------------------------------------------------------------
		// CHECK PLAYER-PLATFORM COLLISON
		------------------------------------------------------------*/
		// update player bounding box
		BoundingBoxUpdate(player1.boundingBox, player1.coord, player1.size, player1.size);
		BoundingBoxUpdate(player2.boundingBox, player2.coord, player2.size, player2.size);

		// checking for player-platform collision
		for (int i = 0; i < MAX_NUM_PLATFORMS; i++)
		{
			// update platform bounding box
			BoundingBoxUpdate(platformA[i].boundingBox, platformA[i].coord, platformA[i].length, platformA[i].height);
			BoundingBoxUpdate(platformB[i].boundingBox, platformB[i].coord, platformB[i].length, platformB[i].height);

			bool player1_collide = CollisionIntersection_RectRect(player1.boundingBox, player1.velocity, platformA[i].boundingBox, platformA[i].velocity);
			bool player2_collide = CollisionIntersection_RectRect(player2.boundingBox, player2.velocity, platformB[i].boundingBox, platformB[i].velocity);

			/*******************
				player 1
			*******************/
			if (player1_collide)
			{
				player1.flag = get_collision_flag(player1.boundingBox, player1.velocity, platformA[i].boundingBox, platformA[i].velocity);

				if (player1.flag == COLLISION_BOTTOM)
				{
					player1.currGround = platformA[i].coord.y + platformA[i].height / 2.0f + player1.size / 2.0f;
					player1.coord.y = player1.currGround;

					player1.jumping = false;
					player1.onSurface = true;

					player1_current_platform = i;
					player1.velocity.y = 0;
				}
			}

			// update onSurface
			if (player1.coord.y == player1.ground) // player on the ground
			{
				player1.currGround = player1.ground;
				player1.onSurface = true;
			}

			else
			{
				if (player1.boundingBox.min.x > platformA[player1_current_platform].coord.x + platformA[player1_current_platform].length / 2.0f ||
					player1.boundingBox.max.x < platformA[player1_current_platform].coord.x - platformA[player1_current_platform].length / 2.0f)
				{
					player1.onSurface = false;
				}
			}

			// if collision on last platform and other player hasn't won
			float platformA_last_y = platformA[MAX_NUM_PLATFORMS - 1].coord.y + platformA[MAX_NUM_PLATFORMS - 1].height / 2.0f + player1.size / 2.0f;
			if ((if_win == 0) && player1.currGround == platformA_last_y)
			{
				if_win = 1;
				Racing_Win(1);
				MatrixCalc(winRacing.transform, winRacing.length, winRacing.height, 0.f, winRacing.coord);
			}

			/*******************
				player 2
			*******************/
			if (player2_collide)
			{
				player2.flag = get_collision_flag(player2.boundingBox, player2.velocity, platformB[i].boundingBox, platformB[i].velocity);

				if (player2.flag == COLLISION_BOTTOM)
				{
					player2.currGround = platformB[i].coord.y + platformB[i].height / 2.0f + player2.size / 2.0f;
					player2.coord.y = player2.currGround;

					player2.jumping = false;
					player2.onSurface = true;

					player2_current_platform = i;
					player2.velocity.y = 0;
				}
			}

			// update onSurface
			if (player2.coord.y == player2.ground) // player on the ground
			{
				player2.currGround = player2.ground;
				player2.onSurface = true;
			}

			else
			{
				if (player2.boundingBox.min.x > platformB[player2_current_platform].coord.x + platformB[player2_current_platform].length / 2.0f ||
					player2.boundingBox.max.x < platformB[player2_current_platform].coord.x - platformB[player2_current_platform].length / 2.0f)
				{
					player2.onSurface = false;
				}
			}

			// if collision on last platform and other player hasn't won
			float platformB_last_y = platformB[MAX_NUM_PLATFORMS - 1].coord.y + platformB[MAX_NUM_PLATFORMS - 1].height / 2.0f + player2.size / 2.0f;
			if ((if_win == 0) && player2.currGround == platformB_last_y)
			{
				if_win = 2;
				Racing_Win(2);
				MatrixCalc(winRacing.transform, winRacing.length, winRacing.height, 0.f, winRacing.coord);
			}
		}

		/*------------------------------------------------------------
		// CHECK FOR PLAYER-BOOST COLLISION
		------------------------------------------------------------*/
		for (int i = 0; i < MAX_NUM_ITEMS; i++)
		{
			BoundingBoxUpdate(racing_boostsA[i].boundingBox, racing_boostsA[i].coord, racing_boostsA[i].size, racing_boostsA[i].size);
			BoundingBoxUpdate(racing_boostsB[i].boundingBox, racing_boostsB[i].coord, racing_boostsB[i].size, racing_boostsB[i].size);
		}

		// checking for player-boost collision
		for (int i = 0; i < MAX_NUM_ITEMS; i++)
		{
			bool player1_collected_boost = CollisionIntersection_RectRect(player1.boundingBox, player1.velocity, racing_boostsA[i].boundingBox, racing_boostsA[i].velocity);
			bool player2_collected_boost = CollisionIntersection_RectRect(player2.boundingBox, player2.velocity, racing_boostsB[i].boundingBox, racing_boostsB[i].velocity);

			/*******************
				player 1
			*******************/
			if (player1_collected_boost && !racing_boostsA[i].collected)
			{
				racing_boostsA[i].collected = true;

				// updated boost bool
				player1_boosted = true;

				// update boost counter
				player1_boost_counter++;
			}

			// boosted jumping mechanism
			if (player1.jumping)
			{
				// boost > used = boost unused
				if (player1_boosted)
				{
					// set new jump height
					player1.jumpHeightMax = BOOST_JUMP_HEIGHT;

					// set new vel
					player1.velocity.y = BOOST_JUMP_VEL;

					player1_use_boost_counter = player1_boost_counter;
				}

				else
				{
					// reset player jump height max
					player1.jumpHeightMax = JUMP_HEIGHT_MAX;

					// reset player vel
					player1.velocity.y = PLAYER_JUMP;
				}
			}

			if (player1_boost_counter == player1_use_boost_counter && player1_boosted && player1.onSurface)
			{
				player1_boosted = false;
			}

			/*******************
				player 2
			*******************/
			if (player2_collected_boost && !racing_boostsB[i].collected)
			{
				racing_boostsB[i].collected = true;

				// updated boost bool
				player2_boosted = true;

				// update boost counter
				player2_boost_counter++;
			}

			// boosted jumping mechanism
			if (player2.jumping)
			{
				// boost > used = boost unused
				if (player2_boosted)
				{
					// set new jump height
					player2.jumpHeightMax = BOOST_JUMP_HEIGHT;

					// set new vel
					player2.velocity.y = BOOST_JUMP_VEL;

					player2_use_boost_counter = player2_boost_counter;
				}

				else
				{
					// reset player jump height max
					player2.jumpHeightMax = JUMP_HEIGHT_MAX;

					// reset player vel
					player2.velocity.y = PLAYER_JUMP;
				}
			}

			if (player2_boost_counter == player2_use_boost_counter && player2_boosted && player2.onSurface)
			{
				player2_boosted = false;
			}
		}

		/*------------------------------------------------------------
		UPDATE PLAYER POSITIONS
		------------------------------------------------------------*/
		player1.coord.x += player1.velocity.x * player1.acceleration * g_dt;
		player1.coord.y += player1.velocity.y * player1.acceleration * g_dt;

		player2.coord.x += player2.velocity.x * player2.acceleration * g_dt;
		player2.coord.y += player2.velocity.y * player2.acceleration * g_dt;

		/*------------------------------------------------------------
		UPDATE WAVES MOVEMENT
		//------------------------------------------------------------*/
		Waves_boundingBox.min.x = wavesRacing.coord.x - wavesRacing.length / 3.0f;
		Waves_boundingBox.min.y = wavesRacing.coord.y - wavesRacing.height / 3.0f;
		Waves_boundingBox.max.x = wavesRacing.coord.x + wavesRacing.length / 3.0f - 8.0f;
		Waves_boundingBox.max.y = wavesRacing.coord.y + wavesRacing.height / 3.0f - 8.0f;

		bool player1_lose = CollisionIntersection_RectRect(player1.boundingBox, player1.velocity, Waves_boundingBox, Waves_vel);
		bool player2_lose = CollisionIntersection_RectRect(player2.boundingBox, player2.velocity, Waves_boundingBox, Waves_vel);

		int section = int(MAX_NUM_PLATFORMS / 10); // divides the platforms to 10 sections

		// if both players pass the 1st section
		if (player1.boundingBox.min.y > platformA[section].boundingBox.max.y && player2.boundingBox.min.y > platformB[section].boundingBox.max.y)
		{
			if (section_done == false)
				wavesRacing.coord.y = CamY - 2.6f * wavesRacing.height;

			// if players passes the halfway mark
			if (player1.boundingBox.min.y > platformA[section * 5].boundingBox.max.y && player2.boundingBox.min.y > platformB[section * 5].boundingBox.max.y)
			{
				section_done = true;
				if (section_done == true)
					wavesRacing.coord.y = CamY - 2.3f * wavesRacing.height;
			}
		}

		// players lose mechanics

		if (player1_lose || player2_lose)
		{

			COLLISION lose1_flag = get_collision_flag(player1.boundingBox, player1.velocity, Waves_boundingBox, Waves_vel);
			COLLISION lose2_flag = get_collision_flag(player2.boundingBox, player2.velocity, Waves_boundingBox, Waves_vel);

			if (lose1_flag == COLLISION_BOTTOM || lose2_flag == COLLISION_BOTTOM)
			{
				next_state = LOSE_BOTHPLAYERS;
			}
		}

		/*------------------------------------------------------------
		UPDATE CAMERA MOVEMENT
		//------------------------------------------------------------*/
		CamY = (player1.coord.y + player2.coord.y - 105) / 2;

		/*------------------------------------------------------------
		MATRIX CALCULATION
		------------------------------------------------------------*/
		// for background
		MatrixCalc(bgRacing.transform, bgRacing.length, bgRacing.height, 0.0f, bgRacing.coord);

		// for players
		MatrixCalc(player1.transform, player1.size, player1.size, 0.f, player1.coord);
		MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.coord);

		// for platforms
		for (int i = 0; i < MAX_NUM_PLATFORMS; i++)
		{
			MatrixCalc(platformA[i].transform, platformA[i].length, platformA[i].height, 0.f, platformA[i].coord);
			MatrixCalc(platformB[i].transform, platformB[i].length, platformB[i].height, 0.f, platformB[i].coord);
		}

		// for boosts
		for (int i = 0; i < MAX_NUM_ITEMS; i++)
		{
			MatrixCalc(racing_boostsA[i].transform, racing_boostsA[i].size, racing_boostsA[i].size, 0.f, racing_boostsA[i].coord);
			MatrixCalc(racing_boostsB[i].transform, racing_boostsB[i].size, racing_boostsB[i].size, 0.f, racing_boostsB[i].coord);
		}

		// for splitscreen
		MatrixCalc(splitscreen.transform, splitscreen.length, splitscreen.height, 0.f, splitscreen.coord);

		// for Waves
		MatrixCalc(wavesRacing.transform, wavesRacing.length, wavesRacing.height, 0.f, wavesRacing.coord);

		// for ground
		MatrixCalc(groundRacing.transform, groundRacing.length, groundRacing.height, 0.f, groundRacing.coord);

		///*------------------------------------------------------------
		// UPDATE - Background Y
		//------------------------------------------------------------*/
		bgRacing.coord.y = CamY;
	}
}



void racing_draw()
{
	if (game_paused)
	{
		pause_draw();
	}

	else
	{
		/*------------------------------------------------------------
		DRAWING BACKGROUND
		------------------------------------------------------------*/
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransform(bgRacing.transform.m);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

		v += bgspeed * g_dt;
		if (v >= 1 || if_win)
		{
			v = 0;
		}

		AEGfxTextureSet(bgRacing.texture, 0.f, v);
		AEGfxMeshDraw(bgRacing.mesh, AE_GFX_MDM_TRIANGLES);

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
		AEGfxTextureSet(player1.texture, 0, 0);
		AEGfxMeshDraw(player1.mesh, AE_GFX_MDM_TRIANGLES);

		// drawing player 2
		AEMtx33Concat(&player2.transform, &player2.transform, &flipTransform2);
		AEGfxSetTransform(player2.transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(player2.texture, 0, 0);
		AEGfxMeshDraw(player2.mesh, AE_GFX_MDM_TRIANGLES);

		/*------------------------------------------------------------
		// DRAWING SPLITSCREEN
		------------------------------------------------------------*/
		splitscreen_draw();

		/*------------------------------------------------------------
		// DRAWING Winner Texture
		------------------------------------------------------------*/
		if (if_win != 0)
		{
			draw_texture(winRacing);
		}
		

		/*------------------------------------------------------------
		// DRAWING Waves BG
		------------------------------------------------------------*/
		AEGfxSetTransform(wavesRacing.transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		w += bgspeed * g_dt;
		if (w >= 1)
		{
			w = 0;
		}

		AEGfxTextureSet(wavesRacing.texture, w, 0);
		AEGfxMeshDraw(wavesRacing.mesh, AE_GFX_MDM_TRIANGLES);

		/*------------------------------------------------------------
		// DRAWING GROUND
		------------------------------------------------------------*/
		draw_texture(groundRacing);

		/*------------------------------------------------------------
		 DRAWING - Camera Movement
		------------------------------------------------------------*/
		AEGfxSetCamPosition(CamX, CamY); // Set Camera's Position to values of CamX & CamY

		/*------------------------------------------------------------
		 DRAWING - Words
		------------------------------------------------------------*/
		char strBuf[1000];
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		memset(strBuf, 0, 1000 * sizeof(char));
		sprintf_s(strBuf, "Time Left: %d:%.0f", racingMinute, racingTime.second);
		AEGfxPrint(font_pixel, strBuf, racingTime.timeleft.x, racingTime.timeleft.y, 0.7f, 1.0f, 1.0f, 1.0f);

		/*------------------------------------------------------------
		// DRAWING TUTORIAL
		------------------------------------------------------------*/
		if (tut_viewed == false)
			GameTutorial_Draw();
	}
}

void racing_free()
{
	/*------------------------------------------------------------
	// Resetting Camera
	------------------------------------------------------------*/
	CamX = 0.0f;
	CamY = 0.f;
	AEGfxSetCamPosition(CamX, CamY);

	/*------------------------------------------------------------
	// free mesh
	------------------------------------------------------------*/
	AEGfxMeshFree(bgRacing.mesh);
	AEGfxMeshFree(wavesRacing.mesh);
	AEGfxMeshFree(groundRacing.mesh);

	AEGfxMeshFree(player1.mesh);
	AEGfxMeshFree(player2.mesh);

	AEGfxMeshFree(winRacing.mesh);

	/*------------------------------------------------------------
	// free platform
	------------------------------------------------------------*/
	racing_map_free();

	/*------------------------------------------------------------
	// Free Split Screen
	------------------------------------------------------------*/
	splitscreen_free();

	/*------------------------------------------------------------
	// free pause
	------------------------------------------------------------*/
	pause_free();

	/*------------------------------------------------------------
	// free tutorial
	------------------------------------------------------------*/
	GameTutorial_Free();
}

void racing_unload()
{
	/*------------------------------------------------------------
	// unload texture
	------------------------------------------------------------*/
	AEGfxTextureUnload(bgRacing.texture);
	AEGfxTextureUnload(wavesRacing.texture);
	AEGfxTextureUnload(groundRacing.texture);

	AEGfxTextureUnload(player1.texture);
	AEGfxTextureUnload(player2.texture);

	/*------------------------------------------------------------
	// Unload Platform
	------------------------------------------------------------*/
	racing_map_unload();

	/*------------------------------------------------------------
	// Unload Win Meshes
	------------------------------------------------------------*/
	AEGfxTextureUnload(winRacing.texture);

	/*------------------------------------------------------------
	// Exit Audio
	------------------------------------------------------------*/
	AEAudioStopGroup(jump.aGroup);
	AEAudioStopGroup(racing_bgm.aGroup);

	/*------------------------------------------------------------
	// unload pause
	------------------------------------------------------------*/
	pause_unload();

	/*------------------------------------------------------------
	// unload tutorial
	------------------------------------------------------------*/
	GameTutorial_Unload();
}
