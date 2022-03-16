#include "UndoAddActivityAction.h"
void UndoAddActivityAction::doUndoOperation()
{
	repository.removeActivityByName(addedActivity.getActivityName());
}