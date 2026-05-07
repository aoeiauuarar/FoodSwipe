#include "User.h"
#include <iostream>

#define TERM_ORANGE "\033[38;5;208m"
#define TERM_RESET  "\033[0m"

User::User(string name) { username = name; }

string User::getName() const { return username; }

// บันทึกร้านที่ผู้ใช้ปัดขวาลงใน liked list
void User::swipeRight(Restaurant* r) {
    likedRestaurants.push_back(r);
}

// ลบร้านออกจาก liked list — ใช้เมื่อผู้ใช้กด Undo
void User::undoSwipe(Restaurant* r) {
    for (auto it = likedRestaurants.begin(); it != likedRestaurants.end(); ++it) {
        if (*it == r) {
            likedRestaurants.erase(it);
            break;
        }
    }
}

vector<Restaurant*> User::getLikedRestaurants() const {
    return likedRestaurants;
}

// แสดงร้านที่ทั้งสอง User ชอบตรงกัน (Group Match Feature) ใน Terminal
void User::findMatchWithFriend(const User& friendUser) const {
    cout << "\n" << TERM_ORANGE
         << "=== Group Match: " << username << " & " << friendUser.getName() << " ==="
         << TERM_RESET << "\n";
    bool foundMatch = false;
    for (Restaurant* myRest : likedRestaurants) {
        for (Restaurant* friendRest : friendUser.getLikedRestaurants()) {
            if (myRest->getName() == friendRest->getName()) {
                cout << "  [Match] " << myRest->getName() << "\n";
                foundMatch = true;
            }
        }
    }
    if (!foundMatch) cout << "  ยังไม่มีร้านที่ใจตรงกัน\n";
}