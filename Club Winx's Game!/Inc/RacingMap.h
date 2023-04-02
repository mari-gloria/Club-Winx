/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This header file declares the functions for the racing game mode layout.
* 
* Primary Author: Shayne Gloria (m.shayne@digipen.edu)
==================================================================================*/

// Split Screen
void splitscreen_load();
void splitscreen_init();
void splitscreen_draw();
void splitscreen_free();


// Platforms
void racing_map_load();
void racing_map_init(f32 start, f32 end, int player);
void racing_map_draw();
void racing_map_free();
void racing_map_unload();
