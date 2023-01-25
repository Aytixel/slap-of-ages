TARGET=main test_timer

LIB_DIR=./lib/linux
INC_DIR=./include/linux

CC=gcc
CFLAGS=-g -Wall -I $(INC_DIR)
LFLAGS=-Wall -L $(LIB_DIR) -Wl,-rpath $(LIB_DIR) -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
TRGS:=$(TARGET:%=$(BIN_DIR)/%)

SOURCES:=$(wildcard $(SRC_DIR)/*.c)
OBJECTS:=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
MAINS:=$(TARGET:%=$(OBJ_DIR)/%.o)
OBJS:=$(filter-out $(MAINS),$(OBJECTS))

all: $(TRGS)

$(TRGS): $(OBJECTS)
	@$(CC) $(subst $(BIN_DIR),$(OBJ_DIR),$@).o $(OBJS) $(LFLAGS) -o $@
	@echo "Linking $(subst $(BIN_DIR)/,,$@) complete!"

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"

.PHONY: clean
clean:
	@rm -f $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@rm -f $(TRGS)
	@echo "Executable removed!"

.PHONY: docs
docs:
	@rm -rf doc/html/
	@rm -rf doc/latex/
	doxygen doc/doxyfile