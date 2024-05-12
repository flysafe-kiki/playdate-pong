#include "pd_api.h"

extern PlaydateAPI* pd;
#include "../sprites.h"
#include "../actors/ballActor.h"
#include "collisionMgr.h"


static void handleBallActorCollision(LCDSprite* ball, SpriteCollisionInfo* collisionInfos, int* collisionsLength) {

    // Read potential collision information in order to change the direction of delta x and y based
	// on collision normals 
	for (int i = 0; i < *collisionsLength; i++) {
		SpriteCollisionInfo *info = &collisionInfos[i];
		if (pd->sprite->getTag(info->other) == SPRITE_KIND_WALL) {
			// TODO Increase left / right ball score based on normal
			// reset ball position
            // increaseP1Score /increaseP2Score
		}

		if (info->normal.x != 0) {
			ballActor_collideX(ball);
		}
		if (info->normal.y != 0) {
			ballActor_collideY(ball);
		}
	}
}

void collisionMgr_handleCollision(LCDSprite* sprite, SpriteCollisionInfo* collisionInfos, int* collisionsLength) {
    if (pd->sprite->getTag(sprite) == SPRITE_KIND_BALL) {
        handleBallActorCollision(sprite, collisionInfos, collisionsLength);
    }
}