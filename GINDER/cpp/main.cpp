#include "LL.h"
#include "User.h"
#include "raylib.h"
#include <iostream>
#include <math.h>
using namespace std;

// สีหลักของแอป
const Color APP_ORANGE = {255, 120, 0, 255};
const Color APP_BG = {245, 245, 245, 255};

// สถานะของหน้าจอ
enum AppState { SWIPING, PROFILE, DETAIL, MATCH };

// สถานะของ Animation การปัดการ์ด
enum SwipeAnimState { IDLE, THROWING_RIGHT, THROWING_LEFT, RESETTING };

int main() {
  // ============================================================
  // 1. เตรียมข้อมูลร้านอาหารใน Doubly Linked List
  //    addRestaurant จะเพิ่มที่หัว List ก่อน sort
  // ============================================================
  RestaurantList appData;

  // ร้านดั้งเดิม
  appData.addRestaurant(new Restaurant("Somtum Pa Jua", 5.2, "Esan Food", 4.5,
                                       "Papaya Salad", "images/somtum.png"));
  appData.addRestaurant(new Restaurant("Shabushi Buffet", 2.1, "Buffet", 4.0,
                                       "Sliced Beef", "images/shabu.png"));
  appData.addRestaurant(new Restaurant("Ramen Ichi", 10.5, "Japanese Food", 4.8,
                                       "Tonkotsu Ramen", "images/ramen.png"));
  appData.addRestaurant(new Restaurant("Krapao Ta Pae", 0.8, "Street Food", 4.2,
                                       "Crispy Pork", "images/krapao.png"));

  // ร้านเพิ่มเติม
  appData.addRestaurant(new Restaurant("Moo Ping Jao", 1.5, "Street Food", 4.6,
                                       "Grilled Pork Skewer",
                                       "images/mooping.png"));
  appData.addRestaurant(new Restaurant("Kuay Teow Reua", 3.3, "Noodle", 4.4,
                                       "Boat Noodles", "images/kuayteow.png"));
  appData.addRestaurant(new Restaurant("K-Grill Seoul", 6.7, "Korean Food", 4.7,
                                       "Samgyeopsal", "images/koreanbq.png"));
  appData.addRestaurant(new Restaurant("La Piazza", 8.0, "Italian Food", 4.3,
                                       "Margherita Pizza", "images/pizza.png"));
  appData.addRestaurant(new Restaurant("Baan Mango", 4.1, "Dessert", 4.9,
                                       "Mango Sticky Rice",
                                       "images/mangosticky.png"));
  appData.addRestaurant(new Restaurant("Tim Dim Sum", 7.2, "Chinese Food", 4.5,
                                       "Har Gow & Siu Mai",
                                       "images/dimsum.png"));

  // เรียงร้านจากใกล้ → ไกล ด้วย Insertion Sort ใน Doubly Linked List
  appData.sortByDistance();

  // ============================================================
  // 2. สร้าง User และกำหนดร้านที่ Friend ชอบไว้ล่วงหน้า
  //    (สำหรับ Demo ฟีเจอร์ Group Match)
  // ============================================================
  User me("Me");

  User friendUser("Friend");
  Node *setupNode = appData.getHead();
  while (setupNode != nullptr) {
    // Friend ชอบร้านที่ rating >= 4.4 เพื่อให้มีร้าน Match กับ Me
    if (setupNode->data->getRating() >= 4.4)
      friendUser.swipeRight(setupNode->data);
    setupNode = setupNode->next;
  }

  Node *currentCardNode = appData.getHead(); // การ์ดปัจจุบันที่แสดงอยู่

  // ============================================================
  // 3. เปิดหน้าต่าง Raylib และโหลด Texture ของทุกร้าน
  // ============================================================
  const int screenWidth = 400;
  const int screenHeight = 650;
  InitWindow(screenWidth, screenHeight, "GINDER - DLL & OOP");
  SetTargetFPS(60);

  // โหลด Texture ทุกร้านหลัง InitWindow เสมอ
  Node *temp = appData.getHead();
  while (temp != nullptr) {
    temp->data->loadTexture();
    temp = temp->next;
  }

  // ============================================================
  // 4. Polymorphism Demo
  //    ใช้ Base class pointer (Location*) ชี้ไปที่ Derived object (Restaurant)
  //    เมื่อเรียก virtual function จะรัน version ของ Restaurant จริง (Runtime
  //    Polymorphism)
  // ============================================================
  cout << "\n=== [Polymorphism Demo] ===\n";
  Node *polyTemp = appData.getHead();
  while (polyTemp != nullptr) {
    Location *basePtr = polyTemp->data; // Base pointer ชี้ Derived object
    basePtr->displayInfo(); // เรียก Restaurant::displayInfo() ผ่าน Location*
    basePtr->logInfo();     // เรียก Restaurant::logInfo()     ผ่าน Location*
    polyTemp = polyTemp->next;
  }
  cout << "==========================\n\n";

  // ============================================================
  // 5. ตัวแปรสำหรับ Game Loop
  // ============================================================
  AppState currentState = SWIPING;
  Restaurant *selectedRestaurant = nullptr; // ร้านที่เลือกดู Detail

  // ตำแหน่งและ Animation ของการ์ด Swipe
  const Vector2 idleCardPos = {(float)screenWidth / 2, 310};
  Vector2 cardPos = idleCardPos;
  float cardRotation = 0.0f;
  Vector2 dragOffset = {0, 0};
  bool isDragging = false;
  SwipeAnimState animState = IDLE;
  const float thresholdX = 120.0f; // ระยะปัดขั้นต่ำที่จะ Throw การ์ด

  // ตัวแปร Scroll ของแต่ละหน้า (ค่าลบ = เลื่อนลง)
  float detailScrollY = 0.0f;
  float profileScrollY = 0.0f;
  float matchScrollY = 0.0f;

  // ============================================================
  // 6. Game Loop หลัก
  // ============================================================
  while (!WindowShouldClose()) {
    Vector2 mousePos = GetMousePosition();

    // Bottom Navigation Bar — แบ่ง 3 Tab เท่ากัน
    float tabW = (float)screenWidth / 3.0f;
    Rectangle swipeBtnRect = {0, (float)screenHeight - 70, tabW, 70};
    Rectangle profileBtnRect = {tabW, (float)screenHeight - 70, tabW, 70};
    Rectangle matchBtnRect = {tabW * 2, (float)screenHeight - 70, tabW, 70};
    Rectangle undoBtnRect = {(float)screenWidth - 80, 20, 60, 40};

    // --- Input: Swipe Gesture ---
    if (currentState == SWIPING) {
      if (currentCardNode != nullptr && animState == IDLE) {
        Rectangle cardRect = {cardPos.x - 170, cardPos.y - 200, 340, 400};

        // ตรวจจับการกดค้างเพื่อลาก
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(mousePos, cardRect)) {
          isDragging = true;
          dragOffset = {mousePos.x - cardPos.x, mousePos.y - cardPos.y};
        }

        // ตอนลาก อัปเดตตำแหน่งและหมุนการ์ดตามการเคลื่อนที่
        if (isDragging) {
          cardPos.x = mousePos.x - dragOffset.x;
          float deltaX = cardPos.x - idleCardPos.x;
          cardRotation = (deltaX / screenWidth) * 40.0f;

          if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            isDragging = false;
            if (deltaX > thresholdX)
              animState = THROWING_RIGHT;
            else if (deltaX < -thresholdX)
              animState = THROWING_LEFT;
            else
              animState = RESETTING;
          }
        }
      }

      // --- Animation: Throw / Reset ---
      const float throwSpeed = 30.0f;
      const float resetSpeed = 0.15f;

      if (animState == THROWING_RIGHT) {
        cardPos.x += throwSpeed;
        if (cardPos.x > screenWidth + 200) {
          me.swipeRight(currentCardNode->data); // บันทึกว่า Like ร้านนี้
          currentCardNode = currentCardNode->next;
          cardPos = idleCardPos;
          cardRotation = 0;
          animState = IDLE;
        }
      } else if (animState == THROWING_LEFT) {
        cardPos.x -= throwSpeed;
        if (cardPos.x < -200) {
          currentCardNode = currentCardNode->next; // ข้ามร้านนี้
          cardPos = idleCardPos;
          cardRotation = 0;
          animState = IDLE;
        }
      } else if (animState == RESETTING) {
        // เด้งการ์ดกลับตำแหน่งกลางด้วย Lerp
        cardPos.x += (idleCardPos.x - cardPos.x) * resetSpeed;
        cardRotation += (0 - cardRotation) * resetSpeed;
        if (fabs(cardPos.x - idleCardPos.x) < 1.0f) {
          cardPos = idleCardPos;
          cardRotation = 0;
          animState = IDLE;
        }
      }
    }

    // --- Input: Tab Switching และ Undo ---
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      if (CheckCollisionPointRec(mousePos, swipeBtnRect)) {
        currentState = SWIPING;
      }
      if (CheckCollisionPointRec(mousePos, profileBtnRect)) {
        currentState = PROFILE;
        profileScrollY = 0.0f;
      }
      if (CheckCollisionPointRec(mousePos, matchBtnRect)) {
        currentState = MATCH;
        matchScrollY = 0.0f;
      }

      // Undo: ย้อนการ์ดกลับ 1 ใบโดยใช้ pointer prev ของ Doubly Linked List
      if (currentState == SWIPING &&
          CheckCollisionPointRec(mousePos, undoBtnRect)) {
        if (currentCardNode != nullptr && currentCardNode->prev != nullptr) {
          currentCardNode = currentCardNode->prev;
          me.undoSwipe(currentCardNode->data); // ยกเลิก Like ถ้าเคยปัดขวาไป
        } else if (currentCardNode == nullptr && appData.getTail() != nullptr) {
          // กรณีปัดจนหมด ดึงการ์ดใบสุดท้ายกลับมาจาก tail
          currentCardNode = appData.getTail();
          me.undoSwipe(currentCardNode->data);
        }
      }
    }

    // ============================================================
    // 7. วาดหน้าจอ
    // ============================================================
    BeginDrawing();
    ClearBackground(APP_BG);

    // Header Bar
    DrawRectangle(0, 0, screenWidth, 80, APP_ORANGE);
    DrawText("GINDER", 20, 25, 30, WHITE);

    // ==================== หน้า SWIPE ====================
    if (currentState == SWIPING) {
      // ปุ่ม Undo มุมบนขวา
      DrawRectangleRounded(undoBtnRect, 0.2f, 10, WHITE);
      DrawText("UNDO", screenWidth - 70, 32, 16, APP_ORANGE);

      if (currentCardNode != nullptr) {
        Restaurant *res = currentCardNode->data;

        // วาดการ์ดสีขาว (หมุนตามการลาก)
        Rectangle cardDest = {cardPos.x, cardPos.y, 340, 400};
        Vector2 cardOrigin = {170, 200};
        DrawRectanglePro(cardDest, cardOrigin, cardRotation, WHITE);

        // วาดรูปอาหารบนการ์ด
        Texture2D tex = res->getTexture();
        Rectangle sourceRec = {0, 0, (float)tex.width, (float)tex.height};
        Rectangle imgDest = {cardPos.x, cardPos.y - 100, 340, 200};
        Vector2 imgOrigin = {170, 100};
        DrawTexturePro(tex, sourceRec, imgDest, imgOrigin, cardRotation, WHITE);

        // วาดข้อความบนการ์ด — ชิดใต้รูปภาพ
        DrawTextPro(GetFontDefault(), res->getName().c_str(),
                    {cardPos.x - 150, cardPos.y + 12}, {0, 0}, cardRotation, 22,
                    2, DARKGRAY);
        DrawTextPro(GetFontDefault(), TextFormat("%.1f km", res->getDistance()),
                    {cardPos.x - 150, cardPos.y + 40}, {0, 0}, cardRotation, 17,
                    2, GRAY);
        DrawTextPro(GetFontDefault(), res->getTag().c_str(),
                    {cardPos.x - 150, cardPos.y + 61}, {0, 0}, cardRotation, 17,
                    2, ORANGE);
        DrawTextPro(GetFontDefault(),
                    TextFormat("Rating: %.1f / 5.0", res->getRating()),
                    {cardPos.x - 150, cardPos.y + 80}, {0, 0}, cardRotation, 17,
                    2, APP_ORANGE);
      } else {
        // ปัดการ์ดหมดแล้ว
        const char *noMoreText = "No more restaurants!";
        const char *undoText = "Click 'UNDO' to go back.";
        DrawText(noMoreText, (screenWidth - MeasureText(noMoreText, 24)) / 2,
                 300, 24, GRAY);
        DrawText(undoText, (screenWidth - MeasureText(undoText, 18)) / 2, 340,
                 18, LIGHTGRAY);
      }
    }

    // ==================== หน้า PROFILE ====================
    else if (currentState == PROFILE) {
      DrawText("Your Liked Food", 110, 100, 24, DARKGRAY);
      vector<Restaurant *> liked = me.getLikedRestaurants();

      if (liked.empty()) {
        DrawText("You haven't liked any food yet.", 50, 300, 20, LIGHTGRAY);
      } else {
        // คำนวณ Scroll range จากจำนวนรายการ
        profileScrollY += GetMouseWheelMove() * 30.0f;
        if (profileScrollY > 0)
          profileScrollY = 0;
        float profileMinY =
            -((float)liked.size() * 80.0f - (screenHeight - 150 - 70));
        if (profileMinY > 0)
          profileMinY = 0;
        if (profileScrollY < profileMinY)
          profileScrollY = profileMinY;

        // ScissorMode: ป้องกัน item วาดทับ header และ footer
        BeginScissorMode(0, 140, screenWidth, screenHeight - 140 - 70);
        for (int i = 0; i < (int)liked.size(); i++) {
          float ry = 150.0f + (i * 80) + profileScrollY;
          Rectangle itemRect = {30, ry, 340, 70};
          DrawRectangleRec(itemRect, WHITE);
          DrawRectangleLinesEx(itemRect, 1, LIGHTGRAY);
          DrawText(liked[i]->getName().c_str(), 50, (int)ry + 25, 20, DARKGRAY);
          DrawText(TextFormat("%.1f km", liked[i]->getDistance()), 280,
                   (int)ry + 25, 16, GRAY);

          // กด item เพื่อดูหน้า Detail
          if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
              CheckCollisionPointRec(mousePos, itemRect)) {
            selectedRestaurant = liked[i];
            currentState = DETAIL;
          }
        }
        EndScissorMode();
      }
    }

    // ==================== หน้า DETAIL ====================
    else if (currentState == DETAIL) {
      if (selectedRestaurant != nullptr) {
        // รับ Scroll จาก Mouse Wheel
        detailScrollY += GetMouseWheelMove() * 30.0f;
        if (detailScrollY > 0)
          detailScrollY = 0;
        if (detailScrollY < -100)
          detailScrollY = -100;

        // ปุ่ม Back (วาดนอก Scissor เพื่อให้ติดหน้าจอ ไม่เลื่อนตาม)
        DrawRectangleRec({10, 90, 80, 35}, LIGHTGRAY);
        DrawText("< Back", 20, 100, 18, DARKGRAY);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(mousePos, {10, 90, 80, 35})) {
          currentState = PROFILE;
          detailScrollY = 0.0f; // รีเซ็ต scroll เมื่อกลับ
        }

        // ScissorMode: เนื้อหาเลื่อนได้ภายในกรอบ
        BeginScissorMode(0, 130, screenWidth, screenHeight - 130 - 70);
        DrawTexturePro(selectedRestaurant->getTexture(),
                       {0, 0, (float)selectedRestaurant->getTexture().width,
                        (float)selectedRestaurant->getTexture().height},
                       {50, 150 + detailScrollY, 300, 300}, {0, 0}, 0, WHITE);
        DrawText(selectedRestaurant->getName().c_str(), 50, 465 + detailScrollY,
                 30, DARKGRAY);
        DrawText(
            TextFormat("Rating: %.1f / 5.0", selectedRestaurant->getRating()),
            50, 505 + detailScrollY, 20, APP_ORANGE);
        DrawText(TextFormat("Tag: %s", selectedRestaurant->getTag().c_str()),
                 50, 535 + detailScrollY, 20, GRAY);
        DrawText(TextFormat("Menu: %s", selectedRestaurant->getMenu().c_str()),
                 50, 565 + detailScrollY, 20, GRAY);
        EndScissorMode();
      }
    }

    // ==================== หน้า MATCH ====================
    if (currentState == MATCH) {
      // Header (fixed — ไม่เลื่อนตาม scroll)
      DrawText("Group Match", 120, 95, 26, DARKGRAY);
      DrawLine(30, 130, screenWidth - 30, 130, LIGHTGRAY);

      vector<Restaurant *> myLikes = me.getLikedRestaurants();
      vector<Restaurant *> friendLikes = friendUser.getLikedRestaurants();

      // หาร้านที่ทั้งสอง User Like ตรงกัน
      vector<Restaurant *> matches;
      for (Restaurant *myR : myLikes)
        for (Restaurant *frR : friendLikes)
          if (myR->getName() == frR->getName())
            matches.push_back(myR);

      DrawText(TextFormat("You liked: %d  |  Friend liked: %d",
                          (int)myLikes.size(), (int)friendLikes.size()),
               20, 140, 17, GRAY);

      if (matches.empty()) {
        const char *noMatchText = "No match yet!";
        const char *swipeMoreText = "Swipe more to find a match :)";
        DrawText(noMatchText, (screenWidth - MeasureText(noMatchText, 24)) / 2,
                 300, 24, GRAY);
        DrawText(swipeMoreText,
                 (screenWidth - MeasureText(swipeMoreText, 18)) / 2, 340, 18,
                 LIGHTGRAY);
      } else {
        DrawText(TextFormat("Matched: %d restaurant(s)!", (int)matches.size()),
                 20, 165, 20, APP_ORANGE);
        DrawLine(30, 190, screenWidth - 30, 190, LIGHTGRAY);

        // คำนวณ Scroll range จากจำนวนรายการ Match
        matchScrollY += GetMouseWheelMove() * 30.0f;
        if (matchScrollY > 0)
          matchScrollY = 0;
        float matchMinY =
            -((float)matches.size() * 90.0f - (screenHeight - 200 - 70));
        if (matchMinY > 0)
          matchMinY = 0;
        if (matchScrollY < matchMinY)
          matchScrollY = matchMinY;

        // ScissorMode: ป้องกัน item ล้น footer
        BeginScissorMode(0, 195, screenWidth, screenHeight - 195 - 70);
        for (int i = 0; i < (int)matches.size(); i++) {
          float rowY = 200.0f + i * 90.0f + matchScrollY;

          // กรอบแต่ละร้าน
          DrawRectangleRounded({20, rowY, (float)screenWidth - 40, 80}, 0.15f,
                               8, WHITE);
          DrawRectangleLinesEx({20, rowY, (float)screenWidth - 40, 80}, 1.5f,
                               APP_ORANGE);

          // รูปย่อ (Thumbnail)
          Texture2D thumb = matches[i]->getTexture();
          DrawTexturePro(thumb, {0, 0, (float)thumb.width, (float)thumb.height},
                         {30, rowY + 10, 60, 60}, {0, 0}, 0, WHITE);

          // ข้อมูลร้าน
          DrawText(matches[i]->getName().c_str(), 100, (int)rowY + 12, 20,
                   DARKGRAY);
          DrawText(TextFormat("Rating: %.1f  |  %.1f km",
                              matches[i]->getRating(),
                              matches[i]->getDistance()),
                   100, (int)rowY + 38, 16, GRAY);
          DrawText(matches[i]->getTag().c_str(), 100, (int)rowY + 58, 15,
                   APP_ORANGE);
        }
        EndScissorMode();
      }
    }

    // ==================== Bottom Navigation Bar ====================
    DrawRectangle(0, screenHeight - 70, screenWidth, 70, WHITE);
    DrawLine(0, screenHeight - 71, screenWidth, screenHeight - 71, LIGHTGRAY);
    DrawRectangleLinesEx(swipeBtnRect, 1, LIGHTGRAY);
    DrawRectangleLinesEx(profileBtnRect, 1, LIGHTGRAY);
    DrawRectangleLinesEx(matchBtnRect, 1, LIGHTGRAY);

    Color swipeColor = (currentState == SWIPING) ? APP_ORANGE : DARKGRAY;
    Color profileColor = (currentState == PROFILE || currentState == DETAIL)
                             ? APP_ORANGE
                             : DARKGRAY;
    Color matchColor = (currentState == MATCH) ? APP_ORANGE : DARKGRAY;

    DrawText("SWIPE", (int)(tabW / 2) - 30, screenHeight - 45, 18, swipeColor);
    DrawText("PROFILE", (int)(tabW + tabW / 2) - 42, screenHeight - 45, 18,
             profileColor);
    DrawText("MATCH", (int)(tabW * 2 + tabW / 2) - 33, screenHeight - 45, 18,
             matchColor);

    EndDrawing();
  }

  // 8. Cleanup: คืน Memory Texture และปิดหน้าต่าง

  temp = appData.getHead();
  while (temp != nullptr) {
    temp->data->unloadTexture();
    temp = temp->next;
  }
  CloseWindow();
  return 0;
}