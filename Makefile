TARGET=main test_timer test_socket test_animation

LIB_DIR=lib/linux
INC_DIR=include/linux
LIB_TARGET=libSDL2-2.0.so.0 libSDL2_ttf-2.0.so.0 libSDL2_image-2.0.so.0

CC=gcc
CFLAGS=-g -Wall -I $(INC_DIR)
LFLAGS=-Wall -L $(LIB_DIR) -Wl,-rpath $(LIB_DIR) -Wl,-rpath ./  -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
TRGS:=$(TARGET:%=$(BIN_DIR)/%)

SOURCES:=$(wildcard $(SRC_DIR)/*.c)
OBJECTS:=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
MAINS:=$(TARGET:%=$(OBJ_DIR)/%.o)
OBJS:=$(filter-out $(MAINS),$(OBJECTS))

all: $(TRGS) copy_lib

$(TRGS): $(OBJECTS)
	@$(CC) $(subst $(BIN_DIR),$(OBJ_DIR),$@).o $(OBJS) $(LFLAGS) -o $@
	@echo "Linking $(subst $(BIN_DIR)/,,$@) complete!"

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"
	
.PHONY: copy_lib
copy_lib: $(LIB_TARGET)

$(LIB_TARGET):
	@cp $(LIB_DIR)/$@ $(BIN_DIR)/$@
	@echo "Library $(LIB_DIR)/$@ -> $(BIN_DIR)/$@ copied correctly!"

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