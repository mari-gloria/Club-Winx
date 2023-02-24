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

#include "AEEngine.h"
#include "AEGameStateMgr.h"

#include "gsm.h"
#include "gamestatelist.h"
#include "boss.h"
#include "general.h"

#include <iostream>
// ---------------------------------------------------------------------------




/*------------------------------------------------------------
GLOBALS
------------------------------------------------------------*/
int const MAX_BULLETS{ 10 }; // number of max bullets on screen 

/*------------------------------------------------------------
PLAYER BULLETS
------------------------------------------------------------*/
struct Bullet {
	AEVec2 bCoord{ 0.0f,0.0f };
	bool shot{ FALSE };
	AEMtx33 transform;
};

Bullet bullets1[MAX_BULLETS], bullets2[MAX_BULLETS];
AEGfxVertexList* pBullet{ nullptr };
const f32 BULLETSPEED = 10.0f;
f64 bulletTimeElapsed = 0.0;
/*-----------------------------------------------------------*/
/*------------------------------------------------------------
HEALTH
------------------------------------------------------------*/
float bossHP = 150.f; //
const float PLAYERDMG = 0.f; //Player's hit
bool bossAlive{ TRUE };
bool bossHPbar{ TRUE };
float  boss_max_hp = 150.f;
float hp_percentage;
float default_hp = 1200.f; //length of hp
float newBar;

//Player Health
float playerHP = 50.f;
/*-----------------------------------------------------------*/



/*------------------------------------------------------------
BOSS ATTACKS
------------------------------------------------------------*/
const int MAXWAVE = { 40 };
struct bosspew {
	AEVec2 coords; // coords
	AEMtx33 transform; // transform 
	AEVec2 velocity; // 
	bool shot{ FALSE };
	f32 direction{ 30.0f }; // 
	f32 speed{100.f};
};

bosspew bossbullets1[MAXWAVE], bossbullets2[MAXWAVE];
AEGfxVertexList* pbossbullet{ nullptr };

const f32 gravity = 9.8f;
f64 bossTimeElapsed = 0.0f; 
f32 bossmovetime = 0.0f;
/*-----------------------------------------------------------*/



/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void boss_load()
{
	std::cout << "boss:Load\n";

	//setting bg
	AEGfxSetBackgroundColor(0.0f, 0.0f, 255.0f);

	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/
	// player 1 mesh 
	//SquareMesh(&player1.pMesh, player1.size, player1.size, 0xFFB62891);
	SquareMesh(&player1.pMesh,  0xFFB62891);
	// player 2 mesh
	//SquareMesh(&player2.pMesh, player2.size, player2.size, 0xFFFF00FF);
	SquareMesh(&player2.pMesh, 0xFFFF00FF);

	//creating bullet mesh
	SquareMesh(&pBullet, 0xFFFFFFFF);
	
	SquareMesh(&boss.pMesh1, 0xFFFFFF00);
	//Creating Health Mesh
	//SquareMesh(&health.pMesh1,0x00FF0000);
	SquareMesh(&health2.pMesh, 0x00999999);
	SquareMesh(&health.pMesh, 0x00FF0000);

	//PLeyar Meash
	SquareMesh(&p1health.pMesh, 0x00FF0000);
	SquareMesh(&p2health.pMesh, 0x00FF0000);

	// boss bullet mesh 
	SquareMesh(&pbossbullet, 0x00FF0000);
	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/


	/*------------------------------------------------------------
	CREATING FONTS
	------------------------------------------------------------*/


}

void boss_init()
{
	std::cout << "boss:Initialize\n";

	player1.pCoord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 50};
	player2.pCoord = { AEGfxGetWinMinX() + 50, AEGfxGetWinMinY() + 200 };

	for (int i = 0; i < MAXWAVE; i++)
	{
		bossbullets1[i].coords = { boss.Bcoord.x - (boss.length / 2.0f), boss.Bcoord.y };

	}

	bossTimeElapsed = 0.0;
	bulletTimeElapsed = 0.0;
	bossmovetime = 0.0;
	bool BossAlive = { TRUE };
	bool bossHP = { TRUE };
}

void boss_update()
{
	std::cout << "boss:Update\n";

	// TIME COUNTERS
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
	PLAYER MOVEMENT
	------------------------------------------------------------*/
	input_handle(); 
	
	/*------------------------------------------------------------
	BOSS MOVEMENT
	------------------------------------------------------------*/
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
		if (bossHP < 3) { //bullets will stop shooting when monster dies

			//bullets1[i].shot = FALSE;
			//bullets2[i].shot = FALSE;
		}

	}
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bullets1[i].shot)
		{
			bullets1[i].bCoord.x += BULLETSPEED; // bullet speed 
		}
		else {
			bullets1[i].bCoord = { player1.pCoord.x + (player1.size / 2.0f), player1.pCoord.y  };
		}

		if (bullets1[i].bCoord.x >= AEGfxGetWinMaxX()) // if exit map 
		{
			bullets1[i].shot = FALSE;
		}

		if (bullets2[i].shot)
		{
			bullets2[i].bCoord.x += BULLETSPEED; // bullet speed s
		}
		else {
			bullets2[i].bCoord = { player2.pCoord.x + (player2.size / 2.0f), player2.pCoord.y  };
		}

		if (bullets2[i].bCoord.x >= AEGfxGetWinMaxX()) // if exit map 
		{
			bullets2[i].shot = FALSE;
		}
		if (bullets1[i].bCoord.x >= 250 && bullets1[i].bCoord.x <= 252 || bullets1[i].bCoord.x >200  && bullets1[i].bCoord.x < 210) //at a nearer distance it is still able to damage the boss
		{

			bossHP -= PLAYERDMG;	//decrease monster health
			std::cout << " monster lives:  " << bossHP << " \n";

		}
		if (bullets2[i].bCoord.x >= 250 && bullets2[i].bCoord.x <= 252 || bullets1[i].bCoord.x > 200 && bullets1[i].bCoord.x < 210)
		{

			bossHP -= PLAYERDMG;
			std::cout << " monster lives:  " << bossHP << " \n";

		}

	}
	// BOSS ATTACKS 
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
		if (bossbullets1[i].shot)
		{
			//bossbullets1[i].coords.x -= 10.0f;
			//bossbullets1[i].coords.y += 0.0005f * (bossbullets1[i].coords.x * bossbullets1[i].coords.x);
			bossbullets1[i].direction = rand_float(-PI,PI); // base direction
			//float curve = sinf(AEFrameRateControllerGetFrameTime()); // determines the strength of the curve
			//bossbullets1[i].coords.x -= bossbullets1[i].velocity.x; // bullet speed
			//bossbullets1[i].coords.y += bossbullets1[i].velocity.y; // bullet speed
			//bossbullets1[i].direction += 1000.f;
			//bossbullets1[i].direction += 50.f * sinf (0.1f * AEFrameRateControllerGetFrameTime());
			//bossbullets1[i].direction = AEWrap(bossbullets1[i].direction, -PI, PI);
			bossbullets1[i].velocity.x = 80.f * sinf((size_t) (i % 180) / PI) * 0.03; // adds curve to x velocity
			bossbullets1[i].velocity.y = 80.f * cosf((size_t)(i % 180) / PI) * 0.01; // adds curve to y velocity
			//bossbullets1[i].velocity =  { cosf(bossbullets1[i].direction)* 10.f, sinf(bossbullets1[i].direction) * 10.f + 50.f } ;
			//bossbullets1[i].velocity *= 10 
			//f32 curve = sinf(AEFrameRateControllerGetFrameTime() * 0.5f);
			//bossbullets1[i].velocity.x += curve * 5.f;
			//bossbullets1[i].velocity.y += curve * 5.0f;

			//bossbullets1[i].velocity.y += -gravity * AEFrameRateControllerGetFrameTime();  // applies gravity

			//AEVec2 added;
			//AEVec2Set(&added, cosf(bossbullets1[i].direction), sinf(bossbullets1[i].direction));
			std::cout << bossbullets1[i].direction << " TEEHEE ";
			//bossbullets1[i].coords.x -= added.x * bossbullets1[i].velocity.x;
			//bossbullets1[i].coords.y += added.y * (bossbullets1[i].velocity.y + gravity);
			//bossbullets1[i].velocity.y -= gravity;
			bossbullets1[i].coords.x -= bossbullets1[i].velocity.x * AEFrameRateControllerGetFrameTime() *bossbullets1[i].speed; // bullet speed
			bossbullets1[i].coords.y += bossbullets1[i].velocity.y * AEFrameRateControllerGetFrameTime() * bossbullets1[i].speed; // bullet speed
			//bossbullets1[i].coords.x = AEWrap(bossbullets1[i].coords.x, AEGfxGetWinMinX(), boss.Bcoord.x + (boss.length / 2));
		}
		else {
			bossbullets1[i].coords = { boss.Bcoord.x - (boss.length / 2.0f), boss.Bcoord.y };
		}
		if (bossbullets1[i].coords.x <= AEGfxGetWinMinX() || bossbullets1[i].coords.y <= AEGfxGetWinMinY() || bossbullets1[i].coords.x >= AEGfxGetWinMaxX() || bossbullets1[i].coords.y >= AEGfxGetWinMaxY()) // if exit map
		{
			bossbullets1[i].shot = FALSE;
		}
	}
	/*------------------------------------------------------------
	MATRIX CALCULATION 
	------------------------------------------------------------*/
	// for players 
	MatrixCalc(player1.transform, player1.size, player1.size, 0.f, player1.pCoord);
	MatrixCalc(player2.transform, player2.size, player2.size, 0.f, player2.pCoord);

	// for bullet 
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets1[i].shot)
		{
			MatrixCalc(bullets1[i].transform, 10.0f, 5.0f, 0.f, bullets1[i].bCoord);
		}
		if (bullets2[i].shot)
		{
			//AEGfxSetPosition(bullets2[i].bCoord.x, bullets2[i].bCoord.y);
			//AEGfxTextureSet(NULL, 0, 0);
			//AEGfxMeshDraw(pBullet, AE_GFX_MDM_TRIANGLES);
			MatrixCalc(bullets2[i].transform, 10.0f, 5.0f, 0.f, bullets2[i].bCoord);
		}

	}

	//for boss
	MatrixCalc(boss.transform, boss.length, boss.height, 0.f, boss.Bcoord);

	//Update Boss's Current HP
	hp_percentage = bossHP / boss_max_hp;
	newBar = hp_percentage * default_hp;
	health.length = newBar;
	MatrixCalc(health.transform, health.length, health.height, 0.f, health.Hcoord);


	//for player2's health bar
	p2health.plength = playerHP;
	p2health.Hcoord2 = { player2.pCoord.x, player2.pCoord.y + 35.0f };
	MatrixCalc(p2health.transform, p2health.plength, p2health.pheight, 0.f, p2health.Hcoord2);

	//for player1's health bar
	p1health.plength = playerHP;
	p1health.Hcoord2 = { player1.pCoord.x, player1.pCoord.y + 35.0f };
	MatrixCalc(p1health.transform, p1health.plength, p1health.pheight, 0.f, p1health.Hcoord2);
	

	// for boss attacks
	for (int i = 0; i < MAXWAVE; i++)
	{
		MatrixCalc(bossbullets1[i].transform, 25.0f, 25.0f, bossbullets1[i].direction, bossbullets1[i].coords);
	}
	
}

void boss_draw()
{
	std::cout << "boss:Draw\n";

	/*------------------------------------------------------------
	DRAWING PLAYERS
	------------------------------------------------------------*/
	// Drawing object 1
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	// Set position for object 1
	//AEGfxSetPosition(player1.pCoord.x, player1.pCoord.y);
	AEGfxSetTransform(player1.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);


	//Drawing HP for player 1
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetPosition(player1.pCoord.x, player1.pCoord.y + 55.0f);

	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxMeshDraw(health.pMesh5, AE_GFX_MDM_TRIANGLES);

	// drawing player 2
	//AEGfxSetPosition(player2.pCoord.x, player2.pCoord.y);
	AEGfxSetTransform(player2.transform.m);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);


	/*------------------------------------------------------------
	DRAWING PLAYERS's Health Bar
	------------------------------------------------------------*/

	//Player 1 health bar
	AEGfxSetTransform(p1health.transform.m);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxMeshDraw(p1health.pMesh, AE_GFX_MDM_TRIANGLES);

	//Player 2 Health Bar
	AEGfxSetTransform(p2health.transform.m);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxMeshDraw(p2health.pMesh, AE_GFX_MDM_TRIANGLES);



	/*------------------------------------------------------------
	DRAWING BULLETS
	------------------------------------------------------------*/
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets1[i].shot)
		{
			//AEGfxSetPosition(bullets1[i].bCoord.x, bullets1[i].bCoord.y);
			AEGfxSetTransform(bullets1[i].transform.m);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxMeshDraw(pBullet, AE_GFX_MDM_TRIANGLES);
		}
		if (bullets2[i].shot)
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
		if (bossbullets1[i].shot)
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

	if (bossHP < 0) {

		bossHPbar = FALSE;
		bossAlive = FALSE; //Boss DIES	
		health.length = bossHP;
	}


	if (bossHPbar == TRUE) {
		// drawing Current Health
		AEGfxSetTransform(health.transform.m);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxMeshDraw(health.pMesh, AE_GFX_MDM_TRIANGLES);
	}
	else {

		// drawing Current Health
		AEGfxSetTransform(health.transform.m);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(0.0f);
		AEGfxMeshDraw(health.pMesh, AE_GFX_MDM_TRIANGLES);
	}

	if (bossAlive == TRUE) {

		// drawing boss
		AEGfxSetTransform(boss.transform.m);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxMeshDraw(boss.pMesh1, AE_GFX_MDM_TRIANGLES);


	}
	else {

		// BOSS DIES
		AEGfxSetTransform(boss.transform.m);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(0.0f);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxMeshDraw(boss.pMesh1, AE_GFX_MDM_TRIANGLES);
	}

	
}

void boss_free()
{
	std::cout << "boss:Free\n";

}

void boss_unload()
{
	std::cout << "boss:Unload\n";

	AEGfxMeshFree(player1.pMesh);
	AEGfxMeshFree(player2.pMesh);
	AEGfxMeshFree(health.pMesh);
	AEGfxMeshFree(health2.pMesh);
	AEGfxMeshFree(p1health.pMesh);
	AEGfxMeshFree(p2health.pMesh);

	AEGfxMeshFree(pbossbullet);

	//for (int i = 0; i < MAX_BULLETS; i++) {
		AEGfxMeshFree(pBullet);
		//AEGfxMeshFree(pBullet);//}

}