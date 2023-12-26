#pragma once
#include "Arduino.h"

#ifndef HS_MALLOC

#if defined(BOARD_HAS_PSRAM)
#define HS_MALLOC ps_malloc
#define HS_CALLOC ps_calloc
#define HS_REALLOC ps_realloc
#define ps_new(X) new (ps_malloc(sizeof(X))) X
#else
#define HS_MALLOC malloc
#define HS_CALLOC calloc
#define HS_REALLOC realloc
#define ps_new(X) new X
#endif
#endif