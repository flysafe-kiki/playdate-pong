//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "ball/ball.h"
#include "ui/fonts.h"
#include "ui/score.h"

PlaydateAPI* pd = NULL;
LCDFont* font = NULL;
uint8_t SPRITE_KIND_WALL = 0;


static int update(void* userdata);
#ifdef _WINDLL
__declspec(dllexport)
#endif

typedef struct Game {
	struct game_ball* ball;
} Game;
Game* game = NULL;


void createBoundary(float x, float y, float width, float height) {
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

void createGameBoundaries(void) {
	createBoundary(-1, 0, 1, pd->display->getHeight()); // left wall
	createBoundary(pd->display->getWidth(), 0, 1, pd->display->getHeight()); // right wall
	createBoundary(0, -1, pd->display->getWidth(), 1); // top wall
	createBoundary(0, pd->display->getHeight(), pd->display->getWidth(), 1); // bottom wall
}

void createNet(void) {
	pd->graphics->fillRect(pd->display->getWidth()/2 - 1, 20, 2, pd->display->getHeight() - 40, kColorBlack);
}

 
void initGameStruct(void) {
	game =  malloc(sizeof(Game));
	game->ball = createGameBallStruct();
}
void destroyGameStruct(void) {
	destroyGameBallStruct(game->ball);
	free(game);
}
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		initGameStruct();
		pd = playdate;
		const char* err;
		const char* fontPath = FONT_PATH;
		font = pd->graphics->loadFont(fontPath, &err);
		
		if ( font == NULL )
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontPath, err);


		// Add various sprites to the scene
		game->ball->createBall();
		createGameBoundaries();

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);
	}

	if (event == kEventTerminate) {
		destroyGameStruct();
	}
	
	return 0;
}

static int update(void* userdata) {
	pd->graphics->clear(kColorWhite);
	pd->graphics->setFont(font);
	

	pd->sprite->updateAndDrawSprites();
	createScores();
	
	// pd->system->drawFPS(0,0);
	
	createNet();
	return 1;
}

