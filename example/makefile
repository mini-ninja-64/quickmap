FILE_NAME = example.bin
CC = gcc
LIB_FOLDER = ..
LIB_NAME = quickmap
OUTPUT_FOLDER = $(LIB_FOLDER)/bin/example
LIB_LOCATION = $(LIB_FOLDER)/bin/shared
SOURCES = main.c
INCLUDE_DIR = $(LIB_FOLDER)/include
OUTPUT_FILE = $(OUTPUT_FOLDER)/$(FILE_NAME)

all:
	$(MAKE) -C $(LIB_FOLDER) all
	mkdir -p $(OUTPUT_FOLDER)
	$(CC) -L$(LIB_LOCATION) -l$(LIB_NAME) $(SOURCES) -o $(OUTPUT_FILE) -I $(INCLUDE_DIR)
