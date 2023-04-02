/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This header file defines the functions required for the main menu.
* 
* Primary Author: Yeo Hui Shan (huishan.y@digipen.edu)
==================================================================================*/

#ifndef MAINMENU_H
#define MAINMENU_H

enum toggle
{
	MAINMENU = 0,
	MODESELECTIONPAGE,
	LEVELSELECTIONPAGE,
	TUTORIAL,
	OPTIONS,
	CREDITS,
	QUITGAME
};

void MainMenu_Load(void);
void MainMenu_Init(void);
void MainMenu_Update(void);
void MainMenu_Draw(void);
void MainMenu_Free(void);
void MainMenu_Unload(void);

#endif