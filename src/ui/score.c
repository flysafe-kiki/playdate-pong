#include "score.h"
#include "fonts.h"

extern PlaydateAPI* pd;
extern LCDFont* font;

static int scoreOffset = 20;
void createScores(void) {
	char* leftBallScore = "00";
	char* rightBallScore = "00";
    PDStringEncoding textEncoding = TEXT_ENCODING;
	pd->graphics->drawText(rightBallScore, strlen(rightBallScore), textEncoding, pd->display->getWidth() / 2 + scoreOffset, scoreOffset);


    int leftBallScoreWidth = pd->graphics->getTextWidth(font, leftBallScore, strlen(leftBallScore), textEncoding, pd->graphics->getTextTracking());
	pd->graphics->drawText(leftBallScore, strlen(leftBallScore), textEncoding, pd->display->getWidth() / 2 - scoreOffset - leftBallScoreWidth, scoreOffset);
}