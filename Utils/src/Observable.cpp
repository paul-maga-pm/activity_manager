#include "Observable.h"
#include <algorithm>
void Observable::addObserver(Observer* observerThatWillBeAdded)
{
	observersVector.push_back(observerThatWillBeAdded);
}

void Observable::removeObserver(Observer* observerThatWillBeRemoved)
{
	observersVector.erase(std::remove(observersVector.begin(), observersVector.end(), observerThatWillBeRemoved), observersVector.end());
}

void Observable::notify()
{
	for (auto observer : observersVector)
		observer->update();
}


