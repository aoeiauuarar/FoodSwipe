#ifndef LOCATION_H
#define LOCATION_H
#include <string>
using namespace std;

// ============================================================
// Base Class: Location
// เก็บข้อมูลพื้นฐานของสถานที่ (ชื่อ + ระยะทาง)
// Restaurant จะ inherit จาก class นี้ (Inheritance)
// ============================================================
class Location {
protected:
    string name;       // ชื่อสถานที่
    double distanceKm; // ระยะทางจากผู้ใช้ (กิโลเมตร)

public:
    Location(string n, double dist);
    virtual ~Location() {}

    // Virtual functions — Derived class (Restaurant) จะ override ทั้งสองนี้
    // ทำให้เกิด Runtime Polymorphism เมื่อเรียกผ่าน Location*
    virtual void displayInfo() const;
    virtual void logInfo() const;

    double getDistance() const;
    string getName() const;
};

#endif