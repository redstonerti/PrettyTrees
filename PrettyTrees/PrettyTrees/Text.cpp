#include "Text.h"
#include "graphics.h"
#include "GlobalState.h"
#include <iostream>
void Text::draw() {
	Vector2Int screenCoords = GetScreenCoords();
	graphics::Brush br;
	GlobalState::setRGBValue(br.fill_color,255);
	graphics::drawText(screenCoords.x,screenCoords.y,fontSize,str,br);
	
}