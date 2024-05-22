#include <stdbool.h>
#include "../systems/collisionMgr.h"
#include "../sprites.h"
#include "ballActor.h"

extern PlaydateAPI* pd;
int MAX_PIXEL_MOVEMENT = 10;
int dx = 5;
int dy = 5;
FilePlayer* hitSound = NULL;
FilePlayer* scoreSound = NULL;


static void draw(LCDSprite* sprite, PDRect bounds, PDRect drawRect) {
	pd->graphics->fillRect(bounds.x, bounds.y, BALL_WIDTH, BALL_HEIGHT, kColorBlack);
};

static void update(LCDSprite* ball) {
	// Move the ball, taking collisions into account
	float actualX = 0;
	float actualY = 0;
	pd->sprite->getPosition(ball, &actualX, &actualY);

	int collisionsLength = 0;

	SpriteCollisionInfo* collisionInfos = pd->sprite->moveWithCollisions(ball, actualX + dx, actualY + dy, &actualX, &actualY, &collisionsLength);
	collisionMgr_handleCollision(ball, collisionInfos, &collisionsLength);
	free(collisionInfos);
}

static SpriteCollisionResponseType planeCollisionResponse(LCDSprite* ball, LCDSprite* other)
{
	return kCollisionTypeSlide;
}

LCDSprite* ballActor_create() {
	LCDSprite *ball = pd->sprite->newSprite();
	PDRect bounds = PDRectMake(0, 0, BALL_WIDTH, BALL_HEIGHT);
	pd->sprite->setBounds(ball, bounds);
	pd->sprite->setTag(ball, SPRITE_KIND_BALL);
	pd->sprite->setUpdateFunction(ball, update);
	pd->sprite->setDrawFunction(ball, draw);
	pd->sprite->setCenter(ball, 0, 0);
	pd->sprite->moveTo(ball, 0, 0);
	
	PDRect cr = PDRectMake(0, 0, BALL_WIDTH, BALL_HEIGHT);
	pd->sprite->setCollideRect(ball, cr);
	pd->sprite->setCollisionResponseFunction(ball, planeCollisionResponse);

	pd->sprite->addSprite(ball);

	hitSound = pd->sound->fileplayer->newPlayer();
	int hitSoundLoaded = pd->sound->fileplayer->loadIntoPlayer(hitSound, "ball_hit");
	pd->system->logToConsole(hitSoundLoaded == 1 ? "Successfully load ball_hit" : "Couldn't find ball_hit");
	scoreSound = pd->sound->fileplayer->newPlayer();
	int scoreSoundLoaded = pd->sound->fileplayer->loadIntoPlayer(scoreSound, "score");
	pd->system->logToConsole(scoreSoundLoaded == 1 ? "Successfully load score" : "Couldn't find score");

	return ball;
}
void ballActor_destroy() {
	pd->sound->fileplayer->freePlayer(hitSound);
	pd->sound->fileplayer->freePlayer(scoreSound);
}
void ballActor_collideX(LCDSprite* ball) {
	dx = -1 * dx;
	pd->sound->fileplayer->play(hitSound, 1);
}
void ballActor_collideY(LCDSprite* ball) {
	dy = -1 * dy;
	pd->sound->fileplayer->play(hitSound, 1);
}
void ballActor_setDeltaY(LCDSprite* ball, int newDeltaY) {
	dy = newDeltaY;
	pd->sound->fileplayer->play(hitSound, 1);
}
void ballActor_reset(LCDSprite* ball, bool resetToTheRight) {
	int resetDirection = resetToTheRight ? -1 : 1;
	dx = 5 * resetDirection;
	dy = 5 * resetDirection;

	int posX = BALL_SPAWN_X_OFFSET;
	if (resetToTheRight) {
		posX = pd->display->getWidth() - BALL_SPAWN_X_OFFSET - BALL_WIDTH;
	}
	pd->sprite->moveTo(ball, posX, pd->display->getHeight() / 2 - BALL_HEIGHT);
	pd->sound->fileplayer->play(scoreSound, 1);
}