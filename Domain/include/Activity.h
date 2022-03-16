#pragma once

#include <string>
#include <iostream>
class Activity
{
private:
	std::string member_name;
	std::string member_description;
	std::string member_type;
	int member_duration;
public:
	Activity() = default;
	/// Constructor used for the class;
	/// Pre: name, description, type - const string references;
	///		 duration - int;
	/// Post: the fields of the object will be initialized with these values;
	Activity(const std::string& name, const std::string& description, const std::string& type, int duration) :
		member_name{ name }, member_description{ description }, member_type{ type }, member_duration{ duration }{}

	// Returns a const string reference of the name;
	const std::string& getActivityName() const noexcept;

	// Returns a const string reference of the description;
	const std::string& getActivityDescription() const noexcept;

	// Returns a const string reference of the type;
	const std::string& getActivityType() const noexcept;

	// Returns an integer representing the duration (in minutes);
	int getActivityDuration() const noexcept;

	Activity(const Activity& other) :
		member_description{ other.member_description }, member_name{ other.member_name },
		member_type{ other.member_type }, member_duration{ other.member_duration }{
		//std::cout << "Copy!\n";
	};
	~Activity() = default;
	Activity& operator=(const Activity& other) = default;
	Activity(Activity&& other) = default;
	Activity& operator =(Activity&& other) = default;
};



