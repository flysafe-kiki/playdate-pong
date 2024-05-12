#define BALL_WIDTH 10
#define BALL_HEIGHT 10

#ifndef ballActor_h
#define ballActor_h

#include "pd_api.h"

LCDSprite* ballActor_create(void);
void ballActor_collideX(LCDSprite* ball);
void ballActor_collideY(LCDSprite* ball);
#endif /* ballActor_h */