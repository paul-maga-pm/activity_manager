#include "ActivityWishlist.h"

#include "ControllerException.h"
#include "IoFileException.h"

#include <random>
#include <fstream>
void ActivityWishlist::removeAllActivitiesFromWishlist() {
	if (member_activitiesList.size())
	{
		member_activitiesList.clear();
		notify();
	}
}

size_t ActivityWishlist::removeActivityFromWishlist(const std::string& name)
{
	auto vectorIterator = member_activitiesList.begin();
	size_t occurences = 0;

	while (vectorIterator != member_activitiesList.end())
	{
		if (vectorIterator->getActivityName() == name)
		{
			vectorIterator = member_activitiesList.erase(vectorIterator);
			occurences++;
		}
		else
			vectorIterator++;
	}
	if (occurences) notify();
	return occurences;
}

void ActivityWishlist::addActivityToWishlist(const std::string& nameOfActivity) {
	auto activity = member_activityRepository.findActivityByName(nameOfActivity);
	member_activitiesList.push_back(activity);
	notify();
}


void ActivityWishlist::generateRandomWishlist(int size) {
	if (!member_activityRepository.getNumberOfActivities())
		return;
	const auto& activitiesFromRepository = member_activityRepository.getAllActivities();
	std::mt19937 randomEngine{ std::random_device{}() };
	const std::uniform_int_distribution<> distribution(0, static_cast<int>(activitiesFromRepository.size()) - 1);

	if (member_activitiesList.size())
		removeAllActivitiesFromWishlist();

	while (size > 0)
	{
		const int randomNumber = distribution(randomEngine);
		member_activitiesList.push_back(activitiesFromRepository.at(randomNumber));
		size--;
	}
	notify();
}

void ActivityWishlist::exportWishlistToCsvFile(const std::string& outputFilePath) const {
	std::ofstream outFileStream;
	outFileStream.open(outputFilePath);

	if (!outFileStream.is_open())
		throw IoFileException("The file can't be opened or created!\n");

	for (const auto& activity : member_activitiesList) {
		outFileStream << activity.getActivityName() << ","
			<< activity.getActivityDescription() << ","
			<< activity.getActivityType() << ","
			<< std::to_string(activity.getActivityDuration()) << std::endl;
	}
}

size_t ActivityWishlist::getWishlistSize() const noexcept {
	return member_activitiesList.size();
}

const std::vector<Activity>& ActivityWishlist::getActivities() const {
	return member_activitiesList;
}