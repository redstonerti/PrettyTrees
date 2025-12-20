#pragma once
#include <chrono>
#include <unordered_map>
#include <string>
#include "Drawable.h"

using namespace std::chrono;
using namespace std;

class GlobalState {
private:
    bool isFullscreen;
    time_point<high_resolution_clock> lastFKeyPress;
    time_point<high_resolution_clock> lastFPSTextUpdate;
    time_point<high_resolution_clock> programStartTime;
    time_point<high_resolution_clock> lastSpaceKeyPress;
    GlobalState() : isFullscreen(false), lastFKeyPress(high_resolution_clock::now()), lastSpaceKeyPress(high_resolution_clock::now()), lastFPSTextUpdate(high_resolution_clock::now()), programStartTime(high_resolution_clock::now()) {}

public:
    struct Color {
        int r;
        int g;
        int b;
    };

    static const int CANVAS_WIDTH = 1920;
    static const int CANVAS_HEIGHT = 1080;
    static const int SLIDER_OUTLINE_SPACING = 6;
    static const int SLIDER_OUTLINE_WIDTH = 6;
    static const int SLIDER_TEXT_SPACING = 6;

    int nodes = 500;
    float leafSize = 0.1;
    float trunkSize = 1.;
	int minBranchesPerNode = 1;
    int maxBranchesPerNode = 3;
    float angle = 0.;
    float length = 20.;
    float sway = 1.;
	float leafSwayMultiplier = 1.5;
    bool isPlayingMusic = false;
	Color leafColor = Color{ 153, 229, 80 };
	Color trunkColor = Color{ 204,65,0 };
    
    // String keys are okay to use here for convenience as the total number of objects in the program will be very low
    unordered_map<string, Drawable*> objectMap;
    static GlobalState instance;

    void init();
    void update(float dt);
    void draw();
    void cleanUp();

    static void setRGBValue(float* arr, int r, int g, int b);
    static void setRGBValue(float* arr, int brightness);
	static void setRGBValue(float* arr, Color color);

    static Color lerpColors(Color color1, Color color2, float lerpValue);
};