
#include "General.h"
#include "../Main.h"
#include "Splash_screen.h"


AEGfxVertexList* pMesh1 = 0;
AEGfxTexture* pTex1;
f32 timer, counter;
AEMtx33 scale;
AEVec2 pos{ 0.0f, 0.0f };



void Splash_Load()
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFF0000, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFF0000, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFF0000, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "fail to create object!!");
}
// ----------------------------------------------------------------------------
// This function initialises the variables for Splash Screen
// ----------------------------------------------------------------------------
void Splash_Initialize()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetCamPosition(0.0f, 0.f);
	AEGfxSetTransparency(1.0f);
	AEMtx33Scale(&scale, (f32)AEGetWindowWidth(), (f32)AEGetWindowHeight());
	pTex1 = AEGfxTextureLoad("Assets/splash.png");
	AE_ASSERT_MESG(pTex1, "fail to load photo!!");
	timer = 4.0f;
	counter = timer;
}

void Splash_Update()
{
	AEInputUpdate();

	if (counter <= 0.0f || AEInputCheckCurr(AEVK_SPACE) || AEInputCheckTriggered(AEVK_ESCAPE))
	{
		next_state = MENU;
	}
	counter -= g_dt;
}

void Splash_Draw()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetPosition(0.0f, 0.0f);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(pTex1, 0.0f, 0.0f);
	AEGfxSetTransform(scale.m);
	AEGfxSetTransparency(counter / timer);
	AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);
}

void Splash_Free()
{
}

void Splash_Unload()
{
	AEGfxMeshFree(pMesh1);
	AEGfxTextureUnload(pTex1);
}