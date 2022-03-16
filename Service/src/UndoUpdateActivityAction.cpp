#include "UndoUpdateActivityAction.h"
void UndoUpdateActivityAction::doUndoOperation()
{
	repository.removeActivityByName(activityBeforeUpdate.getActivityName());
	repository.addActivity(activityBeforeUpdate);
}
