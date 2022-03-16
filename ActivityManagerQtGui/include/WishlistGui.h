#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qbuttongroup.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qspinbox.h>

#include <vector>

#include "Exception.h"

#include "Activity.h"
#include "ActivityManagerServiceWithWishlistAndUndo.h"
#include "Observer.h"


class WishlistGUI :
    public QWidget,
    public Observer
{
public:
    WishlistGUI(ActivityManagerServiceWithWishlistAndUndo&);
    ~WishlistGUI();
    void update() override;

    void            loadActivityToForm(const Activity&);
    void            clearActivityForm();
    void            reloadWishlistActivities(const std::vector<Activity>& wishlist);

private:
    void            addComponentsToWishlistWindow();
    void            linkSignalsToSlots();
    
    void            loadSelectedItem();

    void            showExceptionMessageBox(const Exception& exception);
    void            showInformationMessageBox(const QString& title, const QString& message);
   
    void            addToWishlist();
    void            removeAllFromWishlist();
    void            generateRandomWishlist();
    void            exportWishlistToCSVFile();

    ActivityManagerServiceWithWishlistAndUndo&      managerService;

    QHBoxLayout*    wishlistWindowLayout                = new QHBoxLayout;

    QListWidget*    activitiesListWidget                = new QListWidget;

    QPushButton*    addActivityToWishlistButton         = new QPushButton("Add");
    QPushButton*    removeAllActivitiesButton           = new QPushButton("Remove all");
    QPushButton*    generateRandomWishlistButton        = new QPushButton("Generate");
    QPushButton*    exportWishlistToCSVFileButton       = new QPushButton("Save as");

    QLineEdit*      nameLineEdit                        = new QLineEdit;
    QLineEdit*      descriptionLineEdit                 = new QLineEdit;
    QLineEdit*      typeLineEdit                        = new QLineEdit;
    QLineEdit*      durationLineEdit                    = new QLineEdit;
    QSpinBox*       numberOfActivitiesSpinBox           = new QSpinBox;
    QLineEdit*      curentNumberOfActivitiesLineEdit    = new QLineEdit;
};

