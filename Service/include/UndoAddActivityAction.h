#pragma once
#include "UndoAction.h"
#include "Activity.h"
#include "ActivityRepository.h"

class UndoAddActivityAction :
    public UndoAction
{
public:
	UndoAddActivityAction(const Activity& activity, ActivityRepository& activityRepository) :addedActivity{ activity }, repository{ activityRepository } {}
	void doUndoOperation() override;

private:
	const Activity addedActivity;
	ActivityRepository& repository;
};

