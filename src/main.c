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

PlaydateAPI* pd = NULL;
LCDFont* font = NULL;

#include "sprites.h"
const uint8_t SPRITE_KIND_WALL = 0;
const uint8_t SPRITE_KIND_BALL = 1;
const uint8_t SPRITE_KIND_RACKET = 2;

#ifdef _WINDLL
__declspec(dllexport)
#endif


int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		pd = playdate;
		const char* err;
		const char* fontPath = FONT_PATH;
		font = pd->graphics->loadFont(fontPath, &err);
		if ( font == NULL )
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontPath, err);


		pd->graphics->setFont(font);
		

		game_init();
	} else if ( event == kEventTerminate) {
		game_destroy();
	}
	
	return 0;
}
