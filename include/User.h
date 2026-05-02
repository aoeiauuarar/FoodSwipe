#ifndef USER_H
#define USER_H
#include <string>
#include <vector>
#include "Restaurant.h"

using namespace std;

class User {
private:
    string username;
    vector<Restaurant*> likedRestaurants; // เก็บร้านที่ปัดขวา (ชอบ)

public:
    User(string name);
    string getName() const;
    void swipeRight(Restaurant* r);
    
    // ฟังก์ชันหาร้านที่ใจตรงกันกับเพื่อน (Group Feature)
    void findMatchWithFriend(const User& friendUser) const;
    vector<Restaurant*> getLikedRestaurants() const;
};

#endif