#include "cursor.h"
#include "mask.h"

bool __init();
void __quit();
void __eventHandler(SDL_Event event);

Element cursor = {
	&__init,
	&__quit,
	NULL,
	&__eventHandler
};

static SDL_Surface * mouses[2];
static SDL_Cursor * cursors[2];
static CursorType type = NORMAL_CURSOR;

void setCursor(CursorType t) {
	type = t;
	if (cursors[type] == NULL) {
		fprintf(stderr, "Cursors has not been initialized!\n");
		exitCode = 1;
		return;
	}
    SDL_SetCursor(cursors[type]);
}

bool __init() {
	char path[18];
	for (int i = 0; i < 2; ++i) {
		sprintf(path, "assets/mouse%d.png", i);
		SDL_Surface * raw = IMG_Load(path);
		SDL_Surface * mouse = SDL_CreateRGBSurface(0, raw->w * 2, raw->h * 2, 32, rmask, gmask, bmask, amask);
    	SDL_BlitScaled(raw, NULL, mouse, NULL);
		SDL_SetSurfaceBlendMode(mouse, SDL_BLENDMODE_BLEND);
		mouses[i] = mouse;
		cursors[i] = SDL_CreateColorCursor(mouse, 0, 0);
		SDL_FreeSurface(raw);
	}
    setCursor(NORMAL_CURSOR);
    return true;
}

void __quit() {
	for (int i = 0; i < 2; ++i) {
		SDL_FreeCursor(cursors[i]);
    	SDL_FreeSurface(mouses[i]);
	}
}

static int clicked = 0;

void __eventHandler(SDL_Event event) {
	switch (event.type) {
		case SDL_MOUSEMOTION:
			// TODO: Hover effect
			break;
		case SDL_MOUSEBUTTONUP:
			// TODO: Clickable button, control gameplay
			++clicked;
			// printf("Mouse clicked at (%d, %d)\n", event.button.x, event.button.y);
			if (clicked >= 8) {
				setCursor(!type);
				clicked = 0;
			}
			break;
		default:
			return;
	}
}