DEBUG=yes
VERBOSE=yes

# compiler used
CC=gcc

BUILD_DIR := ./build
SRC_DIR := ./src

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings

ifeq ($(DEBUG), yes)
	CFLAGS=-W -Wall -pedantic -g
	LDFLAGS=
else
	CFLAGS=-W -Wall -ansi -pedantic
	LDFLAGS=
endif

# Find all the .c files in the src directory.
SRC := $(wildcard $(SRC_DIR)/*.c)

# Convert each source file to its corresponding object file
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Program name
EXEC=com

# Libraries used
# -lm for math.h
LIBS=-lm

# --- ---

# Generate
all: $(SRC) $(EXEC)

ifeq ($(DEBUG), yes)
	@echo "Build in debug mode"
else
	@echo "Build in release mode"
endif

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE), yes)
    HIDE =  
else
    HIDE = @
endif

$(EXEC): $(OBJ)
	$(HIDE)$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(HIDE)$(CC) $(CFLAGS) -o $@ -c $< 

.PHONY: clear

# Show used files
ec:
	@echo 'SRC = $(value SRC)'
	@echo 'OBJ = $(value OBJ)'

# Clear all file who can regenerate
clear:
	@echo "Cleaning up"
	$(HIDE)rm -f $(BUILD_DIR)/*.o $(EXEC)