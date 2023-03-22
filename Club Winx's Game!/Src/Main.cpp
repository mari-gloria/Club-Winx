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

/*------------------------------------------------------------
DECLARE GLOBAL VARIABLES
------------------------------------------------------------*/

//setting screen width and height
int const winWIDTH{1080}, winHEIGHT{600};

s8 fontID = 0;

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
	AESysInit(hInstance, nCmdShow, winWIDTH, winHEIGHT, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("Winx Demo!");


	// reset the system modules
	AESysReset();

	fontID = AEGfxCreateFont("Assets/Arial Italic.ttf", 100);




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
		(*fpInit)();

		while (next_state == curr_state) {
			AESysFrameStart();
			AEInputUpdate();
			(*fpUpdate)();
			(*fpDraw)();
			AESysFrameEnd();

			// check if forcing the application to quit
			if (AEInputCheckTriggered(AEVK_Q) || 0 == AESysDoesWindowExist())
				next_state = QUIT;

			//app runtime, delta time
			g_dt = (f32)AEFrameRateControllerGetFrameTime();
			g_appTime += g_dt;
		}

		(*fpFree)();

		if (next_state != RESTART) {
			(*fpUnload)();
		}


		prev_state = curr_state;
		curr_state = next_state;

	}


	/*------------------------------------------------------------
	FREE SYSTEM
	------------------------------------------------------------*/
	AEGfxDestroyFont(fontID);
	AESysExit();
}