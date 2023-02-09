TARGET=main_client main_server
TEST_TARGET=test_timer test_socket test_window test_animation

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

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
TEST_DIR=tests

CC=gcc
CFLAGS=-g -Wall -I$(INC_DIR) -I$(SRC_DIR)

TRGS:=$(TARGET:%=$(BIN_DIR)/%)
TEST_TRGS:=$(TEST_TARGET:%=$(BIN_DIR)/%)

SOURCES:=$(wildcard $(SRC_DIR)/*.c)
TESTS:=$(wildcard $(TEST_DIR)/*.c)

OBJECTS:=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_OBJECTS:=$(TESTS:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

MAINS:=$(TARGET:%=$(OBJ_DIR)/%.o)
TEST_MAINS:=$(TEST_TARGET:%=$(OBJ_DIR)/%.o)

OBJS:=$(filter-out $(MAINS),$(OBJECTS))

all: install_sdl build build_test
	
build: $(TRGS) copy_lib
	
build_test: $(TEST_TRGS) copy_lib

$(TRGS): $(OBJECTS)
	@$(CC) $(subst $(BIN_DIR),$(OBJ_DIR),$@).o $(OBJS) $(LFLAGS) -o $@$(EXE_EXT)
	@echo "Linking $(subst $(BIN_DIR)/,,$@) complete!"

$(TEST_TRGS): $(OBJS) $(TEST_OBJECTS)
	@$(CC) $(subst $(BIN_DIR),$(OBJ_DIR),$@).o $(OBJS) $(LFLAGS) -o $@$(EXE_EXT)
	@echo "Linking $(subst $(BIN_DIR)/,,$@) complete!"
	
$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"
	
$(TEST_OBJECTS): $(OBJ_DIR)/%.o : $(TEST_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"
	
.PHONY: copy_lib
copy_lib: $(LIB_TARGET)

$(LIB_TARGET):
	@$(CP) $(subst /,$(PATH_SEP),$(LIB_TARGET_DIR)/$@ $(BIN_DIR)/$@)
	@echo "Library $(LIB_TARGET_DIR)/$@ -> $(BIN_DIR)/$@ copied correctly!"

.PHONY: clean
clean:
	@$(RM) $(subst /,$(PATH_SEP),$(OBJECTS) $(TEST_OBJECTS))
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(RM) $(addsuffix $(EXE_EXT),$(subst /,$(PATH_SEP),$(TRGS) $(TEST_TRGS)))
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

install_sdl:
ifneq ($(OS), Windows_NT)
	@rm -rf SDL_lib
	@mkdir SDL_lib

	@rm -rf SDL
	@git clone https://github.com/libsdl-org/SDL.git && cd SDL && git checkout release-2.26.2
	@cd SDL && ./configure --prefix=$(shell pwd)/SDL_lib && $(MAKE) -j2 && $(MAKE) -j2 install
	@rm -rf SDL

	@rm -rf SDL_image
	@git clone https://github.com/libsdl-org/SDL_image.git && cd SDL_image && git checkout release-2.6.2
	@cd SDL_image && ./configure --prefix=$(shell pwd)/SDL_lib && $(MAKE) -j2 && $(MAKE) -j2 install
	@rm -rf SDL_image

	@rm -rf SDL_ttf
	@git clone https://github.com/libsdl-org/SDL_ttf.git && cd SDL_ttf && git checkout release-2.0.18
	@cd SDL_ttf && ./configure --prefix=$(shell pwd)/SDL_lib && $(MAKE) -j2 && $(MAKE) -j2 install
	@rm -rf SDL_ttf

	@cp -r SDL_lib/lib/* $(LIB_DIR)
	@cp -r SDL_lib/include/* $(INC_DIR)
	@rm -rf SDL_lib
endif