#include "debug.h"
#include "mask.h"

bool __db_init();
void __db_quit();
void __db_render();
void __db_eventHandler(SDL_Event event);

Element debug = {
	&__db_init,
	&__db_quit,
	&__db_render,
	&__db_eventHandler,
};

static char fpsText[256];
static bool debugEnabled;

void setDebug(bool enable) {
	debugEnabled = enable;
}

bool __db_init() {
	debugEnabled = false;
	return true;
}

void __db_quit() {

}

const SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF}; 
const SDL_Color black = {0x00, 0x00, 0x00, 0xFF}; 

void __db_render() {
	if (!debugEnabled) {
		return;
	}
	sprintf(fpsText, "Input %8.0f fps\nCurrent %6.0f fps \nDraw %9.0f fps\nElapsed %6.0f s", inputFps, avgFps, drawFps, elapsedSeconds);	
	SDL_Surface *bg_surface = TTF_RenderUTF8_Blended_Wrapped(font_outline, fpsText, black, 184); 
	SDL_Surface *fg_surface = TTF_RenderUTF8_Blended_Wrapped(font, fpsText, white, 184); 
	SDL_Rect rect = { 1, 1, fg_surface->w, fg_surface->h}; 
	SDL_SetSurfaceBlendMode(fg_surface, SDL_BLENDMODE_BLEND); 
	SDL_BlitSurface(fg_surface, NULL, bg_surface, &rect);
	SDL_FreeSurface(fg_surface); 
	SDL_Texture * txt = SDL_CreateTextureFromSurface(renderer, bg_surface);
	int w = 0;
    int h = 0;
    SDL_QueryTexture(txt, NULL, NULL, &w, &h);
	SDL_Surface * bg = SDL_CreateRGBSurface(0, w + 2, h + 2, 32, rmask, gmask, bmask, amask);
    SDL_Rect dest = { 10, 10, w, h };
    SDL_Rect bgdest = { dest.x - 2, dest.y - 2, w + 2, h + 2};
    SDL_FillRect(bg, NULL, SDL_MapRGBA(bg->format, 0, 0, 0, 60));
    SDL_Texture * bgTexture = SDL_CreateTextureFromSurface(renderer, bg);
    // uint8_t r,g,b,a;
    // SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
	SDL_RenderDrawRect(renderer, &birdHitbox);
	// SDL_SetRenderDrawColor(renderer, r, g, b, a);

    SDL_RenderCopy(renderer, bgTexture, NULL, &bgdest);
    SDL_RenderCopy(renderer, txt, NULL, &dest);
    SDL_DestroyTexture(bgTexture);
    SDL_FreeSurface(bg);
    SDL_DestroyTexture(txt);
    SDL_FreeSurface(bg_surface);
}

void __db_eventHandler(SDL_Event event) {
	switch (event.type) {
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_F3) {
				setDebug(!debugEnabled);
			}
			break;
		default:
			return;
	}
}