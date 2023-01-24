#pragma once

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
