#include "background.h"
#include "mask.h"
#include "global.h"

bool __bg_init();
void __bg_quit();
void __bg_render();
void __bg_eventHandler(SDL_Event event);

Element background = {
	&__bg_init,
	&__bg_quit,
	&__bg_render,
	&__bg_eventHandler,
};

static SDL_Surface * backgrounds[2];
static SDL_Texture * backgroundsTexture[2];
static BackgroundType type = BG_DAY;
static bool bgChange = false;
static uint32_t w, h;
static float lastChange;

void setBackround(BackgroundType t) {
	type = t;
	bgChange = true;
	lastChange = elapsedSeconds;
}

const int multiple = 5;

bool __bg_init() {
	SDL_Surface * raw, * day, * night;
	raw = IMG_Load("assets/bgDay.png");
	day = SDL_CreateRGBSurface(0, raw->w * multiple, raw->h * multiple, 32, rmask, gmask, bmask, amask);
    SDL_BlitScaled(raw, NULL, day, NULL);
	SDL_FreeSurface(raw);
	raw = IMG_Load("assets/bgNight.png");
	night = SDL_CreateRGBSurface(0, raw->w * multiple, raw->h * multiple, 32, rmask, gmask, bmask, amask);
	SDL_BlitScaled(raw, NULL, night, NULL);
	SDL_FreeSurface(raw);
	w = day->w;
	h = day->h;
	backgrounds[BG_DAY] = day;
	backgrounds[BG_NIGHT] = night;
	backgroundsTexture[BG_DAY] = SDL_CreateTextureFromSurface(renderer, day);
	SDL_SetTextureBlendMode(backgroundsTexture[BG_DAY], SDL_BLENDMODE_BLEND);
	backgroundsTexture[BG_NIGHT] = SDL_CreateTextureFromSurface(renderer, night);
	SDL_SetTextureBlendMode(backgroundsTexture[BG_NIGHT], SDL_BLENDMODE_BLEND);
    return true;
}

void __bg_quit() {
	for (int i = 0; i < 2; ++i) {
		SDL_DestroyTexture(backgroundsTexture[i]);
		SDL_FreeSurface(backgrounds[i]);
	}
}

static const float speed = 30.0f; // pixels per second
static const float interval = 1000.0 / speed;
static uint32_t distance = 0;
static uint32_t step = 1;
static uint32_t prev = 0;

static bool fading = false;
static const float fade = 0.5f; // seconds
static const float fadeInterval = fade * 1000.0; // ticks
static uint32_t fadeStart;
bool bgscroll = true;

void __bg_render() {
	uint32_t elapsed, alpha;
	float percent;
	SDL_Texture * bg = backgroundsTexture[type];
	if (prev == 0) {
		// first render tick
		prev = ticks;
	} else {
		elapsed = ticks - prev;
		if (bgscroll && elapsed >= interval) {
			distance = (distance + step) % w;
			prev = ticks;
		}
	}

	if (elapsedSeconds >= lastChange + 30.0) {
		setBackround(!type);
	}

	if (bgChange) {
		fading = true;
		fadeStart = ticks;
		bgChange = false;
	}
	SDL_Rect slice1 = { distance, 0, w - distance, h };
	SDL_Rect slice2 = { 0, 0, distance, h };
	SDL_Rect dest1 = { 0, 0, w - distance, h };
	SDL_Rect dest2 = { w - distance, 0, distance, h };
	if (fading) {
		elapsed = ticks - fadeStart;
		percent = elapsed / fadeInterval;
		SDL_Texture * obg = backgroundsTexture[!type];
		if (percent >= 1.0) {
			// restore
			SDL_SetTextureAlphaMod(obg, 255);
			SDL_SetTextureAlphaMod(bg, 255);
			fading = false;
		} else {
			alpha = 255 * percent;
			SDL_SetTextureAlphaMod(bg, alpha);
			// SDL_SetTextureAlphaMod(obg, 255 - alpha);
			SDL_RenderCopy(renderer, obg, &slice1, &dest1);
			SDL_RenderCopy(renderer, obg, &slice2, &dest2);
		}
	}
	SDL_RenderCopy(renderer, bg, &slice1, &dest1);
	SDL_RenderCopy(renderer, bg, &slice2, &dest2);
}

static int clicked = 0;

void __bg_eventHandler(SDL_Event event) {
	switch (event.type) {
		case SDL_MOUSEBUTTONUP:
			++clicked;
			// printf("Mouse clicked at (%d, %d)\n", event.button.x, event.button.y);
			if (clicked >= 8) {
				setBackround(!type);
				clicked = 0;
			}
			break;
		default:
			return;
	}
}