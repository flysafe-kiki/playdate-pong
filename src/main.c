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
#include "game.h"
#include "ui/fonts.h"
#include "ui/ui.h"

#define TITLE_INIT_STATE 0
#define TITLE_UPDATE_STATE 1
#define GAME_INIT_STATE 2
#define GAME_UPDATE_STATE 3
PlaydateAPI* pd = NULL;
LCDFont* font = NULL;
PDMenuItem* backToTitleMenuItem = NULL;
int currentGameState = GAME_INIT_STATE;

#include "sprites.h"
const uint8_t SPRITE_KIND_WALL = 0;
const uint8_t SPRITE_KIND_BALL = 1;
const uint8_t SPRITE_KIND_RACKET = 2;

#ifdef _WINDLL
__declspec(dllexport)
#endif

static void title_reset(void* userdata) {
	game_destroy();
	pd->graphics->clear(kColorWhite);
	pd->system->removeMenuItem(backToTitleMenuItem);
	currentGameState = TITLE_INIT_STATE;
}

int update(void* userdata) {
	switch (currentGameState) {
		case TITLE_INIT_STATE:
			title_init();
			currentGameState = TITLE_UPDATE_STATE;
			break;
		case GAME_INIT_STATE:
			game_init();
			backToTitleMenuItem = pd->system->addMenuItem("Return to title", title_reset, NULL);
			currentGameState = GAME_UPDATE_STATE;
			break;
		case GAME_UPDATE_STATE:
			game_update();
			break;
		default:
			break;
	}

	return 1;
}

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg) {
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit ) {
		pd = playdate;
		const char* err;
		const char* fontPath = FONT_PATH;
		font = pd->graphics->loadFont(fontPath, &err);
		if ( font == NULL )
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontPath, err);

		pd->graphics->setFont(font);
		
		pd->system->setUpdateCallback(update, NULL);
	} else if ( event == kEventTerminate) {
		game_destroy();
	}
	
	return 0;
}
