/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This header file declares the functions required for the buttons in our program.
* 
* Primary Author: Yeo Hui Shan (huishan.y@digipen.edu)
==================================================================================*/

#ifndef BUTTONS_H
#define BUTTONS_H

#include "AEEngine.h"
#include "AEGraphics.h"

bool checkHovering(int mouse_x, int mouse_y, float button_w, float button_h, float buttonpos_x, float buttonpos_y);
void drawButtons(float pos_x, float pos_y, AEGfxTexture* texture, AEGfxVertexList* mesh);
#endif 