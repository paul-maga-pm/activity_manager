#pragma once

#include "Activity.h"

class ActivityValidator
{
public:
	/// Validates the given activity;
	/// Pre: activity - const reference to an Activity object;
	/// Post: raises ValidationException in one of the following cases and with the messages:
	///			- name, description, type are empty strings ("Invalid name!\n", "Invalid description!\n", "Invalid type!\n");
	///			- duration is a negative int("Invalid duration!\n");
	void validateActivity(const Activity& activity) const;
};
