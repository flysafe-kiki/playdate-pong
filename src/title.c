#include "title.h"
#include "ui/ui.h"

extern PlaydateAPI* pd;

void title_init() {
    ui_writeText("Some Pong Copy", pd->display->getWidth() / 2, 60);
    ui_writeText("Start", pd->display->getWidth() / 2, pd->display->getHeight() - 60);
}

void title_update() {
    
}