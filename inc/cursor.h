#ifndef H_CURSOR_INCLUDED
#define H_CURSOR_INCLUDED

#include "global.h"

extern Element cursor;

typedef enum {
  NORMAL_CURSOR,
  GOLDEN_CURSOR
} CursorType;

void setCursor(CursorType t);

#endif