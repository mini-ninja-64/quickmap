BUILD_DIR = $(LIB_FOLDER)/build

FILE_NAME = example.bin
SOURCES = main.c
OBJECTS = $(BUILD_DIR)/main.o

CC = gcc
CFLAGS = -Wall -Wextra

LIB_FOLDER = ..
OUTPUT_FOLDER = $(LIB_FOLDER)/bin/example
LIB_FILE = $(LIB_FOLDER)/bin/static/libquickmap.a
INCLUDE_DIR = $(LIB_FOLDER)/include
OUTPUT_FILE = $(OUTPUT_FOLDER)/$(FILE_NAME)

$(OBJECTS): $(SOURCES)
	$(info  Compiling Example)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $(BUILD_DIR)/$@

$(OUTPUT_FILE): $(OBJECTS)
	$(info  Linking Example)
	mkdir -p $(OUTPUT_FOLDER)
	$(CC) $(OBJECTS) $(LIB_FILE) -o $(OUTPUT_FILE)

all: $(OUTPUT_FILE)

.PHONY: clean
clean:
	rm $(OUTPUT_FOLDER)/$(FILE_NAME) $(OBJECTS)
