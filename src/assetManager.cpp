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
    return it->second;

}

// Get a textureAtlas
TextureAtlas& AssetManager::getTextureAtlas(const std::string& name){
    auto it = textureAtlases.find(name);
    return it->second;
}

// Get a sound
Sound& AssetManager::getSound(const std::string& name) {
    auto it = sounds.find(name);
    return it->second;
}

// Get a music
Music& AssetManager::getMusic(const std::string& name) {
    auto it = musics.find(name);
    return it->second;
}

// Get a font
Font& AssetManager::getFont(const std::string& name) {
    auto it = fonts.find(name);
    return it->second;
}



// Load a textureAtlas
void AssetManager::loadTextureAtlas(const std::string& name){
    std::cout << "Loading textureAtlas: " << name << std::endl;
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
        Section section;

        // Write the data to the variables
        ss >> section.name >> section.rect.x >> section.rect.y >> section.rect.width >> section.rect.height;

        // Store the section in the TextureAtlas
        textureAtlas.sections.push_back(section);
    }

    // Close the file
    file.close();

    // Store the textureAtlas
    textureAtlases[name] = textureAtlas;
}

// Load a texture
void AssetManager::loadTexture(const std::string& name, const std::string& filename) {
    std::cout << "Loading texture: " << filename << std::endl;
    textures[name] = LoadTexture((assetPath + "/" + texturesDirectory + "/" + filename).c_str());;
}

// Load a sound
void AssetManager::loadSound(const std::string& name, const std::string& filename) {
    std::cout << "Loading sound: " << filename << std::endl;
    sounds[name] = LoadSound((assetPath + "/" + soundsDirectory + "/" + filename).c_str());;
}

void AssetManager::loadMusic(const std::string& name, const std::string& filename) {
    std::cout << "Loading music: " << filename << std::endl;
    musics[name] = LoadMusicStream((assetPath + "/" + soundsDirectory + "/" + filename).c_str());;
}

// Load a font
void AssetManager::loadFont(const std::string& name, const std::string& filename, int fontSize) {
    std::cout << "Loading font: " << filename << std::endl;
    fonts[name] = LoadFontEx((assetPath + "/" + fontsDirectory + "/" + filename).c_str(), fontSize, 0, 250);
}

// Search for a directory starting from the working directory. The AssetManager will look for resources in this directory
void AssetManager::searchAssetsDirectoryPath(std::string folderName, size_t searchDepth){
    std::filesystem::path currentDir = GetWorkingDirectory();
    std::cout << "Searching for " << folderName << " directory starting from " << currentDir << std::endl;
    
    for (size_t i = 0; i < searchDepth; i++){
        std::filesystem::path potentialPath = currentDir / folderName;
        if (std::filesystem::exists(potentialPath) && std::filesystem::is_directory(potentialPath)) {
            assetPath = potentialPath.string(); // Folder found exit for loop
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
    for (auto& [key, music] : musics) {
        UnloadMusicStream(music);
    }
    for (auto& [key, font] : fonts) {
        UnloadFont(font);
    }
    textures.clear();
    sounds.clear();
    musics.clear();
    fonts.clear();
}

