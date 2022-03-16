#pragma once

#include <vector>
#include <string>

#include "Activity.h"
#include "ActivityRepository.h"

class ActivityRepositoryInMemory :
    public ActivityRepository
{
public:
	ActivityRepositoryInMemory() = default;
	virtual ~ActivityRepositoryInMemory() = default;

	ActivityRepositoryInMemory(const ActivityRepositoryInMemory & otherRepository) = delete;
	ActivityRepositoryInMemory& operator=(const ActivityRepositoryInMemory & otherRepository) = delete;
	ActivityRepositoryInMemory(ActivityRepositoryInMemory && otherRepository) = delete;
	ActivityRepositoryInMemory& operator=(ActivityRepositoryInMemory && otherRepository) = delete;

	/// Adds a new activity into the repository;
	/// Pre: newActivity - Activity object that hasn't been added yet;
	/// Post: newActivity will be added into the repository;
	/// Raises RepositoryException with "Activity already exists!\n" message if the activity already exists;
	virtual void addActivity(const Activity & newActivity) override;

	/// Returns all the activities of the repository;
	/// Pre: the repository is not empty;
	/// Post: a const reference vector with all the activities will be returned;
	virtual const std::vector<Activity>& getAllActivities() override;

	/// Returns the number of activities in the list
	virtual size_t getNumberOfActivities() override;

	/// Returns the activity with the given name;
	/// Pre: the activity with the given name must be in the repository;
	/// Post: a const reference Activity object will be returned;
	/// Raises RepositoryException with "Activity doesn't exist!\n" message if the activity with the given 
	/// name is not in the list;
	virtual Activity findActivityByName(const std::string & name) override;

	/// Removes the activity with the given name
	/// Pre: name - const string reference
	/// Post: the activity with the given name will be removed from the list
	/// Raises RepositoryException with "Activity doesn't exist!\n" message if the activity with the given 
	/// name is not in the list;
	virtual void removeActivityByName(const std::string & name) override;

protected:
	std::vector<Activity> member_activitiesList;
private:
	/// Returns an iterator to the activity with the given name from the vector
	std::vector<Activity>::const_iterator getIteratorToActivityWithName(const std::string & name) const;

};

