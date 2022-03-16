#include "Console.h"
#include "ConsoleUserCommandCodes.h"
#include "Exception.h"
#include "StringParser.h"

#include <string>

#include <vector>
#include <iostream>
#include <stdexcept>


void ConsoleUserInterface::run()
{
	bool printMeniuFlag = true;
	while (true)
	{
		if(printMeniuFlag) printMainMeniuInFrameUi();
		printMeniuFlag = true;

		std::string userCommand;
		std::cout << ">>";
		std::getline(std::cin, userCommand);
		userCommand = stripSpaces(userCommand);

		if (userCommand == "exit")
			return;

		if (userCommand == "meniu")
			continue;

		if (userCommand != "")
		{
			try
			{
				const auto userCommandCode = std::stoi(userCommand);
				if (isUserCommandValid(userCommandCode))
					runUserCommand(userCommandCode);
				else
				{
					std::cout << userCommand << " is not a meniu option!\n";
					printMeniuFlag = false;
				}
			}
			catch (std::invalid_argument&)
			{
				printExceptionMessage(userCommand + " is not a valid integer value!\n");
			}
			catch (const Exception& exception) {
				printExceptionMessage(exception.getExceptionMessage());
			}
		}
		else printMeniuFlag = false;
	}
}

void ConsoleUserInterface::printMainMeniuOptionsUi() const
{
	std::cout << "1. Add activity to list\n";
	std::cout << "2. See all activities\n";
	std::cout << "3. Delete activity\n";
	std::cout << "4. Update activity\n";
	std::cout << "5. Find activity\n";
	std::cout << "6. Filter activities\n";
	std::cout << "7. Sort activities\n";
}

void ConsoleUserInterface::printHeaderWithMessage(const std::string& message) const
{
	std::string delimiter = "----------------------------------------------------------------";
	std::string endOfHeader;
	std::copy(delimiter.begin(), delimiter.begin() + 50 - message.size(), std::back_inserter(endOfHeader));
	endOfHeader = endOfHeader + "\n";
	std::cout << "--------------" + message + endOfHeader;
}

void ConsoleUserInterface::printFooter() const
{
	std::cout << "----------------------------------------------------------------\n";
}

bool ConsoleUserInterface::isUserCommandValid(int userCommandCode) const noexcept
{
	return ADD_ACTIVITY_CODE <= userCommandCode && userCommandCode <= SORT_ACTIVITIES_CODE;
}



void ConsoleUserInterface::runUserCommand(int userCommandCode)
{
	if (userCommandCode == ADD_ACTIVITY_CODE)
		addActivityUi();
	else if (userCommandCode == SEE_ALL_ACTIVITIES_CODE)
		seeAllActivitiesUi();
	else if (userCommandCode == DELETE_ACTIVITY_CODE)
		deleteActivityUi();
	else if (userCommandCode == UPDATE_ACTIVITY_CODE)
		updateActivityUi();
	else if (userCommandCode == FIND_ACTIVITY_CODE)
		findActivityUi();
	else if (userCommandCode == FILTER_ACTIVITIES_CODE)
		filterActivitiesUi();
	else if (userCommandCode == SORT_ACTIVITIES_CODE)
		sortActivitiesUi();
}

void ConsoleUserInterface::printMainMeniuInFrameUi() const 
{
	printHeaderWithMessage("MENIU");
	printMainMeniuOptionsUi();
	printFooter();
}

void ConsoleUserInterface::printActivity(const Activity& activity) const
{
	std::string activityString = "";

	/*activityString += "Name: " + activity.getActivityName();
	activityString += ", Description: " + activity.getActivityDescription();
	activityString += ", Type: " + activity.getActivityType();
	activityString += ", Duration: ";
	std::cout << activityString << activity.getActivityDuration() <<"\n";*/
	std::cout.width(16);
	std::cout << activity.getActivityName();

	std::cout.width(16);
	std::cout << activity.getActivityDescription();

	std::cout.width(16);
	std::cout << activity.getActivityType();

	std::cout.width(12);
	std::cout << activity.getActivityDuration() << "\n";
}


void ConsoleUserInterface::printActivititiesWithHeaderMessage(const std::vector<Activity>& activities, const std::string& headerMessage) const
{
	printHeaderWithMessage(headerMessage);
	std::cout.width(16);
	std::cout << "NAME";

	std::cout.width(16);
	std::cout << "DESCRIPTION";

	std::cout.width(16);
	std::cout << "TYPE";

	std::cout.width(12);
	std::cout << "DURATION" << "\n";

	for (const auto& activity : activities)
		printActivity(activity);
}

void ConsoleUserInterface::printExceptionMessage(const std::string& exceptionMessage) const
{
	printHeaderWithMessage("ERROR");
	std::cout << exceptionMessage;
}

void ConsoleUserInterface::addActivityUi()
{
	std::string name, description, type;
	std::string durationStringBuffer;
	int duration;

	printHeaderWithMessage("ADD ACTIVITY");
	std::cout << "Name: ";
	std::getline(std::cin, name);
	name = stripSpaces(name);

	std::cout << "Description: ";
	std::getline(std::cin, description);
	description = stripSpaces(description);

	std::cout << "Type: ";
	std::getline(std::cin, type);
	type = stripSpaces(type);

	std::cout << "Duration: ";
	std::getline(std::cin, durationStringBuffer);
	duration = stoi(durationStringBuffer);
	
	member_managerService.addActivityService(name, description, type, duration);
}

void ConsoleUserInterface::seeAllActivitiesUi() const
{
	const auto& activities = member_managerService.getAllActivitiesService();

	printActivititiesWithHeaderMessage(activities, "ACTIVITIES");
}

void ConsoleUserInterface::deleteActivityUi()
{
	printHeaderWithMessage("DELETE ACTIVITY");
	std::string name;
	std::cout << "Name: ";
	std::getline(std::cin, name);
	name = stripSpaces(name);

	member_managerService.removeActivityByNameService(name);
}

void ConsoleUserInterface::updateActivityUi()
{
	printHeaderWithMessage("UPDATE ACTIVITY");
	std::string name, description, type, durationInputBuffer;
	int duration;

	std::cout << "Name: ";
	std::getline(std::cin, name);
	name = stripSpaces(name);

	std::cout << "New description: ";
	std::getline(std::cin, description);
	description = stripSpaces(description);

	std::cout << "New type: ";
	std::getline(std::cin, type);
	type = stripSpaces(type);

	std::cout << "New duration: ";
	std::getline(std::cin, durationInputBuffer);
	duration = stoi(durationInputBuffer);

	member_managerService.updateActivityService(name, description, type, duration);
}

void ConsoleUserInterface::findActivityUi() const
{
	printHeaderWithMessage("FIND ACTIVITY");
	std::string name;

	std::cout << "Name: ";
	std::getline(std::cin, name);
	name = stripSpaces(name);

	const auto resultOfSearch = member_managerService.findActivityByNameService(name);
	printActivity(resultOfSearch);
}

void ConsoleUserInterface::filterActivitiesByDescriptionUi() const
{
	
	std::string description;
	std::cout << "Description: ";
	std::getline(std::cin, description);
	description = stripSpaces(description);
	auto resultOfFiltration = member_managerService.filterActivitiesByDescriptionService(description);
	printActivititiesWithHeaderMessage(resultOfFiltration, "ACTIVITIES");
}

void ConsoleUserInterface::filterActivitiesByTypeUi() const
{
	std::string type;
	std::cout << "Type: ";
	std::getline(std::cin, type);
	type = stripSpaces(type);
	auto resultOfFiltration = member_managerService.filterActivitiesByTypeService(type);
	printActivititiesWithHeaderMessage(resultOfFiltration, "ACTIVITIES");
}

void ConsoleUserInterface::filterActivitiesUi() const
{
	printHeaderWithMessage("FILTER");
	std::cout << "Choose the key: <1> description, <2> type\n";
	std::cout << ">>";
	std::string key;
	std::getline(std::cin, key);
	key = stripSpaces(key);
	const int code = std::stoi(key);
	if (code == FILTER_ACTIVITIES_BY_DESCRIPTION_CODE) filterActivitiesByDescriptionUi();
	else if (code == FILTER_ACTIVITIES_BY_TYPE_CODE) filterActivitiesByTypeUi();
	else if (key.size()) std::cout << key + " is not a valid filter option!\n";
}

void ConsoleUserInterface::sortActivitiesByNameUi() const
{
	auto sortedActivities = member_managerService.sortActivitiesByNameService();
	printActivititiesWithHeaderMessage(sortedActivities, "ACTIVITIES");
}

void ConsoleUserInterface::sortActivitiesByDescriptionUi() const
{
	auto sortedActivities = member_managerService.sortActivitiesByDescriptionService();
	printActivititiesWithHeaderMessage(sortedActivities, "ACTIVITIES");
}

void ConsoleUserInterface::sortActivitiesByTypeAndDurationUi() const
{
	auto sortedActivities = member_managerService.sortActivitiesByTypeAndDurationService();
	printActivititiesWithHeaderMessage(sortedActivities, "ACTIVITIES");
}

void ConsoleUserInterface::sortActivitiesUi() const
{
	printHeaderWithMessage("SORT");
	std::cout << "Choose the key: <1> name, <2> description, <3> type and duration\n";
	std::cout << ">>";
	std::string key;
	std::getline(std::cin, key);
	key = stripSpaces(key);
	const int code = std::stoi(key);
	if (code == SORT_ACTIVITIES_BY_NAME_CODE) sortActivitiesByNameUi();
	else if (code == SORT_ACTIVITIES_BY_DESCRIPTION_CODE) sortActivitiesByDescriptionUi();
	else if (code == SORT_ACTIVITIES_BY_TYPE_AND_DURATION_CODE) sortActivitiesByTypeAndDurationUi();
	else if (key.size()) 
		std::cout << key + " is not a valid sort option!\n";
}

void ConsoleUserInterfaceWithWishlistOptionsAndUndo::printMainMeniuInFrameUi() const
{

	ConsoleUserInterface::printHeaderWithMessage("MENIU");
	ConsoleUserInterface::printMainMeniuOptionsUi();
	std::cout << "8. Undo last operation (add, update or delete)\n";
	printWishlistOptions();
	printActivitiesFromWishList();
	printFooter();
}
bool ConsoleUserInterfaceWithWishlistOptionsAndUndo::isUserCommandValid(int commandCode) const noexcept
{

	return ConsoleUserInterface::isUserCommandValid(commandCode) || (UNDO_CODE <= commandCode && commandCode <= EXPORT_TO_CSV_CODE);
}

void ConsoleUserInterfaceWithWishlistOptionsAndUndo::runUserCommand(int commandCode)
{
	ConsoleUserInterface::runUserCommand(commandCode);

	if (commandCode == UNDO_CODE)
		undoUi();
	else if (commandCode == ADD_TO_WISHLIST_CODE)
		addActivityToWishlistUi();
	else if (commandCode == REMOVE_ALL_FROM_WISHLIST_CODE)
		removeAllFromWishListUi();
	else if (commandCode == GENERATE_WISHLIST_CODE)
		generateWishlistUi();
	else if (commandCode == EXPORT_TO_CSV_CODE)
		exportWishlistToCsvFileUi();
}

void ConsoleUserInterfaceWithWishlistOptionsAndUndo::printWishlistOptions() const
{
	printHeaderWithMessage("WISHLIST OPTIONS");
	std::cout << "9. Add activity to wishlist\n";
	std::cout << "10. Remove all from wishlist\n";
	std::cout << "11. Generate wishlist\n";
	std::cout << "12. Export wishlist to file\n";
	const auto wishlistSize = member_managerService.getWishlistSizeService();
	std::cout << "Number of activities in wishlist: " << wishlistSize << std::endl;
}

void ConsoleUserInterfaceWithWishlistOptionsAndUndo::printActivitiesFromWishList() const
{
	if (member_managerService.getWishlistSizeService()) {

		ConsoleUserInterface::printActivititiesWithHeaderMessage(member_managerService.getWishlistActivitiesService(), "WISHLIST");
	}
}

void ConsoleUserInterfaceWithWishlistOptionsAndUndo::addActivityToWishlistUi()
{
	std::string name;
	printHeaderWithMessage("ADD ACTIVITY TO WISHLIST");
	std::cout << "Name: ";
	std::getline(std::cin, name);
	name = stripSpaces(name);

	member_managerService.addActivityToWishlistService(name);
}

void ConsoleUserInterfaceWithWishlistOptionsAndUndo::removeAllFromWishListUi()
{
	member_managerService.removeAllFromWishlistService();
}

void ConsoleUserInterfaceWithWishlistOptionsAndUndo::generateWishlistUi()
{
	printHeaderWithMessage("GENERATE WISHLIST");
	std::cout << "Number of activities to be generated: ";
	std::string sizeString;
	std::getline(std::cin, sizeString);
	sizeString = stripSpaces(sizeString);
	member_managerService.generateRandomWishlistService(std::stoi(sizeString));
}

void ConsoleUserInterfaceWithWishlistOptionsAndUndo::exportWishlistToCsvFileUi()
{
	printHeaderWithMessage("EXPORT WISHLIST TO FILE");
	std::cout << "Text file name: ";
	std::string fileName;
	std::getline(std::cin, fileName);
	fileName = stripSpaces(fileName);
	member_managerService.exportWishlistToCsvFileService(fileName);
}

void ConsoleUserInterfaceWithWishlistOptionsAndUndo::undoUi()
{
	member_managerService.undoService();
}
