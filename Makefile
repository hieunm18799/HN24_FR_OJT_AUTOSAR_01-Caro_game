# Compiler and flags
CC = gcc

# Directories
SOURCE_DIR = src
BUILD_DIR = build
CLIENT_SOURCE_DIR = $(SOURCE_DIR)/client
SERVER_SOURCE_DIR = $(SOURCE_DIR)/server
COMMON_SOURCE_DIR = $(SOURCE_DIR)/common
MODEL_SOURCE_DIR = $(SOURCE_DIR)/model
VIEW_SOURCE_DIR = $(SOURCE_DIR)/view
CONTROL_SOURCE_DIR = $(SOURCE_DIR)/control

CFLAGS = -Wall -Wextra -pthread -I./$(COMMON_SOURCE_DIR) -I./$(MODEL_SOURCE_DIR) -I./$(VIEW_SOURCE_DIR) -I./$(COMMON_SOURCE_DIR)

# Find all source files dynamically
CLIENT_SOURCE_FILES = $(shell find $(CLIENT_SOURCE_DIR) -name '*.c')
SERVER_SOURCE_FILES = $(shell find $(SERVER_SOURCE_DIR) -name '*.c')
COMMON_SOURCE_FILES = $(shell find $(COMMON_SOURCE_DIR) -name '*.c')
MODEL_SOURCE_FILES = $(shell find $(MODEL_SOURCE_DIR) -name '*.c')
VIEW_SOURCE_FILES = $(shell find $(VIEW_SOURCE_DIR) -name '*.c')
CONTROL_SOURCE_FILES = $(shell find $(CONTROL_SOURCE_DIR) -name '*.c')

# Create object files path
CLIENT_OBJECT_FILES = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(CLIENT_SOURCE_FILES) $(COMMON_SOURCE_FILES) $(VIEW_SOURCE_FILES))
SERVER_OBJECT_FILES = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SERVER_SOURCE_FILES) $(COMMON_SOURCE_FILES) $(MODEL_SOURCE_FILES) $(CONTROL_SOURCE_FILES))

# Output binaries in the build directory
CLIENT_EXECUTABLE = $(BUILD_DIR)/client_app
SERVER_EXECUTABLE = $(BUILD_DIR)/server_app

# Build rules
all: $(CLIENT_EXECUTABLE) $(SERVER_EXECUTABLE)

client: $(CLIENT_EXECUTABLE)

server: $(SERVER_EXECUTABLE)

$(CLIENT_EXECUTABLE): $(CLIENT_OBJECT_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_EXECUTABLE): $(SERVER_OBJECT_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)