#pragma once
// This file contains the ui class
// Author: Paul

#include "ActivityManagerService.h"
#include "ActivityManagerServiceWithWishlistAndUndo.h"

#include <string>
#include<vector>


class ConsoleUserInterface
{
public:
	/// Cosntructor for the class;
	/// Pre: managerService - ActivityManagerService object;
	/// Post: the member_managerService will refer to the given object;
	ConsoleUserInterface(ActivityManagerService& managerService)noexcept :member_managerService{managerService}{} 

	virtual ~ConsoleUserInterface() = default;

	// We don't allow the object to be copied;
	ConsoleUserInterface(const ConsoleUserInterface& otherConsoleObject) = delete;
	ConsoleUserInterface& operator=(const ConsoleUserInterface& other) = delete;
	ConsoleUserInterface(ConsoleUserInterface&& other) = delete;
	ConsoleUserInterface& operator=(ConsoleUserInterface&& other) = delete;

	/// Runs the application;
	/// Reads a command from the user and executes it;
	void run();


	void printMainMeniuOptionsUi() const;

	void printHeaderWithMessage(const std::string& message) const;

	void printFooter() const;

	// Prints the meniu in a frame;
	virtual void printMainMeniuInFrameUi() const;

	// Prints the given vector of activities;
	void printActivititiesWithHeaderMessage(const std::vector<Activity>& activities, const std::string& headerMessage) const;

	/// Checks if the command given as a string is valid (is a single char between '1' and '7');
	virtual bool isUserCommandValid(int userCommandCode) const noexcept;

	// Runs the given command;
	virtual void runUserCommand(int userCommandCode);


private:
	ActivityManagerService& member_managerService;


	// Prins the given activity;
	void printActivity(const Activity& activity) const;
	
	// Prints the given message as an error
	void printExceptionMessage(const std::string& exceptionMessage) const;

	/// Reads name(string), description(string), type(string) and duration(int)from the user and adds the given activity;
	/// Raises invaldi_argument exception if the user didn't provide for duration an int;
	/// Raises ValidationException if the activity is not valid;
	/// Raises RepositoryException if the activity has already been added;
	void addActivityUi();

	/// Prints all activities of the list;
	/// Raises RepositoryException if the list is empty;
	void seeAllActivitiesUi() const;

	/// Reads name(string) and deletes the activity with the given name;
	/// Raises ReporitoryException if the activity doesn't exist;
	void deleteActivityUi();
	
	/// Reads name(string), description(string), type(string) and duration(int)from the user and updates the activity
	/// with the given name with the new fields;
	/// Raises invaldi_argument exception if the user didn't provide for duration an int;
	/// Raises ValidationException if the activity is not valid;
	/// Raises RepositoryException if the activity doesn't exist;
	void updateActivityUi();

	/// Reads name(string) and prints the activity with the given name;
	/// Raises ReporitoryException if the activity doesn't exist;
	void findActivityUi() const;

	/// Reads a description(string) from the user and prints all the activities with the given description;
 	/// Raises RepositoryException if the list is empty;
	void filterActivitiesByDescriptionUi() const;

	/// Reads a type(string) from the user and prints all the activities with the given type;
	/// Raises RepositoryException if the list is empty;
	void filterActivitiesByTypeUi() const;
	
	/// Filters the activities after the key provided by the user
	/// Raises RepositoryException if the list is empty;
	void filterActivitiesUi() const;

	/// Sorts the activities by name
	/// Raises RepositoryException if the list is empty;
	void sortActivitiesByNameUi() const;

	/// Sorts the activities by description
	/// Raises RepositoryException if the list is empty;
	void sortActivitiesByDescriptionUi() const;

	/// Sorts the activities by type and duration
	/// Raises RepositoryException if the list is empty;
	void sortActivitiesByTypeAndDurationUi() const;

	/// Sorts the list of activities after the key provided by the user
	/// Raises RepositoryException if the list is empty;
	void sortActivitiesUi() const;
};

class ConsoleUserInterfaceWithWishlistOptionsAndUndo :public ConsoleUserInterface {
public:
	ConsoleUserInterfaceWithWishlistOptionsAndUndo(ActivityManagerServiceWithWishlistAndUndo& managerService) noexcept :
		ConsoleUserInterface(managerService), member_managerService{ managerService }{}
		
	// We don't allow the object to be copied;
	ConsoleUserInterfaceWithWishlistOptionsAndUndo(const ConsoleUserInterfaceWithWishlistOptionsAndUndo& otherConsoleObject) = delete;
	ConsoleUserInterfaceWithWishlistOptionsAndUndo& operator=(const ConsoleUserInterfaceWithWishlistOptionsAndUndo& other) = delete;
	ConsoleUserInterfaceWithWishlistOptionsAndUndo(ConsoleUserInterfaceWithWishlistOptionsAndUndo&& other) = delete;
	ConsoleUserInterfaceWithWishlistOptionsAndUndo& operator=(ConsoleUserInterfaceWithWishlistOptionsAndUndo&& other) = delete;

	~ConsoleUserInterfaceWithWishlistOptionsAndUndo() = default;

	void printMainMeniuInFrameUi() const override;
	bool isUserCommandValid(int commandCode) const noexcept override;
	void runUserCommand(int commandCode) override;

	void printWishlistOptions() const;

	void printActivitiesFromWishList() const;

private:
	ActivityManagerServiceWithWishlistAndUndo& member_managerService;

	void addActivityToWishlistUi();
	void removeAllFromWishListUi();
	void generateWishlistUi();
	void exportWishlistToCsvFileUi();
	void undoUi();
};
