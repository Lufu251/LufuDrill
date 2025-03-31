#pragma once

#include <vector>
#include <string>
#include <raylib.h>

class TextureAtlas{
public:
    std::vector<Rectangle> sections;
    Texture2D texture;
};


