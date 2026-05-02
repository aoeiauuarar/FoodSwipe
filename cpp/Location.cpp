#include "Location.h"
#include <iostream>

Location::Location(string n, double dist) {
    name = n;
    distanceKm = dist;
}

void Location::displayInfo() const {
    cout << name << " (" << distanceKm << " km)";
}

double Location::getDistance() const { return distanceKm; }
string Location::getName() const { return name; }