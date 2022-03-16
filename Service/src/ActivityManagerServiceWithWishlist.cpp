#include "ActivityManagerServiceWithWishlist.h"

void ActivityManagerServiceWithWishlist::removeActivityByNameService(const std::string& name)
{
	auto activityFromRepository = findActivityByNameService(name);
	ActivityManagerService::removeActivityByNameService(name);
	member_wishlist.removeActivityFromWishlist(name);
}

void ActivityManagerServiceWithWishlist::updateActivityService(const std::string& name, const std::string& newDescription, const std::string& newType, int newDuration)
{
	ActivityManagerService::updateActivityService(name, newDescription, newType, newDuration);
	
	auto occurences = member_wishlist.removeActivityFromWishlist(name);
	for (size_t i = 0; i < occurences; i++)
		member_wishlist.addActivityToWishlist(name);
}
