#pragma once

#include <unordered_map>
#include <string>

#include <raylib.h>

#include <textureAtlas.hpp>

class AssetManager{
private:
    // Private string for data folder path
    std::string assetPath;

    // Directory names
    std::string texturesDirectory = "textures";
    std::string soundsDirectory = "sounds";
    std::string fontsDirectory = "fonts";

    // Containers for resources
    std::unordered_map<std::string, TextureAtlas> textureAtlases;
    std::unordered_map<std::string, Texture2D> textures;
    std::unordered_map<std::string, Sound> sounds;
    std::unordered_map<std::string, Music> musics;
    std::unordered_map<std::string, Font> fonts;
    
public:
    // Private constructor
    AssetManager() = default;

    // Get assets by filename
    TextureAtlas& getTextureAtlas(const std::string& name);
    Texture2D& getTexture(const std::string& name);
    Sound& getSound(const std::string& name);
    Music& getMusic(const std::string& name);
    Font& getFont(const std::string& name);

    // Loading assets
    void loadTextureAtlas(const std::string& name);
    void loadTexture(const std::string& name, const std::string& filename);
    void loadSound(const std::string& name, const std::string& filename);
    void loadMusic(const std::string& name, const std::string& filename);
    void loadFont(const std::string& name, const std::string& filename, int fontSize);

    // Search for a directory and set the path to this directory
    void searchAssetsDirectoryPath(std::string dir, size_t searchDepth);

    // Set directories for assets
    void setTexturesDirectory(const std::string& folderName);
    void setSoundsDirectory(const std::string& folderName);
    void setFontsDirectory(const std::string& folderName);

    // Cleanup loaded resources
    void cleanup();
};