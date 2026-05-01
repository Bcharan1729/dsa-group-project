# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinc -Iread_json/inc -I..

# Directories
SRC_DIR = src
INC_DIR = inc
READ_JSON_DIR = read_json
BUILD_DIR = build
PARENT_DIR = ..

# Output binary name in parent directory
TARGET = $(PARENT_DIR)/phase3

# Source files
SRCS = sample.cpp \
       $(SRC_DIR)/algos.cpp \
       $(SRC_DIR)/edge.cpp \
       $(SRC_DIR)/graph.cpp \
       $(SRC_DIR)/node.cpp \
       $(READ_JSON_DIR)/read_graph.cpp \
       $(READ_JSON_DIR)/read_querry.cpp

# Object files (placed in build directory)
OBJS = $(BUILD_DIR)/sample.o \
       $(BUILD_DIR)/algos.o \
       $(BUILD_DIR)/edge.o \
       $(BUILD_DIR)/graph.o \
       $(BUILD_DIR)/node.o \
       $(BUILD_DIR)/read_graph.o \
       $(BUILD_DIR)/read_querry.o

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link object files to create the binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)
	@echo "Binary created at: $(TARGET)"

# Compile sample.cpp (in root of phase-3)
$(BUILD_DIR)/sample.o: sample.cpp
	$(CXX) $(CXXFLAGS) -c sample.cpp -o $(BUILD_DIR)/sample.o

# Compile algos.cpp (in src/)
$(BUILD_DIR)/algos.o: $(SRC_DIR)/algos.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/algos.cpp -o $(BUILD_DIR)/algos.o

# Compile edge.cpp (in src/)
$(BUILD_DIR)/edge.o: $(SRC_DIR)/edge.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/edge.cpp -o $(BUILD_DIR)/edge.o

# Compile graph.cpp (in src/)
$(BUILD_DIR)/graph.o: $(SRC_DIR)/graph.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/graph.cpp -o $(BUILD_DIR)/graph.o

# Compile node.cpp (in src/)
$(BUILD_DIR)/node.o: $(SRC_DIR)/node.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/node.cpp -o $(BUILD_DIR)/node.o

# Compile read_graph.cpp
$(BUILD_DIR)/read_graph.o: $(READ_JSON_DIR)/read_graph.cpp
	$(CXX) $(CXXFLAGS) -c $(READ_JSON_DIR)/read_graph.cpp -o $(BUILD_DIR)/read_graph.o

# Compile read_querry.cpp
$(BUILD_DIR)/read_querry.o: $(READ_JSON_DIR)/read_querry.cpp
	$(CXX) $(CXXFLAGS) -c $(READ_JSON_DIR)/read_querry.cpp -o $(BUILD_DIR)/read_querry.o

# Clean build artifacts and binary
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	@echo "Cleaned build directory and binary"

# Clean and rebuild
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild