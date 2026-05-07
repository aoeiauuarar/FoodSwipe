#ifndef RESTAURANT_H
#define RESTAURANT_H
#include "Location.h"
#include "raylib.h"

// ============================================================
// Derived Class: Restaurant (สืบทอดจาก Location)
// เก็บข้อมูลครบของร้านอาหาร และ Texture สำหรับแสดงผลด้วย Raylib
// ============================================================
class Restaurant : public Location {
private:
    string cuisineTag;      // ประเภทอาหาร เช่น "Thai", "Japanese"
    double rating;          // คะแนนร้าน (0.0 - 5.0)
    string recommendedMenu; // เมนูแนะนำ
    string imagePath;       // พาธไฟล์รูปภาพ
    Texture2D texture;      // Texture ใน GPU (โหลดหลัง InitWindow)

public:
    Restaurant(string n, double dist, string tag, double rate, string menu, string img);

    // Polymorphism: Override virtual functions จาก Location
    void displayInfo() const override;
    void logInfo()     const override;

    // จัดการ Texture แยกออกจาก Constructor
    // เพราะต้องเรียก InitWindow() ก่อน จึงจะโหลด Texture ได้
    void      loadTexture();
    void      unloadTexture();
    Texture2D getTexture() const;

    // Getters สำหรับดึงข้อมูลไปใช้วาดหน้าจอ
    string getTag()    const;
    double getRating() const;
    string getMenu()   const;
};

#endif