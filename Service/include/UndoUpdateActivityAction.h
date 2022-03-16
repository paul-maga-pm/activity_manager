#pragma once
#include "UndoAction.h"
#include "Activity.h"
#include "ActivityRepository.h"
class UndoUpdateActivityAction :
    public UndoAction
{
public:
	UndoUpdateActivityAction(const Activity& activity, ActivityRepository& activityRepository) :activityBeforeUpdate{ activity }, repository{ activityRepository } {}
	void doUndoOperation() override;

private:
	Activity activityBeforeUpdate;
	ActivityRepository& repository;
};

