CC = gcc
CFLAGS = -fPIC -Wall -Wextra

AR = ar
ARFLAGS = rcs

# set build directory
BUILD_DIR = build

# set output directory
OUTPUT_DIR = bin

# set src directory
SRC_DIR = src

# set include directory
INCLUDE_DIR = include
CFLAGS += -I$(INCLUDE_DIR)

# set name of library file
LIB_NAME = quickmap

# lib paths
SHARED_LIB_PATH = $(OUTPUT_DIR)/shared
STATIC_LIB_PATH = $(OUTPUT_DIR)/static
SHARED_LIB_EXTENSION = .so
STATIC_LIB_EXTENSION = .a
SHARED_LIB = $(SHARED_LIB_PATH)/lib$(LIB_NAME)$(SHARED_LIB_EXTENSION)
STATIC_LIB = $(STATIC_LIB_PATH)/lib$(LIB_NAME)$(STATIC_LIB_EXTENSION)

SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

$(OBJECTS): $(SOURCES) $(HEADERS)
	$(info  Compiling library)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SHARED_LIB): $(OBJECTS)
	$(info  Linking shared library)
	mkdir -p $(SHARED_LIB_PATH)
	$(CC) -shared -o $(SHARED_LIB) $(OBJECTS)

$(STATIC_LIB): $(OBJECTS)
	$(info  Linking static library)
	mkdir -p $(STATIC_LIB_PATH)
	$(AR) $(ARFLAGS) $(STATIC_LIB) $(OBJECTS)

all: $(SHARED_LIB) $(STATIC_LIB)

.PHONY: clean
clean:
	rm $(OBJECTS) $(SHARED_LIB) $(STATIC_LIB)
