#ifndef H_GLOBAL_INCLUDED
#define H_GLOBAL_INCLUDED

#include "common.h"

extern int exitCode;
extern SDL_Window * window;
extern SDL_Renderer * renderer;
extern TTF_Font * font;
extern TTF_Font * font_outline;

extern uint32_t ticks;
extern float FPS;
extern float inputFps;
extern float avgFps;
extern float drawFps;
extern float elapsedSeconds;

extern SDL_Rect birdHitbox;

#endif