#include "Activity.h"


const std::string& Activity::getActivityName() const noexcept
{
	return member_name;
}

const std::string& Activity::getActivityDescription() const noexcept
{
	return member_description;
}

const std::string& Activity::getActivityType() const noexcept
{
	return member_type;
}

int Activity::getActivityDuration() const noexcept
{
	return member_duration;
}