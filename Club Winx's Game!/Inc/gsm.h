/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* Primary Author: 
*	Mariah Tahirah (mariahtahirah.b@digipen.edu)
* 
* Secondary Authors:
*	Kristy Lee Yu Xuan (kristyyuxuan.lee@digipen.edu)
==================================================================================*/


typedef void(*FP)(void);

/*------------------------------------------------------------
EXTERNS
------------------------------------------------------------*/
extern int curr_state, prev_state, next_state;
extern FP fpLoad, fpInit, fpUpdate, fpDraw, fpFree, fpUnload;


/*------------------------------------------------------------
DECLARE FUNCTIONS
------------------------------------------------------------*/
void GSM_init(int startingState);
void GSM_update();
