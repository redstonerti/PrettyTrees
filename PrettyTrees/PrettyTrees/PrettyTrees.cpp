#include "GlobalState.h"
#include "graphics.h"
#include <iostream>

void update(float ms)
{
    GlobalState::instance.update(ms * 0.001f);
}

void draw()
{
    GlobalState::instance.draw();
}

int main()
{
    GlobalState::instance.init();
   
    graphics::createWindow(960, 540, "Pretty Trees");
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);
    graphics::setCanvasSize(GlobalState::CANVAS_WIDTH, GlobalState::CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
    graphics::setFont("assets\\Outfit-Medium.ttf");
    graphics::startMessageLoop();

    GlobalState::instance.cleanUp();

    return 0;
}