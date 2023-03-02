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
const f32 PLAYERDMG = 0.5f; //Player's hit
const f32  BOSS_MAX_HP = 500.f;
const f32 PLAYER_MAX_HP = 150.f;
const f32 BOSSATTACK_1_DMG = 5.5f;
/*------------------------------------------------------------
PLAYER BULLETS
------------------------------------------------------------*/
struct Bullet {
	AEVec2 bCoord{ 0.0f,0.0f };
	bool shot{ FALSE };
	AEMtx33 transform{};
	f32 length{ 10.0f };
	f32 height{ 5.0f };
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

//Player Health
//float playerHP = 50.f;
/*-----------------------------------------------------------*/



/*------------------------------------------------------------
BOSS ATTACKS
------------------------------------------------------------*/
const int MAXWAVE = { 40 };
struct bosspew {
	AEVec2 coords{}; // coords
	AEMtx33 transform{}; // transform 
	AEVec2 velocity{}; // 
	bool shot{ FALSE };
	f32 direction{ 30.0f }; // 
	f32 speed{ 100.f };
	f32 size{ 25.f };
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

	AEGfxVertexList* pMesh1{ nullptr };			// mesh 
	AEGfxTexture* pTex{ nullptr };			// texture
	AEMtx33				transform{};						// transform mtx 
	AEVec2				Bcoord{ 380.0f, -30.f };	// position of boss
	bool				alive{ true };
	Health				Bhealth;
	f32 length = 200.0f; //boss length 
	f32 height = 150.f; // boss height
	f32 HP{ BOSS_MAX_HP };
};
//extern Boss boss;

Boss		boss;

/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void boss_load()
{
	std::cout << "boss:Load\n";

	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	bgBoss.bgTex = AEGfxTextureLoad("Assets/Boss_BG.png");		// BG Texture
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
	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/
	player1.pTex = AEGfxTextureLoad("Assets/Player1.png");
	player2.pTex = AEGfxTextureLoad("Assets/Player2.png");
	boss.pTex = AEGfxTextureLoad("Assets/BOSS.png");

	/*------------------------------------------------------------
	CREATING FONTS
	------------------------------------------------------------*/


}

void boss_init()
{
	std::cout << "boss:Initialize\n";

	player1.pCoord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 50};
	player2.pCoord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 200 };

	player1.HP = player2.HP = PLAYER_MAX_HP;

	

	DEFAULT_HP = (f32)AEGetWindowWidth();

	//counters
	bossTimeElapsed = 0.0;
	bulletTimeElapsed = 0.0;
	bossmovetime = 0.0;
	
}

void boss_update()
{
	std::cout << "boss:Update\n";

	// TIME COUNTER 
	bulletTimeElapsed += AEFrameRateControllerGetFrameTime();
	bossTimeElapsed += AEFrameRateControllerGetFrameTime();
	bossmovetime += AEFrameRateControllerGetFrameTime();

	/*------------------------------------------------------------
	CHANGE STATE CONDITION
	------------------------------------------------------------*/
	if (AEInputCheckCurr(AEVK_Q)) {
		next_state = QUIT;
	}
	if (AEInputCheckCurr(AEVK_1)) {
		next_state = RACING;
	}
	if (AEInputCheckCurr(AEVK_3)) {
		next_state = PUZZLE;
	}


	/*------------------------------------------------------------
	PLAYER UPDATE
	------------------------------------------------------------*/
	input_handle(); 
	
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
	if (boss.HP < 0) {

		//bossHPbar = FALSE;
		boss.alive = FALSE; //Boss DIES	
		//boss.Bhealth.length = bossHP;
	}
	/*f32 eyeye = 1.5f;
	if (bossmovetime < 2.0)
	{
		boss.Bcoord.y -=eyeye;
		//bossmovetime = 0.0;
	}
	else
	{
		boss.Bcoord.y += 2.f *eyeye;

	}
	if (bossmovetime > 4.0)
	{
		bossmovetime = 0;
	}*/
	/*------------------------------------------------------------
	BULLET MOVEMENT
	------------------------------------------------------------*/
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bulletTimeElapsed >= 0.5 && bullets1[i].shot == FALSE && bullets2[i].shot == FALSE) // every 2 secs 
		{
			//bossbullets1[i].shot == FALSE && bossbullets2[i].shot == FALSE
			bullets1[i].shot = TRUE;
			//std::cout << " bullet1 no. " << i << "launched\n";
			bullets2[i].shot = TRUE;
			//std::cout << " bullet2 no. " << i << "launched\n";



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
		if (bullets1[i].shot && player1.alive)
		{
			bullets1[i].bCoord.x += BULLETSPEED; // bullet speed 
		}
		else if (player1.alive) {
			bullets1[i].bCoord = { player1.pCoord.x + (player1.size / 2.0f), player1.pCoord.y };
		}

		if (bullets1[i].bCoord.x >= AEGfxGetWinMaxX()) // if exit map 
		{
			bullets1[i].shot = FALSE;
		}

		if (bullets2[i].shot && player2.alive)
		{
			bullets2[i].bCoord.x += BULLETSPEED; // bullet speed s
		}
		else if (player2.alive) {
			bullets2[i].bCoord = { player2.pCoord.x + (player2.size / 2.0f), player2.pCoord.y };
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
			std::cout << " bossbullet1 no. " << i << "launched\n";
			//bossbullets2[i].shot = TRUE;
			std::cout << " bossbullet2 no. " << i << "launched\n";
			bossTimeElapsed = 0.0;
		}
	}

	for (int i = 0; i < MAXWAVE; i++)
	{
		if (bossbullets1[i].shot && boss.alive)
		{
			//bossbullets1[i].coords.x -= 10.0f;
			//bossbullets1[i].coords.y += 0.0005f * (bossbullets1[i].coords.x * bossbullets1[i].coords.x);
			bossbullets1[i].direction = rand_num(-PI, PI); // base direction
			//float curve = sinf(AEFrameRateControllerGetFrameTime()); // determines the strength of the curve
			//bossbullets1[i].coords.x -= bossbullets1[i].velocity.x; // bullet speed
			//bossbullets1[i].coords.y += bossbullets1[i].velocity.y; // bullet speed
			//bossbullets1[i].direction += 1000.f;
			//bossbullets1[i].direction += 50.f * sinf (0.1f * AEFrameRateControllerGetFrameTime());
			//bossbullets1[i].direction = AEWrap(bossbullets1[i].direction, -PI, PI);
			bossbullets1[i].velocity.x = 80.f * sinf((size_t)(i % 180) / PI) * 0.03f; // adds curve to x velocity
			bossbullets1[i].velocity.y = 80.f * cosf((size_t)(i % 180) / PI) * 0.01f; // adds curve to y velocity
			//bossbullets1[i].velocity =  { cosf(bossbullets1[i].direction)* 10.f, sinf(bossbullets1[i].direction) * 10.f + 50.f } ;
			//bossbullets1[i].velocity *= 10 
			//f32 curve = sinf(AEFrameRateControllerGetFrameTime() * 0.5f);
			//bossbullets1[i].velocity.x += curve * 5.f;
			//bossbullets1[i].velocity.y += curve * 5.0f;

			//bossbullets1[i].velocity.y += -gravity * AEFrameRateControllerGetFrameTime();  // applies gravity

			//AEVec2 added;
			//AEVec2Set(&added, cosf(bossbullets1[i].direction), sinf(bossbullets1[i].direction));
			//std::cout << bossbullets1[i].direction << " TEEHEE ";
			//bossbullets1[i].coords.x -= added.x * bossbullets1[i].velocity.x;
			//bossbullets1[i].coords.y += added.y * (bossbullets1[i].velocity.y + gravity);
			//bossbullets1[i].velocity.y -= gravity;
			bossbullets1[i].coords.x -= (f32)(bossbullets1[i].velocity.x * AEFrameRateControllerGetFrameTime() * bossbullets1[i].speed); // bullet speed
			bossbullets1[i].coords.y += (f32)(bossbullets1[i].velocity.y * AEFrameRateControllerGetFrameTime() * bossbullets1[i].speed); // bullet speed
			//bossbullets1[i].coords.x = AEWrap(bossbullets1[i].coords.x, AEGfxGetWinMinX(), boss.Bcoord.x + (boss.length / 2));
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
	for (int i = 0; i < MAX_BULLETS; i++) // for bullet hit boss
	{
		//if (bullets1[i].bCoord.x >= 250 && bullets1[i].bCoord.x <= 252 || bullets1[i].bCoord.x > 200 && bullets1[i].bCoord.x < 210) //at a nearer distance it is still able to damage the boss
		if ((CollisionIntersection_RectRect(bullets1[i].bCoord,bullets1[i].length,bullets1[i].height,boss.Bcoord,boss.length,boss.height)
			|| CollisionIntersection_RectRect(bullets2[i].bCoord, bullets2[i].length, bullets2[i].height, boss.Bcoord, boss.length, boss.height)) && boss.alive) //if player1 or player2 bullet collide with boss && boss is alive
		{

			boss.HP -= PLAYERDMG;	//decrease monster health
			std::cout << " monster lives:  " << boss.HP	 << " \n";

		}
		
	}
	for (int i = 0; i < MAXWAVE; i++) // for bullet hit players
	{
		if (CollisionIntersection_RectRect(bossbullets1[i].coords, bossbullets1[i].size, bossbullets1[i].size, player1.pCoord, player1.size, player1.size) && player1.alive)// if bullet hit player 1 && player alive
		{
			player1.HP -= BOSSATTACK_1_DMG;
		}
		if (CollisionIntersection_RectRect(bossbullets1[i].coords, bossbullets1[i].size, bossbullets1[i].size, player2.pCoord, player2.size, player2.size) && player2.alive)// if bullet hit player 1 && player alive
		{
			player2.HP -= BOSSATTACK_1_DMG;
		}
	}
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

	//Update Boss's Current HP
	hp_percentage = boss.HP / BOSS_MAX_HP ;
	newBar = hp_percentage * DEFAULT_HP;
	boss.Bhealth.length = newBar;
	std::cout << "boss health length " << boss.Bhealth.length << "\n";
	MatrixCalc(boss.Bhealth.transform, boss.Bhealth.length, boss.Bhealth.height, 0.f, boss.Bhealth.Hcoord);


	//for player2's health bar
	//p2health.plength = player2.HP;
	
	p2health.Hcoord2 = { player2.pCoord.x, player2.pCoord.y + 35.0f };
	p2health.plength = player2.HP / PLAYER_MAX_HP * player2.size;
	MatrixCalc(p2health.transform, p2health.plength, p2health.pheight, 0.f, p2health.Hcoord2);
	

	//for player1's health bar
	//p1health.plength = player1.HP;
	p1health.Hcoord2 = { player1.pCoord.x, player1.pCoord.y + 35.0f };
	p1health.plength = player1.HP / PLAYER_MAX_HP * player1.size;
	MatrixCalc(p1health.transform, p1health.plength, p1health.pheight, 0.f, p1health.Hcoord2);
	

	// for boss attacks
	for (int i = 0; i < MAXWAVE; i++)
	{
		MatrixCalc(bossbullets1[i].transform, bossbullets1[i].size, bossbullets1[i].size, bossbullets1[i].direction, bossbullets1[i].coords);
	}
	
}

void boss_draw()
{
	std::cout << "boss:Draw\n";

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
	// Drawing object 1
	
	
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

		//health bar
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

		//health bar
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
	/*------------------------------------------------------------
	 Rendering of Boss Health System
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
	

	



}

void boss_free()
{
	std::cout << "boss:Free\n";

}

void boss_unload()
{
	std::cout << "boss:Unload\n";

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

	AEGfxMeshFree(boss.pMesh1);
	AEGfxTextureUnload(boss.pTex);
	//for (int i = 0; i < MAX_BULLETS; i++) {
		AEGfxMeshFree(pBullet);
		//AEGfxMeshFree(pBullet);//}

}