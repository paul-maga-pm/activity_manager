#pragma once

#include <string>
#include <vector>

#include "Activity.h"
#include "ActivityValidator.h"
#include "ActivityRepository.h"
#include "ActivityWishlist.h"
#include "ActivityManagerService.h"

class ActivityManagerServiceWithWishlist :
    public ActivityManagerService
{
public:
	ActivityManagerServiceWithWishlist(ActivityRepository& activityRepository, ActivityValidator& activityValidator, ActivityWishlist& activityWishlist) noexcept :
		ActivityManagerService(activityRepository, activityValidator), member_wishlist{ activityWishlist }{}

	virtual ~ActivityManagerServiceWithWishlist() {}

	ActivityManagerServiceWithWishlist(const ActivityManagerServiceWithWishlist& otherService) = delete;
	ActivityManagerServiceWithWishlist& operator=(const ActivityManagerServiceWithWishlist& otherService) = delete;

	// We make sure that if one item from the repository is removed, then the item will be removed from the wishlist too
	void removeActivityByNameService(const std::string&) override;

	// We make sure that if one item from the repository is updated, then the item from the wishlist will be updated too
	void updateActivityService(const std::string& name, const std::string& newDescription, const std::string& newType, int newDuration);

	/// Adds an activity into the wishlist;
	/// Pre: nameOfActivity - string;
	/// Post: the activity with the given name (if it exists in the Repository) will be
	/// added to the wishlist;
	/// Raises RepositoryException with "Activity doesn't exist!\n" if the activity with the given 
	/// name doesn't exist
	void addActivityToWishlistService(const std::string name) {
		member_wishlist.addActivityToWishlist(name);
	}


	/// Retuns the wishlist
	const std::vector<Activity>& getWishlistActivitiesService() {
		return member_wishlist.getActivities();
	}

	/// Returns the number of activities from the wishlist
	size_t getWishlistSizeService() const noexcept {
		return member_wishlist.getWishlistSize();
	}

	// Removes all activities from the wishlist;
	void removeAllFromWishlistService() {
		member_wishlist.removeAllActivitiesFromWishlist();
	}

	/// Exports the wishlist to a CSV file;
	/// Pre: outFilePath - string;
	/// Post: the file with the given path will coontain the wishlist;
	/// If the file doesn't exist on disk, it will be created;
	/// If the file exists on disk, it will be overwrite;
	/// Raises ControllerException if the file can't be opened or created
	void exportWishlistToCsvFileService(const std::string& outFilePath) {
		member_wishlist.exportWishlistToCsvFile(outFilePath);
	}

	/// Generates a random wishlist;
	/// Pre: sizeOfGeneratedWishlist - integer;
	/// Post: the wishlist will be populated with sizeOfGeneratedWishList activities;
	/// The activities before the random generation will be removed;
	void generateRandomWishlistService(int size) {
		member_wishlist.generateRandomWishlist(size);
	}

	/// Returns a reference to the wishlist
	ActivityWishlist& getWishlistReference() const noexcept
	{
		return member_wishlist;
	}

private:
	ActivityWishlist& member_wishlist;
};

