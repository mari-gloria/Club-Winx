/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Yeo Hui Shan (huishan.y@digipen.edu)
* Secondary Authors:
*
*
==================================================================================*/

#ifndef BUTTONS_H
#define BUTTONS_H

#include "AEEngine.h"
#include "AEGraphics.h"

// ---------------------------------------------------------------------------
// struct

struct buttons
{
	f32 length{ 200.0f };
	f32 height{ 50.0f };

	AEVec2 coord;

	AEGfxVertexList* mesh{ nullptr };
	AEGfxTexture* texture{ nullptr };

	AEMtx33	transform{};
};

// ---------------------------------------------------------------------------

bool checkHovering(int mouse_x, int mouse_y, float button_w, float button_h, float buttonpos_x, float buttonpos_y);
void drawButtons(float pos_x, float pos_y, AEGfxTexture* texture, AEGfxVertexList* mesh);
void draw_button_withTrans(buttons button);

#endif 