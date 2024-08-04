# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -I../vcpkg/installed/x64-windows/include -I../fast-cpp-csv-parser


# Source files and target
SRC := src/main.cpp src/AVL.cpp src/Car.cpp
TARGET := my_backend

# Libraries
LIBS := -lcpprest -lboost_system -lssl -lcrypto
LDFLAGS := -L../vcpkg/installed/x64-windows/lib

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Clean up build files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean

