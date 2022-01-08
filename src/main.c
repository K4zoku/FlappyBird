#include "global.h"
#include "cursor.h"
#include "debug.h"
#include "bird.h"
#include "background.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const uint32_t rmask = 0xff000000;
    const uint32_t gmask = 0x00ff0000;
    const uint32_t bmask = 0x0000ff00;
    const uint32_t amask = 0x000000ff;
#else
    const uint32_t rmask = 0x000000ff;
    const uint32_t gmask = 0x0000ff00;
    const uint32_t bmask = 0x00ff0000;
    const uint32_t amask = 0xff000000;
#endif

bool init();
void loop();
void quit();

int exitCode;
SDL_Window * window;
SDL_Renderer * renderer;
TTF_Font * font;
TTF_Font * font_outline;

static uint32_t width = 540;
static uint32_t height = 720;
static char title[] = "Fappy";

float FPS = 100.0;
float inputFps;
float avgFps;
float drawFps;
uint32_t ticks;

int main(int argc, char ** argv) {
	// TODO: parse cli options
	__unused__ argc;
	__unused__ argv;

	if (init()) {
		loop();
		quit();
	}

	return exitCode;
}

static uint32_t startTime;

bool init() {
	exitCode = 1;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}
	if (TTF_Init() < 0) {
		fprintf(stderr, "Error initializing TTF: %s\n", SDL_GetError());
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0) {
		fprintf(stderr, "Error initializing IMG: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
	window = SDL_CreateWindow(title, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		width, height, 0);
	if (window == NULL) {
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		return false;	
	}
	char fontPath[] = "assets/ttf/ProfontwindowsBold.ttf";
	int fontSize = 20;
	font = TTF_OpenFont(fontPath, fontSize);
	font_outline = TTF_OpenFont(fontPath, fontSize);
	if (font == NULL || font_outline == NULL) {
		fprintf(stderr, "Error opening font: %s\n", SDL_GetError());
		return false;
	}
	TTF_SetFontOutline(font_outline, 1); 
	TTF_SetFontKerning(font, 0);
	TTF_SetFontKerning(font_outline, 0);
	startTime = ticks = SDL_GetTicks();
	cursor.init(NULL);
	background.init(NULL);
	debug.init(NULL);
	bird.init(NULL);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	exitCode = 0;
	return true;
}

bool eventLoop() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		cursor.handle(event);
		background.handle(event);
		debug.handle(event);
		bird.handle(event);
		if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

static uint32_t totalDraws = 0;
static uint32_t prev = 0;
float elapsedSeconds;
void render() {
	elapsedSeconds = (ticks - startTime) / 1000.0;
	if (prev == 0 || ticks - prev >= 1000.0 / FPS) {
		drawFps = (float) ++totalDraws / elapsedSeconds;
		SDL_RenderClear(renderer);
		background.render();
		bird.render();
		debug.render();
		SDL_RenderPresent(renderer);
		prev = ticks;
	}
}

static uint32_t totalFrames = 0;
static uint32_t totalFrameTicks = 0;

void loop() {
	bool running = true;
	while (running) {
		++totalFrames;
		uint32_t start = ticks = SDL_GetTicks();
		running = eventLoop();
		render();
		uint32_t end = SDL_GetTicks();
		uint32_t elapsed = end - start;
		inputFps = 1.0f / (elapsed / 1000.0f);
		totalFrameTicks += elapsed;
		avgFps = 1.0f / ((float) totalFrameTicks / totalFrames);
	}
}

void quit() {
	background.quit();
	debug.quit();
	cursor.quit();
	bird.quit();
	
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;

	TTF_CloseFont(font);
	font = NULL;
	TTF_CloseFont(font_outline);
	font_outline = NULL;

	IMG_Quit();
    TTF_Quit();
	SDL_Quit();
}