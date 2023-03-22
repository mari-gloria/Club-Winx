
// Include these Header files
#include "General.h"

int Racing_WINNER{ 0 }, Racing_LOSER{ 0 };

// ================================================================================================================ //
// ======================================== FUNCTIONS RELATED FOR RACING ========================================== //
// ================================================================================================================ //

void Racing_Win(bool win, int player)
{
	//auto start_time = std::chrono::high_resolution_clock::now();
	//f32 frame_before = 0.0f, time_before = 0.0f;
	if (win == true) {
		
		switch (player)
		{
			case 1:
				Racing_WINNER = PLAYERONE;
				Racing_LOSER = PLAYERTWO;

				// check if won
				std::cout << "Won Player 1" << std::endl;
				// bool to yes show pic near player head
				winRacing.bgCoord = { player1.pCoord.x + 60.0f, player1.pCoord.y + 60.0f };
				// if press enter, goes to end of round
				if (AEInputCheckTriggered(AEVK_LBUTTON)) {
					next_state = WIN_PLAYERONE;
				}
				break;

			case 2:
				Racing_WINNER = PLAYERTWO;
				Racing_LOSER = PLAYERONE;

				// check if won
				std::cout << "Won Player 2" << std::endl; 
				winRacing.bgCoord = { player2.pCoord.x + 60.0f, player2.pCoord.y + 60.0f };
				if (AEInputCheckTriggered(AEVK_LBUTTON)) {
					next_state = WIN_PLAYERTWO;
				}
				break;
		}
	}

	return;
}

