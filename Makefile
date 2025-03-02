CC = g++
CFLAGS = -O3 -Wall -Wextra -pedantic -std=c++23 -pthread
GTFLAGS = -lgtest -lgtest_main -lpthread
PATH_TO_GTEST = /usr/lib 

# Директории
SRC_DIR = src
BUILD_DIR = build

# Цели
all: $(BUILD_DIR) $(BUILD_DIR)/tests $(BUILD_DIR)/calculate-pi 


$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

test: $(BUILD_DIR)/tests
	@printf "Running tests...\n"
	@./$(BUILD_DIR)/tests

pi: $(BUILD_DIR)/calculate-pi
	@printf "Running calculate-pi...\n"
	@./$(BUILD_DIR)/calculate-pi $(PRECISION)

$(BUILD_DIR)/tests: $(BUILD_DIR)/LongNum.o $(BUILD_DIR)/tests.o
	@printf "Linking tests...\n"
	@$(CC) $(BUILD_DIR)/LongNum.o $(BUILD_DIR)/tests.o -L $(PATH_TO_GTEST) $(GTFLAGS) -o $(BUILD_DIR)/tests
	@printf "Linking tests is successful\n"

$(BUILD_DIR)/calculate-pi: $(BUILD_DIR)/LongNum.o $(BUILD_DIR)/calculate-pi.o
	@printf "Linking calculate-pi...\n"
	@$(CC) $(BUILD_DIR)/LongNum.o $(BUILD_DIR)/calculate-pi.o -o $(BUILD_DIR)/calculate-pi
	@printf "Linking calculate-pi is successful\n"

$(BUILD_DIR)/LongNum.o: $(SRC_DIR)/LongNum.cpp $(SRC_DIR)/LongNum.hpp
	@printf "Compiling LongNum...\n"
	@$(CC) $(CFLAGS) -I $(SRC_DIR) -c $(SRC_DIR)/LongNum.cpp -o $(BUILD_DIR)/LongNum.o

$(BUILD_DIR)/tests.o: tests.cpp $(BUILD_DIR)/LongNum.o
	@printf "Compiling tests...\n"
	@$(CC) $(CFLAGS) -I $(SRC_DIR) -c tests.cpp -o $(BUILD_DIR)/tests.o

$(BUILD_DIR)/calculate-pi.o: $(SRC_DIR)/calculate-pi.cpp $(SRC_DIR)/LongNum.hpp
	@printf "Compiling calculate-pi...\n"
	@$(CC) $(CFLAGS) -I $(SRC_DIR) -c $(SRC_DIR)/calculate-pi.cpp -o $(BUILD_DIR)/calculate-pi.o

clean:
	@printf "Cleaning...\n"
	@rm -rf $(BUILD_DIR)
	@printf "Cleaning successful\n"

.PHONY: all test pi clean 