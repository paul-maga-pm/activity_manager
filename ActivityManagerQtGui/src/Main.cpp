#include "QtWidgets/QApplication"

#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC


#include "ActivityValidator.h"
#include "ActivityRepositoryInFile.h"
#include "ActivityWishlist.h"
#include "ActivityManagerServiceWithWishlistAndUndo.h"
#include "MainWindowGUI.h"
#include "WishlistCrudGui.h"

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    a.setApplicationName("Activity manager");
    
    ActivityValidator validator;
    ActivityRepositoryInFile repositoryInFile("activities.csv");
    ActivityWishlist wishlist(repositoryInFile);
    ActivityManagerServiceWithWishlistAndUndo service(repositoryInFile, validator, wishlist);
    MainWindowGUI* mainWindow = new MainWindowGUI{ service };
    mainWindow->show();

    return a.exec();
}
