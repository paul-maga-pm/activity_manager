#include "MainWindowGui.h"

#include <QtWidgets/qformlayout.h>
#include "QtCore\qstring.h"
#include "QtWidgets\qapplication.h"
#include "QtWidgets/qcheckbox.h"
#include "QtWidgets/qapplication.h"
#include "RepositoryException.h"

#include "WishlistCrudGui.h"
#include "WishlistReadOnlyGui.h"

#define SORT_BY_NAME_CHECK_BOX_ID 1
#define SORT_BY_DESCRIPTION_CHECK_BOX_ID 2
#define SORT_BY_TYPE_AND_DURATION_CHECK_BOX_ID 3

#define FILTER_BY_DESCRIPTION_CHECK_BOX_ID 1
#define FILTER_BY_TYPE_CHECK_BOX_ID 2

MainWindowGUI::MainWindowGUI(ActivityManagerServiceWithWishlistAndUndo& service) :activityManagerService{ service }
{
	addcomponentsToMainWindow();

	try {
		reloadActivitiesList(activityManagerService.getAllActivitiesService());
		reloadTable(activityManagerService.getAllActivitiesService());
	}
	catch (const Exception& exception)
	{
		showExceptionMessageBox(exception);
	}
	linkSignalsToSlots();
}
void MainWindowGUI::addcomponentsToMainWindow()
{
	this->setLayout(mainWindowLayout);
	addComponentsToListDetails();
	addComponentsToActivityDetails();
}

void MainWindowGUI::addComponentsToListDetails()
{
	auto activitiesListDetailsLayout = new QVBoxLayout;
	
	auto listAndTableLayout = new QHBoxLayout;
	
	listAndTableLayout->addWidget(activitiesListWidget);
	listAndTableLayout->addWidget(activitiesTableWidget);

	activitiesListDetailsLayout->addLayout(listAndTableLayout);
	activitiesTableWidget->setColumnCount(4);
	QStringList headerLabels{ "Name","Descritpion", "Type", "Duration" };
	activitiesTableWidget->setHorizontalHeaderLabels(headerLabels);

	auto sortAndFilterDetailsButtonsLayout = new QHBoxLayout;
	auto sortButtonsLayout = new QVBoxLayout;
	auto sortByNameCheckBox = new QCheckBox("Name");
	auto sortByDescriptionCheckBox = new QCheckBox("Description");
	auto sortByTypeAndDurationCheckBox = new QCheckBox("Type and Duration");

	sortButtonsLayout->addWidget(sortByNameCheckBox);
	sortButtonsLayout->addWidget(sortByDescriptionCheckBox);
	sortButtonsLayout->addWidget(sortByTypeAndDurationCheckBox);

	sortButtonGroup->addButton(sortByNameCheckBox, SORT_BY_NAME_CHECK_BOX_ID);
	sortButtonGroup->addButton(sortByDescriptionCheckBox, SORT_BY_DESCRIPTION_CHECK_BOX_ID);
	sortButtonGroup->addButton(sortByTypeAndDurationCheckBox, SORT_BY_TYPE_AND_DURATION_CHECK_BOX_ID);
	sortByNameCheckBox->setChecked(true);
	sortAndFilterDetailsButtonsLayout->addLayout(sortButtonsLayout);

	auto filterButtonsLayout = new QVBoxLayout;
	auto filterByDescriptionCheckBox = new QCheckBox("Description");
	auto filterByTypeCheckBox = new QCheckBox("Type");

	filterButtonGroup->addButton(filterByDescriptionCheckBox, FILTER_BY_DESCRIPTION_CHECK_BOX_ID);
	filterButtonGroup->addButton(filterByTypeCheckBox, FILTER_BY_TYPE_CHECK_BOX_ID);
	filterByDescriptionCheckBox->setChecked(true);

	filterButtonsLayout->addWidget(filterByDescriptionCheckBox);
	filterButtonsLayout->addWidget(filterByTypeCheckBox);
	
	sortAndFilterDetailsButtonsLayout->addLayout(filterButtonsLayout);
	activitiesListDetailsLayout->addLayout(sortAndFilterDetailsButtonsLayout);

	auto sortAndFilterButtonsLayout = new QHBoxLayout;
	sortAndFilterButtonsLayout->addWidget(sortActivitiesButton);
	sortAndFilterButtonsLayout->addWidget(filterActivitiesButton);

	activitiesListDetailsLayout->addLayout(sortAndFilterButtonsLayout);

	mainWindowLayout->addLayout(activitiesListDetailsLayout);
}

void MainWindowGUI::addComponentsToActivityDetails()
{
	QVBoxLayout* activityDetailsLayout = new QVBoxLayout;

	QFormLayout* actvityDetailsFormLayout = new QFormLayout;
	actvityDetailsFormLayout->addRow("Name", activityNameLineEdit);
	actvityDetailsFormLayout->addRow("Description", activityDescriptionLineEdit);
	actvityDetailsFormLayout->addRow("Type", activityTypeLineEdit);
	actvityDetailsFormLayout->addRow("Duration (minutes)", activityDurationLineEdit);

	actvityDetailsFormLayout->addWidget(clearActivityLinesEditButton);

	activityDetailsLayout->addLayout(actvityDetailsFormLayout);

	QVBoxLayout* activityDetailsButtonsLayout = new QVBoxLayout;
	QWidget* activityDetailsButtonsWidget = new QWidget;
	activityDetailsButtonsWidget->setLayout(activityDetailsButtonsLayout);
	
	activityDetailsButtonsLayout->addWidget(storeActivityButton);
	activityDetailsButtonsLayout->addWidget(removeActivityButton);
	activityDetailsButtonsLayout->addWidget(updateActivityButton);
	activityDetailsButtonsLayout->addWidget(findActivityButton);
	activityDetailsButtonsLayout->addWidget(undoOperationButton);
	activityDetailsButtonsLayout->addWidget(wishlistButton);

	activityDetailsLayout->addWidget(activityDetailsButtonsWidget);
	mainWindowLayout->addLayout(activityDetailsLayout);

	activityDetailsButtonsLayout->addWidget(wishlistCrudGuiButton);
	activityDetailsButtonsLayout->addWidget(wishlistReadOnlyButton);

	auto wishlistGenerateAndSpinBoxLayout = new QHBoxLayout;
	wishlistGenerateAndSpinBoxLayout->addWidget(generateWishlistButton);
	wishlistGenerateAndSpinBoxLayout->addWidget(numberOfActivitiesToBeGeneratedSpinBox);

	activityDetailsButtonsLayout->addWidget(addActivityToWishlistButton);
	activityDetailsButtonsLayout->addWidget(removeActivityFromWishlistButton);
	activityDetailsButtonsLayout->addLayout(wishlistGenerateAndSpinBoxLayout);
}

void MainWindowGUI::linkSignalsToSlots()
{
	QObject::connect(activitiesListWidget, &QListWidget::itemSelectionChanged, [&]() {
		loadSelectedListItem();
		});

	QObject::connect(storeActivityButton, &QPushButton::clicked, [&]() {
		storeActivity();
		});

	QObject::connect(removeActivityButton, &QPushButton::clicked, [&]() {
		removeActivity();
		});

	QObject::connect(clearActivityLinesEditButton, &QPushButton::clicked, [&]() {
		clearActivityLinesEdit();
		});

	QObject::connect(updateActivityButton, &QPushButton::clicked, [&]() {
		updateActivity();
		});

	QObject::connect(findActivityButton, &QPushButton::clicked, [&]() {
		findActivity();
		});

	QObject::connect(undoOperationButton, &QPushButton::clicked, [&]() {
		undoActivityOperation();
		});

	QObject::connect(sortActivitiesButton, &QPushButton::clicked, [&]() {
		sortActivities();
		});

	QObject::connect(filterActivitiesButton, &QPushButton::clicked, [&]() {
		filterActivities();
		});

	QObject::connect(wishlistButton, &QPushButton::clicked, [&]() {
		auto wishlistWindow = new WishlistGUI(activityManagerService);
		linkMainWindowGUIandWishlistGUI(wishlistWindow);
		wishlistWindow->show();
		});

	QObject::connect(wishlistReadOnlyButton, &QPushButton::clicked, [=]() {
		auto wishlistReadOnlyWindow = new WishlistReadOnlyGui(activityManagerService.getWishlistReference());
		wishlistReadOnlyWindow->show();
		});

	QObject::connect(wishlistCrudGuiButton, &QPushButton::clicked, [=]() {
		auto wishlistCrudWindow = new WishlistCrudGui(activityManagerService.getWishlistReference());
		wishlistCrudWindow->show();
		});

	QObject::connect(addActivityToWishlistButton, &QPushButton::clicked, [=]() {
		addActivityToWishlist();
		});

	QObject::connect(removeActivityFromWishlistButton, &QPushButton::clicked, [=]() {
		removeActivityFromWishlist();
		});

	QObject::connect(generateWishlistButton, &QPushButton::clicked, [=]() {
		auto numberOfActivitiesToBeGenerated = numberOfActivitiesToBeGeneratedSpinBox->text().toInt();
		activityManagerService.generateRandomWishlistService(numberOfActivitiesToBeGenerated);
		});
}

void MainWindowGUI::linkMainWindowGUIandWishlistGUI(WishlistGUI* wishlistGui)
{
	QObject::connect(activitiesListWidget, &QListWidget::itemSelectionChanged, [=]() {

		if (activitiesListWidget->selectedItems().size())
		{
			auto selectedItem = activitiesListWidget->selectedItems().at(0);
			auto name = selectedItem->text().toStdString();
			try {
				const auto selectedActivity = activityManagerService.findActivityByNameService(name);
				wishlistGui->loadActivityToForm(selectedActivity);
			}
			catch (const Exception& exception)
			{
				showExceptionMessageBox(exception);
			}
		}
		else wishlistGui->clearActivityForm();
		});
}

void MainWindowGUI::reloadActivitiesList(const std::vector<Activity>& activities)
{
	loadActivitiesList(activitiesListWidget, activities);
}

void MainWindowGUI::reloadTable(const std::vector<Activity>& activities)
{
	activitiesTableWidget->clearContents();
	activitiesTableWidget->setRowCount(static_cast<int>(activities.size()));
	
	int curentRow = 0;
	for (const auto& curentActivity : activities)
	{
		auto nameItem = new QTableWidgetItem(QString::fromStdString(curentActivity.getActivityName()));
		activitiesTableWidget->setItem(curentRow, 0, nameItem);

		auto descriptionItem = new QTableWidgetItem(QString::fromStdString(curentActivity.getActivityDescription()));
		activitiesTableWidget->setItem(curentRow, 1, descriptionItem);

		auto typeItem = new QTableWidgetItem(QString::fromStdString(curentActivity.getActivityType()));
		activitiesTableWidget->setItem(curentRow, 2, typeItem);

		auto durationItem = new QTableWidgetItem(QString::number(curentActivity.getActivityDuration()));
		activitiesTableWidget->setItem(curentRow, 3, durationItem);

		curentRow++;
	}
}

void MainWindowGUI::loadActivitiesList(QListWidget* list, const std::vector<Activity>& activities)
{
	list->clear();
	for (const auto& activity : activities)
	{
		const auto& name = activity.getActivityName();
		list->addItem(QString::fromStdString(name));
	}
}

void MainWindowGUI::showExceptionMessageBox(const Exception& exception)
{
	auto exceptionMessageBox = new QMessageBox(QMessageBox::Warning, "Exception", QString::fromStdString(exception.getExceptionMessage()));
	exceptionMessageBox->exec();
}

void MainWindowGUI::showInformationMessageBox(const QString& title, const QString& message)
{
	auto informationMessageBox = new QMessageBox(QMessageBox::Information, title, message);
	informationMessageBox->exec();
}


void MainWindowGUI::loadSelectedListItem()
{
	if (activitiesListWidget->selectedItems().isEmpty())
	{
		clearActivityLinesEdit();
	}
	else
	{
		QListWidgetItem* selectedItem = activitiesListWidget->selectedItems().at(0);
		Activity selectedActivity;
		try {
			selectedActivity = activityManagerService.findActivityByNameService(selectedItem->text().toStdString());
			loadActivityToForm(selectedActivity);
		}
		catch (const Exception& exception)
		{
			showExceptionMessageBox(exception);
		}

	}
}

void MainWindowGUI::loadActivityToForm(const Activity& activity)
{
	auto name = QString::fromStdString(activity.getActivityName());
	auto description = QString::fromStdString(activity.getActivityDescription());
	auto type = QString::fromStdString(activity.getActivityType());
	auto duration = QString::number(activity.getActivityDuration());

	activityNameLineEdit->setText(name);
	activityDescriptionLineEdit->setText(description);
	activityTypeLineEdit->setText(type);
	activityDurationLineEdit->setText(duration);
}

void MainWindowGUI::storeActivity()
{
	auto name = activityNameLineEdit->text().toStdString();
	auto description = activityDescriptionLineEdit->text().toStdString();
	auto type = activityTypeLineEdit->text().toStdString();

	bool isDurationGood;
	auto duration = activityDurationLineEdit->text().toInt(&isDurationGood);

	if (isDurationGood)
	{
		try {
			activityManagerService.addActivityService(name, description, type, duration);
			reloadActivitiesList(activityManagerService.getAllActivitiesService());
			reloadTable(activityManagerService.getAllActivitiesService());
			showInformationMessageBox("Store", "The activity has been succesfully added to the file!");
		}
		catch (const Exception& exception) {
			showExceptionMessageBox(exception);
		}
	}
	else showExceptionMessageBox(Exception("Invalid numerical value for activity duration!"));
}

void MainWindowGUI::removeActivity()
{
	auto name = activityNameLineEdit->text().toStdString();

	try
	{
		activityManagerService.removeActivityByNameService(name);
		reloadActivitiesList(activityManagerService.getAllActivitiesService());
		reloadTable(activityManagerService.getAllActivitiesService());
		showInformationMessageBox("Remove", "The activity has been succesfully removed from the file!");
	}
	catch (const Exception& exception) {
		showExceptionMessageBox(exception);
	}
}

void MainWindowGUI::updateActivity()
{
	auto name = activityNameLineEdit->text().toStdString();
	auto description = activityDescriptionLineEdit->text().toStdString();
	auto type = activityTypeLineEdit->text().toStdString();

	bool isDurationGood;
	auto duration = activityDurationLineEdit->text().toInt(&isDurationGood);

	if (isDurationGood)
	{
		try
		{
			activityManagerService.updateActivityService(name, description, type, duration);
			reloadActivitiesList(activityManagerService.getAllActivitiesService());
			reloadTable(activityManagerService.getAllActivitiesService());
			showInformationMessageBox("Update", "The activity has been succesfully updated!");
		}
		catch (const Exception& exception)
		{
			showExceptionMessageBox(exception.getExceptionMessage());
		}
	}
	else showExceptionMessageBox(Exception("Invalid numerical value for activity duration!"));
}

void MainWindowGUI::findActivity()
{
	Activity resultOfFind;
	try
	{
		auto name = activityNameLineEdit->text().toStdString();
		resultOfFind = activityManagerService.findActivityByNameService(name);
		loadActivityToForm(resultOfFind);
	}
	catch (const Exception& exception)
	{
		showExceptionMessageBox(exception.getExceptionMessage());
	}
}

void MainWindowGUI::undoActivityOperation()
{
	try
	{
		activityManagerService.undoService();
		reloadActivitiesList(activityManagerService.getAllActivitiesService());
		reloadTable(activityManagerService.getAllActivitiesService());
	}
	catch (const Exception& exception)
	{
		showExceptionMessageBox(exception.getExceptionMessage());
	}
}

void MainWindowGUI::sortActivities()
{
	auto clickedCheckBoxId = sortButtonGroup->checkedId();

	try
	{
		switch (clickedCheckBoxId)
		{
		case SORT_BY_NAME_CHECK_BOX_ID: 
		{
			reloadActivitiesList(activityManagerService.sortActivitiesByNameService());
			reloadTable(activityManagerService.sortActivitiesByNameService());
		}
			break;

		case SORT_BY_DESCRIPTION_CHECK_BOX_ID: 
		{
			reloadActivitiesList(activityManagerService.sortActivitiesByDescriptionService());
			reloadTable(activityManagerService.sortActivitiesByDescriptionService());
		}
			break;

		case SORT_BY_TYPE_AND_DURATION_CHECK_BOX_ID: 
		{
			reloadActivitiesList(activityManagerService.sortActivitiesByTypeAndDurationService());
			reloadTable(activityManagerService.sortActivitiesByTypeAndDurationService());
		}
			break;
		default:
			break;
		}
	}
	catch (const Exception& exception)
	{
		showExceptionMessageBox(exception);
	}
}

void MainWindowGUI::filterActivities()
{
	auto clickedCheckBoxId = filterButtonGroup->checkedId();

	try
	{
		switch (clickedCheckBoxId)
		{
		case FILTER_BY_DESCRIPTION_CHECK_BOX_ID:
		{
			auto description = activityDescriptionLineEdit->text().toStdString();
			auto resultOfFiltration = activityManagerService.filterActivitiesByDescriptionService(description);
			auto listOfFIltredActivities = new QListWidget;
			loadActivitiesList(listOfFIltredActivities, resultOfFiltration);
			listOfFIltredActivities->show();
		}break;

		case FILTER_BY_TYPE_CHECK_BOX_ID:
		{
			auto type = activityTypeLineEdit->text().toStdString();
			auto resultOfFiltration = activityManagerService.filterActivitiesByTypeService(type);
			auto listOfFiltredActivities = new QListWidget;
			loadActivitiesList(listOfFiltredActivities, resultOfFiltration);
			listOfFiltredActivities->show();
		}break;
		default:
			break;
		}

	}
	catch (const Exception& exception)
	{
		showExceptionMessageBox(exception);
	}
}

void MainWindowGUI::clearActivityLinesEdit()
{
	activityNameLineEdit->setText("");
	activityDescriptionLineEdit->setText("");
	activityTypeLineEdit->setText("");
	activityDurationLineEdit->setText("");
}

void MainWindowGUI::addActivityToWishlist()
{
	auto name = activityNameLineEdit->text().toStdString();

	try
	{
		activityManagerService.addActivityToWishlistService(name);
	}
	catch (const Exception& exception)
	{
		showExceptionMessageBox(exception);
	}
}

void MainWindowGUI::removeActivityFromWishlist()
{
	auto name = activityNameLineEdit->text().toStdString();
	auto numberOfOccurencesInWishlist = activityManagerService.getWishlistReference().removeActivityFromWishlist(name);
	if (!numberOfOccurencesInWishlist)
		showExceptionMessageBox(Exception("Activity doesn't exist in wishlist!\n"));
}
