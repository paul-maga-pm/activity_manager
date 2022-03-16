#include "AplicationCoordinator.h"
#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC


int main()
{
	AplicationCoordinator appCoordinator;
	appCoordinator.setUpAndRunApplication();
	_CrtDumpMemoryLeaks();
	return 0;
}
