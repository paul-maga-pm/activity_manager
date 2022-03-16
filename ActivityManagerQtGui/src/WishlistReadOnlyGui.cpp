#include "WishlistReadOnlyGui.h"
#include <random>


WishlistReadOnlyGui::WishlistReadOnlyGui(ActivityWishlist& wishlist) :
	activityWishlist{ wishlist }
{
	wishlist.addObserver(this);
}

WishlistReadOnlyGui::~WishlistReadOnlyGui()
{
	activityWishlist.removeObserver(this);
}

void WishlistReadOnlyGui::update()
{
	repaint();
}

void WishlistReadOnlyGui::paintEvent(QPaintEvent*)
{
	auto numberOfImages = activityWishlist.getWishlistSize();
	QPainter painter{ this };
	QImage imageToBeDrawn("rick.jpg");

	auto windowWidth = this->width();
	auto windowheight = this->height();

	if (windowWidth - imageToBeDrawn.width() <= 0 || windowheight - imageToBeDrawn.height() <= 0)
		return;

	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<> xDistribution(0, windowWidth - imageToBeDrawn.width());
	std::uniform_int_distribution<> yDistribution(0, windowheight - imageToBeDrawn.height());

	for (int i = 0; i < numberOfImages; i++)
	{
		auto x = xDistribution(mt);
		auto y = yDistribution(mt);
		painter.drawImage(x, y, imageToBeDrawn);
	}
}
