#pragma once
#include <QtGui/qpainter.h>
#include <QtWidgets/qwidget.h>

#include "ActivityWishlist.h"
#include "Observer.h"

class WishlistReadOnlyGui :
    public QWidget,
    public Observer
{
public:
    WishlistReadOnlyGui(ActivityWishlist& wishlist);
    ~WishlistReadOnlyGui();

    void update() override;

    void paintEvent(QPaintEvent* event) override;

private:
    ActivityWishlist& activityWishlist;
};

