# ============================================================
# Makefile สำหรับ GINDER (macOS)
# วิธีใช้:
#   make        → Compile และสร้างไฟล์ ./GINDER
#   make run    → Compile แล้วรันทันที
#   make clean  → ลบไฟล์ที่ build ไว้
# ============================================================

TARGET  = GINDER
CXX     = g++
CXXFLAGS = -std=c++17 -Wall

# Paths
RAYLIB_INC = /opt/homebrew/Cellar/raylib/5.5/include
RAYLIB_LIB = /opt/homebrew/Cellar/raylib/5.5/lib

INCLUDES = -I include -I $(RAYLIB_INC)
LIBS     = $(RAYLIB_LIB)/libraylib.a \
           -framework OpenGL -framework Cocoa -framework IOKit \
           -framework CoreAudio -framework CoreVideo

SRCS = cpp/main.cpp cpp/LL.cpp cpp/Restaurant.cpp cpp/User.cpp cpp/Location.cpp

# ============================================================
# Rules
# ============================================================

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) $(INCLUDES) $(LIBS) -o $(TARGET)
	@echo "Build complete → ./$(TARGET)"

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
