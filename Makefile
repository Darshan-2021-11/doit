# Color codes for logging
RED=\033[1;31m
GREEN=\033[1;32m
YELLOW=\033[1;33m
BLUE=\033[1;34m
RESET=\033[0m

# Log macros
LOG_SUCCESS=@echo -e "$(GREEN)[SUCCESS]$(RESET) $1"
LOG_ERROR=@echo -e "$(RED)[ERROR]$(RESET) $1"
LOG_INFO=@echo -e "$(BLUE)[INFO]$(RESET) $1"
LOG_WARNING=@echo -e "$(YELLOW)[WARNING]$(RESET) $1"

CC=cc
SRC_DIR=./src
INCLUDE_DIR=./include
BUILD_DIR=./build
CFLAGS=-Wall -Wextra --std=c17 -I$(INCLUDE_DIR)
#-ggdb -DDEBUG
LDFLAGS=

.PHONY: all clean

all: $(BUILD_DIR) $(BUILD_DIR)/doit

$(BUILD_DIR)/doit: $(BUILD_DIR)/log_util.o $(BUILD_DIR)/dir_util.o $(BUILD_DIR)/types.o $(BUILD_DIR)/parse.o $(BUILD_DIR)/doit.o
	$(LOG_INFO) "Linking $@..."
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	$(LOG_SUCCESS) "Program built successfully."

$(BUILD_DIR)/doit.o: $(SRC_DIR)/doit.c
	$(LOG_INFO) "Compiling $<..."
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
	$(LOG_SUCCESS) "$< compiled."

$(BUILD_DIR)/parse.o: $(SRC_DIR)/parse.c $(INCLUDE_DIR)/parse.h
	$(LOG_INFO) "Compiling $<..."
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
	$(LOG_SUCCESS) "$< compiled."

$(BUILD_DIR)/types.o: $(SRC_DIR)/types.c $(INCLUDE_DIR)/types.h
	$(LOG_INFO) "Compiling $<..."
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
	$(LOG_SUCCESS) "$< compiled."

$(BUILD_DIR)/log_util.o: $(SRC_DIR)/log_util.c $(INCLUDE_DIR)/log_util.h
	$(LOG_INFO) "Compiling $<..."
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
	$(LOG_SUCCESS) "$< compiled."

$(BUILD_DIR)/dir_util.o: $(SRC_DIR)/dir_util.c $(INCLUDE_DIR)/dir_util.h
	$(LOG_INFO) "Compiling $<..."
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
	$(LOG_SUCCESS) "$< compiled."

$(BUILD_DIR):
	$(LOG_INFO) "Checking if build directory exists..."
	@mkdir -p $(BUILD_DIR)
	$(LOG_SUCCESS) "Build directory created or already exists."

clean:
	$(LOG_WARNING) "Cleaning up build files..."
	@rm -rf $(BUILD_DIR)
	$(LOG_SUCCESS) "Build files cleaned."
