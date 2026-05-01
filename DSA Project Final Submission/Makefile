# ============================================================
#  Compiler and Flags
# ============================================================
CXX = g++
# High performance flags enabled for all phases
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -march=native -g

# ============================================================
#  Directories
# ============================================================
PHASE1 = phase-1
PHASE2 = phase-2
PHASE3 = phase-3
BUILD_DIR = build

# ============================================================
#  Phase 1 Configuration
# ============================================================
P1_SRC_DIR = $(PHASE1)/src
P1_INC_DIR = $(PHASE1)/inc
P1_JSON_DIR = $(PHASE1)/read_json
P1_INCLUDES = -I$(P1_INC_DIR) -I$(P1_JSON_DIR)/inc -I$(PHASE1)
P1_TARGET = phase1

P1_OBJS = \
    $(BUILD_DIR)/p1_sample.o \
    $(BUILD_DIR)/p1_algos.o \
    $(BUILD_DIR)/p1_edge.o \
    $(BUILD_DIR)/p1_graph.o \
    $(BUILD_DIR)/p1_knn.o \
    $(BUILD_DIR)/p1_node.o \
    $(BUILD_DIR)/p1_read_graph.o \
    $(BUILD_DIR)/p1_read_querry.o

# ============================================================
#  Phase 2 Configuration
# ============================================================
P2_SRC_DIR = $(PHASE2)/src
P2_INC_DIR = $(PHASE2)/inc
P2_JSON_DIR = $(PHASE2)/read_json
P2_INCLUDES = -I$(P2_INC_DIR) -I$(P2_JSON_DIR)/inc -I$(PHASE2)
P2_TARGET = phase2

P2_OBJS = \
    $(BUILD_DIR)/p2_sample.o \
    $(BUILD_DIR)/p2_algos.o \
    $(BUILD_DIR)/p2_edge.o \
    $(BUILD_DIR)/p2_graph.o \
    $(BUILD_DIR)/p2_node.o \
    $(BUILD_DIR)/p2_read_graph.o \
    $(BUILD_DIR)/p2_read_querry.o

# ============================================================
#  Phase 3 Configuration
# ============================================================
P3_SRC_DIR = $(PHASE3)/src
P3_INC_DIR = $(PHASE3)/inc
P3_JSON_DIR = $(PHASE3)/read_json
P3_INCLUDES = -I$(P3_INC_DIR) -I$(P3_JSON_DIR)/inc -I$(PHASE3)
P3_TARGET = phase3

P3_OBJS = \
    $(BUILD_DIR)/p3_sample.o \
    $(BUILD_DIR)/p3_algos.o \
    $(BUILD_DIR)/p3_edge.o \
    $(BUILD_DIR)/p3_graph.o \
    $(BUILD_DIR)/p3_node.o \
    $(BUILD_DIR)/p3_read_graph.o \
    $(BUILD_DIR)/p3_read_querry.o

# ============================================================
#  Default target: build all binaries
# ============================================================
all: $(BUILD_DIR) $(P1_TARGET) $(P2_TARGET) $(P3_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ============================================================
#  Phase 1 Build Rules
# ============================================================
$(P1_TARGET): $(P1_OBJS)
	$(CXX) $(CXXFLAGS) $(P1_OBJS) -o $(P1_TARGET)
	@echo "Built binary: ./phase1"

$(BUILD_DIR)/p1_sample.o: $(PHASE1)/sample.cpp
	$(CXX) $(CXXFLAGS) $(P1_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p1_algos.o: $(P1_SRC_DIR)/algos.cpp
	$(CXX) $(CXXFLAGS) $(P1_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p1_edge.o: $(P1_SRC_DIR)/edge.cpp
	$(CXX) $(CXXFLAGS) $(P1_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p1_graph.o: $(P1_SRC_DIR)/graph.cpp
	$(CXX) $(CXXFLAGS) $(P1_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p1_knn.o: $(P1_SRC_DIR)/knn.cpp
	$(CXX) $(CXXFLAGS) $(P1_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p1_node.o: $(P1_SRC_DIR)/node.cpp
	$(CXX) $(CXXFLAGS) $(P1_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p1_read_graph.o: $(P1_JSON_DIR)/read_graph.cpp
	$(CXX) $(CXXFLAGS) $(P1_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p1_read_querry.o: $(P1_JSON_DIR)/read_querry.cpp
	$(CXX) $(CXXFLAGS) $(P1_INCLUDES) -c $< -o $@

# ============================================================
#  Phase 2 Build Rules
# ============================================================
$(P2_TARGET): $(P2_OBJS)
	$(CXX) $(CXXFLAGS) $(P2_OBJS) -o $(P2_TARGET)
	@echo "Built binary: ./phase2"

$(BUILD_DIR)/p2_sample.o: $(PHASE2)/sample.cpp
	$(CXX) $(CXXFLAGS) $(P2_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p2_algos.o: $(P2_SRC_DIR)/algos.cpp
	$(CXX) $(CXXFLAGS) $(P2_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p2_edge.o: $(P2_SRC_DIR)/edge.cpp
	$(CXX) $(CXXFLAGS) $(P2_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p2_graph.o: $(P2_SRC_DIR)/graph.cpp
	$(CXX) $(CXXFLAGS) $(P2_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p2_node.o: $(P2_SRC_DIR)/node.cpp
	$(CXX) $(CXXFLAGS) $(P2_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p2_read_graph.o: $(P2_JSON_DIR)/read_graph.cpp
	$(CXX) $(CXXFLAGS) $(P2_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p2_read_querry.o: $(P2_JSON_DIR)/read_querry.cpp
	$(CXX) $(CXXFLAGS) $(P2_INCLUDES) -c $< -o $@

# ============================================================
#  Phase 3 Build Rules
# ============================================================
$(P3_TARGET): $(P3_OBJS)
	$(CXX) $(CXXFLAGS) $(P3_OBJS) -o $(P3_TARGET)
	@echo "Built binary: ./phase3"

$(BUILD_DIR)/p3_sample.o: $(PHASE3)/sample.cpp
	$(CXX) $(CXXFLAGS) $(P3_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p3_algos.o: $(P3_SRC_DIR)/algos.cpp
	$(CXX) $(CXXFLAGS) $(P3_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p3_edge.o: $(P3_SRC_DIR)/edge.cpp
	$(CXX) $(CXXFLAGS) $(P3_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p3_graph.o: $(P3_SRC_DIR)/graph.cpp
	$(CXX) $(CXXFLAGS) $(P3_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p3_node.o: $(P3_SRC_DIR)/node.cpp
	$(CXX) $(CXXFLAGS) $(P3_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p3_read_graph.o: $(P3_JSON_DIR)/read_graph.cpp
	$(CXX) $(CXXFLAGS) $(P3_INCLUDES) -c $< -o $@

$(BUILD_DIR)/p3_read_querry.o: $(P3_JSON_DIR)/read_querry.cpp
	$(CXX) $(CXXFLAGS) $(P3_INCLUDES) -c $< -o $@

# ============================================================
#  Clean
# ============================================================
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(P1_TARGET) $(P2_TARGET) $(P3_TARGET)
	@echo "Cleaned all builds and binaries"

rebuild: clean all

.PHONY: all clean rebuild