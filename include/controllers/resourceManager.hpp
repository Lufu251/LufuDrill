#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>

#include <raylib.h>

class ResourceManager
{
private:
    // Private constructor
    ResourceManager() = default;

    // Disable copy and assignment
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Private helper methods for loading
    void loadTexture(const std::string& filename);
    void loadSound(const std::string& filename);
    void loadFont(const std::string& filename, int fontSize);

    // Private string for data folder path
    std::string path;
    std::string texturesDirectory = "textures";
    std::string soundsDirectory = "sounds";
    std::string fontsDirectory = "fonts";

    // Containers for resources
    std::unordered_map<std::string, Texture2D> textures;
    std::unordered_map<std::string, Sound> sounds;
    std::unordered_map<std::string, Font> fonts;
    
public:
    // Access the singleton instance
    static ResourceManager& getInstance();

    // Get texture by filename
    Texture2D& getTexture(const std::string& filename);

    // Get sound by filename
    Sound& getSound(const std::string& filename);

    // Get font by filename
    Font& getFont(const std::string& filename);

    // Preload assets (optional)
    void preloadTexture(const std::string& filename);
    void preloadSound(const std::string& filename);
    void preloadFont(const std::string& filename, int fontSize);

    // Search for a directory and set the path to this directory
    void setAssetDirectoryPath(std::string dir, int searchDepth);

    // Set textures directory. Default is "textures"
    void setTexturesDirectory(const std::string& folderName);

    // Set sounds directory. Default is "sounds"
    void setSoundsDirectory(const std::string& folderName);

    // Set fonts directory. Default is "fonts"
    void setFontsDirectory(const std::string& folderName);

    // Cleanup loaded resources
    void cleanup();
};
