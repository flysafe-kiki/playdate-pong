#include "ball.h"

extern PlaydateAPI* pd;
extern uint8_t SPRITE_KIND_WALL;
int dx = 5;
int dy = 5;


static void drawBall(LCDSprite* sprite, PDRect bounds, PDRect drawRect) {
	pd->graphics->fillRect(bounds.x, bounds.y, BALL_WIDTH, BALL_HEIGHT, kColorBlack);
};

static void updateBall(LCDSprite* ball) {
	// Crank direction actually enables the ball to move either forward or backward
	// depending on the rotation of the crank
	// TODO move that logic somewhere else
	int crank_direction = 0;
	if (pd->system->getCrankChange() > 0) {
		crank_direction = 1;
	} else if (pd->system->getCrankChange() < 0) {
		crank_direction = -1;
	}

	// Move the ball, taking collisions into account
	float actualX = 0;
	float actualY = 0;
	pd->sprite->getPosition(ball, &actualX, &actualY);
	int collisionsLength = 0;
	SpriteCollisionInfo* collisionInfos = pd->sprite->moveWithCollisions(ball, actualX + (dx * crank_direction), actualY + (dy * crank_direction), &actualX, &actualY, &collisionsLength);
	
	// Read potential collision information in order to change the direction of delta x and y based
	// on collision normals 
	for (int i = 0; i < collisionsLength; i++) {
		SpriteCollisionInfo *info = &collisionInfos[i];
		if (pd->sprite->getTag(info->other) == SPRITE_KIND_WALL) {
			// TODO Increase left / right ball score based on normal
			// reset ball position
		}
		if (info->normal.x != 0) {
			dx = dx * -1;
		}
		if (info->normal.y != 0) {
			dy = dy * -1;
		}
	}

	// todo: review if this is the correct way provided that collisionInfos is an array
	free(collisionInfos);
}
static SpriteCollisionResponseType ballPlaneCollisionResponse(LCDSprite* ball, LCDSprite* other)
{
	return kCollisionTypeFreeze;
}

LCDSprite* createBall(void) {
	LCDSprite *ball = pd->sprite->newSprite();
	PDRect bounds = PDRectMake(0, 0, BALL_WIDTH, BALL_HEIGHT);
	pd->sprite->setBounds(ball, bounds);
	pd->sprite->setUpdateFunction(ball, updateBall);
	pd->sprite->setDrawFunction(ball, drawBall);
	pd->sprite->setCenter(ball, 0, 0);
	pd->sprite->moveTo(ball, 0, 0);
	
	PDRect cr = PDRectMake(0, 0, BALL_WIDTH, BALL_HEIGHT);
	pd->sprite->setCollideRect(ball, cr);
	pd->sprite->setCollisionResponseFunction(ball, ballPlaneCollisionResponse);

	pd->sprite->addSprite(ball);

	return ball;
}
void collideLeft(LCDSprite* ball) {
	dx = abs(dx);
}
void collideRight(LCDSprite* ball) {
	dx = -1 * abs(dx);
}
void collideTop(LCDSprite* ball) {
	dy = abs(dy);
}
void collideBottom(LCDSprite* ball) {
	dy = -1 * abs(dy);
}

game_ball* createGameBallStruct(void) {
    game_ball* gb = malloc(sizeof(game_ball));
	gb->createBall = createBall;
	return gb;
}
void destroyGameBallStruct(game_ball* gb) {
	free(gb);
}