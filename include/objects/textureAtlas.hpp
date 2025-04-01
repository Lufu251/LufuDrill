#pragma once

#include <vector>
#include <string>
#include <raylib.h>

class Section{
    public:
    std::string name;
    Rectangle rect;
};

class TextureAtlas{
public:
    std::vector<Section> sections;
    Texture2D texture;
};