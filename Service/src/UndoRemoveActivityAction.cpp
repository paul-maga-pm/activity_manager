#include "UndoRemoveActivityAction.h"
void UndoRemoveActivityAction::doUndoOperation()
{
	repository.addActivity(removedActivity);
}