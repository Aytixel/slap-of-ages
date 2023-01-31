TARGET=main test_timer test_socket

ifeq ($(OS), Windows_NT)

LIB_DIR=lib/windows
INC_DIR=include/windows
LIB_TARGET=SDL2.dll SDL2_ttf.dll SDL2_image.dll
LIB_TARGET_DIR=dll

LFLAGS=-Wall -L$(LIB_DIR) -lmingw32 -lws2_32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

EXE_EXT=.exe
PATH_SEP=\\

RM=del /q /f
RM_DIR=rmdir /q /s
CP=copy

else

LIB_DIR=lib/linux
INC_DIR=include/linux
LIB_TARGET=libSDL2-2.0.so.0 libSDL2_ttf-2.0.so.0 libSDL2_image-2.0.so.0
LIB_TARGET_DIR=$(LIB_DIR)

LFLAGS=-Wall -L $(LIB_DIR) -Wl,-rpath $(LIB_DIR) -Wl,-rpath ./  -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

EXE_EXT=
PATH_SEP=/

RM=rm -f
RM_DIR=rm -rf
CP=cp

endif

CC=gcc
CFLAGS=-g -Wall -I$(INC_DIR)

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
	@$(CC) $(subst $(BIN_DIR),$(OBJ_DIR),$@).o $(OBJS) $(LFLAGS) -o $@$(EXE_EXT)
	@echo "Linking $(subst $(BIN_DIR)/,,$@) complete!"

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"
	
.PHONY: copy_lib
copy_lib: $(LIB_TARGET)

$(LIB_TARGET):
	@$(CP) $(subst /,$(PATH_SEP),$(LIB_TARGET_DIR)/$@ $(BIN_DIR)/$@)
	@echo "Library $(LIB_TARGET_DIR)/$@ -> $(BIN_DIR)/$@ copied correctly!"

.PHONY: clean
clean:
	@$(RM) $(subst /,$(PATH_SEP),$(OBJECTS))
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(RM) $(addsuffix $(EXE_EXT),$(subst /,$(PATH_SEP),$(TRGS)))
	@echo "Executable removed!"

.PHONY: docs
docs:
	@doxygen doc/doxyfile
	@echo "Documentation generation complete!"

.PHONY: clean_docs
clean_docs:
	@$(RM_DIR) $(subst /,$(PATH_SEP),doc/html/)
	@$(RM_DIR) $(subst /,$(PATH_SEP),doc/latex/)
	@echo "Documentation cleanup complete!"