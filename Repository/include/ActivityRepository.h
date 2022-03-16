#pragma once

#include <vector>

#include "Activity.h"

class ActivityRepository
{
public:
	ActivityRepository() = default;
	virtual ~ActivityRepository() = default;

	ActivityRepository(const ActivityRepository & otherRepository) = delete;
	ActivityRepository& operator=(const ActivityRepository & otherRepository) = delete;
	ActivityRepository(ActivityRepository && otherRepository) = delete;
	ActivityRepository& operator=(ActivityRepository && otherRepository) = delete;

	virtual void addActivity(const Activity & newActivity) = 0;

	virtual const std::vector<Activity>& getAllActivities() = 0;

	virtual size_t getNumberOfActivities() = 0;

	virtual Activity findActivityByName(const std::string & name) = 0;

	virtual void removeActivityByName(const std::string & name) = 0;
};

