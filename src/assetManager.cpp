#include <assetManager.hpp>

#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>

// Access the singleton instance
AssetManager& AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

// Get a texture
Texture2D& AssetManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    else{
        throw std::runtime_error("Texture not found and was likely not preloaded: " + name);
        return it->second;
    }
}

// Get a textureAtlas
TextureAtlas& AssetManager::getTextureAtlas(const std::string& name){
    auto it = textureAtlases.find(name);
    if (it != textureAtlases.end()) {
        return it->second;
    }
    else{
        throw std::runtime_error("TextureAtlases not found and was likely not preloaded: " + name);
        return it->second;
    }
}

// Get a sound
Sound& AssetManager::getSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        return it->second;
    }
    else{
        throw std::runtime_error("Sound not found and was likely not preloaded: " + name);
        return it->second;
    }
}

// Get a font
Font& AssetManager::getFont(const std::string& name) {
    auto it = fonts.find(name);
    if (it != fonts.end()) {
        return it->second;
    }
    else{
        throw std::runtime_error("Font not found and was likely not preloaded: " + name);
        return it->second;
    }
}

// Load a texture
void AssetManager::loadTexture(const std::string& name, const std::string& filename) {
    Texture2D texture = LoadTexture((assetPath + "/" + texturesDirectory + "/" + filename).c_str());
    if (texture.id == 0) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }
    textures[name] = texture;
}

// Load a textureAtlas
void AssetManager::loadTextureAtlas(const std::string& name){
    // Load texture to textureAtlas
    TextureAtlas textureAtlas;
    textureAtlas.texture = LoadTexture((assetPath + "/" + texturesDirectory + "/" + name + "/" + name + ".png").c_str());

    if (textureAtlas.texture.id == 0) {
        throw std::runtime_error("Failed to load texture to textureAtlas: " + name + ".png");
    }

    // Load sections to textureAtlas
    std::ifstream file(assetPath + "/" + texturesDirectory + "/" + name + "/" + name + ".data");

    // Check if the file is successfully opened
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open textureAtlas data file: " + name +".data");
    }

    // Read all the data from the file untile nothing is left
    std::string data;
    while (std::getline(file, data, ';')) {
        std::istringstream ss(data);
        std::string sectionName;
        Rectangle rect;

        // Write the data to the variables
        ss >> sectionName >> rect.x >> rect.y >> rect.width >> rect.height;

        // Store the section in the TextureAtlas
        textureAtlas.sections[sectionName] = std::move(rect);
    }

    // Close the file
    file.close();

    // Store the textureAtlas
    textureAtlases[name] = std::move(textureAtlas);
}

// Load a sound
void AssetManager::loadSound(const std::string& name, const std::string& filename) {
    Sound sound = LoadSound((assetPath + "/" + soundsDirectory + "/" + filename).c_str());
    if (!sound.stream.buffer) {
        throw std::runtime_error("Failed to load sound: " + filename);
    }
    sounds[name] = sound;
}

// Load a font
void AssetManager::loadFont(const std::string& name, const std::string& filename, int fontSize) {
    Font font = LoadFontEx((assetPath + "/" + fontsDirectory + "/" + filename).c_str(), fontSize, nullptr, 0);
    if (font.baseSize == 0) {
        throw std::runtime_error("Failed to load font: " + filename);
    }
    fonts[name] = font;
}

// Search for a directory starting from the working directory. The AssetManager will look for resources in this directory
void AssetManager::searchAssetsDirectoryPath(std::string folderName, int searchDepth){
    std::filesystem::path currentDir = GetWorkingDirectory();
    std::cout << "Searching for " << folderName << " directory starting from " << currentDir << std::endl;
    
    for (size_t i = 0; i < searchDepth; i++){
        std::filesystem::path potentialPath = currentDir / folderName;
        if (std::filesystem::exists(potentialPath) && std::filesystem::is_directory(potentialPath)) {
            assetPath = potentialPath; // Folder found exit for loop
            std::cout << "Path to directory " << folderName << " found. Path is " << assetPath << std::endl;
            break;
        }
        currentDir = currentDir.parent_path(); // Move up one level
    }

    if(assetPath == ""){
        throw std::runtime_error("Failed to find directory: " + folderName);
    }
}

void AssetManager::setTexturesDirectory(const std::string& folderName){
    texturesDirectory = folderName;
}

void AssetManager::setSoundsDirectory(const std::string& folderName){
    soundsDirectory = folderName;
}

void AssetManager::setFontsDirectory(const std::string& folderName){
    fontsDirectory = folderName;
}

// Cleanup all resources
void AssetManager::cleanup() {
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

