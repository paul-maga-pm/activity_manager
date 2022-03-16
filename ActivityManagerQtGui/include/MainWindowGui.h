#pragma once

#include <QtWidgets/qwidget.h>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout> 

#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qbuttongroup.h>
#include <QtWidgets/qtablewidget.h>
#include <QtWidgets/qspinbox.h>
#include <vector>
#include <string>

#include "WishlistGUI.h"


#include "Exception.h"
#include "Activity.h"
#include "ActivityManagerServiceWithWishlistAndUndo.h"

class MainWindowGUI :
    public QWidget
{
public:
    MainWindowGUI(ActivityManagerServiceWithWishlistAndUndo&);

private:
   
    ActivityManagerServiceWithWishlistAndUndo& activityManagerService;

    void            addcomponentsToMainWindow();

    void            addComponentsToListDetails();
    void            addComponentsToActivityDetails();

    void            linkSignalsToSlots();
    void            linkMainWindowGUIandWishlistGUI(WishlistGUI*);

    void            reloadActivitiesList(const std::vector<Activity>&);
    void            reloadTable(const std::vector<Activity>&);
    void            loadActivitiesList(QListWidget*, const std::vector<Activity>&);

    void            showExceptionMessageBox(const Exception& exception);
    void            showInformationMessageBox(const QString& title, const QString& message);

    void            loadSelectedListItem();
    void            loadActivityToForm(const Activity&);

    void            storeActivity();
    void            removeActivity();
    void            updateActivity();
    void            findActivity();
    void            undoActivityOperation();

    void            sortActivities();
    void            filterActivities();

    void            clearActivityLinesEdit();
  
    void            addActivityToWishlist();
    void            removeActivityFromWishlist();

private:
    QHBoxLayout*    mainWindowLayout = new QHBoxLayout; 

    QListWidget*    activitiesListWidget            = new QListWidget;
    QTableWidget*   activitiesTableWidget           = new QTableWidget;

    QLineEdit*      activityNameLineEdit            = new QLineEdit();
    QLineEdit*      activityDescriptionLineEdit     = new QLineEdit();
    QLineEdit*      activityTypeLineEdit            = new QLineEdit();
    QLineEdit*      activityDurationLineEdit        = new QLineEdit();
    QPushButton*    clearActivityLinesEditButton    = new QPushButton("Clear");

    QPushButton*    storeActivityButton             = new QPushButton("Store");
    QPushButton*    removeActivityButton            = new QPushButton("Remove");
    QPushButton*    updateActivityButton            = new QPushButton("Update");
    QPushButton*    findActivityButton              = new QPushButton("Find");
    QPushButton*    undoOperationButton             = new QPushButton("Undo");

    QPushButton*    sortActivitiesButton            = new QPushButton("Sort activities");
    QPushButton*    filterActivitiesButton          = new QPushButton("Filter activities");

    QButtonGroup*   sortButtonGroup                 = new QButtonGroup;
    QButtonGroup*   filterButtonGroup               = new QButtonGroup;

    QPushButton*    wishlistButton                  = new QPushButton("Wishlist");
    QPushButton*    wishlistCrudGuiButton           = new QPushButton("Wishlist CRUD");
    QPushButton*    wishlistReadOnlyButton          = new QPushButton("Wishlist ReadOnly");

    QPushButton*    addActivityToWishlistButton     = new QPushButton("Add to wishlist");
    QPushButton*    removeActivityFromWishlistButton= new QPushButton("Remove from Wishlist");
    QPushButton*    generateWishlistButton          = new QPushButton("Generate Wishlist");
    QSpinBox*       numberOfActivitiesToBeGeneratedSpinBox = new QSpinBox;
};

