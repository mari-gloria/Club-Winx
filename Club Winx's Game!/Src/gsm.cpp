/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Mariah Tahirah (mariahtahirah.b@digipen.edu)
* Secondary Authors:
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu)
*
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "AEGameStateMgr.h"

#include "gsm.h"
#include "gamestatelist.h"
#include "win_lose.h"

#include "racing.h"
#include "boss.h"

#include <iostream>
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
	case RACING:
		fpLoad =	racing_load;
		fpInit =	racing_init;
		fpUpdate =	racing_update;
		fpDraw =	racing_draw;
		fpFree =	racing_free;
		fpUnload =	racing_unload;
		break;
	case BOSS:
		fpLoad =	boss_load;
		fpInit =	boss_init;
		fpUpdate =	boss_update;
		fpDraw =	boss_draw;
		fpFree =	boss_free;
		fpUnload =	boss_unload;
		break;
	//case END_OF_ROUND:
	//	fpLoad = EOR_load;
	//	fpInit = EOR_init;
	//	fpUpdate = EOR_update;
	//	fpDraw = EOR_draw;
	//	fpFree = EOR_free;
	//	fpUnload = EOR_unload;
	//	break;
	case RESTART:
		break;
	case QUIT:
		break;
	default:
		AE_FATAL_ERROR("Invalid state!");
	}
}