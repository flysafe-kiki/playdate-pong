#define BALL_WIDTH 10
#define BALL_HEIGHT 10
#define BALL_SPAWN_X_OFFSET 75

#ifndef ballActor_h
#define ballActor_h

#include <stdbool.h>
#include "pd_api.h"

LCDSprite* ballActor_create(void);
void ballActor_destroy(void);
void ballActor_reset(LCDSprite*, bool);
void ballActor_collideX(LCDSprite*);
void ballActor_collideY(LCDSprite*);
void ballActor_setDeltaY(LCDSprite*, int);
#endif /* ballActor_h */