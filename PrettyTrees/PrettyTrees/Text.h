#pragma once
#include <string>
#include <iostream>
#include "Drawable.h"
class Text : public Drawable {
public:
	std::string str;
	int fontSize;
	void draw() override;
	void updateFunc() override {}
	void spawnChildren() override{}
	Text(Drawable::Vector2 pos,std::string str, int fontSize = 40){
		this->fontSize = fontSize;
		this->str = str;
		this->pos = pos;
	}
};