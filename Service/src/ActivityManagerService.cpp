#include "ActivityManagerService.h"

#include <algorithm>
void ActivityManagerService::addActivityService(const std::string& name, const std::string& description, const std::string& type, int duration)
{
	Activity newActivity{ name, description, type, duration };
	member_activityValidator.validateActivity(newActivity);

	member_activityRepository.addActivity(newActivity);
}

const std::vector<Activity>& ActivityManagerService::getAllActivitiesService() const
{
	return member_activityRepository.getAllActivities();
}

size_t ActivityManagerService::getNumberOfActivitiesService() const
{
	return member_activityRepository.getNumberOfActivities();
}

void ActivityManagerService::removeActivityByNameService(const std::string& name)
{
	member_activityRepository.removeActivityByName(name);
}

Activity ActivityManagerService::findActivityByNameService(const std::string& name) const
{
	return member_activityRepository.findActivityByName(name);
}

void ActivityManagerService::updateActivityService(const std::string& name, const std::string& newDescription, const std::string& newType, int newDuration)
{
	Activity updatedActivity{ name, newDescription, newType, newDuration };
	member_activityValidator.validateActivity(updatedActivity);
	member_activityRepository.removeActivityByName(name);
	member_activityRepository.addActivity(updatedActivity);
}


std::vector<Activity> ActivityManagerService::filterActivitiesBy(isFieldFilterFunction checkField) const
{
	std::vector<Activity> resultOfFiltration;
	const auto& allActivities = member_activityRepository.getAllActivities();
	std::copy_if(allActivities.begin(), allActivities.end(),
		std::back_inserter(resultOfFiltration),
		checkField);
	return resultOfFiltration;
}


std::vector<Activity> ActivityManagerService::filterActivitiesByDescriptionService(const std::string& descriptionKey) const
{
	const auto checkIfDescriptionIs = [descriptionKey](const Activity& activity)  noexcept
	{
		return activity.getActivityDescription() == descriptionKey;
	};
	return filterActivitiesBy(checkIfDescriptionIs);
}

std::vector<Activity> ActivityManagerService::filterActivitiesByTypeService(const std::string& typeKey) const
{
	const auto checkIfTypeIs = [typeKey](const Activity& activity)  noexcept
	{
		return activity.getActivityType() == typeKey;
	};
	return filterActivitiesBy(checkIfTypeIs);

}



std::vector<Activity> ActivityManagerService::sortActivitiesByNameService()  const
{
	auto activities = member_activityRepository.getAllActivities();
	const auto compareFunction = [](const Activity& thisActivity, const Activity& otherActivity) noexcept
	{
		return thisActivity.getActivityName() < otherActivity.getActivityName();
	};
	std::sort(activities.begin(), activities.end(), compareFunction);
	return activities;
}

std::vector<Activity> ActivityManagerService::sortActivitiesByDescriptionService() const
{
	auto activities = member_activityRepository.getAllActivities();
	const auto compareFunction = [](const Activity& thisActivity, const Activity& otherActivity) noexcept
	{
		return thisActivity.getActivityDescription() < otherActivity.getActivityDescription();
	};
	std::sort(activities.begin(), activities.end(), compareFunction);
	return activities;
}

bool compareActivitiesByTypeAndDuration(const Activity& thisActivity, const Activity& otherActivity)  noexcept
{
	if (thisActivity.getActivityType() < otherActivity.getActivityType())
		return true;
	if (thisActivity.getActivityType() == otherActivity.getActivityType())
		if (thisActivity.getActivityDuration() < otherActivity.getActivityDuration())
			return true;
	return false;
}

std::vector<Activity>  ActivityManagerService::sortActivitiesByTypeAndDurationService() const
{
	auto activities = member_activityRepository.getAllActivities();
	std::sort(activities.begin(), activities.end(), compareActivitiesByTypeAndDuration);
	return activities;
}
