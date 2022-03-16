#include "AplicationCoordinator.h"

#include "ActivityRepositoryInFile.h"
#include "ActivityValidator.h"
#include "ActivityManagerServiceWithWishlistAndUndo.h"
#include "Console.h"


void AplicationCoordinator::setUpAndRunApplication()
{
	ActivityRepositoryInFile activityRepository("test_file_1.txt");
	
	ActivityValidator activityValidator;


	ActivityWishlist wishlist(activityRepository);
	ActivityManagerServiceWithWishlistAndUndo managerService(activityRepository, activityValidator, wishlist);
	ConsoleUserInterfaceWithWishlistOptionsAndUndo consoleInterface{ managerService };

	consoleInterface.run();
}
