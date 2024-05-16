#include "../sprites.h"
#include "racketActor.h"

extern PlaydateAPI* pd;

static void draw(LCDSprite* sprite, PDRect bounds, PDRect drawRect) {
	pd->graphics->fillRect(bounds.x, bounds.y, RACKET_WIDTH, RACKET_HEIGHT, kColorBlack);
};

// Bit dirty, feels like this shouldn't be handled by the racket itself
static void playerControlledUpdate(LCDSprite* racket) {
    float crankChange = pd->system->getCrankChange();
    int dy = 0;
    
    if (crankChange > 0) {
        dy = 5;
    } else if (crankChange < 0) {
        dy = -5;
    }

	float actualX = 0;
	float actualY = 0;
	pd->sprite->getPosition(racket, &actualX, &actualY);

	int collisionsLength = 0;
	SpriteCollisionInfo* collisionInfos = pd->sprite->moveWithCollisions(racket, actualX, actualY + dy, &actualX, &actualY, &collisionsLength);
	free(collisionInfos);
}

static SpriteCollisionResponseType planeCollisionResponse(LCDSprite* racket, LCDSprite* other) {
    return kCollisionTypeFreeze;
}

LCDSprite* racketActor_create(bool isPlayerControlled, int posX) {
    LCDSprite* racket = pd->sprite->newSprite();
    PDRect bounds = PDRectMake(0, 0, RACKET_WIDTH, RACKET_HEIGHT);
    pd->sprite->setBounds(racket, bounds);
    pd->sprite->setTag(racket, SPRITE_KIND_RACKET);
    if (isPlayerControlled) {
        pd->sprite->setUpdateFunction(racket, playerControlledUpdate);
    }
    pd->sprite->setDrawFunction(racket, draw);
    pd->sprite->setCenter(racket, 0, 0);
    pd->sprite->moveTo(racket, posX, pd->display->getHeight() / 2 - (RACKET_HEIGHT / 2));
    
    PDRect cr = PDRectMake(0, 0, RACKET_WIDTH, RACKET_HEIGHT);
    pd->sprite->setCollideRect(racket, cr);
    pd->sprite->setCollisionResponseFunction(racket, planeCollisionResponse);

    pd->sprite->addSprite(racket);

    return racket;
}
void racketActor_move(LCDSprite*, int);