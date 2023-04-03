/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This source file contains the main function of the program.
* 
* Primary Author:
*	Mariah Tahirah (mariahtahirah.b@digipen.edu) 20%
* 
* Secondary Authors: 
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu) 80% 
==================================================================================*/

// ---------------------------------------------------------------------------
// includes
#include "general.h"

// ---------------------------------------------------------------------------





// ============================ variables ============================ //

//setting screen width and height
int const winLENGTH{1080}, winHEIGHT{600};

//fonts
extern s8 font_arial = 0;
extern s8 font_pixel = 1;

//game pause
bool game_paused = false;





// ============================ main function ============================ //
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	/*------------------------------------------------------------
	INITIALIZATION
	------------------------------------------------------------*/

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, winLENGTH, winHEIGHT, 0, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("Up For Orbital (UFO)");


	// reset the system modules
	AESysReset();

	//fonts
	font_pixel = AEGfxCreateFont("Assets/Fonts/Pixel.ttf", 30);
	font_arial = AEGfxCreateFont("Assets/Fonts/Arial Italic.ttf", 100);

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
	AEGfxDestroyFont(font_arial);
	AEGfxDestroyFont(font_pixel);

	/*------------------------------------------------------------
	FREE AUDIO
	------------------------------------------------------------*/
	AEAudioExit();

	AESysExit();
}
