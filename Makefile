# 定义源文件目录和头文件目录
SRC_DIR := test
INC_DIR := include
# 定义构建目录和二进制文件目录
BUILD_DIR := build
BIN_DIR := bin

# 获取所有 .cpp 文件
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# 定义目标文件目录
OBJ_DIR := $(BUILD_DIR)

# 生成目标文件列表和可执行文件列表
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
EXEC_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%, $(SRC_FILES))

# 编译器和编译选项
CXX := g++
CXXFLAGS := -I$(INC_DIR) -Wall -O2

# 默认目标
all: $(EXEC_FILES)

# 链接目标文件生成可执行文件
$(BIN_DIR)/%: $(OBJ_DIR)/%.o | $(BIN_DIR)
	$(CXX) -o $@ $<

# 编译 .cpp 文件生成目标文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 创建构建目录和二进制文件目录
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# 清理生成的文件
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean