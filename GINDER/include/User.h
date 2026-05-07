#ifndef USER_H
#define USER_H
#include <string>
#include <vector>
#include "Restaurant.h"
using namespace std;

// ============================================================
// Class: User
// เก็บข้อมูลผู้ใช้และรายการร้านที่กด Like ไว้
// ใช้สำหรับฟีเจอร์ Swipe, Undo, และ Group Match
// ============================================================
class User {
private:
    string username;
    vector<Restaurant*> likedRestaurants; // รายการร้านที่ผู้ใช้ปัดขวา (Like)

public:
    User(string name);
    string getName() const;

    void swipeRight(Restaurant* r);                        // เพิ่มร้านเข้า liked list
    void undoSwipe(Restaurant* r);                         // ลบร้านออกจาก liked list (Undo)
    void findMatchWithFriend(const User& friendUser) const; // แสดง Group Match ใน Terminal

    vector<Restaurant*> getLikedRestaurants() const;
};

#endif