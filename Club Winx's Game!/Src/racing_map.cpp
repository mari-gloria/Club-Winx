// ---------------------------------------------------------------------------
// includes
#include "AEEngine.h"
#include "AEGameStateMgr.h"

#include "gsm.h"
#include "gamestatelist.h"
#include "racing.h"
#include "general.h"
#include "racing_map.h"

#include <iostream>
// ---------------------------------------------------------------------------



/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/

// loads the platform meshes in the map in racing_load function
void racing_map_load()
{
	// 17 platforms in a map
	
	// platform for player 1
	SquareMesh(&platform1.PMesh, platform1.length, platform1.height, platform1.colour);

	// platform for player 2
	SquareMesh(&platform2.PMesh, platform2.length, platform2.height, platform2.colour);

	return;
}

// function that will draw the map within a certain x values, the purpose of this is to duplicate the map for player 1 & 2
void racing_map_init()
{
	// Variables
	/*float min_limit = start + 50.0f;
	float max_limit = end - 50.0f;*/

	// INIT PLATFORMS :: actually may need to create a function to initialise platforms, in order for randomisation | but for now use this to try it out 
	// set spawning point to be from above ground. 
	platform1.Pspawn = player1.pGround + 60.0f;
	platform2.Pspawn = player2.pGround + 60.0f;


	platform1.PCoord = { (AEGfxGetWinMinX() / 2) - 20.0f , platform1.Pspawn };
	platform2.PCoord = { (AEGfxGetWinMaxX() / 2) - 20.0f , platform2.Pspawn };

	return;
}

void racing_map_draw()
{
	// Variables
	/*float min_limit = start + 50.0f;
	float max_limit = end - 50.0f;*/

	/*------------------------------------------------------------
	DRAWING PLATFORMS
	------------------------------------------------------------*/
	// Drawing platform 1
	AEGfxSetPosition(platform1.PCoord.x, platform1.PCoord.y);
	// No texture
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(platform1.PMesh, AE_GFX_MDM_TRIANGLES);

	// Drawing platform 2
	AEGfxSetPosition(platform2.PCoord.x, platform2.PCoord.y);
	// No texture
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(platform2.PMesh, AE_GFX_MDM_TRIANGLES);

	return;
}

void racing_map_unload()
{
	AEGfxMeshFree(platform1.PMesh);
	AEGfxMeshFree(platform2.PMesh);
	
	return;
}