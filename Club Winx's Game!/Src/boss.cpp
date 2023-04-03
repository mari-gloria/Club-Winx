/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This source file defines the functions required for the boss game mode.
* 
* Primary Author: 
*	Mariah Tahirah (mariahtahirah.b@digipen.edu) -> player bullets, boss bullets, boss movements, collision 60%
* 
* Secondary Authors:
*   Amirah Isa (amirah.b@digipen.edu) -> boss, mobs, potion and health system rendering/collision, SFX when potion being collected 30%
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu) -> win/lose conditon 5 %
*	Yeo Hui Shan (huishan.y@digipen.edu) -> Audio (BGM + shoot) 5 % 
==================================================================================*/

// ---------------------------------------------------------------------------
// includes
#include "General.h"

// ---------------------------------------------------------------------------





/*------------------------------------------------------------
CONSTANTS
------------------------------------------------------------*/
int const MAX_BULLETS{ 10 }; // number of max bullets on screen 
const f32 BULLETSPEED = 20.0f; // speed of players bullet
const f32 PLAYERDMG = 5.5f; //Player's hit
const f32 BOSS_MAX_HP = 600.f; //  boss hp
const f32 PLAYER_MAX_HP = 100.f; // players hp
const f32 PLAYER2_MAX_HP = 100.f;
const f32 BOSSATTACK_1_DMG = 8.5f;  // boss attack dmg
const f32 MOBSATTACK_DMG = 1.5f;  // mobs attack dmg
const f32 MOBS_MAX_HP = 50.f;  // mobs hp 
const f32 bgspeed = 0.05F; // bg speed 
const f32 bossMoveLimit = 130.f; // limit of boss movement Y value 
const f32 bossSpeed = 50.f; // speed of boss
const f32 knockBackDistance = 50.0f; // boss knockback when hit 
const f32 knockBackDuration = 0.2f;
const f32 healAmount = 0.3f; // potion heal percentage of MAX HP 






/*------------------------------------------------------------
PLAYER BULLETS
------------------------------------------------------------*/
struct Bullet {
	AEVec2				coord{ 0.0f,0.0f };

	bool				shot{ FALSE };
	
	AEMtx33				transform{};
	
	f32					length{ 20.0f };
	f32					height{ 15.0f };
	
	AABB				boundingBox;
	
	AEVec2				velocity;
	
};
Bullet bullets1[MAX_BULLETS], bullets2[MAX_BULLETS];

AEGfxTexture*			TexBullet1{};
AEGfxVertexList*		pBullet1{ nullptr };
AEGfxTexture*			TexBullet2{};
AEGfxVertexList*		pBullet2{ nullptr };
f64						bulletTimeElapsed = 0.0;





/*------------------------------------------------------------
HEALTH
------------------------------------------------------------*/
struct Health
{

	AEGfxVertexList*	mesh{ nullptr };    
	AEGfxTexture*		texture{ nullptr };

	AEMtx33				transform{};

	AEVec2				Hcoord{ 0.0f, 300.0f };
	AEVec2				Hcoord2{};

	f32					plength{};
	f32					pheight{ 10.0f };
	f32					length{ 1200 };
	f32					height{ 30.0f };
};
Health p1health, p2health;

//Boss Health
f32						hp_percentage;
f32						DEFAULT_HP; //length of hp
float					newBar;


//PLAYER 1's health
f32						player_hp_percentage;
f32						player_default_hp; //length of default hp
float					playernewbar;
//PLAYER 2's health
f32						player2_hp_percentage;
f32						player2_default_hp; //length of default hp
float					player2newbar;





/*------------------------------------------------------------
BOSS ATTACKS
------------------------------------------------------------*/
const int MAXWAVE = { 40 };

struct bosspew {
	AEVec2				coord{};

	AEMtx33				transform{};

	AEVec2				velocity{};

	bool				shot{ FALSE };

	f32					direction{ 30.0f };
	f32					speed{ 100.f };

	f32					size{ 30.f };
	
	AABB				boundingBox;
};
bosspew bossbullets1[MAXWAVE], bossbullets2[MAXWAVE];

AEGfxVertexList*		pbossbullet{ nullptr };
AEGfxTexture*			TexBossBullet{};
f64						bossTimeElapsed = 0.0f;
bool					bossSwitch;
float					knockBackTimer = 0.0f;
bool					isKnockedBack = false;
float					originalX;





/*--------------------------------------------------------------------------
Boss
---------------------------------------------------------------------------*/
struct Boss { // initialise in each game mode before use 

	AEGfxVertexList*	mesh{ nullptr };
	AEGfxTexture*		texture{ nullptr };

	AEGfxTexture*		hitTexture{};
	AEGfxVertexList*	hitMesh{};

	AEMtx33				transform{};

	AEVec2				coord{ 380.0f, -30.f };
	AEVec2				velocity{ 0.0f, 0.0f };
	AABB				boundingBox;

	bool				alive{ true };
	Health				health;

	f32 length = 200.0f; //boss length 
	f32 height = 150.f; // boss height
	f32 HP;

};
Boss boss;





/*------------------------------------------------------------
*POTION
------------------------------------------------------------*/
struct Potion {

	AEGfxVertexList*	mesh{ nullptr }; 
	AEGfxTexture*		texture{ nullptr };

	AEMtx33				transform{};

	f32					size{ 60.0f };
	f32					height{ 100.0f };
	AEVec2				vector{ -800,50 };

	AEVec2				velocity{ 0.0f, 0.0f }; //velocity for the item

	bool				collected; //when item is being collected
	AABB				boundingBox; //collision
}; Potion potion;

//potion movement
#define	ht_potion_jump -250
#define width_potion 1 
#define y_change_direction 1
#define potion_start_positonX -560
#define potion_start_positonY  0

int max_potion = 6; //number of max potion produce
int timer;
bool check = false;
bool potion_stop = false;
bool potion_produce = false;





/*------------------------------------------------------------
* MOBS
------------------------------------------------------------*/
struct Mobs {

	AEGfxVertexList*	mesh{ nullptr };
	AEGfxTexture*		texture{ nullptr };

	AEGfxVertexList*	hitMesh{ nullptr };
	AEGfxTexture*		hitTexture{ nullptr };

	AEMtx33				transform{};

	f32					size{ 60.0f };
	AEVec2				vector{ 700.0f,0 };

	AEVec2				velocity{ 0.0f, 0.0f }; //velocity for the item 
	AABB				boundingBox; //collision

	f32					HP;
}; Mobs mobs;

//mobs movement
#define MOBS_start_positonX 400
#define MOBS_start_positonY  0 //center of window

double radians_mob = 0;
int max_mobs = 4; //number of max mobs produce
bool mobscheck = false;
bool mobs_stop = false;
bool mobs_spawn = false;
f32 mobsHitTimer = 0.0f;
bool mobsHit = false;
bool mobs_moving = false;

Audio ooz, oof;



/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void boss_load()
{
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	bgBoss.texture = AEGfxTextureLoad("Assets/BossAssets/BOSS_BG.png");	// BG Texture
	SquareMesh(&bgBoss.mesh, 0);							// BG Mesh
	bgBoss.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgBoss.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();


	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/
	// player 1 mesh 
	SquareMesh(&player1.mesh, 0xFFB62891);
	// player 2 mesh
	SquareMesh(&player2.mesh, 0xFFFF00FF);
	//creating bullet mesh
	SquareMesh(&pBullet1, 0xFFFFFFFF);
	SquareMesh(&pBullet2, 0xFFFFFFFF);

	//boss mesh
	SquareMesh(&boss.mesh, 0xFFFFFF00);
	SquareMesh(&boss.hitMesh, 0);
	//Creating Health Mesh
	SquareMesh(&boss.health.mesh, 0x00FF0000);
	//PLayer health mesh
	SquareMesh(&p1health.mesh, 0x00FF0000);
	SquareMesh(&p2health.mesh, 0x00FF0000);

	// boss bullet mesh 
	SquareMesh(&pbossbullet, 0x00FF0000);
	//Creating Potion
	SquareMesh(&potion.mesh, 0x00FFFFFF);
	//mobs mesh
	SquareMesh(&mobs.mesh, 0x003333FF);
	SquareMesh(&mobs.hitMesh, 0x003333FF);
	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/
	player1.texture = AEGfxTextureLoad("Assets/PLAYER1.png");
	player2.texture = AEGfxTextureLoad("Assets/PLAYER2.png");

	boss.texture = AEGfxTextureLoad("Assets/BossAssets/BOSS.png");
	boss.hitTexture = AEGfxTextureLoad("Assets/BossAssets/BOSS_ATTACKED.png");

	potion.texture = AEGfxTextureLoad("Assets/BossAssets/POTION.png");

	mobs.texture = AEGfxTextureLoad("Assets/BossAssets/MONSTER.png");
	mobs.hitTexture = AEGfxTextureLoad("Assets/BossAssets/MONSTER_ATTACKED.png");

	TexBullet1 = AEGfxTextureLoad("Assets/BossAssets/PLAYER1_BULLET.png");
	TexBullet2 = AEGfxTextureLoad("Assets/BossAssets/PLAYER2_BULLET.png");
	TexBossBullet = AEGfxTextureLoad("Assets/BossAssets/BOSS_BULLET.png");

	/*------------------------------------------------------------
	LOAD SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	collect.audio = AEAudioLoadSound("Assets/Audio/collect.wav");
	collect.aGroup = AEAudioCreateGroup();

	boss_bgm.audio = AEAudioLoadSound("Assets/Audio/bossMusic.wav");
	boss_bgm.aGroup = AEAudioCreateGroup();
	
	shoot.audio = AEAudioLoadSound("Assets/Audio/shoot.wav");
	shoot.aGroup = AEAudioCreateGroup();

	ooz.audio = AEAudioLoadSound("Assets/Audio/ooz.wav");
	ooz.aGroup = AEAudioCreateGroup();

	oof.audio = AEAudioLoadSound("Assets/Audio/oof.wav");
	oof.aGroup = AEAudioCreateGroup();

	//load pause
	pause_load();

	// load tut screen
	GameTutorial_Load();
}

void boss_init()
{
	//std::cout << "boss:Initialize\n";
	player1.coord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 50 };
	player2.coord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 200 };
	player1.alive = true;
	player2.alive = true;
	player1.size = 50.0f;
	player2.size = 50.0f;

	DEFAULT_HP = (f32)AEGetWindowWidth() + 70.0f;
	player_default_hp = player2.size; //default player's hp size
	player2_default_hp = player1.size;
	player1.HP = PLAYER_MAX_HP;
	player2.HP = PLAYER2_MAX_HP; //initialise the first length of max_hp

	boss.HP = BOSS_MAX_HP;
	boss.alive = false;
	boss.coord = { 380.f, -30.f }; // 380, -30.f
	originalX = boss.coord.x;
	isKnockedBack = false;

	//counters
	bossTimeElapsed = 0.0;
	bulletTimeElapsed = 0.0;
	knockBackTimer = 0.0f;
	mobsHitTimer = 0.0f;

	bossSwitch = false;
	for (int i = 0; i < MAXWAVE; i++)
	{
		bossbullets1[i].shot = false;
		bossbullets1[i].coord = { boss.coord.x - (boss.length / 2.0f), boss.coord.y };
		
	}
	
	timer = 0;
	mobs_stop = false;
	max_mobs = 4;
	mobs.HP = MOBS_MAX_HP;
	mobs.vector = { MOBS_start_positonX,MOBS_start_positonY};
	mobsHit = false;
	mobs_moving = false;

	potion_stop = false;
    potion.vector = { potion_start_positonX, potion_start_positonY};

	/*------------------------------------------------------------
	// PLAY SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	AEAudioPlay(boss_bgm.audio, boss_bgm.aGroup, 0.75, 1, -1);

	/*------------------------------------------------------------
	// INIT TUT_game
	------------------------------------------------------------*/
	tut_viewed = false;
	GameTutorial_Init(0.0f, 0.0f);


	//pause init
	pause_init();
}

void boss_update()
{

	if (game_paused)
	{
		pause_update();
	}

	else
	{
		if (arcadeMode)
		{
			tut_viewed = true;
		}
		// Game Tutorial
		if (tut_viewed == false)
		{
			GameTutorial_Update();
			AEAudioPauseGroup(boss_bgm.aGroup);
		}
		else
		{
			AEAudioResumeGroup(boss_bgm.aGroup);
			input_handle();
		}
			

		// TIME COUNTER 
		bulletTimeElapsed += g_dt;
		bossTimeElapsed += g_dt;
		
		/*------------------------------------------------------------
		CHANGE STATE CONDITION
		------------------------------------------------------------*/
		//players win
		if (!boss.alive && boss.HP <=0)
		{
			next_state = ENDGAME;
		}

		//players lose
		if (!player1.alive && !player2.alive)
		{
			next_state = LOSE_BOTHPLAYERS;
		}

		/*------------------------------------------------------------
		PLAYER UPDATE
		------------------------------------------------------------*/

		if (player1.HP < 0)
		{
			player1.alive = false;
		}
		if (player2.HP < 0)
		{
			player2.alive = false;
		}
		/*------------------------------------------------------------
		BOSS UPDATE
		------------------------------------------------------------*/
		if (boss.HP < 0) 
		{
			boss.alive = FALSE; //Boss DIES	
		}

		if (mobs_stop == true && boss.HP > 0)
		{
			boss.alive = true; 
		}
		else
		{
			boss.alive = false;
		}
		/*------------------------------------------------------------
		MOBS UPDATE
		------------------------------------------------------------*/
		if (mobs.HP < 0) {

			max_mobs -= 1; //decrease mobs spawn
			mobs.vector = { -1000,-1000 };
			mobs.HP = MOBS_MAX_HP; //so that the next mobs can spawn
			mobs.vector = { MOBS_start_positonX,MOBS_start_positonY };

		}
		if (mobsHit)
		{
			if (mobsHitTimer < knockBackDuration)
			{
				mobsHitTimer += g_dt;
			}
			else
			{
				mobsHit = false;
				mobsHitTimer = 0.0f;
			}
		}

		/*------------------------------------------------------------
		BULLET MOVEMENT
		------------------------------------------------------------*/
		if (tut_viewed == true) {
			for (int i = 0; i < MAX_BULLETS; i++)
			{
				if (bulletTimeElapsed >= 0.5 && bullets1[i].shot == FALSE && bullets2[i].shot == FALSE) // every 2 secs 
				{
					AEAudioPlay(shoot.audio, shoot.aGroup, 0.2f, 1.f, 0);
					bullets1[i].shot = TRUE;
					bullets2[i].shot = TRUE;
					bulletTimeElapsed = 0.0;
				}
				if (boss.alive == FALSE && boss.HP <= 0) { //bullets will stop shooting when monster dies

					bullets1[i].shot = FALSE;
					bullets2[i].shot = FALSE;
				}

			}
			for (int i = 0; i < MAX_BULLETS; i++)
			{

				if (player1.alive) {

					if (bullets1[i].shot)
					{
						bullets1[i].velocity.x = BULLETSPEED; // bullet speed 
						bullets1[i].coord.x += bullets1[i].velocity.x;
					}
					else
					{
						bullets1[i].coord = { player1.coord.x + (player1.size / 2.0f), player1.coord.y };
					}
				}
				else //if player ded
				{
					bullets1[i].shot = false;
				}

				if (bullets1[i].coord.x >= AEGfxGetWinMaxX()) // if exit map 
				{
					bullets1[i].shot = FALSE;
				}

				if (player2.alive) {

					if (bullets2[i].shot)
					{
						bullets2[i].velocity.x = BULLETSPEED; // bullet speed 
						bullets2[i].coord.x += bullets2[i].velocity.x;
					}
					else
					{
						bullets2[i].coord = { player2.coord.x + (player2.size / 2.0f), player2.coord.y };
					}
				}
				else //if player ded
				{
					bullets2[i].shot = false;
				}
				if (bullets2[i].coord.x >= AEGfxGetWinMaxX()) // if exit map 
				{
					bullets2[i].shot = FALSE;
				}

			}

		}
		/*------------------------------------------------------------
		BOSS ATTACKS
		------------------------------------------------------------*/
		for (int i = 0; i < MAXWAVE; i++)
		{
			if (bossTimeElapsed >= 0.5 && bossbullets1[i].shot == FALSE && bossbullets2[i].shot == FALSE) // every 2 secs
			{
				bossbullets1[i].shot = TRUE;
				bossTimeElapsed = 0.0;
			}
		}

		for (int i = 0; i < MAXWAVE; i++)
		{
			
			if (bossbullets1[i].shot && boss.alive)
			{
				bossbullets1[i].direction = rand_num(-PI, PI); // base direction
				bossbullets1[i].velocity.x = 80.f * sinf((size_t)(i % 180) / PI) * 0.03f; // adds curve to x velocity
				bossbullets1[i].velocity.y = 80.f * cosf((size_t)(i % 180) / PI) * 0.01f; // adds curve to y velocity
			}
			if (!bossbullets1[i].shot || !boss.alive) {
				bossbullets1[i].coord = { boss.coord.x - (boss.length / 2.0f), boss.coord.y };
			}
			if (bossbullets1[i].coord.x <= AEGfxGetWinMinX() || bossbullets1[i].coord.y <= AEGfxGetWinMinY() || bossbullets1[i].coord.x >= AEGfxGetWinMaxX() || bossbullets1[i].coord.y >= AEGfxGetWinMaxY()) // if exit map
			{
				bossbullets1[i].shot = FALSE;
			}

			bossbullets1[i].coord.x -= (f32)(bossbullets1[i].velocity.x * AEFrameRateControllerGetFrameTime() * bossbullets1[i].speed); // bullet speed
			bossbullets1[i].coord.y += (f32)(bossbullets1[i].velocity.y * AEFrameRateControllerGetFrameTime() * bossbullets1[i].speed); // bullet speed
		}

		/*------------------------------------------------------------
		BOSS MOVEMENT
		------------------------------------------------------------*/
		if (boss.alive)
		{
			if (isKnockedBack)
			{
				// move boss along x axis away from point of impact
				if (knockBackTimer < knockBackDuration)
				{
					boss.coord.x += knockBackDistance * g_dt;
					knockBackTimer += g_dt;
				}
				else
				{
					// reset knock-back variables and return to original x position
					isKnockedBack = false;
					knockBackTimer = 0.0f;
					boss.coord.x = originalX;
				}
			}
			else {

				if (boss.coord.y <= -bossMoveLimit)
				{
					bossSwitch = true;
				}
				else if (boss.coord.y >= bossMoveLimit)
				{
					bossSwitch = false;
				}

				if (bossSwitch)
				{
					boss.velocity.y = bossSpeed;
				}
				else
				{
					boss.velocity.y = -bossSpeed;
				}
			}
			
		}

		boss.coord.y += boss.velocity.y * g_dt;
		boss.coord.x += boss.velocity.x * g_dt;

		/*------------------------------------------------------------
		COLLISION CHECKS
		------------------------------------------------------------*/

		//update player bounding box
		BoundingBoxUpdate(player1.boundingBox, player1.coord, player1.size, player1.size);
		BoundingBoxUpdate(player2.boundingBox, player2.coord, player2.size, player2.size);
		//update boss bounding box
		BoundingBoxUpdate(boss.boundingBox, boss.coord, boss.length, boss.length);
		//update potion bounding box
		BoundingBoxUpdate(potion.boundingBox, potion.vector, potion.size, potion.size);
		//update mobs bounding box
		BoundingBoxUpdate(mobs.boundingBox, mobs.vector, mobs.size, mobs.size);

		for (int i = 0; i < MAX_BULLETS; i++) // for bullet hit boss
		{
			//update bounding box
			BoundingBoxUpdate(bullets1[i].boundingBox, bullets1[i].coord, bullets1[i].length, bullets1[i].height);
			BoundingBoxUpdate(bullets2[i].boundingBox, bullets2[i].coord, bullets2[i].length, bullets2[i].height);

			//Shoot BOSS
			if (boss.alive)
			{
				if (CollisionIntersection_RectRect(bullets1[i].boundingBox, bullets1[i].velocity, boss.boundingBox, boss.velocity) && bullets1[i].shot) //if player1 or player2 bullet collide with boss && boss is alive
				{
					if (player1.alive) {

						boss.HP -= PLAYERDMG; //decrease monster health
						bullets1[i].shot = false;
						isKnockedBack = true;

						AEAudioPlay(oof.audio, oof.aGroup, 0.7f, 1.f, 0);
					}



				}
				if (CollisionIntersection_RectRect(bullets2[i].boundingBox, bullets2[i].velocity, boss.boundingBox, boss.velocity) && bullets2[i].shot)
				{
					if (player2.alive) {

						boss.HP -= PLAYERDMG; //decrease monster health
						bullets2[i].shot = false;
						isKnockedBack = true;

						AEAudioPlay(oof.audio, oof.aGroup, 0.7f, 1.f, 0);
					}



				}
			}
				

		}
		for (int i = 0; i < MAXWAVE; i++) // for bullet hit players
		{
			//update bounding box
			BoundingBoxUpdate(bossbullets1[i].boundingBox, bossbullets1[i].coord, bossbullets1[i].size, bossbullets1[i].size);

			if (mobs_stop == true) {

				if (CollisionIntersection_RectRect(bossbullets1[i].boundingBox, bossbullets1[i].velocity, player1.boundingBox, player1.velocity) && player1.alive)// if bullet hit player 1 && player alive
				{
					player1.HP -= BOSSATTACK_1_DMG;
					bossbullets1[i].shot = false;
				}
				if (CollisionIntersection_RectRect(bossbullets1[i].boundingBox, bossbullets1[i].velocity, player2.boundingBox, player2.velocity) && player2.alive)// if bullet hit player 1 && player alive
				{
					player2.HP -= BOSSATTACK_1_DMG;
					bossbullets1[i].shot = false;
				}

			}
			
		}

		/*------------------------------------------------------------
		POTION COLLECTION
		------------------------------------------------------------*/
		if (tut_viewed == true) 
		{
			timer = timer + 1;
			potion_position(potion.vector.x, potion.vector.y, potion_produce, check, potion_stop, timer);

			if (potion_stop != true) {

				if (potion_produce == true) {

					if (CollisionIntersection_RectRect(player2.boundingBox, player2.velocity, potion.boundingBox, potion.velocity))
					{
						float hpToHeal = PLAYER2_MAX_HP * healAmount; // amount of HP to heal (50% of max HP)
						float hpMissing = PLAYER2_MAX_HP - player2.HP; //  HP needed to reach max HP

						if (hpToHeal > hpMissing)
						{
							hpToHeal = hpMissing;
						}

						player2.HP += hpToHeal; // Heal the player
						potion.vector = { -1000,-1000 };
						max_potion -= 1;
						AEAudioPlay(collect.audio, collect.aGroup, 1, 1, 0);


					}
					if (CollisionIntersection_RectRect(player1.boundingBox, player1.velocity, potion.boundingBox, potion.velocity))
					{
						float hpToHeal = PLAYER_MAX_HP * healAmount; // amount of HP to heal (50% of max HP)
						float hpMissing = PLAYER_MAX_HP - player1.HP; //  HP needed to reach max HP

						if (hpToHeal > hpMissing) 
						{
							hpToHeal = hpMissing; // 
						}

						player1.HP += hpToHeal; // Heal the player

						potion.vector = { -1000,-1000 };
						max_potion -= 1;

						AEAudioPlay(collect.audio, collect.aGroup, 1, 1, 0);


					}

				}

			}
			//stop spawning potion
			if (max_potion == 0) {
				potion_stop = true;
			}

		}

		/*------------------------------------------------------------
		MOBS SPAWN
		------------------------------------------------------------*/
		// spawn after tut has been viewed
		if (tut_viewed == true) {
			mobs_position(mobs.vector.x, mobs.vector.y, mobs_spawn, mobscheck, mobs_stop, timer);

			//collision for mobs that are movinf
			if (player1.coord.x <= MOBS_start_positonX) {
				mobs_moving = true;
			}
			if (player2.coord.x <= MOBS_start_positonX) {
				mobs_moving = true;
			}
			if (mobs_stop != true) {

				if (mobs_spawn == true) {

					if (CollisionIntersection_RectRect(player2.boundingBox, player2.velocity, mobs.boundingBox, mobs.velocity) &&mobs_moving == true) {
						player2.HP -= MOBSATTACK_DMG;

					}
					if (CollisionIntersection_RectRect(player1.boundingBox, player1.velocity, mobs.boundingBox, mobs.velocity) && mobs_moving == true) {
						player1.HP -= MOBSATTACK_DMG;

					}
					for (int i = 0; i < MAX_BULLETS; i++)
					{
						//Shoot MOBS
						if (CollisionIntersection_RectRect(bullets1[i].boundingBox, bullets1[i].velocity, mobs.boundingBox, mobs.velocity) && mobs_moving == true) //if player1 or player2 bullet collide with boss && boss is alive
						{
							mobs.HP -= PLAYERDMG;
							bullets1[i].shot = false;
							mobsHit = true;
							AEAudioPlay(ooz.audio, ooz.aGroup, 0.7f, 1.f, 0);

						}
						if (CollisionIntersection_RectRect(bullets2[i].boundingBox, bullets2[i].velocity, mobs.boundingBox, mobs.velocity) && mobs_moving == true) //if player1 or player2 bullet collide with boss && boss is alive
						{
							mobs.HP -= PLAYERDMG;
							bullets2[i].shot = false;
							mobsHit = true;
							AEAudioPlay(ooz.audio, ooz.aGroup, 0.7f, 1.f, 0);

						}
					}
				}

			}
			//stop spawning mobs
			if (max_mobs == 0) {
				mobs_stop = true;
			}
		}



		/*------------------------------------------------------------
		UPDATE PLAYER POSITIONS
		------------------------------------------------------------*/
		player1.coord.x += player1.velocity.x * g_dt;
		player1.coord.y += player1.velocity.y * g_dt;

		player2.coord.x += player2.velocity.x * g_dt;
		player2.coord.y += player2.velocity.y * g_dt;



		/*------------------------------------------------------------
		MATRIX CALCULATION
		------------------------------------------------------------*/
		// for background
		MatrixCalc(bgBoss.transform, bgBoss.length, bgBoss.height, 0.0f, bgBoss.coord);

		// for players 
		MatrixCalc(player1.transform, player1.size, player1.size, 0.0f, player1.coord);
		MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.coord);

		// for bullet 
		for (int i = 0; i < MAX_BULLETS; i++) {

			if (bullets1[i].shot || bullets2[i].shot)
			{
				
				if (bullets1[i].shot)
				{
					
					MatrixCalc(bullets1[i].transform, bullets1[i].length, bullets1[i].height, 0.f, bullets1[i].coord);
				}
				if (bullets2[i].shot)
				{
					
					MatrixCalc(bullets2[i].transform, bullets2[i].length, bullets2[i].height, 0.f, bullets2[i].coord);
				}
			}

		}

		//for boss
		MatrixCalc(boss.transform, boss.length, boss.height, 0.f, boss.coord);

		/*------------------------------------------------------------
		BOSS HEALTH
		------------------------------------------------------------*/
		//Update Boss's Current HP
		hp_percentage = boss.HP / BOSS_MAX_HP;
		newBar = hp_percentage * DEFAULT_HP;
		boss.health.length = newBar;
		MatrixCalc(boss.health.transform, boss.health.length, boss.health.height, 0.f, boss.health.Hcoord);

		/*------------------------------------------------------------
		PLAYERS' HEALTH
		------------------------------------------------------------*/
		//for player2's health bar
		player2_hp_percentage = player2.HP / PLAYER2_MAX_HP;
		player2newbar = player2_hp_percentage * player2_default_hp;
		p2health.plength = player2newbar;
		p2health.Hcoord2 = { player2.coord.x, player2.coord.y + 35.0f };
		//p2health.plength = player2.HP / PLAYER_MAX_HP * player2.size;
		MatrixCalc(p2health.transform, p2health.plength, p2health.pheight, 0.f, p2health.Hcoord2);


		//for player1's health bar
		player_hp_percentage = player1.HP / PLAYER_MAX_HP;
		playernewbar = player_hp_percentage * player_default_hp;
		p1health.plength = playernewbar;
		p1health.Hcoord2 = { player1.coord.x, player1.coord.y + 35.0f };
		MatrixCalc(p1health.transform, p1health.plength, p1health.pheight, 0.f, p1health.Hcoord2);


		// for boss attacks
		for (int i = 0; i < MAXWAVE; i++)
		{
			MatrixCalc(bossbullets1[i].transform, bossbullets1[i].size, bossbullets1[i].size, bossbullets1[i].direction, bossbullets1[i].coord);
		}

		//Health potion
		MatrixCalc(potion.transform, potion.size, potion.size, 0.f, potion.vector);


		//Mobs
		MatrixCalc(mobs.transform, mobs.size, mobs.size, 0.f, mobs.vector);

		/*------------------------------------------------------------
		CURSOR
		------------------------------------------------------------*/
		AEInputShowCursor(false);
	}
}
float w = 0;
void boss_draw()
{
	if (game_paused)
	{
		pause_draw();
	}

	else 
	{
		//std::cout << "boss:Draw\n";
		/*------------------------------------------------------------
		DRAWING BACKGROUND
		------------------------------------------------------------*/
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		//AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
		AEGfxSetTransform(bgBoss.transform.m);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		w += bgspeed * g_dt;
		if (w >= 1)
		{
			w = 0;
		}
		AEGfxTextureSet(bgBoss.texture, w, 0.f);
		AEGfxMeshDraw(bgBoss.mesh, AE_GFX_MDM_TRIANGLES);

		/*------------------------------------------------------------
		DRAWING PLAYERS
		------------------------------------------------------------*/
		// Set position for object 1
		if (player1.alive)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetTransform(player1.transform.m);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1.0f);
			AEGfxTextureSet(player1.texture, 0, 0);
			AEGfxMeshDraw(player1.mesh, AE_GFX_MDM_TRIANGLES);

			// Render health bar
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxSetTransform(p1health.transform.m);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxSetBlendMode(AE_GFX_BM_NONE);
			AEGfxMeshDraw(p1health.mesh, AE_GFX_MDM_TRIANGLES);
		}
		// drawing player 2
		if (player2.alive)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetTransform(player2.transform.m);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1.0f);
			AEGfxTextureSet(player2.texture, 0, 0);
			AEGfxMeshDraw(player2.mesh, AE_GFX_MDM_TRIANGLES);

			// Render health bar
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxSetTransform(p2health.transform.m);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxSetBlendMode(AE_GFX_BM_NONE);
			AEGfxMeshDraw(p2health.mesh, AE_GFX_MDM_TRIANGLES);
		}

		/*------------------------------------------------------------
		DRAWING BULLETS
		------------------------------------------------------------*/
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bullets1[i].shot && player1.alive)
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetTransform(bullets1[i].transform.m);
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTransparency(1.0f);
				AEGfxTextureSet(TexBullet1, 0, 0);
				AEGfxMeshDraw(pBullet1, AE_GFX_MDM_TRIANGLES);
			}
			if (bullets2[i].shot && player2.alive)
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTransparency(1.0f);
				AEGfxTextureSet(TexBullet2, 0, 0);
				AEGfxSetTransform(bullets2[i].transform.m);
				AEGfxMeshDraw(pBullet2, AE_GFX_MDM_TRIANGLES);
			}

		}
		/*------------------------------------------------------------
		DRAWING  BOSS ATTACKS
		------------------------------------------------------------*/
		for (int i = 0; i < MAXWAVE; i++) {
			if (bossbullets1[i].shot && boss.alive)
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetTransform(bossbullets1[i].transform.m);
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTransparency(1.0f);
				AEGfxTextureSet(TexBossBullet, 0, 0);
				AEGfxMeshDraw(pbossbullet, AE_GFX_MDM_TRIANGLES);
			}
		}


		if (mobs_stop == false) {

			//Render Mobs
			if (mobsHit)
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetTransform(mobs.transform.m);
				AEGfxTextureSet(mobs.hitTexture, 0, 0);
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTransparency(1.0f);
				AEGfxMeshDraw(mobs.hitMesh, AE_GFX_MDM_TRIANGLES);
			}
			else
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetTransform(mobs.transform.m);
				AEGfxTextureSet(mobs.texture, 0, 0);
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTransparency(1.0f);
				AEGfxMeshDraw(mobs.mesh, AE_GFX_MDM_TRIANGLES);
			}

		}

		/*------------------------------------------------------------
		 Rendering of Boss Health System/ BOSS
		------------------------------------------------------------*/
		if (boss.alive) {

			// drawing boss
			if(isKnockedBack)
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetTransform(boss.transform.m);
				AEGfxTextureSet(boss.hitTexture, 0, 0);
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTransparency(1.0f);
				AEGfxMeshDraw(boss.hitMesh, AE_GFX_MDM_TRIANGLES);
			}
			else
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetTransform(boss.transform.m);
				AEGfxTextureSet(boss.texture, 0, 0);
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTransparency(1.0f);
				AEGfxMeshDraw(boss.mesh, AE_GFX_MDM_TRIANGLES);
			}
			// drawing Current boss.health
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxSetTransform(boss.health.transform.m);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxSetBlendMode(AE_GFX_BM_NONE);
			AEGfxMeshDraw(boss.health.mesh, AE_GFX_MDM_TRIANGLES);


		}

		/*------------------------------------------------------------
		 Rendering of Boss Health System/ BOSS
		------------------------------------------------------------*/
		if (potion_stop == false) {

			// drawing potion
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetTransform(potion.transform.m);
			AEGfxTextureSet(potion.texture, 0, 0);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1.0f);
			AEGfxMeshDraw(potion.mesh, AE_GFX_MDM_TRIANGLES);
		}

		/*------------------------------------------------------------
		// DRAWING TUTORIAL
		------------------------------------------------------------*/
		if (tut_viewed == false)
			GameTutorial_Draw();


	}
}

void boss_free()
{
	//reset counters
	bossTimeElapsed = 0.0;
	bulletTimeElapsed = 0.0;

	/*------------------------------------------------------------
	// free meshes
	------------------------------------------------------------*/
	AEGfxMeshFree(bgBoss.mesh);

	AEGfxMeshFree(player1.mesh);
	AEGfxMeshFree(player2.mesh);


	// HEALTH BARS
	AEGfxMeshFree(boss.health.mesh);
	AEGfxMeshFree(p1health.mesh);
	AEGfxMeshFree(p2health.mesh);

	AEGfxMeshFree(pbossbullet);

	AEGfxMeshFree(mobs.mesh);
	AEGfxMeshFree(mobs.hitMesh);

	AEGfxMeshFree(potion.mesh);

	AEGfxMeshFree(boss.mesh);
	AEGfxMeshFree(boss.hitMesh);

	AEGfxMeshFree(pBullet1);
	AEGfxMeshFree(pBullet2);

	/*------------------------------------------------------------
	// free pause
	------------------------------------------------------------*/
	pause_free();

	/*------------------------------------------------------------
	// free tutorial
	------------------------------------------------------------*/
	GameTutorial_Free();
}

void boss_unload()
{
	/*------------------------------------------------------------
	// unload textures
	------------------------------------------------------------*/
	AEGfxTextureUnload(bgBoss.texture);
	
	
	AEGfxTextureUnload(player1.texture);
	AEGfxTextureUnload(player2.texture);
	


	//boss bullet
	AEGfxTextureUnload(TexBossBullet);

	// MOBS 
	AEGfxTextureUnload(mobs.texture);
	AEGfxTextureUnload(mobs.hitTexture);

	
	AEGfxTextureUnload(potion.texture);
	// BOSS
	
	AEGfxTextureUnload(boss.texture);
	AEGfxTextureUnload(boss.hitTexture);

	//PLAYERS BULLETS
	
	AEGfxTextureUnload(TexBullet1);
	AEGfxTextureUnload(TexBullet2);

	/*------------------------------------------------------------
	// pause unload
	------------------------------------------------------------*/
	pause_unload();


	/*------------------------------------------------------------
	// tutorial unload
	------------------------------------------------------------*/
	GameTutorial_Unload();


	/*------------------------------------------------------------
	// Exit Audio
	------------------------------------------------------------*/
	AEAudioStopGroup(boss_bgm.aGroup);
	AEAudioStopGroup(shoot.aGroup);
	AEAudioStopGroup(ooz.aGroup);
	AEAudioStopGroup(oof.aGroup);
	AEAudioStopGroup(collect.aGroup);
	

}
void potion_position(float& x, float& y, bool& potion_produce, bool& check, bool& potion_stop, int& timer)
{
	if (potion_stop != true) 
	{

		if (timer % 100 == 0 && potion_produce != true) 
		{
			x = potion_start_positonX; //starting position
			y = potion_start_positonY;
			potion_produce = true;
		}

		if (timer % 1000 == 0) 
		{
			potion_produce = false;
		}

		if (potion_produce == true) 
		{
			if ((y != 0) && (check == false)) 
			{
				y += y_change_direction;
			}

			else 
			{
				check = true;
				y -= y_change_direction;
			}

			if (y == ht_potion_jump) 
			{
				check = false; // amplitude of the graph
			}

			if (x >= potion_start_positonX) 
			{
				x += width_potion; //period 

			}
		}
	}
}

void mobs_position(float& x, float& y, bool& mobs_spawn, bool& mobscheck, bool& mobs_stop, int& timer)
{

	if (mobs_stop != true) 
	{

		if (timer % 20 == 0 && mobs_spawn != true)
		{ //start spawning
			x = MOBS_start_positonX;
			y = MOBS_start_positonY;
			mobs_spawn = true;
		}

		if (timer % 550 == 0)
		{ //stop spawning
			mobs_spawn = false;
		}

		if (mobs_spawn == true) 
		{
			if ((mobscheck == false)) {
				x -= 2.0f;
			}
		}
	}
}