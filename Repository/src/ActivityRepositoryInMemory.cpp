#include "ActivityRepositoryInMemory.h"

#include "RepositoryException.h"

void ActivityRepositoryInMemory::addActivity(const Activity& newActivity)
{
	if (getIteratorToActivityWithName(newActivity.getActivityName()) == member_activitiesList.end())
		member_activitiesList.push_back(newActivity);
	else throw RepositoryException("Activity already exists!\n");
}

const std::vector<Activity>& ActivityRepositoryInMemory::getAllActivities()
{
	return member_activitiesList;
}

size_t ActivityRepositoryInMemory::getNumberOfActivities()
{
	return member_activitiesList.size();
}
Activity ActivityRepositoryInMemory::findActivityByName(const std::string& name)
{
	auto iteratorToActivity = getIteratorToActivityWithName(name);
	if (iteratorToActivity != member_activitiesList.end())
		return *iteratorToActivity;
	throw RepositoryException("Activity doesn't exist!\n");}

void ActivityRepositoryInMemory::removeActivityByName(const std::string& name)
{
	auto iteratorToActivity = getIteratorToActivityWithName(name);
	if (iteratorToActivity != member_activitiesList.end())
		member_activitiesList.erase(iteratorToActivity);
	else
		throw RepositoryException("Activity doesn't exist!\n");
}

std::vector<Activity>::const_iterator ActivityRepositoryInMemory::getIteratorToActivityWithName(const std::string& name)  const
{
	const auto checkIfNameIs = [&name](const Activity& curentActivity) noexcept {
		return curentActivity.getActivityName() == name;
	};
	auto iteratorToFoundActivity = std::find_if(member_activitiesList.begin(),
		member_activitiesList.end(),
		checkIfNameIs);
	return iteratorToFoundActivity;
}