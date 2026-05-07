#include "User.h"
#include <iostream>

#define ORANGE "\033[38;5;208m"
#define RESET "\033[0m"

User::User(string name) { username = name; }

string User::getName() const { return username; }

void User::swipeRight(Restaurant* r) {
    likedRestaurants.push_back(r);
}

vector<Restaurant*> User::getLikedRestaurants() const {
    return likedRestaurants;
}

void User::findMatchWithFriend(const User& friendUser) const {
    cout << "\n" << ORANGE << "🔥 ร้านที่ " << username << " และ " << friendUser.getName() << " ใจตรงกัน! 🔥" << RESET << "\n";
    bool foundMatch = false;
    
    for (Restaurant* myRest : likedRestaurants) {
        for (Restaurant* friendRest : friendUser.getLikedRestaurants()) {
            if (myRest->getName() == friendRest->getName()) {
                cout << "❤️ " << myRest->getName() << "\n";
                foundMatch = true;
            }
        }
    }
    if (!foundMatch) cout << "ยังไม่มีร้านที่ใจตรงกันเลย ลองปัดเพิ่มนะ!\n";
}