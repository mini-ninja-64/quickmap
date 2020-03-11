CC = gcc
LIB_FOLDER = ..
LIB_NAME = quickmap
LIB_LOCATION = $(LIB_FOLDER)/bin/shared

SOURCES = main.c
INCLUDE_DIR = $(LIB_FOLDER)/include
OUTPUT_FILE = example1

all:
	$(MAKE) -C $(LIB_FOLDER) all
	$(CC) -L$(LIB_LOCATION) -l$(LIB_NAME) $(SOURCES) -o $(OUTPUT_FILE) -I $(INCLUDE_DIR)
