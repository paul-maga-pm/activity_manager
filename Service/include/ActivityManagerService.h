#pragma once


#include <string>
#include <vector>
#include <functional>

#include "Activity.h"
#include "ActivityValidator.h"
#include "ActivityRepository.h"

class ActivityManagerService
{
public:
	// Constructor of the class
	ActivityManagerService(ActivityRepository& activityRepository, ActivityValidator& activityValidator) noexcept :

		member_activityRepository{ activityRepository }, member_activityValidator{ activityValidator }
	{}

	virtual ~ActivityManagerService() {};
	// We dont't allow the service to be copied
	ActivityManagerService(const ActivityManagerService& otherService) = delete;
	ActivityManagerService& operator=(const ActivityManagerService& otherService) = delete;

	/// Adds a new activity in the list if it hasn't been added yet;
	/// Pre: name, description, type - const string references;
	///		 duration - integer;
	///	Post: the activity will be added in the list if it hasn't already been added;
	/// Raises ValidationException if the activity is not valid;
	/// Raises RepositoryException if the activity has already been added;
	virtual void addActivityService(const std::string& name, const std::string& description, const std::string& type, int duration);

	/// Returns all the activities of the repository;
	/// Pre: the repository is not empty;
	/// Post: a const reference vector with all the activities will be returned;
	const std::vector<Activity>& getAllActivitiesService() const;

	/// Returns the number of activities in the list
	size_t getNumberOfActivitiesService() const;

	/// Removes the activity with the given name
	/// Pre: name - const string reference
	/// Post: the activity with the given name will be removed from the list
	/// Raises RepositoryException with "Activity doesn't exist!\n" message if the activity with the given 
	/// name is not in the list;
	virtual void removeActivityByNameService(const std::string& name);

	/// Returns the activity with the given name;
	/// Pre: the activity with the given name must be in the repository;
	/// Post: a const reference Activity object will be returned;
	/// Raises RepositoryException with "Activity doesn't exist!\n" message if the activity with the given 
	/// name is not in the list;
	Activity findActivityByNameService(const std::string& name) const;

	/// Updates the fields of the activity with the given name with the new fields;
	/// Pre: name, newDescription, newType - const referenece strings;
	///		 newDuration - int;
	/// Post: the activity will be updated;
	/// Raises RepositoryException with "Activity doesn't exist!\n" message if the activity with the given 
	/// name is not in the list;
	/// Raises ValidationException if the given fields are not valid;
	virtual void updateActivityService(const std::string& name, const std::string& newDescription, const std::string& newType, int newDuration);



	/// Filters the list after description;
	/// Pre: descriptionKey - string which represents the key of filtration;
	/// Post: returns a vector with all the activities with the given description;
	std::vector<Activity> filterActivitiesByDescriptionService(const std::string& descriptionKey) const;

	/// Filters the list after type;
	/// Pre: typeKey - string which represents the key of filtration;
	/// Post: returns a vector with all the activities of the given type;
	std::vector<Activity> filterActivitiesByTypeService(const std::string& typeKey) const;


	/// Sorts the activities by name
	/// Pre: activities - vector of Activity objects
	/// Post: the given vector will be sorted in acending order by name
	std::vector<Activity> sortActivitiesByNameService() const;

	// Sorts the activities by description
	/// Pre: activities - vector of Activity objects
	/// Post: the given vector will be sorted in acending order by description
	std::vector<Activity> sortActivitiesByDescriptionService() const;

	// Sorts the activities by type and duration
	/// Pre: activities - vector of Activity objects
	/// Post: the given vector will be sorted in acending order by type and duration
	std::vector<Activity> sortActivitiesByTypeAndDurationService() const;

	
protected:
	ActivityRepository& member_activityRepository;
	ActivityValidator& member_activityValidator;

private:
	typedef std::function<bool(const Activity&)> isFieldFilterFunction;
	/// Generic function used for filtration after one field;
	/// Pre: isFilterFieldFunction - bool function that checks if one activity respects a given criteria for a single
	/// field;
	/// Post: returns a vector with all activities that respect the given criteria;
	std::vector<Activity>filterActivitiesBy(isFieldFilterFunction) const;

};

