#include "LL.h"

RestaurantList::RestaurantList() {
  head = nullptr;
  tail = nullptr;
}

// Destructor: คืน Memory ทั้ง Node และ Restaurant object ที่สร้างด้วย new
RestaurantList::~RestaurantList() {
  Node *current = head;
  while (current != nullptr) {
    Node *next = current->next;
    delete current->data; // คืน Memory ของ Restaurant
    delete current;       // คืน Memory ของ Node
    current = next;
  }
}

// เพิ่มร้านที่หัว List (prepend) พร้อมอัปเดต prev และ tail
void RestaurantList::addRestaurant(Restaurant *r) {
  Node *newNode = new Node(r);
  if (head == nullptr) {
    head = tail = newNode;
  } else {
    newNode->next = head;
    head->prev = newNode;
    head = newNode;
  }
}

// Insertion Sort: เรียงร้านจากระยะใกล้ → ไกล
// พร้อมอัปเดต pointer prev ทุกโหนดให้ถูกต้องตาม Doubly Linked List
void RestaurantList::sortByDistance() {
  if (!head || !head->next)
    return;

  Node *sorted = nullptr;
  Node *current = head;

  while (current != nullptr) {
    Node *next = current->next;
    current->prev = current->next = nullptr; // ตัดสาย pointer ชั่วคราวก่อนย้าย

    if (sorted == nullptr) {
      sorted = current;
    } else if (sorted->data->getDistance() >= current->data->getDistance()) {
      // แทรกที่หัว sorted list
      current->next = sorted;
      sorted->prev = current;
      sorted = current;
    } else {
      // หาตำแหน่งที่เหมาะสมแล้วแทรก
      Node *temp = sorted;
      while (temp->next != nullptr &&
             temp->next->data->getDistance() < current->data->getDistance()) {
        temp = temp->next;
      }
      current->next = temp->next;
      if (temp->next != nullptr)
        temp->next->prev = current;
      temp->next = current;
      current->prev = temp;
    }
    current = next;
  }
  head = sorted;

  // อัปเดต tail ให้ชี้ไปโหนดสุดท้ายหลัง sort เสร็จ
  Node *tempTail = head;
  while (tempTail && tempTail->next)
    tempTail = tempTail->next;
  tail = tempTail;
}

Node *RestaurantList::getHead() const { return head; }
Node *RestaurantList::getTail() const { return tail; }