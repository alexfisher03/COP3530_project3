# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -I../fast-cpp-csv-parser

# Source files and target
SRC := src/main.cpp src/AVL.cpp src/Car.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := car_cache

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) -lpthread

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJ)

# Phony targets
.PHONY: all clean
