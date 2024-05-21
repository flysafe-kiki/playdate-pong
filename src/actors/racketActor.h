#include <stdbool.h>

#define RACKET_WIDTH 10
#define RACKET_HEIGHT 60

#ifndef racketActor_h
#define racketActor_h

#include "pd_api.h"

LCDSprite* racketActor_create(int, LCDSpriteUpdateFunction*);
void racketActor_playerControlledUpdate(LCDSprite*);
void racketActor_aiControlledUpdate(LCDSprite*);
#endif /* racketActor_h */