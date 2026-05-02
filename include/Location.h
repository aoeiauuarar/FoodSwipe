#ifndef LOCATION_H
#define LOCATION_H
#include <string>

using namespace std;

class Location {
protected:
    string name;
    double distanceKm; // ระยะทางจากผู้ใช้
public:
    Location(string n, double dist);
    virtual ~Location() {}
    
    // Polymorphism: Virtual function
    virtual void displayInfo() const; 
    
    double getDistance() const;
    string getName() const;
};

#endif