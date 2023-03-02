
// variables

enum end_condtion { NONE = 0, PLAYERONE, PLAYERTWO, RE_ROUND};

// Functions for Racing //

//bool Racing_win();
//bool Racing_lose();
//void Racing_win_lose_condition();
void Racing_Win(bool win, int player);


// Functions for state change //
void EOR_load();
void EOR_init();
void EOR_update();
void EOR_draw();
void EOR_free();
void EOR_unload();