#pragma once
class UndoAction
{
public:
	UndoAction() = default;
	virtual ~UndoAction() {};
	UndoAction(const UndoAction & other) = delete;
	UndoAction& operator=(const UndoAction & other) = delete;
	UndoAction(UndoAction && other) = delete;
	UndoAction& operator=(UndoAction && other) = delete;

	/// Method that all undoAction classes must implement
	virtual void doUndoOperation() = 0;
};

