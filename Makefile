CC = g++
# Path to generated single-header ryml_all.hpp
RYML_SINGLE_HEADER = libs/ryml_all.hpp
STB_IMAGE_WRITE_SINGLE_HEADER = libs/stb_image_write.h

CXXFLAGS = -std=c++17 -Wall -g -Isrc -Iinclude -isystem libs
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = build
TARGET = $(BIN_DIR)/rt

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(STB_IMAGE_WRITE_SINGLE_HEADER) $(RYML_SINGLE_HEADER) $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CXXFLAGS) -o $@ $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/rt

# Generate single-header YAML parser
$(RYML_SINGLE_HEADER):
	mkdir -p $(dir $@)
	python3 third_party/rapidyaml/tools/amalgamate.py $@