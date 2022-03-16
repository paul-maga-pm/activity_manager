#include "ActivityRepositoryInFile.h"

#include "IoFileException.h"
#include <fstream>

void ActivityRepositoryInFile::storeToFile()
{
	std::ofstream outActivitiesFileStream;

	outActivitiesFileStream.open(member_activitiesFileName);

	if (!outActivitiesFileStream.is_open())
		throw IoFileException("Can't open/create the file!\n");

	for (const auto& activity : member_activitiesList) {
		std::string line = activity.getActivityName() + ",";

		line += activity.getActivityDescription() + ",";
		line += activity.getActivityType() + ",";
		line += std::to_string(activity.getActivityDuration()) + '\n';
		outActivitiesFileStream << line;
	}
	outActivitiesFileStream.close();
}


void ActivityRepositoryInFile::loadFromFile()
{
	std::ifstream inActivitiesFileStream;
	inActivitiesFileStream.open(member_activitiesFileName);

	if (!inActivitiesFileStream.is_open())
		throw IoFileException("Can't open the file!\n");

	member_activitiesList.clear();
	while (!inActivitiesFileStream.eof()) {
		std::string name;
		std::getline(inActivitiesFileStream, name, ',');

		if (inActivitiesFileStream.eof())
			break;

		std::string description;
		std::getline(inActivitiesFileStream, description, ',');

		std::string type;
		std::getline(inActivitiesFileStream, type, ',');

		std::string durationString;
		std::getline(inActivitiesFileStream, durationString, '\n');

		member_activitiesList.emplace_back(name.c_str(), description.c_str(), type.c_str(), std::stoi(durationString));
	}
	inActivitiesFileStream.close();
}
