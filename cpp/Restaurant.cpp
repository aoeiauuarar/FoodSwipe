#include "Restaurant.h"
#include <iostream>

// โค้ดสีส้ม-ขาว สำหรับ Terminal
#define ORANGE "\033[38;5;208m"
#define WHITE "\033[97m"
#define RESET "\033[0m"

Restaurant::Restaurant(string n, double dist, string tag, double rate, string menu) 
    : Location(n, dist) {
    cuisineTag = tag;
    rating = rate;
    recommendedMenu = menu;
}

void Restaurant::displayInfo() const override {
    cout << ORANGE << "=== " << WHITE << name << ORANGE << " ===" << RESET << "\n";
    cout << "📍 ระยะทาง:   " << distanceKm << " กม.\n";
    cout << "🏷️ แนวอาหาร:  " << cuisineTag << "\n";
    cout << "⭐ คะแนน:     " << rating << "/5.0\n";
    cout << "🍲 เมนูแนะนำ: " << recommendedMenu << "\n";
    cout << ORANGE << "=========================" << RESET << "\n";
}