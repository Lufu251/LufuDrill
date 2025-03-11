#include <dataManager.hpp>

#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

// Access the singleton instance
DataManager& DataManager::getInstance() {
    static DataManager instance;
    return instance;
}

// Search for a directory starting from the working directory. The AssetManager will look for resources in this directory
void DataManager::searchDataDirectoryPath(std::string folderName, int searchDepth){
    std::filesystem::path currentDir = GetWorkingDirectory();
    std::cout << "Searching for " << folderName << " directory starting from " << currentDir << std::endl;
    
    for (size_t i = 0; i < searchDepth; i++){
        std::filesystem::path potentialPath = currentDir / folderName;
        if (std::filesystem::exists(potentialPath) && std::filesystem::is_directory(potentialPath)) {
            dataPath = potentialPath; // Folder found exit for loop
            std::cout << "Path to directory " << folderName << " found. Path is " << dataPath << std::endl;
            break;
        }
        currentDir = currentDir.parent_path(); // Move up one level
    }

    if(dataPath == ""){
        throw std::runtime_error("Failed to find directory: " + folderName);
    }
}

void DataManager::loadSettingConfig(const std::string& name){
    std::ifstream file(dataPath + "/" + configDirectory + "/" + name);

    // Check if opening file failed
    if (!file) {
        std::cerr << "Error opening file!\n";
    }

    nlohmann::json settings;
    file >> settings;

    // Parse each setting section

    // GENERAL

    // WINDOW
    screenWidth = settings["window"]["screenWidth"];
    screenHeight = settings["window"]["screenHeight"];

    // PHYSICS

}

void DataManager::loadToolConfig(const std::string& name){
    std::ifstream file(dataPath + "/" + configDirectory + "/" + name);

    // Check if opening file failed
    if (!file) {
        std::cerr << "Error opening file!\n";
    }

    nlohmann::json jsonData;
    file >> jsonData;

    // Parse each tool type
    for (const auto& item : jsonData["drill"]) {
        drills.emplace_back(item["name"], item["cost"], item["power"]);
    }
    for (const auto& item : jsonData["fueltank"]) {
        fuelTanks.emplace_back(item["name"], item["cost"], item["fuel"]);
    }
    for (const auto& item : jsonData["hull"]) {
        hulls.emplace_back(item["name"], item["cost"], item["health"]);
    }
    for (const auto& item : jsonData["cargobay"]) {
        cargoBays.emplace_back(item["name"], item["cost"], item["capacity"]);
    }
    for (const auto& item : jsonData["engine"]) {
        engines.emplace_back(item["name"], item["cost"], item["power"]);
    }
    for (const auto& item : jsonData["equipment"]) {
        equipments.emplace_back(item["name"], item["cost"]);
    }
}