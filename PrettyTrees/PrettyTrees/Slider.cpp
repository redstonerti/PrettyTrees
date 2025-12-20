#include <iostream>
#include "Slider.h"
#include "graphics.h"
#include "GlobalState.h"
#include "Text.h"
void Slider::draw() {
	graphics::Brush br;
	GlobalState::setRGBValue(br.fill_color, 255);
	br.fill_opacity = 1.;
	br.outline_opacity = 0.;
	br.outline_width = 0.;
	Vector2Int screenCoords = GetScreenCoords();

	//Draw box on the inside
	graphics::drawRect(
		screenCoords.x + dimensions.x * GlobalState::CANVAS_WIDTH * value * 0.5 - dimensions.x * GlobalState::CANVAS_WIDTH * 0.5,
		screenCoords.y,
		dimensions.x * GlobalState::CANVAS_WIDTH * value,
		dimensions.y * GlobalState::CANVAS_HEIGHT,
		br
	);

	//Draw custom outline, it's just 4 rectangles, 1 on each side
	int spacing = GlobalState::SLIDER_OUTLINE_SPACING;
	int outline_width = GlobalState::SLIDER_OUTLINE_WIDTH;
	GlobalState::setRGBValue(br.fill_color,255,255,255);
	graphics::drawRect(
		screenCoords.x, 
		screenCoords.y + dimensions.y * GlobalState::CANVAS_HEIGHT * 0.5 + spacing + outline_width * 0.5,
		dimensions.x * GlobalState::CANVAS_WIDTH + spacing * 2. + outline_width * 2.,
		outline_width,
		br
	);
	graphics::drawRect(
		screenCoords.x, 
		screenCoords.y - dimensions.y * GlobalState::CANVAS_HEIGHT * 0.5 - spacing - outline_width * 0.5,
		dimensions.x * GlobalState::CANVAS_WIDTH + spacing * 2. + outline_width * 2.,
		outline_width,
		br
	);
	graphics::drawRect(
		screenCoords.x - dimensions.x * GlobalState::CANVAS_WIDTH * 0.5 - spacing - outline_width * 0.5, 
		screenCoords.y,
		outline_width,
		dimensions.y * GlobalState::CANVAS_HEIGHT + spacing * 2. + outline_width * 2.,
		br
	);
	graphics::drawRect(
		screenCoords.x + dimensions.x * GlobalState::CANVAS_WIDTH * 0.5 + spacing + outline_width * 0.5,
		screenCoords.y,
		outline_width,
		dimensions.y * GlobalState::CANVAS_HEIGHT + spacing * 2. + outline_width * 2.,
		br
	);
}

void Slider::updateFunc() {
	graphics::MouseState ms;
	graphics::getMouseState(ms);
	Vector2Int screenCoords = GetScreenCoords();
	Vector2 mouseCoords = Vector2(graphics::windowToCanvasX(ms.cur_pos_x), graphics::windowToCanvasY(ms.cur_pos_y));
	
	// Yeah, this expression is huge. Deal with it lmao.
	if (mouseCoords.x >= screenCoords.x - dimensions.x * GlobalState::CANVAS_WIDTH * 0.5 - GlobalState::SLIDER_OUTLINE_SPACING - GlobalState::SLIDER_OUTLINE_WIDTH && mouseCoords.x <= screenCoords.x + dimensions.x * GlobalState::CANVAS_WIDTH * 0.5 + GlobalState::SLIDER_OUTLINE_SPACING + GlobalState::SLIDER_OUTLINE_WIDTH && mouseCoords.y >= screenCoords.y - dimensions.y * GlobalState::CANVAS_HEIGHT * 0.5 - GlobalState::SLIDER_OUTLINE_SPACING - GlobalState::SLIDER_OUTLINE_WIDTH && mouseCoords.y <= screenCoords.y + dimensions.y * GlobalState::CANVAS_HEIGHT * 0.5 + GlobalState::SLIDER_OUTLINE_SPACING + GlobalState::SLIDER_OUTLINE_WIDTH && ms.button_left_pressed) {
		isCurrentlySelected = true;
		graphics::playSound("assets\\select.mp3",0.4,false);
	}
	if (isCurrentlySelected) {
		value = max(min((mouseCoords.x - screenCoords.x + dimensions.x * GlobalState::CANVAS_WIDTH * 0.5) / (dimensions.x * GlobalState::CANVAS_WIDTH), 1.), 0.);
		float range = floatRange.max - floatRange.min;
		if (!isFloatSlider) {
			value = round(value * range) / range; // Snap
		}

		setNewValue(value);

		if (constraint) {
			switch (constraint->constraintType) {
				case ConstraintType::LESS_THAN:
					constraint->otherSlider->setNewValue(max(constraint->otherSlider->value, value));
					break;

				case ConstraintType::GREATER_THAN:
					constraint->otherSlider->setNewValue(min(constraint->otherSlider->value, value));
					break;
			}
		}

		if (ms.button_left_released) {
			isCurrentlySelected = false;
			graphics::playSound("assets\\deselect.mp3", 0.4, false);
		}
	}
}
void Slider::setNewValue(float newValue) {
	this->value = newValue;
	Text* valueText = (Text*)GlobalState::instance.objectMap[baseName + "ValueText"];
	valueText->str = getValueTextString();
	float unNormalizedValue = getUnNormalizedValue();
	if (isFloatSlider) {
		GlobalState::instance.*floatField = unNormalizedValue;
	}
	else {
		GlobalState::instance.*intField = round(unNormalizedValue);
	}

	// If the callback is valid and the value has been changed, call it
	if (previousValue != value) {
		previousValue = value;
		if (onValueChangedFunction)onValueChangedFunction();
	}
}

void Slider::spawnChildren() {

	// The good thing about this line is that I don't think an AI would ever come up with it, because it's so bad
	// An unfortunate byproduct of the fact that I decided to use Normalized Device Coordinates instead of pixels

	GlobalState::instance.objectMap.insert({baseName + "NameText",new Text(Vector2{pos.x - (dimensions.x + GlobalState::SLIDER_OUTLINE_SPACING * 2.f / GlobalState::CANVAS_WIDTH + GlobalState::SLIDER_OUTLINE_WIDTH *2.f / GlobalState::CANVAS_WIDTH),pos.y + (GlobalState::SLIDER_OUTLINE_SPACING * 2.f / GlobalState::CANVAS_HEIGHT + GlobalState::SLIDER_OUTLINE_WIDTH * 2.f / GlobalState::CANVAS_HEIGHT + dimensions.y + GlobalState::SLIDER_TEXT_SPACING * 2.f / GlobalState::CANVAS_HEIGHT)},text)});
	GlobalState::instance.objectMap.insert({ baseName + "ValueText",new Text(Vector2{pos.x + (dimensions.x + GlobalState::SLIDER_OUTLINE_SPACING * 4.f / GlobalState::CANVAS_WIDTH + GlobalState::SLIDER_OUTLINE_WIDTH * 2.f / GlobalState::CANVAS_WIDTH),pos.y - (dimensions.y + GlobalState::SLIDER_OUTLINE_SPACING * 2.f / GlobalState::CANVAS_HEIGHT)},getValueTextString()) });
}

float Slider::getUnNormalizedValue() {
	return (floatRange.max - floatRange.min) * value + floatRange.min;
}

string Slider::getValueTextString() {
	
	if (isFloatSlider) {
		return std::format("{:.2f}", getUnNormalizedValue());
	}
	else {
		return std::format("{:.0f}", round(getUnNormalizedValue()));
	}
}