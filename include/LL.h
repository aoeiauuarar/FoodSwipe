#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Restaurant.h"

// โครงสร้าง Node ของ Linked List
struct Node {
    Restaurant* data;
    Node* next;
    Node(Restaurant* r) : data(r), next(nullptr) {}
};

class RestaurantList {
private:
    Node* head;

public:
    RestaurantList();
    ~RestaurantList();

    void addRestaurant(Restaurant* r);
    
    // Sorting Algorithm สำหรับ Linked List (Insertion Sort)
    void sortByDistance(); 
    
    Node* getHead() const;
};

#endif