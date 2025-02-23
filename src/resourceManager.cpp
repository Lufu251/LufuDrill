#include <resourceManager.hpp>

#include <stdexcept>
#include <filesystem>

// Access the singleton instance
ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

// Get a texture
Texture2D& ResourceManager::getTexture(const std::string& filename) {
    auto it = textures.find(filename);
    if (it != textures.end()) {
        return it->second;
    }

    // Load the texture if not already loaded
    loadTexture(filename);
    return textures[filename];
}

// Get a sound
Sound& ResourceManager::getSound(const std::string& filename) {
    auto it = sounds.find(filename);
    if (it != sounds.end()) {
        return it->second;
    }

    // Load the sound if not already loaded
    loadSound(filename);
    return sounds[filename];
}

// Get a font
Font& ResourceManager::getFont(const std::string& filename) {
    auto it = fonts.find(filename);
    if (it != fonts.end()) {
        return it->second;
    }

    // Load the font with a default size if not already loaded
    loadFont(filename, 20); // Default font size
    return fonts[filename];
}

// Preload texture
void ResourceManager::preloadTexture(const std::string& filename) {
    // Check if it was already loaded
    if (textures.find(filename) == textures.end()) {
        loadTexture(filename);
    }
}

// Preload sound
void ResourceManager::preloadSound(const std::string& filename) {
    // Check if it was already loaded
    if (sounds.find(filename) == sounds.end()) {
        loadSound(filename);
    }
}

// Preload font
void ResourceManager::preloadFont(const std::string& filename, int fontSize) {
    // Check if it was already loaded
    if (fonts.find(filename) == fonts.end()) {
        loadFont(filename, fontSize);
    }
}

// Load a texture
void ResourceManager::loadTexture(const std::string& filename) {
    Texture2D texture = LoadTexture((path + "/" + texturesDirectory + "/" + filename).c_str());
    if (texture.id == 0) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }
    textures[filename] = texture;
}

// Load a sound
void ResourceManager::loadSound(const std::string& filename) {
    Sound sound = LoadSound((path + "/" + soundsDirectory + "/" + filename).c_str());
    if (!sound.stream.buffer) {
        throw std::runtime_error("Failed to load sound: " + filename);
    }
    sounds[filename] = sound;
}

// Load a font
void ResourceManager::loadFont(const std::string& filename, int fontSize) {
    Font font = LoadFontEx((path + "/" + fontsDirectory + "/" + filename).c_str(), fontSize, nullptr, 0);
    if (font.baseSize == 0) {
        throw std::runtime_error("Failed to load font: " + filename);
    }
    fonts[filename] = font;
}

// Search for a directory starting from the working directory. The ResourceManager will look for resources in this directory
void ResourceManager::setAssetDirectoryPath(std::string folderName, int searchDepth){
    std::filesystem::path currentDir = GetWorkingDirectory();
    std::cout << "Searching for " << folderName << " directory starting from " << currentDir << std::endl;
    
    for (size_t i = 0; i < searchDepth; i++){
        std::filesystem::path potentialPath = currentDir / folderName;
        if (std::filesystem::exists(potentialPath) && std::filesystem::is_directory(potentialPath)) {
            path = potentialPath; // Folder found exit for loop
            std::cout << "Path to directory " << folderName << " found. Path is " << path << std::endl;
            break;
        }
        currentDir = currentDir.parent_path(); // Move up one level
    }

    if(path == ""){
        throw std::runtime_error("Failed to find directory: " + folderName);
    }
}

void ResourceManager::setTexturesDirectory(const std::string& folderName){
    texturesDirectory = folderName;
}

void ResourceManager::setSoundsDirectory(const std::string& folderName){
    soundsDirectory = folderName;
}

void ResourceManager::setFontsDirectory(const std::string& folderName){
    fontsDirectory = folderName;
}

// Cleanup all resources
void ResourceManager::cleanup() {
    for (auto& [key, texture] : textures) {
        UnloadTexture(texture);
    }
    for (auto& [key, sound] : sounds) {
        UnloadSound(sound);
    }
    for (auto& [key, font] : fonts) {
        UnloadFont(font);
    }
    textures.clear();
    sounds.clear();
    fonts.clear();
}

