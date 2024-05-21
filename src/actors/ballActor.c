#include <stdbool.h>
#include "../systems/collisionMgr.h"
#include "../sprites.h"
#include "ballActor.h"

extern PlaydateAPI* pd;
int MAX_PIXEL_MOVEMENT = 10;
int dx = 5;
int dy = 5;


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
	return kCollisionTypeFreeze;
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

	return ball;
}
void ballActor_collideX(LCDSprite* ball) {
	dx = -1 * dx;
}
void ballActor_collideY(LCDSprite* ball) {
	dy = -1 * dy;
}
void ballActor_setDeltaY(LCDSprite* ball, int newDeltaY) {
	if (newDeltaY > MAX_PIXEL_MOVEMENT) {
		dy = MAX_PIXEL_MOVEMENT;
	} else if (newDeltaY < -1 * MAX_PIXEL_MOVEMENT) {
		dy = -1 * MAX_PIXEL_MOVEMENT;
	} else {
		dy = newDeltaY;
	}
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
}