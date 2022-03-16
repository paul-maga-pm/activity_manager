#pragma once
#include "UndoAction.h"
#include "Activity.h"
#include "ActivityRepository.h"
class UndoRemoveActivityAction :
    public UndoAction
{
public:
	UndoRemoveActivityAction(const Activity& activity, ActivityRepository& activityRepository) :removedActivity{ activity }, repository{ activityRepository } {}
	void doUndoOperation() override;

private:
	Activity removedActivity;
	ActivityRepository& repository;
};

