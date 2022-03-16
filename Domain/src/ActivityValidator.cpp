#include "ActivityValidator.h"

#include "ValidationException.h"

void ActivityValidator::validateActivity(const Activity& activity) const
{
	std::string exceptionMessage = "";

	if (activity.getActivityName() == "")
		exceptionMessage += "Invalid name!\n";
	if (activity.getActivityDescription() == "")
		exceptionMessage += "Invalid description!\n";
	if (activity.getActivityType() == "")
		exceptionMessage += "Invalid type!\n";
	if (activity.getActivityDuration() <= 0)
		exceptionMessage += "Invalid duration!\n";
	if (exceptionMessage != "")
		throw ValidationException(exceptionMessage);
}
