#include <iostream>
#include <string>
#include <random>
#include "GlobalState.h"
#include "graphics.h"
#include "Text.h"
#include "Drawable.h"
#include "Slider.h"
#include "Tree.h"
using namespace std::chrono;

GlobalState GlobalState::instance;

void GlobalState::init()
{
    
    // Text
    objectMap.insert({ "fpsText",new Text(Drawable::Vector2(0.85,0.93), "FPS: ")});
    objectMap.insert({ "fullscreenText", new Text(Drawable::Vector2{-0.98,-0.97}, "Press F to fullscreen") });
    objectMap.insert({ "spaceToGenerateText", new Text(Drawable::Vector2{0.164,-0.8}, "Press SPACE") });

    // Treew
    objectMap.insert({ "tree",new Tree(Drawable::Vector2{0.28,-0.7}) });
    Tree* tree = (Tree*)(objectMap["tree"]);
    
    // Sliders
    objectMap.insert({ "nodeAmountSlider",new Slider(Drawable::Vector2{-0.75,0.5},Drawable::Vector2{0.2,0.02},"Nodes",&GlobalState::nodes,Slider::IntRange(50,2000), [tree]() { tree->generateTree(); })});
    objectMap.insert({ "swayMultiplierSlider",new Slider(Drawable::Vector2{-0.75,0.3},Drawable::Vector2{0.2,0.02},"Sway",&GlobalState::sway,Slider::FloatRange(0.04,3.)) });
    objectMap.insert({ "leafSwayMultiplierSlider",new Slider(Drawable::Vector2{-0.75,0.1},Drawable::Vector2{0.2,0.02},"Leaf sway multiplier",&GlobalState::leafSwayMultiplier,Slider::FloatRange(0.04,3.)) });
    objectMap.insert({ "minBranchesSlider",new Slider(Drawable::Vector2{-0.75,-0.1},Drawable::Vector2{0.2,0.02},"Min branches",&GlobalState::minBranchesPerNode,Slider::IntRange(1,7), [tree]() { tree->generateTree(); }) });
    objectMap.insert({ "maxBranchesSlider",new Slider(Drawable::Vector2{-0.75,-0.3},Drawable::Vector2{0.2,0.02},"Max branches",&GlobalState::maxBranchesPerNode,Slider::IntRange(1,7), [tree]() { tree->generateTree(); }) });
    objectMap.insert({ "lengthSlider",new Slider(Drawable::Vector2{-0.75,-0.5},Drawable::Vector2{0.2,0.02},"Length",&GlobalState::length,Slider::FloatRange(1.,30.), [tree]() { tree->generateTree(); }) });

    Slider* minBranchesSlider = (Slider*)(objectMap["minBranchesSlider"]);
    Slider* maxBranchesSlider = (Slider*)(objectMap["maxBranchesSlider"]);

    //min should be less than max
	minBranchesSlider->setConstraint(maxBranchesSlider, Slider::LESS_THAN);
    //max should be greater than min
	maxBranchesSlider->setConstraint(minBranchesSlider, Slider::GREATER_THAN);

    // Spawn object's children. Give as input the object's name so they can use it to make new string identifiers
    std::vector<std::string> keysToProcess;
    for (auto& pair : objectMap) {
        keysToProcess.push_back(pair.first);
    }

    for (const auto& key : keysToProcess) {
		objectMap[key]->baseName = key;
        objectMap[key]->spawnChildren();
    }
}

void GlobalState::cleanUp() {
    for (auto& pair : objectMap) {
        std::cout << "Deleting " + pair.first << std::endl;
        delete pair.second;
    }
    objectMap.clear();
}


void GlobalState::update(float dt)
{
    static std::mt19937 rng{ std::random_device{}() };

    std::uniform_int_distribution<int> soundFileDist(
        1,
        5
    );

    //Check if 300ms have passed since the last fullscreen toggle
    if (duration_cast<milliseconds>(high_resolution_clock::now() - lastFKeyPress).count() > 300) {
        if (graphics::getKeyState(graphics::scancode_t::SCANCODE_F)) {
            isFullscreen = !isFullscreen;
            graphics::setFullScreen(isFullscreen);
            lastFKeyPress = high_resolution_clock::now();
            graphics::playSound("assets\\fullscreen.mp3",1.0,false);
        }
    }    
    //Check if 200ms have passed since the last tree generation
    if (duration_cast<milliseconds>(high_resolution_clock::now() - lastSpaceKeyPress).count() > 300) {
        if (graphics::getKeyState(graphics::scancode_t::SCANCODE_SPACE)) {
            Tree* tree = (Tree*)objectMap["tree"];
			tree->generateTree();
            lastSpaceKeyPress = high_resolution_clock::now();
            std::string generateSoundPath = std::format("assets\\generate_tree_{}.mp3", soundFileDist(rng));
            graphics::playSound(generateSoundPath, 0.7, false);
        }
    }
    // Check if 200ms have passed since the last text update
    if (duration_cast<milliseconds>(high_resolution_clock::now() - lastFPSTextUpdate).count() > 200) {
        Text* fpsText = (Text*)objectMap["fpsText"];
        fpsText->str = "FPS: " + std::to_string(int((1.0/dt)));
        lastFPSTextUpdate = high_resolution_clock::now();
    }

    // Play music after one second
    if ((duration_cast<milliseconds>(high_resolution_clock::now() - programStartTime).count()) > 1000 && !isPlayingMusic)
    {
        isPlayingMusic = true;
        graphics::playSound("assets\\original_music.mp3", 1.0, true);
    }

}

void GlobalState::draw()
{
    //Gradient Background
    graphics::Brush br;
    setRGBValue(br.fill_color, 231, 100, 88);
    setRGBValue(br.fill_secondary_color, 0, 100, 67);
    br.gradient = true;
    br.gradient_dir_u = 0.0f;
    br.gradient_dir_v = 1.0f;
    br.outline_opacity = 0.0f;
    graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
    br.gradient = false;


    for (auto& pair : objectMap) {
        pair.second->updateFunc();
        pair.second->draw();
    }
}

void GlobalState::setRGBValue(float* arr, int r, int g, int b)
{
    arr[0] = r / 255.0f;
    arr[1] = g / 255.0f;
    arr[2] = b / 255.0f;
}

void GlobalState::setRGBValue(float* arr, int brightness)
{
    arr[0] = arr[1] = arr[2] = brightness / 255.0f;
}

void GlobalState::setRGBValue(float* arr, Color color)
{
    arr[0] = color.r / 255.0f;
    arr[1] = color.g / 255.0f;
    arr[2] = color.b / 255.0f;
}

GlobalState::Color GlobalState::lerpColors(Color color1, Color color2, float lerpValue) {
    Color newColor;
	newColor.r = lerpValue * color2.r + (1. - lerpValue) * color1.r;
    newColor.g = lerpValue * color2.g + (1. - lerpValue) * color1.g;
    newColor.b = lerpValue * color2.b + (1. - lerpValue) * color1.b;
    return newColor;
}
