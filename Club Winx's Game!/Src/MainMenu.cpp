/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: Yeo Hui Shan (huishan.y@digipen.edu)
* Secondary Authors:
*
*
==================================================================================*/

#include "General.h"

// Window Size
#define WINDOW_W 1080
#define WINDOW_H 600

// Sizes of Buttons on Main Menu
#define MAIN_MENU_BUTTON_W 200 //300
#define MAIN_MENU_BUTTON_H 50 //75

// Sizes of Buttons on Mode Selection Page & Level Selection Page
#define MODE_SELECTION_BUTTON_W 200 //300
#define MODE_SELECTION_BUTTON_H 70 //100

// Offset for Main Menu Buttons
#define BUTTON_OFFSET 0.333333f

// ----- MESH DECLARATIONS ----- //
// Background
AEGfxVertexList* Main_Menu_Mesh;
// Buttons on Main Menu
AEGfxVertexList* MM_Button_Mesh;
// Buttons on Mode Selection Page & Level Selection Page
AEGfxVertexList* MS_Button_Mesh;

// ----- TEXTURE DECLARATIONS ----- //
// Textures on Main Menu
AEGfxTexture* Main_Menu_BG;
AEGfxTexture* Plain_BG; //BG used for tutorial, options & credits page
AEGfxTexture* Start_Button;
AEGfxTexture* Tutorial_Button;
AEGfxTexture* Options_Button;
AEGfxTexture* Credits_Button;
AEGfxTexture* Quit_Button;

// Textures on Mode Selection Page
AEGfxTexture* Story_Mode_Button;
AEGfxTexture* Arcade_Mode_Button;
AEGfxTexture* Back_Button;

// Textures on Level Selection Page
AEGfxTexture* Racing_Button;
AEGfxTexture* Puzzle_Button;
AEGfxTexture* Boss_Button;
AEGfxTexture* Back_Mode_Button;

// ----- MOUSE INPUT DECLARATIONS ----- //
int mouseInput_x = 0;
int mouseInput_y = 0;

// ----- POSITIONS DECLARATIONS ----- //
// Buttons on Main Menu
float start_pos_x, start_pos_y;
float tutorial_pos_x, tutorial_pos_y;
float options_pos_x, options_pos_y;
float credits_pos_x, credits_pos_y;
float quit_pos_x, quit_pos_y;

// Buttons on Mode Selection Page
float story_pos_x, story_pos_y;
float arcade_pos_x, arcade_pos_y;
float back_pos_x, back_pos_y;

// Buttons on Level Selection Page
float racing_pos_x, racing_pos_y;
float puzzle_pos_x, puzzle_pos_y;
float boss_pos_x, boss_pos_y;
float backmode_pos_x, backmode_pos_y;

// ----- BOOLEANS DECLARATIONS ----- //
// Hovering Buttons on Main Menu
bool hover_start;
bool hover_tutorial;
bool hover_options;
bool hover_credits;
bool hover_quit;

//Hovering Buttons on Mode Selection Page
bool hover_story;
bool hover_arcade;
bool hover_back;

//Hovering Buttons on Level Selection Page
bool hover_racing;
bool hover_puzzle;
bool hover_boss;
bool hover_backmode;

// Clicked Buttons on Main Menu
bool start_click;
bool tutorial_click;
bool options_click;
bool credits_click;
bool quit_click;

// Clicked Buttons on Mode Selection Page
bool story_click = false;
bool arcade_click = false;
bool back_click = false;

// Clicked Buttons on Level Selection Page
bool racing_click = false;
bool puzzle_click = false;
bool boss_click = false;
bool backmode_click = false;

// ----- ENUM CONTROLLER ----- //
int toggle;

/*------------------------------------------------------------
LOADS ASSETS NEEDED FOR MAIN MENU
------------------------------------------------------------*/
void MainMenu_Load()
{
	// Textures for Main Menu
	Main_Menu_BG = AEGfxTextureLoad("Assets/MAIN_MENU.png");
	AE_ASSERT_MESG(Main_Menu_BG, "Failed to create Main_Menu_BG!!");

	Plain_BG = AEGfxTextureLoad("Assets/MAIN_MENU_PLAIN_BG.png");
	AE_ASSERT_MESG(Plain_BG, "Failed to create Plain_BG!!");

	Start_Button = AEGfxTextureLoad("Assets/START_BUTTON.png");
	AE_ASSERT_MESG(Start_Button, "Failed to create Start_Button!!");

	Tutorial_Button = AEGfxTextureLoad("Assets/TUTORIAL_BUTTON.png");
	AE_ASSERT_MESG(Tutorial_Button, "Failed to create Tutorial_Button!!");

	Options_Button = AEGfxTextureLoad("Assets/OPTIONS_BUTTON.png");
	AE_ASSERT_MESG(Options_Button, "Failed to create Options_Button!!");

	Credits_Button = AEGfxTextureLoad("Assets/CREDITS_BUTTON.png");
	AE_ASSERT_MESG(Credits_Button, "Failed to create Credits_Button!!");

	Quit_Button = AEGfxTextureLoad("Assets/QUIT_BUTTON.png");
	AE_ASSERT_MESG(Quit_Button, "Failed to create Quit_Button!!");

	// Textures for Mode Selection Page
	Story_Mode_Button = AEGfxTextureLoad("Assets/STORY_BUTTON.png");
	AE_ASSERT_MESG(Story_Mode_Button, "Failed to create Story_Mode_Button!!");

	Arcade_Mode_Button = AEGfxTextureLoad("Assets/ARCADE_BUTTON.png");
	AE_ASSERT_MESG(Arcade_Mode_Button, "Failed to create Arcade_Mode_Button!!");

	Back_Button = AEGfxTextureLoad("Assets/BACK_BUTTON.png");
	AE_ASSERT_MESG(Back_Button, "Failed to create Back_Button!!");

	// Textures for Level Selection Page
	Racing_Button = AEGfxTextureLoad("Assets/RACING_BUTTON.png");
	AE_ASSERT_MESG(Racing_Button, "Failed to create Racing_Button!!");

	Puzzle_Button = AEGfxTextureLoad("Assets/PUZZLE_BUTTON.png");
	AE_ASSERT_MESG(Puzzle_Button, "Failed to create Puzzle_Button!!");

	Boss_Button = AEGfxTextureLoad("Assets/BOSS_BUTTON.png");
	AE_ASSERT_MESG(Boss_Button, "Failed to create Boss_Button!!");

	Back_Mode_Button = AEGfxTextureLoad("Assets/BACK_MODE_BUTTON.png");
	AE_ASSERT_MESG(Back_Mode_Button, "Failed to create Back_Mode_Button!!");
}

/*------------------------------------------------------------
INITIALISE BUTTONS AND VARIABLES FOR MAIN MENU
------------------------------------------------------------*/
void MainMenu_Init()
{
	toggle = MAINMENU; //start at main menu page

	// Buttons on Main Menu
	start_pos_x = -160.0f, start_pos_y = -60.0f;
	tutorial_pos_x = 160.0f, tutorial_pos_y = -60.0f;
	options_pos_x = -160.0f, options_pos_y = -145.0f;
	credits_pos_x = 160.0f, credits_pos_y = -145.0f;
	quit_pos_x = 0.0f, quit_pos_y = -230.0f;

	// Buttons on Mode Selection Page
	story_pos_x = -165.0f, story_pos_y = -75.0f;
	arcade_pos_x = 165.0f, arcade_pos_y = -75.0f;
	back_pos_x = 0.0f, back_pos_y = -200.0f;

	// Buttons on Level Selection Page
	racing_pos_x = -165.0f, racing_pos_y = -75.0f;
	puzzle_pos_x = 165.0f, puzzle_pos_y = -75.0f;
	boss_pos_x = -165.0f, boss_pos_y = -200.0f;
	backmode_pos_x = 165.0f, backmode_pos_y = -200.0f;

	// Booleans
	start_click = false, tutorial_click = false, options_click = false, credits_click = false, quit_click = false;
	hover_start = false, hover_tutorial = false, hover_options = false, hover_credits = false, hover_quit = false;
	hover_story = false, hover_arcade = false, hover_back = false;
	hover_racing = false, hover_puzzle = false, hover_boss = false, hover_backmode = false;

	//Load background mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-WINDOW_W / 2, -WINDOW_H / 2, 0x00FFFFFF, 0.0f, 1.0f,
		WINDOW_W / 2, -WINDOW_H / 2, 0x00FFFFFF, 1.0f, 1.0f,
		-WINDOW_W / 2, WINDOW_H / 2, 0x00FFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		WINDOW_W / 2, -WINDOW_H / 2, 0x00FFFFFF, 1.0f, 1.0f,
		WINDOW_W / 2, WINDOW_H / 2, 0x00FFFFFF, 1.0f, 0.0f,
		-WINDOW_W / 2, WINDOW_H / 2, 0x00FFFFFF, 0.0f, 0.0f);
	Main_Menu_Mesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(Main_Menu_Mesh, "Failed to create Main_Menu_Mesh!!");

	//Load Main Menu Button Mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-MAIN_MENU_BUTTON_W / 2, -MAIN_MENU_BUTTON_H / 2, 0x00FFFFFF, 0.0f, 1.0f,
		MAIN_MENU_BUTTON_W / 2, -MAIN_MENU_BUTTON_H / 2, 0x00FFFFFF, 1.0f, 1.0f,
		-MAIN_MENU_BUTTON_W / 2, MAIN_MENU_BUTTON_H / 2, 0x00FFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		MAIN_MENU_BUTTON_W / 2, -MAIN_MENU_BUTTON_H / 2, 0x00FFFFFF, 1.0f, 1.0f,
		MAIN_MENU_BUTTON_W / 2, MAIN_MENU_BUTTON_H / 2, 0x00FFFFFF, 1.0f, 0.0f,
		-MAIN_MENU_BUTTON_W / 2, MAIN_MENU_BUTTON_H / 2, 0x00FFFFFF, 0.0f, 0.0f);
	MM_Button_Mesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(MM_Button_Mesh, "Failed to create Main_Menu_Mesh!!");

	// Mode Selection & Level Selection Button Mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-MODE_SELECTION_BUTTON_W / 2, -MODE_SELECTION_BUTTON_H / 2, 0x00FFFFFF, 0.0f, 1.0f,
		MODE_SELECTION_BUTTON_W / 2, -MODE_SELECTION_BUTTON_H / 2, 0x00FFFFFF, 1.0f, 1.0f,
		-MODE_SELECTION_BUTTON_W / 2, MODE_SELECTION_BUTTON_H / 2, 0x00FFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		MODE_SELECTION_BUTTON_W / 2, -MODE_SELECTION_BUTTON_H / 2, 0x00FFFFFF, 1.0f, 1.0f,
		MODE_SELECTION_BUTTON_W / 2, MODE_SELECTION_BUTTON_H / 2, 0x00FFFFFF, 1.0f, 0.0f,
		-MODE_SELECTION_BUTTON_W / 2, MODE_SELECTION_BUTTON_H / 2, 0x00FFFFFF, 0.0f, 0.0f);

	MS_Button_Mesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(MS_Button_Mesh, "Failed to create MS_Button_Mesh!!");
}

/*------------------------------------------------------------
INITIALISE BUTTONS AND VARIABLES FOR MAIN MENU
------------------------------------------------------------*/
void MainMenu_Update()
{
	AEInputUpdate();
	AEInputGetCursorPosition(&mouseInput_x, &mouseInput_y);
	std::cout << mouseInput_x << ", " << mouseInput_y << std::endl;

	if (AEInputCheckCurr(AEVK_1)) {
		next_state = RACING;
	}
	if (AEInputCheckCurr(AEVK_2)) {
		next_state = BOSS;
	}
	if (AEInputCheckCurr(AEVK_Q)) {
		next_state = QUIT;
	}
	if (AEInputCheckCurr(AEVK_3)) {
		next_state = PUZZLE;
	}

	// ----- Menu Buttons State Manager ----- //
	if (toggle == MAINMENU)
	{
		//Start button
		if (checkHovering(mouseInput_x, mouseInput_y, MAIN_MENU_BUTTON_W, MAIN_MENU_BUTTON_H, start_pos_x, start_pos_y))
		{
			hover_start = true;
			std::cout << "start hover" << std::endl;

			if (AEInputCheckCurr(AEVK_LBUTTON))
			{
				start_click = true;
				toggle = MODESELECTIONPAGE;
				std::cout << "start click" << std::endl;

			}
		}
		//Tutorial button
		else if (checkHovering(mouseInput_x, mouseInput_y, MAIN_MENU_BUTTON_W, MAIN_MENU_BUTTON_H, tutorial_pos_x, tutorial_pos_y))
		{
			hover_tutorial = true;
			std::cout << "tutorial hover" << std::endl;

			if (AEInputCheckCurr(AEVK_LBUTTON))
			{
				tutorial_click = true;
				toggle = TUTORIAL;
			}
		}
		//Options button
		else if (checkHovering(mouseInput_x, mouseInput_y, MAIN_MENU_BUTTON_W, MAIN_MENU_BUTTON_H, options_pos_x, options_pos_y))
		{
			hover_options = true;
			std::cout << "options hover" << std::endl;

			if (AEInputCheckCurr(AEVK_LBUTTON))
			{
				options_click = true;
				toggle = OPTIONS;
			}
		}
		//Credits button
		else if (checkHovering(mouseInput_x, mouseInput_y, MAIN_MENU_BUTTON_W, MAIN_MENU_BUTTON_H, credits_pos_x, credits_pos_y))
		{
			hover_credits = true;
			std::cout << "credit hover" << std::endl;

			if (AEInputCheckCurr(AEVK_LBUTTON))
			{
				credits_click = true;
				toggle = CREDITS;
			}
		}
		//Quit button
		else if (checkHovering(mouseInput_x, mouseInput_y, MAIN_MENU_BUTTON_W, MAIN_MENU_BUTTON_H, quit_pos_x, quit_pos_y))
		{
			hover_quit = true;
			std::cout << "quit hover" << std::endl;

			if (AEInputCheckCurr(AEVK_LBUTTON))
			{
				quit_click = true;
				toggle = QUITGAME;
			}
		}
		else
		{
			hover_start = false, hover_tutorial = false, hover_options = false, hover_credits = false, hover_quit = false;
			start_click = false, tutorial_click = false, options_click = false, credits_click = false, quit_click = false;
		}
	}
	else if (toggle == MODESELECTIONPAGE)
	{
		//Story button
		if (checkHovering(mouseInput_x, mouseInput_y, MODE_SELECTION_BUTTON_W, MODE_SELECTION_BUTTON_H, story_pos_x, story_pos_y))
		{
			hover_story = true;
		}
		//Arcade button
		else if (checkHovering(mouseInput_x, mouseInput_y, MODE_SELECTION_BUTTON_W, MODE_SELECTION_BUTTON_H, arcade_pos_x, arcade_pos_y))
		{
			hover_arcade = true;
		}
		//Back button
		else if (checkHovering(mouseInput_x, mouseInput_y, MODE_SELECTION_BUTTON_W, MODE_SELECTION_BUTTON_H, back_pos_x, back_pos_y))
		{
			hover_back = true;
		}
		else
		{
			hover_story = false, hover_arcade = false, hover_back = false;
			story_click = false, arcade_click = false, back_click = false;
		}

		if (AEInputCheckCurr(AEVK_LBUTTON))
		{
			if (hover_story == true)
			{
				story_click = true;
				next_state = RACING; // TO CHANGE INTO PUZZLE 
			}
			else if (hover_arcade == true)
			{
				arcade_click = true;
				toggle = LEVELSELECTIONPAGE;
			}
			else if (hover_back == true)
			{
				back_click = true;
				toggle = MAINMENU;
			}

		}
	}
	else if (toggle == LEVELSELECTIONPAGE)
	{
		//Racing button
		if (checkHovering(mouseInput_x, mouseInput_y, MODE_SELECTION_BUTTON_W, MODE_SELECTION_BUTTON_H, racing_pos_x, racing_pos_y))
		{
			hover_racing = true;

			if (AEInputCheckCurr(AEVK_LBUTTON))
			{
				racing_click = true;
				next_state = RACING;
			}
		}
		//Puzzle button
		else if (checkHovering(mouseInput_x, mouseInput_y, MODE_SELECTION_BUTTON_W, MODE_SELECTION_BUTTON_H, puzzle_pos_x, puzzle_pos_y))
		{
			hover_puzzle = true;

			if (AEInputCheckCurr(AEVK_LBUTTON))
			{
				puzzle_click = true;
				next_state = PUZZLE;
			}
		}
		//Boss button
		else if (checkHovering(mouseInput_x, mouseInput_y, MODE_SELECTION_BUTTON_W, MODE_SELECTION_BUTTON_H, boss_pos_x, boss_pos_y))
		{
			hover_boss = true;

			if (AEInputCheckCurr(AEVK_LBUTTON))
			{
				boss_click = true;
				next_state = BOSS;
			}
		}
		//Back Mode button
		else if (checkHovering(mouseInput_x, mouseInput_y, MODE_SELECTION_BUTTON_W, MODE_SELECTION_BUTTON_H, backmode_pos_x, backmode_pos_y))
		{
			hover_backmode = true;

			if (AEInputCheckCurr(AEVK_LBUTTON))
			{
				backmode_click = true;
				toggle = MODESELECTIONPAGE;
			}
		}
		else
		{
			hover_racing = false, hover_puzzle = false, hover_boss = false, hover_backmode = false;
			racing_click = false, puzzle_click = false, boss_click = false, backmode_click = false;
		}
	}
	else if (toggle == QUITGAME)
	{
		next_state = QUIT;
	}

	if (toggle != MAINMENU)
	{
		if (AEInputCheckCurr(AEVK_ESCAPE)) //press ESC to go back to Main Menu
		{
			toggle = MAINMENU;
		}
	}
}

/*------------------------------------------------------------
DRAW TEXTURES
------------------------------------------------------------*/
void MainMenu_Draw()
{
	// Backgrounds Screen
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0.0f, 0.0f);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);

	// Background texture
	switch (toggle)
	{
	case MAINMENU:
		AEGfxTextureSet(Main_Menu_BG, 0.0f, 0.0f);
		break;
	case MODESELECTIONPAGE:
		AEGfxTextureSet(Main_Menu_BG, 0.0f, 0.0f);
		break;
	case LEVELSELECTIONPAGE: //when click "Arcade Mode"
		AEGfxTextureSet(Main_Menu_BG, 0.0f, 0.0f);
		break;
	case TUTORIAL:
		AEGfxTextureSet(Plain_BG, 0.0f, 0.0f);
		break;
	case OPTIONS:
		AEGfxTextureSet(Plain_BG, 0.0f, 0.0f);
		break;
	case CREDITS:
		AEGfxTextureSet(Plain_BG, 0.0f, 0.0f);
		break;
	case QUITGAME:
		AEGfxTextureSet(Main_Menu_BG, 0.0f, 0.0f);
		break;
	default:
		break;
	}

	AEGfxMeshDraw(Main_Menu_Mesh, AE_GFX_MDM_TRIANGLES);

	if (toggle == MAINMENU)
	{
		// Start Button
		drawButtons(start_pos_x, start_pos_y, Start_Button, MM_Button_Mesh);

		// Tutorial
		drawButtons(tutorial_pos_x, tutorial_pos_y, Tutorial_Button, MM_Button_Mesh);

		// Options
		drawButtons(options_pos_x, options_pos_y, Options_Button, MM_Button_Mesh);

		// Credits
		drawButtons(credits_pos_x, credits_pos_y, Credits_Button, MM_Button_Mesh);

		// Quit
		drawButtons(quit_pos_x, quit_pos_y, Quit_Button, MM_Button_Mesh);
	}

	if (toggle == MODESELECTIONPAGE)
	{
		// Story Mode
		drawButtons(story_pos_x, story_pos_y, Story_Mode_Button, MS_Button_Mesh);

		// Arcade Mode
		drawButtons(arcade_pos_x, arcade_pos_y, Arcade_Mode_Button, MS_Button_Mesh);

		// Back to Main Menu
		drawButtons(back_pos_x, back_pos_y, Back_Button, MS_Button_Mesh);
	}

	if (toggle == LEVELSELECTIONPAGE)
	{
		// Racing
		drawButtons(racing_pos_x, racing_pos_y, Racing_Button, MS_Button_Mesh);

		// Puzzle
		drawButtons(puzzle_pos_x, puzzle_pos_y, Puzzle_Button, MS_Button_Mesh);

		// Boss
		drawButtons(boss_pos_x, boss_pos_y, Boss_Button, MS_Button_Mesh);

		// Back to Mode Selection
		drawButtons(backmode_pos_x, backmode_pos_y, Back_Mode_Button, MS_Button_Mesh);
	}
}

/*------------------------------------------------------------
FREE MESHES USED FOR MAIN MENU
------------------------------------------------------------*/
void MainMenu_Free()
{
	AEGfxMeshFree(Main_Menu_Mesh);
	AEGfxMeshFree(MM_Button_Mesh);
	AEGfxMeshFree(MS_Button_Mesh);
}

/*------------------------------------------------------------
UNLOAD TEXTURES USED FOR MAIN MENU
------------------------------------------------------------*/
void MainMenu_Unload()
{
	AEGfxTextureUnload(Main_Menu_BG);
	AEGfxTextureUnload(Start_Button);
	AEGfxTextureUnload(Tutorial_Button);
	AEGfxTextureUnload(Options_Button);
	AEGfxTextureUnload(Credits_Button);
	AEGfxTextureUnload(Quit_Button);

	AEGfxTextureUnload(Story_Mode_Button);
	AEGfxTextureUnload(Arcade_Mode_Button);
	AEGfxTextureUnload(Back_Button);

	AEGfxTextureUnload(Racing_Button);
	AEGfxTextureUnload(Puzzle_Button);
	AEGfxTextureUnload(Boss_Button);
	AEGfxTextureUnload(Back_Mode_Button);
}