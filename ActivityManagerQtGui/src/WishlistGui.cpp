#include "WishlistGui.h"

#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlabel.h>
#include <QtCore/qdebug.h>
#include <QtWidgets/qapplication.h>

WishlistGUI::WishlistGUI(ActivityManagerServiceWithWishlistAndUndo& service) :managerService{service} 
{
	addComponentsToWishlistWindow();
	linkSignalsToSlots();
	reloadWishlistActivities(service.getWishlistActivitiesService());
	service.getWishlistReference().addObserver(this);
}

WishlistGUI::~WishlistGUI()
{
	managerService.getWishlistReference().removeObserver(this);
}

void WishlistGUI::update()
{
	reloadWishlistActivities(managerService.getWishlistActivitiesService());
}

void WishlistGUI::reloadWishlistActivities(const std::vector<Activity>& wishlist)
{
	activitiesListWidget->clear();

	for (const auto& activity : wishlist)
	{
		auto name = QString::fromStdString(activity.getActivityName());
		activitiesListWidget->addItem(name);
	}
	curentNumberOfActivitiesLineEdit->setText(QString::number(wishlist.size()));
}

void WishlistGUI::addComponentsToWishlistWindow()
{
	this->setLayout(wishlistWindowLayout);

	auto activitiesListDetailsLayout = new QVBoxLayout;
	
	auto numberOfActivitieDetailsLayout = new QHBoxLayout;
	curentNumberOfActivitiesLineEdit->setReadOnly(true);
	numberOfActivitieDetailsLayout->addWidget(new QLabel("Number of activities in wishlist:"));
	numberOfActivitieDetailsLayout->addWidget(curentNumberOfActivitiesLineEdit);

	activitiesListDetailsLayout->addLayout(numberOfActivitieDetailsLayout);
	activitiesListDetailsLayout->addWidget(activitiesListWidget);
	wishlistWindowLayout->addLayout(activitiesListDetailsLayout);


	auto activityDetailsLayout = new QVBoxLayout;
	auto activityFormLayout = new QFormLayout;
	nameLineEdit->setReadOnly(true);
	descriptionLineEdit->setReadOnly(true);
	typeLineEdit->setReadOnly(true);
	durationLineEdit->setReadOnly(true);
	activityFormLayout->addRow("Name", nameLineEdit);
	activityFormLayout->addRow("Description", descriptionLineEdit);
	activityFormLayout->addRow("Type", typeLineEdit);
	activityFormLayout->addRow("Duration (minutes)", durationLineEdit);
	activityDetailsLayout->addLayout(activityFormLayout);

	auto buttonsLayout = new QVBoxLayout;
	buttonsLayout->addWidget(addActivityToWishlistButton);
	buttonsLayout->addWidget(removeAllActivitiesButton);

	auto generateButtonAndLineEditLayout = new QHBoxLayout;
	generateButtonAndLineEditLayout->addWidget(generateRandomWishlistButton);
	generateButtonAndLineEditLayout->addWidget(numberOfActivitiesSpinBox);
	buttonsLayout->addLayout(generateButtonAndLineEditLayout);

	buttonsLayout->addWidget(exportWishlistToCSVFileButton);
	activityDetailsLayout->addLayout(buttonsLayout);

	wishlistWindowLayout->addLayout(activityDetailsLayout);
	
}

void WishlistGUI::linkSignalsToSlots()
{
	QObject::connect(activitiesListWidget, &QListWidget::itemSelectionChanged, [&]() {
		loadSelectedItem();
		});

	QObject::connect(addActivityToWishlistButton, &QPushButton::clicked, [&]() {
		addToWishlist();
		});

	QObject::connect(removeAllActivitiesButton, &QPushButton::clicked, [&]() {
		removeAllFromWishlist();
		});

	QObject::connect(generateRandomWishlistButton, &QPushButton::clicked, [&]() {
		generateRandomWishlist();
		});

	QObject::connect(exportWishlistToCSVFileButton, &QPushButton::clicked, [&]() {
		exportWishlistToCSVFile();
		});
}

void WishlistGUI::clearActivityForm()
{
	nameLineEdit->setText("");
	descriptionLineEdit->setText("");
	typeLineEdit->setText("");
	durationLineEdit->setText("");
}

void WishlistGUI::loadSelectedItem()
{
	if (!activitiesListWidget->selectedItems().isEmpty())
	{
		auto selectedItem = activitiesListWidget->selectedItems().at(0);
		auto name = selectedItem->text().toStdString();
		try
		{
			const auto activity = managerService.findActivityByNameService(name);
			loadActivityToForm(activity);
		}
		catch (const Exception& exception)
		{
			showExceptionMessageBox(exception);
		}
	}
	else
		clearActivityForm();
}

void WishlistGUI::loadActivityToForm(const Activity& activity)
{
	nameLineEdit->setText(QString::fromStdString(activity.getActivityName()));
	descriptionLineEdit->setText(QString::fromStdString(activity.getActivityDescription()));
	typeLineEdit->setText(QString::fromStdString(activity.getActivityType()));
	durationLineEdit->setText(QString::number(activity.getActivityDuration()));
}

void WishlistGUI::showExceptionMessageBox(const Exception& exception)
{
	auto exceptionMessageBox = new QMessageBox(QMessageBox::Warning, "Exception", QString::fromStdString(exception.getExceptionMessage()));
	exceptionMessageBox->exec();
}

void WishlistGUI::showInformationMessageBox(const QString& title, const QString& message)
{
	auto informationMessageBox = new QMessageBox(QMessageBox::Information, title, message);
	informationMessageBox->exec();
}

void WishlistGUI::addToWishlist()
{
	auto name = nameLineEdit->text().toStdString();
	
	try
	{
		managerService.addActivityToWishlistService(name);
		reloadWishlistActivities(managerService.getWishlistActivitiesService());
	}
	catch (const Exception& exception)
	{
		showExceptionMessageBox(exception);
	}
}

void WishlistGUI::removeAllFromWishlist()
{
	managerService.removeAllFromWishlistService();
	reloadWishlistActivities(managerService.getWishlistActivitiesService());
	clearActivityForm();
}

void WishlistGUI::generateRandomWishlist()
{
	auto numberOfActivities = numberOfActivitiesSpinBox->text().toInt();
	try {
		managerService.generateRandomWishlistService(numberOfActivities);
		reloadWishlistActivities(managerService.getWishlistActivitiesService());
	}
	catch (const Exception& exception)
	{
		showExceptionMessageBox(exception);
	}
}

void WishlistGUI::exportWishlistToCSVFile()
{
	QString filePathQString = QFileDialog::getSaveFileName(this, "Save wishlist", "Wishlist", "Excel (*.csv)");
	auto filePathStdString = filePathQString.toStdString();

	if(filePathStdString.size())
		try {
			managerService.exportWishlistToCsvFileService(filePathStdString);
			showInformationMessageBox("Wishlist", "The wishlist has been succesfully saved!");
		}
		catch (const Exception& exception)
		{
			showExceptionMessageBox(exception);
		}
}	
