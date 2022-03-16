#pragma once

#include <string>
#include <vector>

#include "Activity.h"
#include "ActivityRepositoryInMemory.h"

class ActivityRepositoryInFile :
    public ActivityRepositoryInMemory
{
public:
	ActivityRepositoryInFile(std::string activitiesFileName) :ActivityRepositoryInMemory(), member_activitiesFileName{ activitiesFileName }{}
	~ActivityRepositoryInFile() = default;
	ActivityRepositoryInFile(const ActivityRepositoryInFile & otherRepository) = delete;
	ActivityRepositoryInFile& operator=(const ActivityRepositoryInFile & otherRepository) = delete;
	ActivityRepositoryInFile(ActivityRepositoryInFile && otherRepository) = delete;
	ActivityRepositoryInFile& operator=(ActivityRepositoryInFile && otherRepository) = delete;

	void addActivity(const Activity & newActivity) override {
		loadFromFile();
		ActivityRepositoryInMemory::addActivity(newActivity);
		storeToFile();
	}

	const std::vector<Activity>& getAllActivities()  override {
		loadFromFile();
		return ActivityRepositoryInMemory::getAllActivities();
	}

	size_t getNumberOfActivities() override {
		loadFromFile();
		return ActivityRepositoryInMemory::getNumberOfActivities();
	}

	Activity findActivityByName(const std::string & name) override {
		loadFromFile();
		return ActivityRepositoryInMemory::findActivityByName(name);
	}

	void removeActivityByName(const std::string & name) override {
		loadFromFile();
		ActivityRepositoryInMemory::removeActivityByName(name);
		storeToFile();
	}

	/// Stores the activities in the file
	/// Raises IoFileException if the file can't be created or opened
	void storeToFile();

	/// Loads the activities from file 
	/// Raises IoFileException if the file can't be opened
	void loadFromFile();

private:
	std::string member_activitiesFileName;
};

