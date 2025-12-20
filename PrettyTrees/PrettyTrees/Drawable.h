#pragma once
#include <string>
class Drawable {
public:
	struct Vector2 {
		float x;
		float y;
	};
	struct Vector2Int {
		int x;
		int y;
	};
	std::string baseName;
	Vector2 pos;
	Vector2Int GetScreenCoords();
	// TODO Get more comfortable with virtual functions
	virtual void draw() = 0;
	virtual void updateFunc() = 0;
	virtual void spawnChildren() = 0;
};