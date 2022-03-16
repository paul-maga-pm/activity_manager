#include "ActivityManagerServiceWithWishlistAndUndo.h"

#include "ControllerException.h"
#include "UndoAddActivityAction.h"
#include "UndoRemoveActivityAction.h"
#include "UndoUpdateActivityAction.h"

void ActivityManagerServiceWithWishlistAndUndo::undoService()
{
	if (undoActionsList.size() == 0)
		throw ControllerException("Can't do undo!\n");
	undoActionsList.back()->doUndoOperation();
	undoActionsList.pop_back();
}

void ActivityManagerServiceWithWishlistAndUndo::addActivityService(const std::string& name, const std::string& description, const std::string& type, int duration)
{
	ActivityManagerServiceWithWishlist::addActivityService(name, description, type, duration);
	undoActionsList.push_back(std::make_unique<UndoAddActivityAction>(Activity{ name, description, type, duration }, member_activityRepository));
}

void ActivityManagerServiceWithWishlistAndUndo::removeActivityByNameService(const std::string& name)
{
	const auto& activity = member_activityRepository.findActivityByName(name);
	ActivityManagerServiceWithWishlist::removeActivityByNameService(name);
	undoActionsList.push_back(std::make_unique<UndoRemoveActivityAction>(activity, member_activityRepository));
}

void ActivityManagerServiceWithWishlistAndUndo::updateActivityService(const std::string& name, const std::string& newDescription, const std::string& newType, int newDuration)
{
	member_activityValidator.validateActivity({ name, newDescription, newType, newDuration });
	const auto& activity = member_activityRepository.findActivityByName(name);
	ActivityManagerServiceWithWishlist::updateActivityService(name, newDescription, newType, newDuration);
	undoActionsList.push_back(std::make_unique<UndoUpdateActivityAction>(activity, member_activityRepository));
}