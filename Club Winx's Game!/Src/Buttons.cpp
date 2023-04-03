/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This source file defines the functions for buttons.
* 
* Primary Author: Yeo Hui Shan (huishan.y@digipen.edu) 100%
==================================================================================*/

#include "General.h"

// ----------------------------------------------------------------------------
// Check whether mouse is hovering above the button
// ----------------------------------------------------------------------------
bool checkHovering(int mouse_x, int mouse_y, float button_w, float button_h, float buttonpos_x, float buttonpos_y)
{
	int toGlobal_x = static_cast<int>(buttonpos_x + 440.0f);
	int toGlobal_y = static_cast<int>(-(buttonpos_y - 324.0f));

	if (mouse_x <= (toGlobal_x + button_w) && // Max X position
		mouse_x >= (toGlobal_x) && // Min X position
		mouse_y >= (toGlobal_y - button_h) && // Min Y position
		mouse_y <= (toGlobal_y))   // Max Y position
		return true;
	else
		return false;
}

// ----------------------------------------------------------------------------
// Draws the buttons
// ----------------------------------------------------------------------------
void drawButtons(float xPos, float yPos, AEGfxTexture* texture, AEGfxVertexList* mesh)
{
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(xPos, yPos);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);
	AEGfxTextureSet(texture, 0.0f, 0.0f);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}
