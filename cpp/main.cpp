#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#include <math.h> 

using namespace std;

// ==========================================
// ส่วนที่ 1: การจัดการข้อมูล (ครบตาม Requirements)
// ==========================================

class Location {
protected:
    string name;
    double distanceKm;
public:
    Location(string n, double dist) : name(n), distanceKm(dist) {}
    virtual ~Location() {}
    double getDistance() const { return distanceKm; }
    string getName() const { return name; }
    virtual void logInfo() const { cout << "[Location] " << name << endl; }
};

class Restaurant : public Location {
private:
    string cuisineTag;
    double rating;
    string recommendedMenu;
    string imagePath;
    Texture2D texture; 
public:
    Restaurant(string n, double dist, string tag, double rate, string menu, string img) 
        : Location(n, dist), cuisineTag(tag), rating(rate), recommendedMenu(menu), imagePath(img) {
        texture = { 0, 0, 0, 0, 0 }; 
    }
    
    void loadTexture() { texture = LoadTexture(imagePath.c_str()); }
    void unloadTexture() { UnloadTexture(texture); }
    
    Texture2D getTexture() const { return texture; }
    string getTag() const { return cuisineTag; }
    double getRating() const { return rating; }
    string getMenu() const { return recommendedMenu; }

    void logInfo() const override { cout << "[Restaurant] " << name << " Tag:" << cuisineTag << endl; }
};

struct Node {
    Restaurant* data;
    Node* next;
    Node(Restaurant* r) : data(r), next(nullptr) {}
};

class RestaurantList {
private:
    Node* head;
public:
    RestaurantList() { head = nullptr; }
    void addRestaurant(Restaurant* r) {
        Node* newNode = new Node(r);
        newNode->next = head;
        head = newNode;
    }
    void sortByDistance() {
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
    Node* getHead() const { return head; }
};

class User {
private:
    string username;
    vector<Restaurant*> likedRestaurants; 
public:
    User(string name) : username(name) {}
    void swipeRight(Restaurant* r) { likedRestaurants.push_back(r); }
    vector<Restaurant*> getLikedRestaurants() const { return likedRestaurants; }
};

// ==========================================
// ส่วนที่ 2: Raylib (UX/UI & Animations)
// ==========================================

// 🛠️ FIX 1: เปลี่ยนจาก #define เป็น const Color เพื่อไม่ให้ C++ สับสนตอนประมวลผล
const Color APP_ORANGE = { 255, 120, 0, 255 };
const Color APP_BG = { 245, 245, 245, 255 };

enum AppState { SWIPING, PROFILE, DETAIL };
enum SwipeAnimState { IDLE, THROWING_RIGHT, THROWING_LEFT, RESETTING };

int main() {
    RestaurantList appData;
    appData.addRestaurant(new Restaurant("Somtum Pa Jua", 5.2, "Esan Food", 4.5, "Papaya Salad", "images/somtum.png"));
    appData.addRestaurant(new Restaurant("Shabushi Buffet", 2.1, "Buffet", 4.0, "Sliced Beef", "images/shabu.png"));
    appData.addRestaurant(new Restaurant("Ramen ", 10.5, "Japanese Food", 4.8, "Tonkotsu Ramen", "images/ramen.png"));
    appData.addRestaurant(new Restaurant("Krapao Ta Pae", 0.8, "Street Food", 4.2, "Crispy Pork", "images/krapao.png"));
    
    appData.sortByDistance();

    User me("คุณ");
    Node* currentCardNode = appData.getHead();

    const int screenWidth = 400;
    const int screenHeight = 650;
    InitWindow(screenWidth, screenHeight, "FoodSwipe MVP - By You");
    SetTargetFPS(60);

    Node* temp = appData.getHead();
    while (temp != nullptr) {
        temp->data->loadTexture();
        temp = temp->next;
    }

    AppState currentState = SWIPING;
    Restaurant* selectedRestaurant = nullptr; 

    const Vector2 idleCardPos = { (float)screenWidth/2, 310 };
    Vector2 cardPos = idleCardPos;
    float cardRotation = 0.0f;
    Vector2 dragOffset = { 0, 0 };
    bool isDragging = false;
    SwipeAnimState animState = IDLE;
    const float thresholdX = 120.0f; 

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        if (currentState == SWIPING) {
            if (currentCardNode != nullptr && animState == IDLE) {
                Rectangle cardRect = { cardPos.x - 170, cardPos.y - 200, 340, 400 };

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, cardRect)) {
                    isDragging = true;
                    dragOffset = { mousePos.x - cardPos.x, mousePos.y - cardPos.y };
                }

                if (isDragging) {
                    cardPos.x = mousePos.x - dragOffset.x;
                    float deltaX = cardPos.x - idleCardPos.x;
                    cardRotation = (deltaX / screenWidth) * 40.0f; 

                    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                        isDragging = false;
                        if (deltaX > thresholdX) {
                            animState = THROWING_RIGHT; 
                        } else if (deltaX < -thresholdX) {
                            animState = THROWING_LEFT; 
                        } else {
                            animState = RESETTING; 
                        }
                    }
                }
            }

            const float throwSpeed = 30.0f;
            const float resetSpeed = 0.15f; 

            if (animState == THROWING_RIGHT) {
                cardPos.x += throwSpeed; 
                if (cardPos.x > screenWidth + 200) {
                    me.swipeRight(currentCardNode->data); 
                    currentCardNode = currentCardNode->next; 
                    cardPos = idleCardPos;
                    cardRotation = 0;
                    animState = IDLE;
                }
            } else if (animState == THROWING_LEFT) {
                cardPos.x -= throwSpeed; 
                if (cardPos.x < -200) {
                    currentCardNode = currentCardNode->next; 
                    cardPos = idleCardPos;
                    cardRotation = 0;
                    animState = IDLE;
                }
            } else if (animState == RESETTING) {
                cardPos.x += (idleCardPos.x - cardPos.x) * resetSpeed;
                cardRotation += (0 - cardRotation) * resetSpeed;
                
                if (fabs(cardPos.x - idleCardPos.x) < 1.0f) {
                    cardPos = idleCardPos;
                    cardRotation = 0;
                    animState = IDLE;
                }
            }
        }
        
        Rectangle swipeBtnRect = { 0, (float)screenHeight - 70, (float)screenWidth/2, 70 };
        Rectangle profileBtnRect = { (float)screenWidth/2, (float)screenHeight - 70, (float)screenWidth/2, 70 };
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePos, swipeBtnRect)) currentState = SWIPING;
            if (CheckCollisionPointRec(mousePos, profileBtnRect)) currentState = PROFILE;
        }

        BeginDrawing();
        ClearBackground(APP_BG);

        DrawRectangle(0, 0, screenWidth, 80, APP_ORANGE);
        DrawText("FoodSwipe", 120, 25, 30, WHITE);

        if (currentState == SWIPING) {
            if (currentCardNode != nullptr) {
                Restaurant* res = currentCardNode->data;
                Rectangle cardDest = { cardPos.x, cardPos.y, 340, 400 };
                Vector2 cardOrigin = { 170, 200 }; 

                DrawRectanglePro(cardDest, cardOrigin, cardRotation, WHITE);
                
                Texture2D tex = res->getTexture();
                Rectangle sourceRec = { 0, 0, (float)tex.width, (float)tex.height };
                Rectangle imgDest = { cardPos.x, cardPos.y - 100, 340, 200 };
                Vector2 imgOrigin = { 170, 100 };
                DrawTexturePro(tex, sourceRec, imgDest, imgOrigin, cardRotation, WHITE);

                DrawTextPro(GetFontDefault(), res->getName().c_str(), { cardPos.x - 150, cardPos.y + 110 }, { 0, 0 }, cardRotation, 28, 2, DARKGRAY);
                DrawTextPro(GetFontDefault(), TextFormat("%.1f km", res->getDistance()), { cardPos.x - 150, cardPos.y + 145 }, { 0, 0 }, cardRotation, 20, 2, GRAY);
                DrawTextPro(GetFontDefault(), res->getTag().c_str(), { cardPos.x - 150, cardPos.y + 170 }, { 0, 0 }, cardRotation, 20, 2, ORANGE);

                // 🛠️ FIX 2: เอาเส้นขอบออกไปเลยเพราะพื้นหลังสีขาวของการ์ดสวยพอแล้ว และแก้ปัญหา Scope Error 

            } else {
                DrawText("🎉 No more restaurants!", 80, 300, 24, GRAY);
                DrawText("Check terminal for logic log.", 70, 350, 18, LIGHTGRAY);
            }
        } 
        else if (currentState == PROFILE) {
            DrawText("Your Liked Food", 110, 100, 24, DARKGRAY);
            vector<Restaurant*> liked = me.getLikedRestaurants();
            
            if (liked.empty()) {
                DrawText("You haven't liked any food yet.", 50, 300, 20, LIGHTGRAY);
            } else {
                for (int i = 0; i < liked.size(); i++) {
                    Rectangle itemRect = { 30, (float)150 + (i * 80), 340, 70 };
                    DrawRectangleRec(itemRect, WHITE);
                    DrawRectangleLinesEx(itemRect, 1, LIGHTGRAY);
                    
                    DrawText(liked[i]->getName().c_str(), 50, 175 + (i * 80), 20, DARKGRAY);
                    DrawText(TextFormat("%.1f km", liked[i]->getDistance()), 280, 175 + (i * 80), 16, GRAY);
                    
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, itemRect)) {
                        selectedRestaurant = liked[i];
                        currentState = DETAIL;
                    }
                }
            }
        }
        else if (currentState == DETAIL) {
            if (selectedRestaurant != nullptr) {
                DrawRectangleRec({ 10, 90, 80, 35 }, LIGHTGRAY);
                DrawText("< Back", 20, 100, 18, DARKGRAY);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, { 10, 90, 80, 35 })) {
                    currentState = PROFILE;
                }

                DrawTexturePro(selectedRestaurant->getTexture(), { 0, 0, (float)selectedRestaurant->getTexture().width, (float)selectedRestaurant->getTexture().height }, { 50, 150, 300, 300 }, { 0, 0 }, 0, WHITE);
                
                DrawText(selectedRestaurant->getName().c_str(), 50, 470, 30, DARKGRAY);
                DrawText(TextFormat("🏷️ Tag: %s", selectedRestaurant->getTag().c_str()), 50, 510, 20, GRAY);
                DrawText(TextFormat("🥘 Menu: %s", selectedRestaurant->getMenu().c_str()), 50, 540, 20, GRAY);
                DrawText(TextFormat("⭐ Rating: %.1f / 5.0", selectedRestaurant->getRating()), 200, 480, 20, ORANGE);
            }
        }

        DrawRectangle(0, screenHeight - 70, screenWidth, 70, WHITE);
        DrawLine(0, screenHeight - 71, screenWidth, screenHeight - 71, LIGHTGRAY);
        
        Color swipeColor = (currentState == SWIPING) ? APP_ORANGE : DARKGRAY;
        DrawText("SWIPE", 65, screenHeight - 45, 20, swipeColor);
        DrawRectangleLinesEx(swipeBtnRect, 1, LIGHTGRAY);
        
        Color profileColor = (currentState == PROFILE || currentState == DETAIL) ? APP_ORANGE : DARKGRAY;
        DrawText("PROFILE", 260, screenHeight - 45, 20, profileColor);
        DrawRectangleLinesEx(profileBtnRect, 1, LIGHTGRAY);

        EndDrawing();
    }

    temp = appData.getHead();
    while (temp != nullptr) {
        temp->data->unloadTexture();
        temp = temp->next;
    }
    CloseWindow();
    return 0;
}