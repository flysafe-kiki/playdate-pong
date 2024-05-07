#define BALL_WIDTH 10
#define BALL_HEIGHT 10

#ifndef ball_h
#define ball_h

#include "pd_api.h"

typedef struct game_ball
{
    LCDSprite* (*createBall)(void);
} game_ball;

game_ball* createGameBallStruct(void);
void destroyGameBallStruct(game_ball* ball);

#endif /* ball_h */