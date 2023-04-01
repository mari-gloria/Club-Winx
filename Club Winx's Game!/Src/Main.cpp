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

#include "general.h"

// ---------------------------------------------------------------------------

bool game_paused = false;

/*------------------------------------------------------------
DECLARE GLOBAL VARIABLES
------------------------------------------------------------*/

//setting screen width and height
int const winLENGTH{1080}, winHEIGHT{600};

extern s8 fontID = 0;
extern s8 text = 1;

// main
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/*------------------------------------------------------------
	VARIABLE DECLARATION
	------------------------------------------------------------*/

	//int game_running = 1;


	/*------------------------------------------------------------
	INITIALIZATION
	------------------------------------------------------------*/

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, winLENGTH, winHEIGHT, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("Up For Orbital!");


	// reset the system modules
	AESysReset();


	text = AEGfxCreateFont("Assets/PressStart2P.ttf", 30);
	fontID = AEGfxCreateFont("Assets/Arial Italic.ttf", 100);

	GSM_init(SPLASH);

	/*------------------------------------------------------------
	GAME LOOP
	------------------------------------------------------------*/
	while (curr_state != QUIT)
	{
		AESysReset();


		if (curr_state != RESTART) {
			GSM_update();
			(*fpLoad)();
		}

		else {
			next_state = prev_state;
			curr_state = prev_state;
		}

		// Initalize current game state
		fpInit();                                                                           

		while (next_state == curr_state) {
			AESysFrameStart();
			AEInputUpdate();
			AEAudioUpdate(); //take notes

			fpUpdate();
			fpDraw();


			// check if forcing the application to quit
			//if (AEInputCheckTriggered(AEVK_Q) || 0 == AESysDoesWindowExist())
				//next_state = QUIT;


			//pause game
			if (AEInputCheckReleased(AEVK_ESCAPE) && (curr_state == PUZZLE || curr_state == RACING || curr_state == BOSS)) {
				game_paused = (game_paused == true) ? false : true;
			}

			/*-----------------
			* for testing
			-----------------*/
			if (AEInputCheckCurr(AEVK_1)) {
				next_state = PUZZLE;
			}
			if (AEInputCheckCurr(AEVK_2)) {
				next_state = RACING;
			}
			if (AEInputCheckCurr(AEVK_3)) {
				next_state = BOSS;
			}
			if (AEInputCheckCurr(AEVK_4))
			{
				next_state = WIN_BOTHPLAYERS;
			}
			if (AEInputCheckCurr(AEVK_5))
			{
				next_state = LOSE_BOTHPLAYERS;
			}
			if (AEInputCheckCurr(AEVK_6))
			{
				next_state = ENDGAME;
			}
			if (AEInputCheckCurr(AEVK_Q)) {
				next_state = QUIT;
			}

			AESysFrameEnd();

			//app runtime, delta time
			g_dt = (f32)AEFrameRateControllerGetFrameTime();
			g_appTime += g_dt;
		}


		fpFree();

		if (next_state != RESTART) {
			fpUnload();
		}


		prev_state = curr_state;
		curr_state = next_state;

	}


	/*------------------------------------------------------------
	FREE SYSTEM
	------------------------------------------------------------*/
	AEGfxDestroyFont(fontID);
	AEGfxDestroyFont(text);

	AESysExit();
}
