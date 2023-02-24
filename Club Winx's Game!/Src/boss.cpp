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
#include "AEGraphics.h"

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

struct Bullet {
	AEVec2 bCoord{ 0.0f,0.0f };
	bool shot{ FALSE };
	AEMtx33 transform;
};

Bullet bullets1[MAX_BULLETS], bullets2[MAX_BULLETS];
AEGfxVertexList* pBullet{ nullptr };

f64 bossTimeElapsed = 0.0;

//Boss Health
float bossHP = 150.f; //
float damage = 10.f; //Player's hit
bool bossAlive{ TRUE };
bool bossHPbar{ TRUE };
float  boss_max_hp = 150.f;
float hp_percentage;
float default_hp = 1200.f; //length of hp
float newBar;

//Player Health
float playerHP = 50.f;

// BOSS ATTACKS
const int MAXWAVE = { 5 };
struct bosspew {
	AEVec2 coords; // coords
	AEMtx33 transform; // transform 
	AEVec2 velocity; // 
	bool shot{ FALSE };
	f32 direction; // 
};

bosspew bossbullets1[MAXWAVE], bossbullets2[MAXWAVE];
AEGfxVertexList* pbossbullet{ nullptr };

const f32 gravity = 9.8f;




/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void boss_load()
{
	std::cout << "boss:Load\n";

	/*------------------------------------------------------------
	SETTING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetBackgroundColor(0.0f, 0.0f, 255.0f);
	bgBoss.bgTex = AEGfxTextureLoad("Assets/Boss_BG.png");		// BG Texture
	SquareMesh(&bgBoss.bgMesh, 0);							// BG Mesh
	bgBoss.length = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	bgBoss.height = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
	

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

	bossTimeElapsed = 0.0;
	bool BossAlive = { TRUE };
	bool bossHP = { TRUE };
}

void boss_update()
{
	std::cout << "boss:Update\n";

	// TIME COUNTER 
	bossTimeElapsed += AEFrameRateControllerGetFrameTime();

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
	BULLET MOVEMENT
	------------------------------------------------------------*/
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bossTimeElapsed >= 0.5 && bullets1[i].shot == FALSE && bullets2[i].shot == FALSE) // every 2 secs 
		{
			//bossbullets1[i].shot == FALSE && bossbullets2[i].shot == FALSE
			bullets1[i].shot = TRUE;
			std::cout << " bullet1 no. " << i << "launched\n";
			bullets2[i].shot = TRUE;
			std::cout << " bullet2 no. " << i << "launched\n";
			

			/*bossbullets1[i].shot = TRUE;
			std::cout << " bossbullet1 no. " << i << "launched\n";
			bossbullets2[i].shot = TRUE;
			std::cout << " bossbullet2 no. " << i << "launched\n";*/
			//bossTimeElapsed = 0.0;
			bossTimeElapsed = 0.0;
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
			bullets1[i].bCoord.x += 10.0f; // bullet speed 
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
			bullets2[i].bCoord.x += 10.0f; // bullet speed to change to const variable
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

			bossHP -= damage;	//decrease monster health
			std::cout << " monster lives:  " << bossHP << " \n";

		}
		if (bullets2[i].bCoord.x >= 250 && bullets2[i].bCoord.x <= 252 || bullets1[i].bCoord.x > 200 && bullets1[i].bCoord.x < 210)
		{

			bossHP -= damage; 
			std::cout << " monster lives:  " << bossHP << " \n";

		}

	}
	// BOSS ATTACKS 
	//for (int i = 0; i < MAXWAVE; i++)
	//{
		/*if (bossTimeElapsed >= 0.5 && bossbullets1[i].shot == FALSE && bossbullets2[i].shot == FALSE) // every 2 secs
		{
			bossbullets1[i].shot = TRUE;
			std::cout << " bossbullet1 no. " << i << "launched\n";
			bossbullets2[i].shot = TRUE;
			std::cout << " bossbullet2 no. " << i << "launched\n";
			bossTimeElapsed = 0.0;
		}
	//}
	for (int i = 0; i < MAXWAVE; i++)
	{
		if (bossbullets1[i].shot)
		{
			bossbullets1[i].coords.x -= 10.0f; // bullet speed
		}
		else {
			bossbullets1[i].coords = { boss.Bcoord.x + (boss.length / 2.0f), boss.Bcoord.y };
		}
		if (bossbullets1[i].coords.x <= AEGfxGetWinMinX()) // if exit map
		{
			bossbullets1[i].shot = FALSE;
		}
	}*/
	/*------------------------------------------------------------
	MATRIX CALCULATION 
	------------------------------------------------------------*/
	// for background
	MatrixCalc(bgBoss.transform, bgBoss.length, bgBoss.height, 0.0f, bgBoss.bgCoord);

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
		MatrixCalc(bossbullets1[i].transform, 20.0f, 20.0f, 0.f, bossbullets1[i].coords);
	}
	
}

void boss_draw()
{
	std::cout << "boss:Draw\n";

	/*------------------------------------------------------------
	DRAWING BACKGROUND
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTextureMode(AE_GFX_TM_PRECISE);
	AEGfxSetTransform(bgBoss.transform.m);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(bgBoss.bgTex, 0.f, 0.f);
	AEGfxMeshDraw(bgBoss.bgMesh, AE_GFX_MDM_TRIANGLES);

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

	AEGfxMeshFree(bgBoss.bgMesh); // free BG Mesh
	AEGfxTextureUnload(bgBoss.bgTex); // Unload Texture

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