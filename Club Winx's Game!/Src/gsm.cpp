#include "gsm.h"
#include "racing.h"

#include <iostream>




int current = 0, previous = 0, next = 0;

FP fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;

void GSM_Initialize(int startingState)
{
	current = previous = next = startingState;  //initialize all state to be the same 


	std::cout << "GSM:Initialize\n";
}

void GSM_Update()
{
	std::cout << "GSM:Update\n";


	/*switch (current)
	{
	case racing:
		fpLoad = racing_load;
		fpInitialize = racing_init;
		fpUpdate = racing_update;
		fpDraw = racing_draw;
		fpFree = racing_free;
		fpUnload = racing_unload;
		break;
	case :
		
		break;
	case restart:

		break;
	case quit:
		break;
	default:
		break;
	}*/

}