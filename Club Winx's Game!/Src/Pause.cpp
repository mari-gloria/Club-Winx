/*= ================================================================================ =
*All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
*Course: CSD1451
* Group Name : Club Winx
* 
* Brief: This header file defines the functions required for the pause state.
* 
* Primary Author : 
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu)
* 
* Secondary Authors :
*	Mariah Tahirah (mariahtahirah.b@digipen.edu) -> debug
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "General.h"

// ---------------------------------------------------------------------------





// ============================ variables ============================ //
Textures continue_button, back_button;

int mouse_x, mouse_y;





void pause_load()
{
	//set background - black
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	//button mesh
	SquareMesh(&continue_button.mesh, 0);
	SquareMesh(&back_button.mesh, 0);

	//load button textures
	continue_button.texture = AEGfxTextureLoad("Assets/Buttons/CONTINUE_BUTTON.png");
	back_button.texture = AEGfxTextureLoad("Assets/Buttons/BACK_BUTTON.png");
}



void pause_init()
{
	continue_button.height = 50.0f;
	continue_button.length = 200.0f;
	continue_button.coord.x = -125.0f;
	continue_button.coord.y = 0.0f;

	back_button.height = 50.0f;
	back_button.length = 200.0f;
	back_button.coord.x = 125.0f;
	back_button.coord.y = 0.0f;
}



void pause_update()
{
	AEInputShowCursor(true);

	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{
		//next_state = MENU;
		game_paused = false;
	}

	//update mouse coord
	AEInputGetCursorPosition(&mouse_x, &mouse_y);
	AEGfxSetCamPosition(0, 0);
	
	if (checkHovering(mouse_x, mouse_y, continue_button.length, continue_button.height, continue_button.coord.x, continue_button.coord.y) && AEInputCheckTriggered(AEVK_LBUTTON))
	{
		game_paused = false;
	}

	if (checkHovering(mouse_x, mouse_y, back_button.length, back_button.height, back_button.coord.x, back_button.coord.y) && AEInputCheckTriggered(AEVK_LBUTTON))
	{
		game_paused = false;
		next_state = MENU;
	}
	                  

	//update matrix
	MatrixCalc(continue_button.transform, continue_button.length, continue_button.height, 0.0f, continue_button.coord);
	MatrixCalc(back_button.transform, back_button.length, back_button.height, 0.0f, back_button.coord);
}



void pause_draw()
{
	draw_texture(continue_button);
	draw_texture(back_button);

	/*------------------------------------------------------------
	"GAME PAUSED" TEXT
	------------------------------------------------------------*/
	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));

	sprintf_s(strBuffer, "GAME PAUSED");
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(font_pixel, strBuffer, -0.25f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f);
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

