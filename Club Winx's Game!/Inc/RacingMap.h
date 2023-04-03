/*==================================================================================
* All content - 2023 DigiPen Institute of Technology Singapore, all rights reserved.
*
* Course: CSD1451
* Group Name: Club Winx
* 
* Brief: This header file declares the functions for the racing game mode layout.
* 
* Primary Author: Shayne Gloria (m.shayne@digipen.edu)
==================================================================================*/

// ---------------------------------------------------------------------------
//includes
#include "General.h"

// ---------------------------------------------------------------------------




// ============================ racing mode ============================ //
#define MAX_NUM_PLATFORMS 50
#define MAX_NUM_ITEMS 5

struct RacingBoosts {
	AEVec2				coord{ 0.0f, 0.0f };
	AEGfxVertexList* mesh{ nullptr };
	AEGfxTexture* texture{ nullptr };

	f32					size{ 20.0f };
	AABB				boundingBox;
	AEVec2				velocity{ 0.0f, 0.0f };

	bool				collected;

	AEMtx33				transform{};
};
extern RacingBoosts racing_boostsA[MAX_NUM_ITEMS], racing_boostsB[MAX_NUM_ITEMS];



struct Platform {
	AEVec2				coord{ 0.0f, 0.0f };

	AEGfxVertexList* mesh{ nullptr };
	AEGfxTexture* texture{ nullptr };
	AEVec2				velocity{ 0.0f, 0.0f };

	AABB				boundingBox;

	f32					length{ 145.0f };
	f32					height{ 15.0f };

	bool				stepped{ false };
	AEMtx33				transform{};

	f32					acceleration{ 0.0f };
};
extern Platform platformA[MAX_NUM_PLATFORMS], platformB[MAX_NUM_PLATFORMS];



struct Line {
	AEVec2				coord{ 0.0f, 0.0f };

	AEGfxVertexList* mesh{ nullptr };
	AEGfxTexture* texture{ nullptr };

	f32					length{ 10.0f };
	f32					height{ 0.0f };

	u32					colour{ 0xFF000000 };

	AEMtx33				transform{};
};
extern Line	splitscreen, startingline;






// Split Screen
void splitscreen_load();
void splitscreen_init();
void splitscreen_draw();
void splitscreen_free();


// Platforms
void racing_map_load();
void racing_map_init(f32 start, f32 end, int player);
void racing_map_draw();
void racing_map_free();
void racing_map_unload();
