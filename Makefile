CC:=x86_64-w64-mingw32-gcc
CCFLAGS:=`x86_64-w64-mingw32-pkg-config --libs --cflags sdl2 SDL2_ttf SDL2_image` -ggdb3 -Wall -Wextra -O2 -std=gnu11
LIBS:=-lSDL2_image -lSDL2_ttf -lm
CCEXTRAS:=-fPIC

SRC:=src
INCLUDES:=inc
ASSETS:=assets
DIST:=dist
OBJ:=$(DIST)/obj
BIN:=$(DIST)/bin

NAME:=fappy

SOURCES:=$(shell find $(SRC) -name '*.c')
OBJECTS:=$(SOURCES:$(SRC)/%.c=$(OBJ)/%.o)

default: clean prepare compile run

prepare:
	@echo Preparing...
	@mkdir -p $(OBJ)
	@mkdir -p $(BIN)
	@cp -r assets $(BIN)

clean:
	@echo Cleaning...
	@rm -rf $(OBJ)/*
	@rm -rf $(BIN)/*

compile: $(OBJECTS)
	@echo Linking...
	@$(CC) -I$(INCLUDES) $(CCFLAGS) $^ /usr/x86_64-w64-mingw32/lib/libSDL2_image.a /usr/x86_64-w64-mingw32/lib/libSDL2_ttf.a /usr/x86_64-w64-mingw32/lib/libSDL2.a -o $(BIN)/$(NAME) $(LIBS) $(CCEXTRAS)

$(OBJ)/%.o: $(SRC)/%.c
	@echo Compiling $<
	@mkdir -p $$(dirname $@)
	@$(CC) -I$(INCLUDES) $(CCFLAGS) -c $< -o $@ $(LIBS) $(CCEXTRAS)

run:
	@echo Running...
	@$(BIN)/$(NAME)