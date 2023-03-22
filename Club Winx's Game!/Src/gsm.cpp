/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Mariah Tahirah (mariahtahirah.b@digipen.edu)
* Secondary Authors:
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu)
*	Yeo Hui Shan (huishan.y@digipen.edu)
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "General.h"
// ---------------------------------------------------------------------------

/*------------------------------------------------------------
DECLARE GLOBAL VARIABLES
------------------------------------------------------------*/
int curr_state = 0, prev_state = 0, next_state = 0;
FP fpLoad = nullptr, fpInit = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;


/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/

/*------------------------------------------------------------
Function: GSM_update

initializes curr_state, prev_state and next_state to the
initial game state
------------------------------------------------------------*/
void GSM_init(int startingState)
{
	curr_state = prev_state = next_state = startingState;  //initialize all state to be the same 
	std::cout << "GSM:Initialize\n";
}


/*------------------------------------------------------------
Function: GSM_update

updates function pointer according to the current game state
------------------------------------------------------------*/
void GSM_update()
{
	std::cout << "GSM:Update\n";

	switch (curr_state)
	{
	case SPLASH:
		fpLoad = Splash_Load;
		fpInit = Splash_Initialize;
		fpUpdate = Splash_Update;
		fpDraw = Splash_Draw;
		fpFree = Splash_Free;
		fpUnload = Splash_Unload;
		break;

	case MENU:
		fpLoad = MainMenu_Load;
		fpInit = MainMenu_Init;
		fpUpdate = MainMenu_Update;
		fpDraw = MainMenu_Draw;
		fpFree = MainMenu_Free;
		fpUnload = MainMenu_Unload;
		break;

	case RACING:
		fpLoad = racing_load;
		fpInit = racing_init;
		fpUpdate = racing_update;
		fpDraw = racing_draw;
		fpFree = racing_free;
		fpUnload = racing_unload;
		break;

	case BOSS:
		fpLoad = boss_load;
		fpInit = boss_init;
		fpUpdate = boss_update;
		fpDraw = boss_draw;
		fpFree = boss_free;
		fpUnload = boss_unload;
		break;

	case PUZZLE:
		fpLoad = puzzle_load;
		fpInit = puzzle_init;
		fpUpdate = puzzle_update;
		fpDraw = puzzle_draw;
		fpFree = puzzle_free;
		fpUnload = puzzle_unload;
		break;

	case WIN_PLAYERONE:
		fpLoad = WIN_PLAYERONE_load;
		fpInit = WIN_PLAYERONE_init;
		fpUpdate = WIN_PLAYERONE_update;
		fpDraw = WIN_PLAYERONE_draw;
		fpFree = WIN_PLAYERONE_free;
		fpUnload = WIN_PLAYERONE_unload;
		break;

	case WIN_PLAYERTWO:
		fpLoad = WIN_PLAYERTWO_load;
		fpInit = WIN_PLAYERTWO_init;
		fpUpdate = WIN_PLAYERTWO_update;
		fpDraw = WIN_PLAYERTWO_draw;
		fpFree = WIN_PLAYERTWO_free;
		fpUnload = WIN_PLAYERTWO_unload;
		break;

	case WIN_BOTHPLAYERS:
		fpLoad = WIN_BOTHPLAYERS_load;
		fpInit = WIN_BOTHPLAYERS_init;
		fpUpdate = WIN_BOTHPLAYERS_update;
		fpDraw = WIN_BOTHPLAYERS_draw;
		fpFree = WIN_BOTHPLAYERS_free;
		fpUnload = WIN_BOTHPLAYERS_unload;
		break;

	case LOSE_BOTHPLAYERS:
		fpLoad = LOSE_BOTHPLAYERS_load;
		fpInit = LOSE_BOTHPLAYERS_init;
		fpUpdate = LOSE_BOTHPLAYERS_update;
		fpDraw = LOSE_BOTHPLAYERS_draw;
		fpFree = LOSE_BOTHPLAYERS_free;
		fpUnload = LOSE_BOTHPLAYERS_unload;
		break;

	case ENDGAME:
		fpLoad = ENDGAME_load;
		fpInit = ENDGAME_init;
		fpUpdate = ENDGAME_update;
		fpDraw = ENDGAME_draw;
		fpFree = ENDGAME_free;
		fpUnload = ENDGAME_unload;
		break;

	case RESTART:
		break;

	case QUIT:
		break;

	default:
		AE_FATAL_ERROR("Invalid state!");
	}
}