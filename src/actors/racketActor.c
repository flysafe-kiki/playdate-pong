#include <stdio.h>

#include "../sprites.h"
#include "racketActor.h"

extern PlaydateAPI *pd;
extern LCDSprite *ball;

static int MAX_MOVEMENT = 6;
static void draw(LCDSprite *sprite, PDRect bounds, PDRect drawRect)
{
    pd->graphics->fillRect(bounds.x, bounds.y, RACKET_WIDTH, RACKET_HEIGHT, kColorBlack);
};

// Bit dirty, feels like this shouldn't be handled by the racket itself
void racketActor_playerControlledUpdate(LCDSprite *racket)
{
    double crankChange = pd->system->getCrankChange();

    if (crankChange != 0)
    {
        pd->system->logToConsole("%lf", crankChange);
    }
    int dy = round(crankChange * 0.3);

    float actualX = 0;
    float actualY = 0;
    pd->sprite->getPosition(racket, &actualX, &actualY);

    int collisionsLength = 0;
    SpriteCollisionInfo *collisionInfos = pd->sprite->moveWithCollisions(racket, actualX, actualY + dy, &actualX, &actualY, &collisionsLength);
    free(collisionInfos);
}
void racketActor_aiControlledUpdate(LCDSprite *racket)
{
    float ballY = 0;
    pd->sprite->getPosition(ball, NULL, &ballY);

    float actualX = 0;
    float actualY = 0;
    pd->sprite->getPosition(racket, &actualX, &actualY);
    float dy = 0;

    float racketMiddle = actualY + RACKET_HEIGHT / 2;
    dy = abs(ballY - racketMiddle);
    if (dy > MAX_MOVEMENT)
    {
        dy = MAX_MOVEMENT;
    }
    dy = ballY - racketMiddle > 0 ? dy : -1 * dy;

    int collisionsLength = 0;
    SpriteCollisionInfo *collisionInfos = pd->sprite->moveWithCollisions(racket, actualX, actualY + dy, &actualX, &actualY, &collisionsLength);
    free(collisionInfos);
}

static SpriteCollisionResponseType planeCollisionResponse(LCDSprite *racket, LCDSprite *other)
{
    return kCollisionTypeFreeze;
}

LCDSprite *racketActor_create(int posX, LCDSpriteUpdateFunction updateFn)
{
    LCDSprite *racket = pd->sprite->newSprite();
    PDRect bounds = PDRectMake(0, 0, RACKET_WIDTH, RACKET_HEIGHT);
    pd->sprite->setBounds(racket, bounds);
    pd->sprite->setTag(racket, SPRITE_KIND_RACKET);
    pd->sprite->setUpdateFunction(racket, updateFn);
    pd->sprite->setDrawFunction(racket, draw);
    pd->sprite->setCenter(racket, 0, 0);
    pd->sprite->moveTo(racket, posX, pd->display->getHeight() / 2 - (RACKET_HEIGHT / 2));

    PDRect cr = PDRectMake(0, 0, RACKET_WIDTH, RACKET_HEIGHT);
    pd->sprite->setCollideRect(racket, cr);
    pd->sprite->setCollisionResponseFunction(racket, planeCollisionResponse);

    pd->sprite->addSprite(racket);

    return racket;
}