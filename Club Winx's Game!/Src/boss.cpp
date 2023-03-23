/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu)
* Secondary Authors:
*	Mariah Tahirah (mariahtahirah.b@digipen.edu) -> bullet rendering and movement from player
*   Amirah Isa (amirah.b@digipen.edu) -> boss and health rendering
*
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "General.h"
// ---------------------------------------------------------------------------




/*------------------------------------------------------------
CONSTANTS
------------------------------------------------------------*/
int const MAX_BULLETS{ 10 }; // number of max bullets on screen 
const f32 BULLETSPEED = 10.0f;
const f32 PLAYERDMG = 10.5f; //Player's hit
const f32 BOSS_MAX_HP = 500.f;
const f32 PLAYER_MAX_HP = 150.f;
const f32 PLAYER2_MAX_HP = 150.f;
const f32 BOSSATTACK_1_DMG = 5.5f;
const f32 MOBSATTACK_DMG = 1.5f;


/*------------------------------------------------------------
PLAYER BULLETS
------------------------------------------------------------*/
struct Bullet {
	AEVec2 bCoord{ 0.0f,0.0f };
	bool shot{ FALSE };
	AEMtx33 transform{};
	f32 length{ 10.0f };
	f32 height{ 5.0f };
	AABB boundingBox;
	AEVec2 bVel;
};

Bullet bullets1[MAX_BULLETS], bullets2[MAX_BULLETS];
AEGfxVertexList* pBullet{ nullptr };
f64 bulletTimeElapsed = 0.0;
/*-----------------------------------------------------------*/

/*------------------------------------------------------------
HEALTH
------------------------------------------------------------*/

//Boss Health
f32 hp_percentage;
f32 DEFAULT_HP; //length of hp
float newBar;


//PLAYER 1's health
f32 player_hp_percentage;
f32 player_default_hp; //length of default hp
float playernewbar;
//PLAYER 2's health
f32 player2_hp_percentage;
f32 player2_default_hp; //length of default hp
float player2newbar;

/*-----------------------------------------------------------*/



/*------------------------------------------------------------
BOSS ATTACKS
------------------------------------------------------------*/
const int MAXWAVE = { 40 };
struct bosspew {
	AEVec2	coords{}; // coords
	AEMtx33 transform{}; // transform 
	AEVec2 velocity{}; // 
	bool shot{ FALSE };
	f32 direction{ 30.0f };
	f32 speed{ 100.f };
	f32 size{ 25.f };
	AABB boundingBox;
};

bosspew bossbullets1[MAXWAVE], bossbullets2[MAXWAVE];
AEGfxVertexList* pbossbullet{ nullptr };

const f32 gravity = 9.8f;
f64 bossTimeElapsed = 0.0f;
f64 bossmovetime = 0.0f;
/*-----------------------------------------------------------*/

/*--------------------------------------------------------------------------
Boss
---------------------------------------------------------------------------*/
struct Boss { // initialise in each game mode before use 

	AEGfxVertexList*	pMesh1{ nullptr };			// mesh 
	AEGfxTexture*		pTex{ nullptr };			// texture
	AEMtx33				transform{};				// transform mtx 

	AEVec2				Bcoord{ 380.0f, -30.f };	// position of boss
	AEVec2				bossVel{ 0.0f, 0.0f };
	AABB				boundingBox;

	bool				alive{ true };
	Health				Bhealth;

	f32 length = 200.0f; //boss length 
	f32 height = 150.f; // boss height
	f32 HP;

};
//extern Boss boss;
Boss		boss;

/*------------------------------------------------------------
*POTION
------------------------------------------------------------*/
struct Potion {

	AEGfxVertexList* pMesh{ nullptr }; // mesh 
	AEGfxTexture* pTex{ nullptr };			// texture
	AEMtx33				PotionTransform{};
	f32					size{ 60.0f };
	f32					height{ 100.0f };
	AEVec2				vector{ -700,50 };
	AEVec2				pVelocity{ 0.0f, 0.0f }; //velocity for the item 
	bool				collected; //when item is being collected
	AABB				boundingBox; //collision


}; Potion potion;

/*------------------------------------------------------------
* POTION MOVEMENT
------------------------------------------------------------*/
#define ht_potion_jump -250
#define width_potion 1 
#define y_change_direction 2
#define potion_start_positonX -370
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

	AEGfxVertexList* pMesh{ nullptr }; // mesh 
	AEGfxTexture* pTex{ nullptr };			// texture
	AEMtx33				MobsTransform{};
	f32					size{ 60.0f };
	AEVec2				vector{ 700.0f,0 };
	AEVec2				MobsVelocity{ 0.0f, 0.0f }; //velocity for the item 
	AABB				boundingBox; //collision



}; Mobs mobs;

/*------------------------------------------------------------
* MOBS MOVEMENT
------------------------------------------------------------*/
#define MOBS_start_positonX 300
#define MOBS_start_positonY  0 //center of window
double radians_mob = 0;
int max_mobs = 3; //number of max potion produce
bool mobscheck = false;
bool mobs_stop = false;
bool mobs_spawn = false;

/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void boss_load()
{
	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	bgBoss.bgTex = AEGfxTextureLoad("Assets/Boss_BG.png");	// BG Texture
	SquareMesh(&bgBoss.bgMesh, 0);							// BG Mesh
	bgBoss.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgBoss.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();


	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/
	// player 1 mesh 
	//SquareMesh(&player1.pMesh, player1.size, player1.size, 0xFFB62891);
	SquareMesh(&player1.pMesh, 0xFFB62891); //0xFFB62891
	// player 2 mesh
	//SquareMesh(&player2.pMesh, player2.size, player2.size, 0xFFFF00FF);
	SquareMesh(&player2.pMesh, 0xFFFF00FF);

	//creating bullet mesh
	SquareMesh(&pBullet, 0xFFFFFFFF);

	SquareMesh(&boss.pMesh1, 0xFFFFFF00);
	//Creating Health Mesh
	//SquareMesh(&health.pMesh1,0x00FF0000);
	//SquareMesh(&health2.pMesh, 0x00999999);
	SquareMesh(&boss.Bhealth.pMesh, 0x00FF0000);

	//PLeyar Meash
	SquareMesh(&p1health.pMesh, 0x00FF0000);
	SquareMesh(&p2health.pMesh, 0x00FF0000);

	// boss bullet mesh 
	SquareMesh(&pbossbullet, 0x00FF0000);

	//Creating Potion
	SquareMesh(&potion.pMesh, 0x00FFFFFF);

	//mobs mesh
	SquareMesh(&mobs.pMesh, 0x003333FF);

	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/
	player1.pTex = AEGfxTextureLoad("Assets/Player1.png");
	player2.pTex = AEGfxTextureLoad("Assets/Player2.png");
	boss.pTex = AEGfxTextureLoad("Assets/BOSS.png");
	potion.pTex = AEGfxTextureLoad("Assets/potion.png");
	mobs.pTex = AEGfxTextureLoad("Assets/mobs.png");


	/*------------------------------------------------------------
	LOAD SOUND EFFECTS/AUDIO
	------------------------------------------------------------*/
	collect.audio = AEAudioLoadSound("Assets/Audio/collect.wav");
	collect.aGroup = AEAudioCreateGroup();

}

void boss_init()
{
	player1.pCoord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 50 };
	player2.pCoord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 200 };
	player1.alive = true;
	player2.alive = true;


	DEFAULT_HP = (f32)AEGetWindowWidth() + 70.0f;
	player_default_hp = player2.size; //default player's hp size
	player2_default_hp = player1.size;
	player1.HP = PLAYER_MAX_HP;
	player2.HP = PLAYER2_MAX_HP; //initialise the first length of max_hp

	boss.HP = BOSS_MAX_HP;
	boss.alive = true;
	boss.Bcoord = { 380.0f, -30.f };
	//counters
	bossTimeElapsed = 0.0;
	bulletTimeElapsed = 0.0;
	bossmovetime = 0.0;

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bossbullets1[i].shot = false;
		bossbullets1[i].shot = false;
	}

}

void boss_update()
{
	// TIME COUNTER 
	bulletTimeElapsed += AEFrameRateControllerGetFrameTime();
	bossTimeElapsed += AEFrameRateControllerGetFrameTime();
	bossmovetime += AEFrameRateControllerGetFrameTime();

	/*------------------------------------------------------------
	CHANGE STATE CONDITION
	------------------------------------------------------------*/
	//NOT DONE!! still testing
	//players win
	if (!boss.alive)
	{
		next_state = ENDGAME;
	}

	//players lose
	if (!player1.alive && !player2.alive)
	{
		next_state = LOSE_BOTHPLAYERS;
	}


	//for testing
	if (AEInputCheckCurr(AEVK_1)) {
		next_state = PUZZLE;
	}
	if (AEInputCheckCurr(AEVK_2)) {
		next_state = RACING;
	}
	if (AEInputCheckCurr(AEVK_Q)) {
		next_state = QUIT;
	}
	if (AEInputCheckCurr(AEVK_ESCAPE)) {
		curr_state = RESTART;
		next_state = MENU;
	}

	/*------------------------------------------------------------
	PLAYER UPDATE
	------------------------------------------------------------*/
	input_handle();
	AEAudioUpdate();

	if (player1.HP < 0)
	{
		player1.alive = false;
	}
	if (player2.HP < 0)
	{
		player2.alive = false;
		//std::cout << "player 2 dead \n";
	}
	/*------------------------------------------------------------
	BOSS UPDATE
	------------------------------------------------------------*/
	if (boss.HP < 0) {

		//bossHPbar = FALSE;
		boss.alive = FALSE; //Boss DIES	
		//boss.Bhealth.length = bossHP;
	}
	

	/*------------------------------------------------------------
	BULLET MOVEMENT
	------------------------------------------------------------*/
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bulletTimeElapsed >= 0.5 && bullets1[i].shot == FALSE && bullets2[i].shot == FALSE) // every 2 secs 
		{
			bullets1[i].shot = TRUE;
			bullets2[i].shot = TRUE;
	
			//bossTimeElapsed = 0.0;
			bulletTimeElapsed = 0.0;
		}
		if (boss.alive == FALSE) { //bullets will stop shooting when monster dies

			bullets1[i].shot = FALSE;
			bullets2[i].shot = FALSE;
		}

	}
	for (int i = 0; i < MAX_BULLETS; i++)
	{

		if (player1.alive) {

			if (bullets1[i].shot)
			{
				bullets1[i].bVel.x = BULLETSPEED; // bullet speed 
				bullets1[i].bCoord.x += bullets1[i].bVel.x;
				//std::cout << "bullets 1 no. " << i << " launched \n";
			}
			else
			{
				bullets1[i].bCoord = { player1.pCoord.x + (player1.size / 2.0f), player1.pCoord.y };
			}
		}
		else //if player ded
		{
			bullets1[i].shot = false;
		}

		if (bullets1[i].bCoord.x >= AEGfxGetWinMaxX()) // if exit map 
		{
			bullets1[i].shot = FALSE;
		}

		if (player2.alive) {

			if (bullets2[i].shot)
			{
				bullets2[i].bVel.x = BULLETSPEED; // bullet speed 
				bullets2[i].bCoord.x += bullets2[i].bVel.x;
				//std::cout << "bullets 2 no. " << i << " launched \n";
			}
			else
			{
				bullets2[i].bCoord = { player2.pCoord.x + (player2.size / 2.0f), player2.pCoord.y };
			}
		}
		else //if player ded
		{
			bullets2[i].shot = false;
		}
		if (bullets2[i].bCoord.x >= AEGfxGetWinMaxX()) // if exit map 
		{
			bullets2[i].shot = FALSE;
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
			bossbullets1[i].coords.x -= (f32)(bossbullets1[i].velocity.x * AEFrameRateControllerGetFrameTime() * bossbullets1[i].speed); // bullet speed
			bossbullets1[i].coords.y += (f32)(bossbullets1[i].velocity.y * AEFrameRateControllerGetFrameTime() * bossbullets1[i].speed); // bullet speed
		}
		else if (!bossbullets1[i].shot || !boss.alive) {
			bossbullets1[i].coords = { boss.Bcoord.x - (boss.length / 2.0f), boss.Bcoord.y };
		}
		if (bossbullets1[i].coords.x <= AEGfxGetWinMinX() || bossbullets1[i].coords.y <= AEGfxGetWinMinY() || bossbullets1[i].coords.x >= AEGfxGetWinMaxX() || bossbullets1[i].coords.y >= AEGfxGetWinMaxY()) // if exit map
		{
			bossbullets1[i].shot = FALSE;
		}
	}
	/*------------------------------------------------------------
	COLLISION CHECKS
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



	//update boss bounding box
	boss.boundingBox.min.x = boss.Bcoord.x - boss.length / 2.0f;
	boss.boundingBox.min.y = boss.Bcoord.y - boss.height / 2.0f;
	boss.boundingBox.max.x = boss.Bcoord.x + boss.length / 2.0f;
	boss.boundingBox.max.y = boss.Bcoord.y + boss.height / 2.0f;

	//update potion bounding box
	potion.boundingBox.min.x = potion.vector.x - potion.size / 2.0f;
	potion.boundingBox.min.y = potion.vector.y - potion.size / 2.0f;
	potion.boundingBox.max.x = potion.vector.x + potion.size / 2.0f;
	potion.boundingBox.max.y = potion.vector.y + potion.size / 2.0f;

	//update mobs bounding box
	mobs.boundingBox.min.x = mobs.vector.x - mobs.size / 2.0f;
	mobs.boundingBox.min.y = mobs.vector.y - mobs.size / 2.0f;
	mobs.boundingBox.max.x = mobs.vector.x + mobs.size / 2.0f;
	mobs.boundingBox.max.y = mobs.vector.y + mobs.size / 2.0f;


	for (int i = 0; i < MAX_BULLETS; i++) // for bullet hit boss
	{
		//update bounding box
		bullets1[i].boundingBox.min.x = bullets1[i].bCoord.x - bullets1[i].length / 2.0f;
		bullets1[i].boundingBox.min.y = bullets1[i].bCoord.y - bullets1[i].height / 2.0f;
		bullets1[i].boundingBox.max.x = bullets1[i].bCoord.x + bullets1[i].length / 2.0f;
		bullets1[i].boundingBox.max.y = bullets1[i].bCoord.y + bullets1[i].height / 2.0f;

		bullets2[i].boundingBox.min.x = bullets2[i].bCoord.x - bullets2[i].length / 2.0f;
		bullets2[i].boundingBox.min.y = bullets2[i].bCoord.y - bullets2[i].height / 2.0f;
		bullets2[i].boundingBox.max.x = bullets2[i].bCoord.x + bullets2[i].length / 2.0f;
		bullets2[i].boundingBox.max.y = bullets2[i].bCoord.y + bullets2[i].height / 2.0f;

		//Shoot BOSS
		//if (bullets1[i].bCoord.x >= 250 && bullets1[i].bCoord.x <= 252 || bullets1[i].bCoord.x > 200 && bullets1[i].bCoord.x < 210) //at a nearer distance it is still able to damage the boss
		if (CollisionIntersection_RectRect(bullets1[i].boundingBox, bullets1[i].bVel, boss.boundingBox, boss.bossVel) && boss.alive) //if player1 or player2 bullet collide with boss && boss is alive
		{
			if (player1.alive ) {

				boss.HP -= PLAYERDMG; //decrease monster health
				bullets1[i].shot = false;
			}
				
			

		}
		if (CollisionIntersection_RectRect(bullets2[i].boundingBox, bullets2[i].bVel, boss.boundingBox, boss.bossVel) && boss.alive)
		{
			if (player2.alive) {

				boss.HP -= PLAYERDMG; //decrease monster health
				bullets2[i].shot = false;
			}



		}
		//Shoot MOBS
		if (CollisionIntersection_RectRect(bullets2[i].boundingBox, bullets2[i].bVel, mobs.boundingBox, mobs.MobsVelocity)
			|| CollisionIntersection_RectRect(bullets1[i].boundingBox, bullets1[i].bVel, mobs.boundingBox, mobs.MobsVelocity)) //if player1 or player2 bullet collide with boss && boss is alive
		{

			mobs.vector = { -1000,-1000 };
			max_mobs -= 1; //decrease mobs spawn


		}

	}
	for (int i = 0; i < MAXWAVE; i++) // for bullet hit players
	{
		//update bounding box
		bossbullets1[i].boundingBox.min.x = bossbullets1[i].coords.x - bossbullets1[i].size / 2.0f;
		bossbullets1[i].boundingBox.min.y = bossbullets1[i].coords.y - bossbullets1[i].size / 2.0f;
		bossbullets1[i].boundingBox.max.x = bossbullets1[i].coords.x + bossbullets1[i].size / 2.0f;
		bossbullets1[i].boundingBox.max.y = bossbullets1[i].coords.y + bossbullets1[i].size / 2.0f;

		if (CollisionIntersection_RectRect(bossbullets1[i].boundingBox, bossbullets1[i].velocity, player1.boundingBox, player1.pVel) && player1.alive)// if bullet hit player 1 && player alive
		{
			player1.HP -= BOSSATTACK_1_DMG;
			bossbullets1[i].shot = false;
		}
		if (CollisionIntersection_RectRect(bossbullets1[i].boundingBox, bossbullets1[i].velocity, player2.boundingBox, player2.pVel) && player2.alive)// if bullet hit player 1 && player alive
		{
			player2.HP -= BOSSATTACK_1_DMG;
			bossbullets1[i].shot = false;
		}
	}

	/*------------------------------------------------------------
	POTION COLLECTION
	------------------------------------------------------------*/
	timer = timer + 1;
	potion_position(potion.vector.x, potion.vector.y, potion_produce, check, potion_stop, timer);



	if (potion_stop != true) {

		if (potion_produce == true) {

			if (CollisionIntersection_RectRect(player2.boundingBox, player2.pVel, potion.boundingBox, potion.pVelocity))
			{
				
				player2.HP = PLAYER2_MAX_HP; //refill to full hp bar
				potion.vector = { -1000,-1000 }; 
				max_potion -= 1;
				AEAudioPlay(collect.audio, collect.aGroup, 1, 1, 0);


			}
			if (CollisionIntersection_RectRect(player1.boundingBox, player1.pVel, potion.boundingBox, potion.pVelocity))
			{
				
				player1.HP = PLAYER_MAX_HP; //refill to full hp bar

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

	/*------------------------------------------------------------
	MOBS SPAWN
	------------------------------------------------------------*/
	mobs_position(mobs.vector.x, mobs.vector.y, mobs_spawn, mobscheck, mobs_stop, timer);



	if (mobs_stop != true) {

		if (mobs_spawn == true) {

			if (CollisionIntersection_RectRect(player2.boundingBox, player2.pVel, mobs.boundingBox, mobs.MobsVelocity)) {
				player2.HP -= MOBSATTACK_DMG;

			}
			if (CollisionIntersection_RectRect(player1.boundingBox, player1.pVel, mobs.boundingBox, mobs.MobsVelocity)) {
				player1.HP -= MOBSATTACK_DMG;

			}

		}

	}
	//stop spawning mobs
	if (max_mobs == 0) {
		mobs_stop = true;
	}



	/*------------------------------------------------------------
	UPDATE PLAYER POSITIONS
	------------------------------------------------------------*/
	player1.pCoord.x += player1.pVel.x * player1.pAcceleration * g_dt;
	player1.pCoord.y += player1.pVel.y * player1.pAcceleration * g_dt;

	player2.pCoord.x += player2.pVel.x * player2.pAcceleration * g_dt;
	player2.pCoord.y += player2.pVel.y * player2.pAcceleration * g_dt;



	/*------------------------------------------------------------
	MATRIX CALCULATION
	------------------------------------------------------------*/
	// for background
	MatrixCalc(bgBoss.transform, bgBoss.length, bgBoss.height, 0.0f, bgBoss.bgCoord);

	// for players 
	MatrixCalc(player1.transform, player1.size, player1.size, 0.0f, player1.pCoord);
	MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.pCoord);

	// for bullet 
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets1[i].shot)
		{
			MatrixCalc(bullets1[i].transform, bullets1[i].length, bullets1[i].height, 0.f, bullets1[i].bCoord);
		}
		if (bullets2[i].shot)
		{
			MatrixCalc(bullets2[i].transform, bullets2[i].length, bullets2[i].height, 0.f, bullets2[i].bCoord);
		}

	}

	//for boss
	MatrixCalc(boss.transform, boss.length, boss.height, 0.f, boss.Bcoord);

	/*------------------------------------------------------------
	BOSS HEALTH
	------------------------------------------------------------*/
	//Update Boss's Current HP
	hp_percentage = boss.HP / BOSS_MAX_HP;
	newBar = hp_percentage * DEFAULT_HP;
	boss.Bhealth.length = newBar;
	MatrixCalc(boss.Bhealth.transform, boss.Bhealth.length, boss.Bhealth.height, 0.f, boss.Bhealth.Hcoord);

	/*------------------------------------------------------------
	PLAYERS' HEALTH
	------------------------------------------------------------*/
	//for player2's health bar
	player2_hp_percentage = player2.HP / PLAYER2_MAX_HP;
	player2newbar = player2_hp_percentage * player2_default_hp;
	p2health.plength = player2newbar;
	p2health.Hcoord2 = { player2.pCoord.x, player2.pCoord.y + 35.0f };
	//p2health.plength = player2.HP / PLAYER_MAX_HP * player2.size;
	MatrixCalc(p2health.transform, p2health.plength, p2health.pheight, 0.f, p2health.Hcoord2);


	//for player1's health bar
	player_hp_percentage = player1.HP / PLAYER_MAX_HP;
	playernewbar = player_hp_percentage * player_default_hp;
	p1health.plength = playernewbar;
	p1health.Hcoord2 = { player1.pCoord.x, player1.pCoord.y + 35.0f };
	MatrixCalc(p1health.transform, p1health.plength, p1health.pheight, 0.f, p1health.Hcoord2);


	// for boss attacks
	for (int i = 0; i < MAXWAVE; i++)
	{
		MatrixCalc(bossbullets1[i].transform, bossbullets1[i].size, bossbullets1[i].size, bossbullets1[i].direction, bossbullets1[i].coords);
	}

	//Health potion
	MatrixCalc(potion.PotionTransform, potion.size, potion.size, 0.f, potion.vector);


	//Mobs
	MatrixCalc(mobs.MobsTransform, mobs.size, mobs.size, 0.f, mobs.vector);

}

void boss_draw()
{

	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	AEGfxSetTransform(bgBoss.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(bgBoss.bgTex, 0.f, 0.f);
	AEGfxMeshDraw(bgBoss.bgMesh, AE_GFX_MDM_TRIANGLES);

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
		AEGfxTextureSet(player1.pTex, 0, 0);
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);

		// Render health bar
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTransform(p1health.transform.m);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxMeshDraw(p1health.pMesh, AE_GFX_MDM_TRIANGLES);
	}
	// drawing player 2
	if (player2.alive)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransform(player2.transform.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(player2.pTex, 0, 0);
		AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);

		// Render health bar
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTransform(p2health.transform.m);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxMeshDraw(p2health.pMesh, AE_GFX_MDM_TRIANGLES);
	}

	/*------------------------------------------------------------
	DRAWING BULLETS
	------------------------------------------------------------*/
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets1[i].shot && player1.alive)
		{
			//AEGfxSetPosition(bullets1[i].bCoord.x, bullets1[i].bCoord.y);
			AEGfxSetTransform(bullets1[i].transform.m);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxMeshDraw(pBullet, AE_GFX_MDM_TRIANGLES);
		}
		if (bullets2[i].shot && player2.alive)
		{
			//AEGfxSetPosition(bullets2[i].bCoord.x, bullets2[i].bCoord.y);
			AEGfxSetTransform(bullets2[i].transform.m);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxMeshDraw(pBullet, AE_GFX_MDM_TRIANGLES);
		}

	}
	/*------------------------------------------------------------
	DRAWING  BOSS ATTACKS
	------------------------------------------------------------*/
	for (int i = 0; i < MAXWAVE; i++) {
		if (bossbullets1[i].shot && boss.alive)
		{
			//AEGfxSetPosition(bullets1[i].bCoord.x, bullets1[i].bCoord.y);
			AEGfxSetTransform(bossbullets1[i].transform.m);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxMeshDraw(pbossbullet, AE_GFX_MDM_TRIANGLES);
		}
	}


	if (mobs_stop == false) {

		//Render Mobs

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransform(mobs.MobsTransform.m);
		AEGfxTextureSet(mobs.pTex, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxMeshDraw(mobs.pMesh, AE_GFX_MDM_TRIANGLES);

	}

	/*------------------------------------------------------------
	 Rendering of Boss Health System/ BOSS
	------------------------------------------------------------*/
	if (boss.alive) {

		// drawing boss
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransform(boss.transform.m);
		AEGfxTextureSet(boss.pTex, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxMeshDraw(boss.pMesh1, AE_GFX_MDM_TRIANGLES);
		// drawing Current boss.Bhealth
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTransform(boss.Bhealth.transform.m);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxMeshDraw(boss.Bhealth.pMesh, AE_GFX_MDM_TRIANGLES);


	}
	/*------------------------------------------------------------
	 Rendering of Boss Health System/ BOSS
	------------------------------------------------------------*/
	if (potion_stop == false) {

		// drawing potion
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransform(potion.PotionTransform.m);
		AEGfxTextureSet(potion.pTex, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxMeshDraw(potion.pMesh, AE_GFX_MDM_TRIANGLES);

	}






}

void boss_free()
{
	//counters
	bossTimeElapsed = 0.0;
	bulletTimeElapsed = 0.0;
	bossmovetime = 0.0;

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bossbullets1[i].shot = false;
		bossbullets1[i].shot = false;
	}
}

void boss_unload()
{
	AEGfxMeshFree(bgBoss.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgBoss.bgTex); // Unload Texture

	AEGfxMeshFree(player1.pMesh);
	AEGfxTextureUnload(player1.pTex);
	AEGfxTextureUnload(player2.pTex);
	AEGfxMeshFree(player2.pMesh);
	AEGfxMeshFree(boss.Bhealth.pMesh);
	AEGfxMeshFree(p1health.pMesh);
	AEGfxMeshFree(p2health.pMesh);
	AEGfxMeshFree(pbossbullet);
	AEGfxTextureUnload(mobs.pTex);
	AEGfxTextureUnload(potion.pTex);
	AEGfxMeshFree(mobs.pMesh);
	AEGfxMeshFree(potion.pMesh);

	AEGfxMeshFree(boss.pMesh1);
	AEGfxTextureUnload(boss.pTex);
	AEGfxMeshFree(pBullet);
	AEAudioExit();

}
void potion_position(float& x, float& y, bool& potion_produce, bool& check, bool& potion_stop, int& timer)
{
	if (potion_stop != true) {

		if (timer % 100 == 0 && potion_produce != true) {
			x = potion_start_positonX; //starting position
			y = potion_start_positonY;
			potion_produce = true;
		}

		if (timer % 900 == 0) {
			potion_produce = false;
		}

		if (potion_produce == true) {
			if ((y != 0) && (check == false)) {
				y += y_change_direction;
			}
			else {
				check = true;
				y -= y_change_direction;
			}

			if (y == ht_potion_jump) {
				check = false; // amplitude of the graph
			}

			if (x >= potion_start_positonX) {
				x += width_potion; //period 

			}
		}
	}
}

void mobs_position(float& x, float& y, bool& mobs_spawn, bool& mobscheck, bool& mobs_stop, int& timer)
{

	if (mobs_stop != true) {

		if (timer % 50 == 0 && mobs_spawn != true) { //start spawning
			x = MOBS_start_positonX;
			y = MOBS_start_positonY;
			mobs_spawn = true;
		}


		if (timer % 900 == 0) { //stop spawning
			mobs_spawn = false;
		}

		if (mobs_spawn == true) {
			if ((mobscheck == false)) {
				x -= (float)cos(radians_mob);
			}



		}
	}
}