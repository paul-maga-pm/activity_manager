#include "WishlistCrudGui.h"
#include "QtWidgets/QVBoxLayout"

WishlistCrudGui::WishlistCrudGui(ActivityWishlist& wishlist) :
	wishlist_m{ wishlist }
{
	addComponentsToWindow();
	linkSignalsToSlots();
	reloadActivitiesTable(wishlist_m.getActivities());
	wishlist.addObserver(this);
}

void WishlistCrudGui::update()
{
	reloadActivitiesTable(wishlist_m.getActivities());
}

WishlistCrudGui::~WishlistCrudGui()
{
	wishlist_m.removeObserver(this);

}

void WishlistCrudGui::reloadActivitiesTable(const std::vector<Activity> activitiesThatWillBeLoaded)
{
	activityTableWidget->clear();
	activityTableWidget->setRowCount(static_cast<int>(activitiesThatWillBeLoaded.size()));
	int curentRow = 0;

	for (const auto& activity : activitiesThatWillBeLoaded)
	{
		auto name = QString::fromStdString(activity.getActivityName());
		auto description = QString::fromStdString(activity.getActivityDescription());
		auto type = QString::fromStdString(activity.getActivityType());
		auto duration = QString::number(activity.getActivityDuration());
		activityTableWidget->setItem(curentRow, 0, new QTableWidgetItem(name));
		activityTableWidget->setItem(curentRow, 1, new QTableWidgetItem(description));
		activityTableWidget->setItem(curentRow, 2, new QTableWidgetItem(type));
		activityTableWidget->setItem(curentRow, 3, new QTableWidgetItem(duration));
		curentRow++;
	}
}

void WishlistCrudGui::addComponentsToWindow()
{
	auto mainLayout = new QVBoxLayout;
	this->setLayout(mainLayout);
	mainLayout->addWidget(activityTableWidget);
	activityTableWidget->setColumnCount(10);
	QStringList headerLabels{ "Name","Descritpion", "Type", "Duration" };
	activityTableWidget->setHorizontalHeaderLabels(headerLabels);

	auto wishlistGenerateAndSpinBoxLayout = new QHBoxLayout;
	wishlistGenerateAndSpinBoxLayout->addWidget(generateWishlistButton);
	wishlistGenerateAndSpinBoxLayout->addWidget(numberOfActivitiesToBeGeneratedSpinBox);

	mainLayout->addWidget(clearWishlistButton);
	mainLayout->addLayout(wishlistGenerateAndSpinBoxLayout);
}

void WishlistCrudGui::linkSignalsToSlots()
{
	QObject::connect(clearWishlistButton, &QPushButton::clicked, [=]() {
		wishlist_m.removeAllActivitiesFromWishlist();
		});
	QObject::connect(generateWishlistButton, &QPushButton::clicked, [=]() {
		auto numberOfActivitiesToBeGenerated = numberOfActivitiesToBeGeneratedSpinBox->text().toInt();
		wishlist_m.generateRandomWishlist(numberOfActivitiesToBeGenerated);
		});
}
