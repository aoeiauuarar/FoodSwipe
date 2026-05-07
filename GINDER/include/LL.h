#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Restaurant.h"

// ============================================================
// Node ของ Doubly Linked List
// มี pointer next (ไปข้างหน้า) และ prev (ย้อนกลับ)
// prev ใช้สำหรับฟีเจอร์ Undo — ถอยกลับไปการ์ดก่อนหน้าได้
// ============================================================
struct Node {
    Restaurant* data;
    Node* next;
    Node* prev;
    Node(Restaurant* r) : data(r), next(nullptr), prev(nullptr) {}
};

// ============================================================
// RestaurantList: Doubly Linked List เก็บร้านอาหารทั้งหมด
// head = ร้านแรกที่แสดง (ใกล้สุดหลัง sort)
// tail = ร้านสุดท้าย (ใช้คืนค่าตอน Undo หลังปัดหมด)
// ============================================================
class RestaurantList {
private:
    Node* head;
    Node* tail;

public:
    RestaurantList();
    ~RestaurantList();

    void addRestaurant(Restaurant* r);
    void sortByDistance(); // เรียงจากใกล้ → ไกล ด้วย Insertion Sort

    Node* getHead() const;
    Node* getTail() const;
};

#endif