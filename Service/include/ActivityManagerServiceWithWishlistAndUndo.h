#pragma once

#include <string>

#include "ActivityManagerServiceWithWishlist.h"
#include "UndoAction.h"


class ActivityManagerServiceWithWishlistAndUndo :
    public ActivityManagerServiceWithWishlist
{
public:
	ActivityManagerServiceWithWishlistAndUndo(ActivityRepository& activityRepository, ActivityValidator& activityValidator, ActivityWishlist& activityWishlist) noexcept :
		ActivityManagerServiceWithWishlist::ActivityManagerServiceWithWishlist(activityRepository, activityValidator, activityWishlist)
	{

	}

	ActivityManagerServiceWithWishlistAndUndo(const ActivityManagerServiceWithWishlistAndUndo& otherService) = delete;
	ActivityManagerServiceWithWishlistAndUndo& operator=(const ActivityManagerServiceWithWishlistAndUndo& otherService) = delete;
	
	/// Undo method on the elements from the repository;
	/// Raises ControllerException with "Can't do undo!\n" message if the list of undo actions is empty;
	void undoService();


	/// Adds a new activity in the list if it hasn't been added yet;
	/// Pre: name, description, type - const string references;
	///		 duration - integer;
	///	Post: the activity will be added in the list if it hasn't already been added;
	/// Raises ValidationException if the activity is not valid;
	/// Raises RepositoryException if the activity has already been added;
	/// If the activity was added into the list, then an undoAddActivityAction unique pointer with a copy of the activity will be
	/// added at the end of the undoActionsList;
	void addActivityService(const std::string& name, const std::string& description, const std::string& type, int duration) override;


	/// Removes the activity with the given name
	/// Pre: name - const string reference
	/// Post: the activity with the given name will be removed from the list
	/// Raises RepositoryException with "Activity doesn't exist!\n" message if the activity with the given 
	/// name is not in the list;
	/// If the activity was removed the list, then an undoRemoveActivityAction unique pointer with a copy of the activity will be
	/// added at the end of the undoActionsList;
	void removeActivityByNameService(const std::string& name) override;


	/// Updates the fields of the activity with the given name with the new fields; 
	/// Pre: name, newDescription, newType - const referenece strings;
	///		 newDuration - int;
	/// Post: the activity will be updated;
	/// Raises RepositoryException with "Activity doesn't exist!\n" message if the activity with the given 
	/// name is not in the list;
	/// Raises ValidationException if the given fields are not valid;
	/// If the activity was updated, then an undoUpdateActivityAction unique pointer with a copy of the activity will be
	/// added at the end of the undoActionsList;
	void updateActivityService(const std::string& name, const std::string& newDescription, const std::string& newType, int newDuration) override;
private:
	std::vector<std::unique_ptr<UndoAction>> undoActionsList;
};

