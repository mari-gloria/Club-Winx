/*= ================================================================================ =
*All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
*Course: CSD1451
* Group Name : Club Winx
* Primary Author : Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu)
* Secondary Authors :
*
*
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "general.h"

// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// struct

struct pause_buttons
{
	f32 length{ 200.0f };
	f32 height{ 50.0f };

	AEVec2 coord;

	AEGfxVertexList* mesh{nullptr};
	AEGfxTexture* texture{nullptr};

	AEMtx33	transform{};
};
pause_buttons continue_button, back_button;
// ---------------------------------------------------------------------------




// ---------------------------------------------------------------------------
//helper functions
void draw_pause_buttons(pause_buttons button)
{
	AEGfxSetTransform(button.transform.m);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(button.texture, 0, 0);
	AEGfxMeshDraw(button.mesh, AE_GFX_MDM_TRIANGLES);
}

// ---------------------------------------------------------------------------


//variables
int mouse_x, mouse_y;



void pause_load()
{
	//set background - black
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	//button mesh
	SquareMesh(&continue_button.mesh, 0);
	SquareMesh(&back_button.mesh, 0);

	//load button textures
	continue_button.texture = AEGfxTextureLoad("Assets/CONTINUE_BUTTON.png");
	back_button.texture = AEGfxTextureLoad("Assets/BACK_BUTTON.png");
}

void pause_init()
{
	continue_button.coord.x = -125.0f;
	continue_button.coord.y = 0.0f;

	back_button.coord.x = 125.0f;
	back_button.coord.y = 0.0f;
}

void pause_update()
{
	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{
		next_state = MENU;
	}

	//update mouse coord
	AEInputGetCursorPosition(&mouse_x, &mouse_y);

	if (checkHovering(mouse_x, mouse_y, continue_button.length, continue_button.height, continue_button.coord.x, continue_button.coord.y) && AEInputCheckTriggered(AEVK_LBUTTON))
	{
		next_state = prev_state;
	}

	if (checkHovering(mouse_x, mouse_y, back_button.length, back_button.height, back_button.coord.x, back_button.coord.y) && AEInputCheckTriggered(AEVK_LBUTTON))
	{
		next_state = MENU;
	}


	//update matrix
	MatrixCalc(continue_button.transform, continue_button.length, continue_button.height, 0.0f, continue_button.coord);
	MatrixCalc(back_button.transform, back_button.length, back_button.height, 0.0f, back_button.coord);
}

void pause_draw()
{
	/*------------------------------------------------------------
	"GAME PAUSED" TEXT
	------------------------------------------------------------*/
	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));

	sprintf_s(strBuffer, "GAME PAUSED");
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(text, strBuffer, -0.25f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f);

	/*------------------------------------------------------------
	draw buttons
	------------------------------------------------------------*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	draw_pause_buttons(continue_button);
	draw_pause_buttons(back_button);
}

void pause_free()
{
	AEGfxMeshFree(continue_button.mesh);
	AEGfxMeshFree(back_button.mesh);
}

void pause_unload()
{
	AEGfxTextureUnload(continue_button.texture);
	AEGfxTextureUnload(back_button.texture);
}

