# Makefile

# 编译器
CXX = g++

# 编译选项
CXXFLAGS = -Iinclude -Wall -O2

# 链接选项
LDFLAGS =

# 可执行文件输出目录
BIN_DIR = bin

# 中间文件（对象文件）输出目录
OBJ_DIR = build

# 测试源文件目录
TEST_SRC_DIR = test

# 查找所有测试源文件
TEST_SOURCES = $(wildcard $(TEST_SRC_DIR)/*.cpp)

# 生成对应的可执行文件列表
TEST_EXECUTABLES = $(patsubst $(TEST_SRC_DIR)/%.cpp, $(BIN_DIR)/%, $(TEST_SOURCES))

# 默认目标：编译所有测试
all: directories $(TEST_EXECUTABLES)

# 创建输出目录
directories:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

# 编译每个测试文件
$(BIN_DIR)/%: $(TEST_SRC_DIR)/%.cpp | directories
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# 清理生成的文件
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

# 支持指定单个文件编译
.PHONY: all clean compile

compile: directories
	@echo "compile $(TEST_FILE).cpp..."
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$(TEST_FILE) $(TEST_SRC_DIR)/$(TEST_FILE).cpp $(LDFLAGS)

# 获取测试文件名（不带路径和扩展名）
TEST_FILE ?= $(notdir $(basename $(wildcard $(TEST_SRC_DIR)/*.cpp)))

# 示例：如何编译单个文件
# 使用方法: make compile TEST_FILE=stack_test