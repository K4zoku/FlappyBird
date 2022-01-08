#include "bird.h"
#include "mask.h"

bool __br_init();
void __br_quit();
void __br_render();
void __br_eventHandler(SDL_Event event);

Element bird = {
	&__br_init,
	&__br_quit,
	&__br_render,
	&__br_eventHandler,
};
SDL_Rect birdHitbox;
static SDL_Surface * birdSurface[3];
static SDL_Texture * birdTexture[3];
static uint32_t lastUpdate;

bool __br_init() {
	char path[18];
	for (int i = 0; i < 3; ++i) {
		sprintf(path, "assets/bird%d.png", i);
		SDL_Surface * raw = IMG_Load(path);
		if (raw == NULL) {
			return false;
		}
		birdSurface[i] = SDL_CreateRGBSurface(0, raw->w * 3, raw->h * 3, 32, rmask, gmask, bmask, amask);
		SDL_BlitScaled(raw, NULL, birdSurface[i], NULL);
		SDL_SetSurfaceBlendMode(birdSurface[i], SDL_BLENDMODE_BLEND);
		birdTexture[i] = SDL_CreateTextureFromSurface(renderer, birdSurface[i]);
		SDL_FreeSurface(raw);
	}
	lastUpdate = ticks;
	return true;
}

void __br_quit() {
	for (int i = 0; i < 3; ++i) {
		SDL_DestroyTexture(birdTexture[i]);
		SDL_FreeSurface(birdSurface[i]);
	}
}

static const float GRAVITY = 2940.00189875f; // pixel per second squared
// static const float GRAVITY = 750.0f; // pixel per second squared
static float yvelocity;
static int32_t y = center(720, 48);
static double angle = 0;
static double rotateSpeed = 240.0F; // degree per second;
static bool flapping = false;
static uint32_t flapStart;
static const float FLAP_ACCELERATION = -1.5 * GRAVITY;

void __br_render() {
	int birdSprite = ticks / 50 % 3;
	int32_t w = birdSurface[birdSprite]->w;
	int32_t h = birdSurface[birdSprite]->h;
	float dT = (ticks - lastUpdate) / 1000.0f;
	float acceleration = GRAVITY;
	if (flapping) {
		float fT = (ticks - flapStart) / 1000.0f;
		acceleration += FLAP_ACCELERATION - FLAP_ACCELERATION * fT;
		if (acceleration >= 0) {
			flapping = false;
		}
	} else {
		angle += dT * rotateSpeed;
		if (angle >= 30) {
			angle = 30;
		}
	}

	yvelocity += dT * acceleration;
	y += yvelocity * dT;
	lastUpdate = ticks;

	if (y < 0) {
		y = 0;
		flapping = false;
		if (yvelocity < 0) {
			yvelocity = 0;
		}
	} else if (y >= 600 - h) {
		y = 600 - h;
	}

	SDL_Rect dest = { center(540, w), y, w, h };
	birdHitbox = dest;
	SDL_RenderCopyEx(renderer, birdTexture[birdSprite], NULL, &dest, angle, NULL, SDL_FLIP_NONE);
}
static int flapBoost = 0;
static uint32_t freeze = 0;
void flap() {
	if (!flapping) {
		flapping = true;
		yvelocity = 0;
		angle = -30;
	} else {
		flapBoost = (flapBoost + 1) % 3;
		if (flapBoost == 0) {
			freeze = ticks + 768;
		}
	}
	flapStart = ticks;
}

void __br_eventHandler(SDL_Event event) {
	switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
		case SDL_KEYDOWN:
			if (!freeze || ticks >= freeze) flap();
			break;
		default:
			return;
	}
}