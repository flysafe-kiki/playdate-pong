#include "pd_api.h"

extern PlaydateAPI* pd;
#include "../sprites.h"
#include "../actors/ballActor.h"
#include "collisionMgr.h"
#include "scoring.h"


static void handleBallActorCollision(LCDSprite* ball, SpriteCollisionInfo* collisionInfos, int* collisionsLength) {

    // Read potential collision information in order to change the direction of delta x and y based
	// on collision normals 
	for (int i = 0; i < *collisionsLength; i++) {
		SpriteCollisionInfo *info = &collisionInfos[i];
		if (pd->sprite->getTag(info->other) == SPRITE_KIND_WALL) {
			if (info->normal.x == -1) {
				setScore(p1Score + 1, p2Score);
				ballActor_reset(ball);
			} else if (info->normal.x == 1) {
				setScore(p1Score, p2Score + 1);
				ballActor_reset(ball);
			}  else if (info->normal.y != 0) {
				ballActor_collideY(ball);
			}
		} else {
			if (info->normal.x != 0) {
				ballActor_collideX(ball);
			}
			if (info->normal.y != 0) {
				ballActor_collideY(ball);
			}
		}
	}
}

void collisionMgr_handleCollision(LCDSprite* sprite, SpriteCollisionInfo* collisionInfos, int* collisionsLength) {
    if (pd->sprite->getTag(sprite) == SPRITE_KIND_BALL) {
        handleBallActorCollision(sprite, collisionInfos, collisionsLength);
    }
}