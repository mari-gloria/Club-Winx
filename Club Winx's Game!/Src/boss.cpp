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

struct Bullet {
	AEVec2 bCoord{ 0.0f,0.0f };
	bool shot{ FALSE };
	AEGfxVertexList* pBullet{ nullptr };
};

Bullet bullets1[MAX_BULLETS], bullets2[MAX_BULLETS];

f64 bossTimeElapsed = 0.0;

//Boss Damage
int bossHP = 10;
int damage = 1; //Player's hit





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
	SquareMesh(&player1.pMesh, player1.size, player1.size, 0xFFB62891);

	// player 2 mesh
	SquareMesh(&player2.pMesh, player2.size, player2.size, 0xFFFF00FF);

	//bullet mesh
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		SquareMesh(&bullets1[i].pBullet, 10.0f, 5.0f, 0xFFFFFFFF);
		std::cout << "bullet1 no. " << i << "meshed\n";
		SquareMesh(&bullets2[i].pBullet, 10.0f, 5.0f, 0xFFFFFFFF);
		std::cout << "bullet2 no. " << i << "meshed\n";
	}
	//Creating Boss Mesh
	SquareMesh(&boss.pMesh1, boss.size, boss.size, 0xFFFFFF00);
	SquareMesh(&boss.pMesh2, boss.size, boss.size, 0x000000FF);
	//Creating Boss HP Bar
	SquareMesh(&health.pMesh1, health.length, health.height, 0x00FF0000);
	SquareMesh(&health.pMesh2, health.length - 40.0f, health.height, 0x00FF0000);
	SquareMesh(&health.pMesh3, health.length - 60.0f, health.height, 0x00FF0000);

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
	player2.pCoord = { AEGfxGetWinMinX() + 95, AEGfxGetWinMinY() + 50 };

	bossTimeElapsed = 0.0;
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

	/*------------------------------------------------------------
	PLAYER MOVEMENT
	------------------------------------------------------------*/
	input_handle(); 
	
	/*------------------------------------------------------------
	BULLET MOVEMENT
	------------------------------------------------------------*/
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bossTimeElapsed >= 1.0 && bullets1[i].shot == FALSE && bullets2[i].shot == FALSE) // every 2 secs 
		{
			bullets1[i].shot = TRUE;
			std::cout << " bullet1 no. " << i << "launched\n";
			bullets2[i].shot = TRUE;
			std::cout << " bullet2 no. " << i << "launched\n";
			bossTimeElapsed = 0.0;
		}
		if (bossHP < 2) { //bullets will stop shooting when monster dies

			bullets1[i].shot = FALSE;
			bullets2[i].shot = FALSE;
		}

	}
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bullets1[i].shot)
		{
			bullets1[i].bCoord.x += 10.0f; // bullet speed 
		}
		else {
			bullets1[i].bCoord = { player1.pCoord.x + (player1.size / 2.0f), player1.pCoord.y + (player1.size / 2.0f) };
		}

		if (bullets1[i].bCoord.x >= AEGfxGetWinMaxX()) // if exit map 
		{
			bullets1[i].shot = FALSE;
		}

		if (bullets2[i].shot)
		{
			bullets2[i].bCoord.x += 10.0f; // bullet speed 
		}
		else {
			bullets2[i].bCoord = { player2.pCoord.x + (player2.size / 2.0f), player2.pCoord.y + (player2.size / 2.0f) };
		}

		if (bullets2[i].bCoord.x >= AEGfxGetWinMaxX()) // if exit map 
		{
			bullets2[i].shot = FALSE;
		}
		if (bullets1[i].bCoord.x >= 250 && bullets1[i].bCoord.x <= 252 || bullets1[i].bCoord.x >200  && bullets1[i].bCoord.x < 210)
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
	AEGfxSetPosition(player1.pCoord.x, player1.pCoord.y);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(player1.pMesh, AE_GFX_MDM_TRIANGLES);

	// drawing player 2
	AEGfxSetPosition(player2.pCoord.x, player2.pCoord.y);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(player2.pMesh, AE_GFX_MDM_TRIANGLES);



	/*------------------------------------------------------------
	DRAWING BULLETS
	------------------------------------------------------------*/
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets1[i].shot)
		{
			AEGfxSetPosition(bullets1[i].bCoord.x, bullets1[i].bCoord.y);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxMeshDraw(bullets1[i].pBullet, AE_GFX_MDM_TRIANGLES);
		}
		if (bullets2[i].shot)
		{
			AEGfxSetPosition(bullets2[i].bCoord.x, bullets2[i].bCoord.y);
			AEGfxTextureSet(NULL, 0, 0);
			AEGfxMeshDraw(bullets2[i].pBullet, AE_GFX_MDM_TRIANGLES);
		}

	}
	/*------------------------------------------------------------
	 Rendering of Boss HP Bar
	------------------------------------------------------------*/
	if (bossHP > 9) {
		//Drawing Boss
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(250, -250);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(boss.pMesh1, AE_GFX_MDM_TRIANGLES);

		//Drawing health bar
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(250, -170);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(health.pMesh1, AE_GFX_MDM_TRIANGLES);

	}
	else if (bossHP > 5) {

		//Drawing Boss
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(250, -250);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(boss.pMesh1, AE_GFX_MDM_TRIANGLES);

		//Drawing health bar
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(250, -170);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(health.pMesh2, AE_GFX_MDM_TRIANGLES);


	}
	else if (bossHP > 3) {

		//Drawing Boss
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(250, -250);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(boss.pMesh1, AE_GFX_MDM_TRIANGLES);

		//Drawing health bar
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(250, -170);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(health.pMesh3, AE_GFX_MDM_TRIANGLES);


	}
	else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(250, -250);
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxMeshDraw(boss.pMesh2, AE_GFX_MDM_TRIANGLES);
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
	AEGfxMeshFree(boss.pMesh1);
	AEGfxMeshFree(boss.pMesh2);
	AEGfxMeshFree(health.pMesh1);
	AEGfxMeshFree(health.pMesh2);
	AEGfxMeshFree(health.pMesh3);
	
	for (int i = 0; i < MAX_BULLETS; i++) {
		AEGfxMeshFree(bullets1[i].pBullet);
		AEGfxMeshFree(bullets2[i].pBullet);
	}

}