
// Include these Header files
#include "General.h"

int Racing_WINNER{ 0 }, Racing_LOSER{ 0 };

// ================================================================================================================ //
// ======================================== FUNCTIONS RELATED FOR RACING ========================================== //
// ================================================================================================================ //

// Purpose: Checks who won the game
// Returns: if there's a winner
//bool Racing_win()
//{
//	// variable
//	bool have_winner = 0;
//	bool test = 0;
//	
//	// initalise winner to be no one
//	Racing_WINNER = NONE;
//
//	if (test == 0)
//	{
//		Racing_WINNER = PLAYERONE;
//		have_winner = 1;
//	}
//	else if (test != 0)
//	{
//		Racing_WINNER = PLAYERTWO;
//		have_winner = 1;
//	}
//	
//	return have_winner; 
//}
//
//// Purpose: Checks who lost the game
//// Returns: if there's a loser
//bool Racing_lose()
//{
//	// Variables
//	bool have_loser = 0;
//	bool test = 0;
//	
//	// initalise winner to be no one
//	Racing_LOSER = NONE;
//
//	if ( test == 0)
//	{
//		Racing_LOSER = PLAYERONE;
//		have_loser = 1;
//	}
//	else if ( test != 0)
//	{
//		Racing_LOSER = PLAYERTWO;
//		have_loser = 1;
//	}
//
//	// If condition, if one player already lost and then the other one lost as well, return RESTART
//	if ((Racing_LOSER == PLAYERONE || Racing_LOSER == PLAYERTWO) && have_loser == 1) 
//	{
//		// If lose condition also applies for the other player, 
//		if ( test == 1)
//		{
//			Racing_LOSER = RE_ROUND;
//
//		}
//	}
//	
//	return have_loser;
//}
//
//// Purpose: Using the WIN value & LOSE value, display frames
//void Racing_win_lose_condition()
//{
//	bool have_winner = Racing_win, have_loser = Racing_lose;
//
//	next_state = END_OF_ROUND;
//	
//	if (have_winner == true)
//	{
//		if (Racing_WINNER == PLAYERONE)
//		{
//			// display winner frame with playerone as the winner
//		}
//		else if (Racing_WINNER == PLAYERTWO)
//		{
//			//display winner frame with playertwo as the winner
//		}
//	}
//	else if ((have_loser == true) && Racing_LOSER == RESTART)
//	{
//		// display loser frame 
//	}
//
//	return;
//}


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

