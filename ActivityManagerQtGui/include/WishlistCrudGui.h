#pragma once

#include "QtWidgets/qwidget.h"
#include "Observer.h"
#include "ActivityWishlist.h"
#include "QtWidgets/qtablewidget.h"
#include "QtWidgets/qpushbutton"
#include "QtWidgets/qspinbox.h"

class WishlistCrudGui :
    public QWidget,
    public Observer
{
public:

    WishlistCrudGui(ActivityWishlist& wishlist);
    void update() override;
    ~WishlistCrudGui();

private:
    ActivityWishlist& wishlist_m;

    QTableWidget*   activityTableWidget                   = new QTableWidget;
    QPushButton*    clearWishlistButton                     = new QPushButton("Clear Wishlist");
    QPushButton*    generateWishlistButton                  = new QPushButton("Generate wishlist");
    QSpinBox*       numberOfActivitiesToBeGeneratedSpinBox  = new QSpinBox;

private:
    void reloadActivitiesTable(const std::vector<Activity> activitiesThatWillBeLoaded);

    void addComponentsToWindow();
    void linkSignalsToSlots();
};

