#pragma once


#include <string>
#include <vector>

#include "Activity.h"
#include "ActivityRepository.h"

#include "Observable.h"
class ActivityWishlist:
	public Observable
{
public:
	ActivityWishlist(ActivityRepository& activityRepository) noexcept :
		member_activityRepository{ activityRepository } {}
	~ActivityWishlist() = default;

	// We dont't allow the whishlist to be copied 
	ActivityWishlist(const ActivityWishlist & other) = delete;
	ActivityWishlist& operator=(const ActivityWishlist & other) = delete;
	ActivityWishlist(ActivityWishlist && other) = delete;
	ActivityWishlist& operator=(ActivityWishlist && other) = delete;

	// Removes all activities from the wishlist;
	void removeAllActivitiesFromWishlist();

	// Removes all aparitions of the given activity from the wishlist
	// Returns the number of occurences in the list
	size_t removeActivityFromWishlist(const std::string& nameOfActivity);

	/// Adds an activity into the wishlist;
	/// Pre: nameOfActivity - string;
	/// Post: the activity with the given name (if it exists in the Repository) will be
	/// added to the wishlist;
	/// Raises RepositoryException with "Activity doesn't exist!\n" if the activity with the given 
	/// name doesn't exist
	void addActivityToWishlist(const std::string & nameOfActivity);

	/// Generates a random wishlist;
	/// Pre: sizeOfGeneratedWishlist - integer;
	/// Post: the wishlist will be populated with sizeOfGeneratedWishList activities;
	/// The activities before the random generation will be removed;
	void generateRandomWishlist(int size);

	/// Exports the wishlist to a CSV file;
	/// Pre: outputFilePath - string;
	/// Post: the file with the given path will contain the wishlist;
	/// If the file doesn't exist on disk, it will be created;
	/// If the file exists on disk, it will be overwrite;
	/// Raises IoFileException if the file can't be opened or created
	void exportWishlistToCsvFile(const std::string & outputFilePath) const;

	/// Returns the number of activities from the wishlist
	size_t getWishlistSize() const noexcept;

	/// Retuns the wishlist
	const std::vector<Activity>& getActivities() const;
private:
	std::vector<Activity> member_activitiesList;
	ActivityRepository& member_activityRepository;
};

