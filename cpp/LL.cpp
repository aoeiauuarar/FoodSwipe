#include "LL.h"

RestaurantList::RestaurantList() { head = nullptr; }

RestaurantList::~RestaurantList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void RestaurantList::addRestaurant(Restaurant* r) {
    Node* newNode = new Node(r);
    newNode->next = head;
    head = newNode;
}

// เรียงลำดับร้านอาหารตามระยะทาง (ใกล้ไปไกล) ด้วย Insertion Sort
void RestaurantList::sortByDistance() {
    if (!head || !head->next) return;

    Node* sorted = nullptr;
    Node* current = head;

    while (current != nullptr) {
        Node* next = current->next;

        if (sorted == nullptr || sorted->data->getDistance() >= current->data->getDistance()) {
            current->next = sorted;
            sorted = current;
        } else {
            Node* temp = sorted;
            while (temp->next != nullptr && temp->next->data->getDistance() < current->data->getDistance()) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

Node* RestaurantList::getHead() const { return head; }