#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <numbers>
#include "GlobalState.h"
class Node{
public:
	static int x0;
	static int y0;
	static float scale;
	struct Vector2Int {
		int x;
		int y;
	};
	static int maxDepth;
	Node* parent = NULL;
	std::vector<Node*> children;
	int depth;
	float angleRad;
	float length;
	float x;
	float y;
	float swayOffset;
	void draw();
	void removeChildren();
	GlobalState::Color getColor();
	float getDepthPercentage();
	float getNodeSize();
	Vector2Int getPixelCoords(float x, float y) {
		return Vector2Int(x0 + (int)(x * scale), y0 - (int)(y * scale));//Tree::x0 + (int)(x * Tree::scale),Tree::y0 + (int)(y * Tree::scale));
	}
	Node() {
		depth = 0;
		angleRad = 0;
		x = 0.;
		y = 0.;
		length = 0.;
		swayOffset = 0.;
	}
	Node(float angleRad, float length, Node* parent = NULL) {
		this->depth = parent ? parent->depth + 1 : 0;
		this->parent =parent;
		if (parent == NULL) {
			this->angleRad = angleRad;
		}
		else {
			this->angleRad = parent->angleRad + angleRad;
			parent->children.push_back(this);
		}
		this->length = length;
		if (this->depth > Node::maxDepth) {
			Node::maxDepth = this->depth;
		}
		this->length /= (log((float)this->depth) + 1.);

		static std::mt19937 rng{ std::random_device{}() };

		std::uniform_real_distribution<float> swayOffsetDist(
			0.,
			std::numbers::pi_v<float> * 0.5
		);

		swayOffset = swayOffsetDist(rng);
	}
};