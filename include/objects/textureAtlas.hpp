#pragma once

#include <unordered_map>
#include <string>
#include <raylib.h>

class TextureAtlas{
public:
    std::unordered_map<std::string, Rectangle> sections;
    Texture2D texture;
};


