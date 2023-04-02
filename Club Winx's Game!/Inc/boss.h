/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This header file declares the functions requried for the boss game mode.
* 
* Primary Author: Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu)
* Secondary Author: Amirah Isa (amirah.b@digipen.edu) -> potion_position, mobs_position
==================================================================================*/


void boss_load();
void boss_init();
void boss_update();
void boss_draw();
void boss_free();
void boss_unload();
void potion_position(float& x, float& y, bool& potion_produce, bool& check, bool& potion_stop, int& timer);
void mobs_position(float& x, float& y, bool& mobs_spawn, bool& mobscheck, bool& mobs_stop, int& timer);