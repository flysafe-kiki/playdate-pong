#include "../systems/collisionMgr.h"
#include "../sprites.h"
#include "ballActor.h"

extern PlaydateAPI* pd;
int dx = 5;
int dy = 5;


static void draw(LCDSprite* sprite, PDRect bounds, PDRect drawRect) {
	pd->graphics->fillRect(bounds.x, bounds.y, BALL_WIDTH, BALL_HEIGHT, kColorBlack);
};

static void update(LCDSprite* ball) {
	// Crank direction actually enables the ball to move either forward or backward
	// depending on the rotation of the crank
	// TODO move that logic somewhere else
	int crankDirection = 0;
	if (pd->system->getCrankChange() > 0) {
		crankDirection = 1;
	} else if (pd->system->getCrankChange() < 0) {
		crankDirection = -1;
	}

	// Move the ball, taking collisions into account
	float actualX = 0;
	float actualY = 0;
	pd->sprite->getPosition(ball, &actualX, &actualY);

	int collisionsLength = 0;
	// TODO: figure out why ball no longer bounces with our collide logic
	SpriteCollisionInfo* collisionInfos = pd->sprite->moveWithCollisions(ball, actualX + (dx * crankDirection), actualY + (dy * crankDirection), &actualX, &actualY, &collisionsLength); 
	collisionMgr_handleCollision(ball, collisionInfos, &collisionsLength);
	free(collisionInfos);
}

static SpriteCollisionResponseType planeCollisionResponse(LCDSprite* ball, LCDSprite* other)
{
	return kCollisionTypeFreeze;
}

LCDSprite* ballActor_create(void) {
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
	dx = -1 * abs(dx);
}
void ballActor_collideY(LCDSprite* ball) {
	dy = -1 * abs(dy);
}