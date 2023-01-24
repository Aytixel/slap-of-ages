TARGET=main test_timer test_socket

CC=gcc
CFLAGS=-g -Wall
LFLAGS=-Wall

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
TRGS:=$(TARGET:%=$(BIN_DIR)/%)

SOURCES:=$(wildcard $(SRC_DIR)/*.c)
INCLUDES:=$(wildcard $(SRC_DIR)/*.h)
OBJECTS:=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
MAINS:=$(TARGET:%=$(OBJ_DIR)/%.o)
OBJS:=$(filter-out $(MAINS),$(OBJECTS))

all: $(TRGS)

$(TRGS): $(OBJECTS)
	@$(CC) $(subst $(BIN_DIR),$(OBJ_DIR),$@).o $(OBJS) $(LFLAGS) -o $@
	@echo "Linking $(subst $(BIN_DIR)/,,$@) complete!"

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@rm -f $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@rm -f $(BIN_DIR)/$(TARGET)
	@echo "Executable removed!"

.PHONY: docs
docs:
	@rm -rf doc/html/
	@rm -rf doc/latex/
	doxygen doc/doxyfile