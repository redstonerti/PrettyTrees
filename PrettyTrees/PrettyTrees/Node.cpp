#include <iostream>
#include <numbers>
#include "Node.h"
#include "graphics.h"
#include "GlobalState.h"

void Node::draw() {
	graphics::Brush br;
	
	br.outline_opacity = 0.;
	
	Vector2Int pixelCoords = getPixelCoords(x, y);
	GlobalState::setRGBValue(br.fill_color, getColor());
	graphics::drawDisk(pixelCoords.x, pixelCoords.y, getNodeSize() * 10., br);

	if (parent != NULL) {
		float angleAddition = sin(graphics::getGlobalTime() * 0.001 * 0.3 + swayOffset) * GlobalState::instance.sway * pow(depth,GlobalState::instance.leafSwayMultiplier) * 0.01;
		x = parent->x + cos(angleRad + angleAddition + std::numbers::pi * 0.5) * length * GlobalState::instance.length;
		y = parent->y + sin(angleRad + angleAddition + std::numbers::pi * 0.5) * length * GlobalState::instance.length;

		// Draw branch
		Vector2Int rectPixelCoords = getPixelCoords((x + parent->x) * 0.5, (y + parent->y) * 0.5);
		float dx = x - parent->x;
		float dy = y - parent->y;
		float angleDeg = std::atan2(dy, dx) * 180.0f / std::numbers::pi;

		GlobalState::setRGBValue(br.fill_secondary_color, getColor());
		GlobalState::setRGBValue(br.fill_color, parent->getColor());
		br.gradient = true;
		br.gradient_dir_u = 1.0;
		br.gradient_dir_v = 0.0;

		graphics::setOrientation(angleDeg);
		graphics::drawRect(rectPixelCoords.x, rectPixelCoords.y,sqrt(pow((dx),2.) + pow((dy), 2.)) * scale,getNodeSize() * 10. * 2.,br);
		graphics::resetPose();

		br.gradient = false;
	}

	for (Node* child : children) {
		child->draw();
	}
}

void Node::removeChildren() {
	for (Node* child : children) {
		child->removeChildren();
		delete child;
	}
	children.clear();
}

GlobalState::Color Node::getColor() {
	return GlobalState::lerpColors(GlobalState::instance.trunkColor, GlobalState::instance.leafColor, getDepthPercentage() * getDepthPercentage());
}

float Node::getDepthPercentage() {
	return (float)depth / Node::maxDepth;
}
float Node::getNodeSize() {
	return (1. - getDepthPercentage()) * GlobalState::instance.trunkSize + getDepthPercentage() * GlobalState::instance.leafSize;
}
int Node::x0 = 0;
int Node::y0 = 0;
float Node::scale = 10.;
int Node::maxDepth = 0;