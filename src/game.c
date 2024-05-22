#include "game.h"
#include "actors/ballActor.h"
#include "actors/racketActor.h"
#include "systems/scoring.h"
#include "ui/ui.h"
#include "sprites.h"

LCDSprite* ball = NULL;
extern PlaydateAPI* pd;

static int update(void* userdata) {
	pd->graphics->clear(kColorWhite);
	

	pd->sprite->updateAndDrawSprites();
	ui_updateScores(p1Score, p2Score);
	
	// pd->system->drawFPS(0,0);
	
	ui_createNet();
	return 1;
}


static void createBoundary(float x, float y, float width, float height) {
	LCDSprite *wall = pd->sprite->newSprite();
	pd->sprite->setTag(wall, SPRITE_KIND_WALL);
	PDRect bounds = PDRectMake(0, 0, width, height);
	pd->sprite->setBounds(wall, bounds);
	pd->sprite->setCenter(wall, 0, 0);
	
	PDRect cr = PDRectMake(0, 0, width, height);
	pd->sprite->setCollideRect(wall, cr);


	pd->sprite->moveTo(wall, x, y);
	pd->sprite->addSprite(wall);
}

static void createGameBoundaries(void) {
	createBoundary(-1, 0, 1, pd->display->getHeight()); // left wall
	createBoundary(pd->display->getWidth(), 0, 1, pd->display->getHeight()); // right wall
	createBoundary(0, -1, pd->display->getWidth(), 1); // top wall
	createBoundary(0, pd->display->getHeight(), pd->display->getWidth(), 1); // bottom wall
}

void game_init() {
    // Add various sprites to the scene
    setScore(0,0);
    ball = ballActor_create();
    int racketOffset = 50;
    racketActor_create(racketOffset, racketActor_playerControlledUpdate);
    racketActor_create(pd->display->getWidth() - racketOffset - RACKET_WIDTH, racketActor_aiControlledUpdate);
    createGameBoundaries();

    // Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
    pd->system->setUpdateCallback(update, pd);
}

void game_destroy() {
    ballActor_destroy();
}