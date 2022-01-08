#ifndef H_COMMON_INCLUDED
#define H_COMMON_INCLUDED 

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define center(a, b) ((a - b) / 2)

#define __unused__ (void)

typedef void (* runnable)(void);
typedef bool (* predicate)(void *);
typedef void (* event_handler)(SDL_Event);

typedef struct {
	predicate init;
	runnable quit;
	runnable render;
	event_handler handle;
} Element;

#endif