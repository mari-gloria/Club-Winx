/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This source file defines the functions to display racing winning texture (before win/lose screen)
* 
* Primary Author: Shayne Gloria (m.shayne@digipen.edu) 100%
==================================================================================*/

// ---------------------------------------------------------------------------
// includes

#include "General.h"

// ---------------------------------------------------------------------------






int Racing_WINNER{ 0 }, Racing_LOSER{ 0 };


void Racing_Win(int player)
{
	//auto start_time = std::chrono::high_resolution_clock::now();
	//f32 frame_before = 0.0f, time_before = 0.0f;

	switch (player)
	{
		case 1:
			Racing_WINNER = PLAYERONE;
			Racing_LOSER = PLAYERTWO;

			// check if won
			// bool to yes show pic near player head
			winRacing.coord = { player1.coord.x + 60.0f, player1.coord.y + 60.0f };
			// if press enter, goes to end of round
			if (AEInputCheckTriggered(AEVK_LBUTTON)) {
					next_state = WIN_BOTHPLAYERS;
			}
			break;

		case 2:
			Racing_WINNER = PLAYERTWO;
			Racing_LOSER = PLAYERONE;

			// check if won
			winRacing.coord = { player2.coord.x + 60.0f, player2.coord.y + 60.0f };
			if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        next_state = WIN_BOTHPLAYERS;
			}
			break;
		case 0:
			winRacing.coord = { -600, 600 };
			break;
	}

	return;
}