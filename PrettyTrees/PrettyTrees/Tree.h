#pragma once
#include "Drawable.h"
#include "Node.h"
#include <queue>
class Tree : public Drawable {
public:
	Node startingNode;
	void draw() override;
	void updateFunc() {}
	void spawnChildren() {}
	void generateTree();
	Tree(Vector2 pos) {
		this->pos = pos;
		Node::x0 = GetScreenCoords().x;
		Node::y0 = GetScreenCoords().y;
		generateTree();
	}
};