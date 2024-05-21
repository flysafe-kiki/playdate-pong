#include "pd_api.h"

extern PlaydateAPI* pd;
#include "../sprites.h"
#include "../actors/ballActor.h"
#include "../actors/racketActor.h"
#include "collisionMgr.h"
#include "scoring.h"


static void handleBallActorCollision(LCDSprite* ball, SpriteCollisionInfo* collisionInfos, int* collisionsLength) {

    // Read potential collision information in order to change the direction of delta x and y based
	// on collision normals 
	for (int i = 0; i < *collisionsLength; i++) {
		SpriteCollisionInfo *info = &collisionInfos[i];
		uint8_t collidedSprite = pd->sprite->getTag(info->other);
		if (collidedSprite == SPRITE_KIND_WALL) {
			if (info->normal.x == -1) {
				setScore(p1Score + 1, p2Score);
				ballActor_reset(ball, false);
			} else if (info->normal.x == 1) {
				setScore(p1Score, p2Score + 1);
				ballActor_reset(ball, true);
			}  else if (info->normal.y != 0) {
				ballActor_collideY(ball);
			}
		} else if (collidedSprite == SPRITE_KIND_RACKET) {
			float racketY = 0;
			pd->sprite->getPosition(info->other, NULL, &racketY);
			float racketMiddle = racketY + RACKET_HEIGHT / 2;
			
			int newDeltaY = round(info->touch.y - racketMiddle);
			ballActor_setDeltaY(ball, newDeltaY);
			ballActor_collideX(ball);
		}
	}
}

void collisionMgr_handleCollision(LCDSprite* sprite, SpriteCollisionInfo* collisionInfos, int* collisionsLength) {
    if (pd->sprite->getTag(sprite) == SPRITE_KIND_BALL) {
        handleBallActorCollision(sprite, collisionInfos, collisionsLength);
    }
}