#include "Location.h"
#include <iostream>

Location::Location(string n, double dist) {
    name       = n;
    distanceKm = dist;
}

// แสดงข้อมูลพื้นฐาน — จะถูก override โดย Restaurant (Polymorphism)
void Location::displayInfo() const {
    cout << name << " (" << distanceKm << " km)";
}

// บันทึก log สั้น — จะถูก override โดย Restaurant (Polymorphism)
void Location::logInfo() const {
    cout << "[Location] " << name << endl;
}

double Location::getDistance() const { return distanceKm; }
string Location::getName()     const { return name; }