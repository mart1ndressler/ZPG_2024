#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Subject
{
public:
    void addObserver(Observer* observer)
    {
        observers.push_back(observer);
        cout << "Observer added.\n";
    }

    void removeObserver(Observer* observer)
    {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
        cout << "Observer removed.\n";
    }

    void notifyObservers()
    {
        cout << "Notifying observers...\n";
        for(Observer* observer : observers) observer->update();
    }

private:
    vector<Observer*> observers;
};
#endif