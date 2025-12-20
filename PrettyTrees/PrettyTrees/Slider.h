#pragma once
#include <functional>
#include <optional>
#include "Drawable.h"
#include "GlobalState.h"


// Admittedly, this class could have been a parent class for FloatSlider and IntSlider, but honestly, that probably would have been more work.
// The differences are a couple of lines at most, so I don't think subclasses are warranted.

class Slider : public Drawable {
private:
	// Pointers to member fields are cool
	float GlobalState::* floatField;
	int GlobalState::* intField;
public:
	enum ConstraintType {
		LESS_THAN,
		GREATER_THAN
	};
	struct Constraint {
		Slider* otherSlider;
		ConstraintType constraintType;
	};
	struct FloatRange {
		float min;
		float max;
	};
	struct IntRange {
		int min;
		int max;
	};
	Vector2 dimensions;
	float value;
	float previousValue;
	bool isCurrentlySelected;
	bool isFloatSlider;
	FloatRange floatRange;
	IntRange intRange;
	std::string text;
	std::function<void()> onValueChangedFunction;
	std::optional<Constraint> constraint;
	void draw() override;
	void updateFunc() override;
	void spawnChildren();
	void setNewValue(float value);
	std::string getValueTextString();
	float getUnNormalizedValue();
	Slider(Vector2 pos, Vector2 dimensions, std::string text,float GlobalState::* floatField, FloatRange floatRange, std::function<void()> onValueChangedFunction = {}) {
		this->pos = pos;
		this->dimensions = dimensions;
		//Gets normalized value from default and range (min 3, max 5, default 4 -> (4-3)/(5-3) = 0.5)
		this->value = ((GlobalState::instance.*floatField) - floatRange.min) / (floatRange.max - floatRange.min);
		this->previousValue = this->value;
		this->isCurrentlySelected = false;
		this->text = text;
		this->isFloatSlider = true;
		this->floatRange = floatRange;
		this->floatField =floatField;
		this->onValueChangedFunction = onValueChangedFunction;
		this->constraint = std::nullopt;
	}
	Slider(Vector2 pos, Vector2 dimensions, std::string text, int GlobalState::* intField, IntRange intRange, std::function<void()> onValueChangedFunction = {}) {
		this->pos = pos;
		this->dimensions = dimensions;
		this->value = ((float)(GlobalState::instance.*intField) - (float)intRange.min) / ((float)intRange.max - (float)intRange.min);
		this->previousValue = this->value;
		this->isCurrentlySelected = false;
		this->text = text;
		this->isFloatSlider = false;
		this->floatRange = FloatRange((float)intRange.min,(float)intRange.max);
		this->intField = intField;
		this->onValueChangedFunction = onValueChangedFunction;
		this->constraint = std::nullopt;
	}
	void setConstraint(Slider* otherSlider, ConstraintType constraintType) {
		constraint = Constraint{ otherSlider, constraintType };
	}
};