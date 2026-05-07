#ifndef RESTAURANT_H
#define RESTAURANT_H
#include "Location.h"

class Restaurant : public Location {
private:
    string cuisineTag; // เช่น "อาหารไทย", "ปิ้งย่าง"
    double rating;
    string recommendedMenu;

public:
    // Constructor สืบทอดจาก Base Class
    Restaurant(string n, double dist, string tag, double rate, string menu);
    
    // Polymorphism: Override ฟังก์ชันจาก Base Class
    void displayInfo() const override; 
};

#endif