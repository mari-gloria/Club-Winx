// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "AEGameStateMgr.h"

#include "gsm.h"
#include "gamestatelist.h"
#include "boss.h"

#include <iostream>
// ---------------------------------------------------------------------------




/*------------------------------------------------------------
GLOBALS
------------------------------------------------------------*/
AEGfxVertexList* pMesh2 = 0;



/*------------------------------------------------------------
FUNCTIONS
------------------------------------------------------------*/
void boss_load()
{
	std::cout << "boss:Load\n";

	AEGfxSetBackgroundColor(0.0f, 0.0f, 255.0f);

	/*------------------------------------------------------------
	CREATING OBJECTS AND SHAPES
	------------------------------------------------------------*/

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(-25.5f,	-25.5f,	0xFFFF0000, 0.0f, 0.0f,
				25.5f,	0.0f,	0xFFFF0000, 0.0f, 0.0f,
				-25.5f,	25.5f,	0xFFFF0000, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	pMesh2 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh2, "Failed to create mesh 1!!");


	/*------------------------------------------------------------
	LOADING TEXTIRES (IMAGES)
	------------------------------------------------------------*/


	/*------------------------------------------------------------
	CREATING FONTS
	------------------------------------------------------------*/


}

void boss_init()
{
	std::cout << "boss:Initialize\n";
}

void boss_update()
{
	std::cout << "boss:Update\n";

	if (AEInputCheckTriggered(AEVK_2)) {
		next_state = QUIT;
	}

}

void boss_draw()
{
	std::cout << "boss:Draw\n";

	// Drawing object 1
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	// Set position for object 1
	AEGfxSetPosition(0.0f, 0.0f);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);
}

void boss_free()
{
	std::cout << "boss:Free\n";

}

void boss_unload()
{
	std::cout << "boss:Unload\n";
	AEGfxMeshFree(pMesh2);
}