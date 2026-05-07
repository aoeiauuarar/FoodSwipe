#include "Restaurant.h"
#include <iostream>

// สีสำหรับแต่งข้อความใน Terminal
#define TERM_ORANGE "\033[38;5;208m"
#define TERM_WHITE  "\033[97m"
#define TERM_RESET  "\033[0m"

// Constructor: รับข้อมูลร้านทั้งหมด, กำหนด texture เป็นค่าว่างก่อน (โหลดทีหลัง)
Restaurant::Restaurant(string n, double dist, string tag, double rate, string menu, string img)
    : Location(n, dist) {
    cuisineTag      = tag;
    rating          = rate;
    recommendedMenu = menu;
    imagePath       = img;
    texture = { 0, 0, 0, 0, 0 };
}

// Override: แสดงข้อมูลร้านละเอียดใน Terminal (Polymorphism)
void Restaurant::displayInfo() const {
    cout << TERM_ORANGE << "=== " << TERM_WHITE << name << TERM_ORANGE << " ===" << TERM_RESET << "\n";
    cout << "distance:   "        << distanceKm    << " กม.\n";
    cout << "Cuisine:  "          << cuisineTag     << "\n";
    cout << "Rating:     "        << rating         << "/5.0\n";
    cout << "Recommended Menu: "  << recommendedMenu<< "\n";
    cout << TERM_ORANGE << "=========================" << TERM_RESET << "\n";
}

// Override: บันทึก log สั้นๆ ของร้าน (Polymorphism)
void Restaurant::logInfo() const {
    cout << "[Restaurant] " << name << " Tag:" << cuisineTag << endl;
}

// โหลด Texture จากไฟล์รูป (ต้องเรียกหลัง InitWindow เสมอ)
void Restaurant::loadTexture()  { texture = LoadTexture(imagePath.c_str()); }

// คืน Memory ของ Texture ใน GPU ก่อนปิดโปรแกรม
void Restaurant::unloadTexture(){ UnloadTexture(texture); }

Texture2D Restaurant::getTexture() const { return texture; }
string    Restaurant::getTag()     const { return cuisineTag; }
double    Restaurant::getRating()  const { return rating; }
string    Restaurant::getMenu()    const { return recommendedMenu; }