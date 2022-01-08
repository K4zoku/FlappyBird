#ifndef H_BACKGROUND_INCLUDED
#define H_BACKGROUND_INCLUDED

#include "global.h"

extern Element background;

typedef enum {
  BG_DAY,
  BG_NIGHT
} BackgroundType;

void setBackround(BackgroundType t);

#endif