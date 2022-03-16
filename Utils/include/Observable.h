#pragma once

#include "Observer.h"
#include <vector>

class Observable
{
public:
	virtual ~Observable() = default;
	void addObserver(Observer* observerThatWillBeAdded);
	void removeObserver(Observer* observerThatWillBeRemoved);
	void notify();

private:
	std::vector<Observer*> observersVector;
};

