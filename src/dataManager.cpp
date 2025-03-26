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

// Search for a directory starting from the working directory. The DataManager will look for resources in this directory
void DataManager::searchDataDirectoryPath(std::string folderName, size_t searchDepth){
    std::filesystem::path currentDir = GetWorkingDirectory();
    std::cout << "Searching for " << folderName << " directory starting from " << currentDir << std::endl;
    
    for (size_t i = 0; i < searchDepth; i++){
        std::filesystem::path potentialPath = currentDir / folderName;
        if (std::filesystem::exists(potentialPath) && std::filesystem::is_directory(potentialPath)) {
            dataPath = potentialPath.string(); // Folder found exit for loop
            std::cout << "Path to directory " << folderName << " found. Path is " << dataPath << std::endl;
            break;
        }
        currentDir = currentDir.parent_path(); // Move up one level
    }
    if(dataPath == ""){
        throw std::runtime_error("Failed to find directory: " + folderName);
    }
}

void DataManager::loadGameState(const std::string& name){
    // Read JSON from a file
    std::ifstream fromFile(dataPath + "/" + savesDirectory + "/" + name);
    if (!fromFile.is_open()) {
        std::cerr << "Failed to open file!\n";
    }

    nlohmann::json json;
    fromFile >> json; // Parse JSON from file
    fromFile.close();

    // Load Map ---------------------------------------------------
    json.at("world").get_to(world); // Convert JSON to Grid object
    // Init map, set position and size ob aabb
    for (size_t x = 0; x < world.mGrid.gridSizeX; x++){
        for (size_t y = 0; y < world.mGrid.gridSizeY; y++){
            world.mGrid(x,y).position = {static_cast<float>(x * world.mBlockSize), static_cast<float>(y * world.mBlockSize)};
            world.mGrid(x,y).size = {static_cast<float>(world.mBlockSize), static_cast<float>(world.mBlockSize)};
        }
    }

    // Load Player ---------------------------------------------------
    //json.at("player").get_to(player);

    // Load Other ---------------------------------------------------
    // ...
}

void DataManager::saveGameState(const std::string& name){
    // Write JSON to a file
    nlohmann::json json = world;
    json = {
        {"world", world} // Convert the Grid object to JSON
    }; 


    // Write JSON to a file
    std::ofstream toFile(dataPath + "/" + savesDirectory + "/" + name);
    if (toFile.is_open()) {
        toFile << json.dump(4); // Pretty print with 4 spaces indentation
        toFile.close();
        std::cout << "GameState data saved to grid_data.json\n";
    } else {
        std::cerr << "Failed to open file!\n";
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

    // GAME
    thrustForce = settings["game"]["thrustForce"];
    sideThrustForce = settings["game"]["sideThrustForce"];
    discoverRange = settings["game"]["discoverRange"];
    startDrillThreshold = settings["game"]["startDrillThreshold"];
    passivFuelUsage = settings["game"]["passivFuelUsage"];
    movingFuelUsage = settings["game"]["movingFuelUsage"];
    drillingFuelUsage = settings["game"]["drillingFuelUsage"];

    // SOUND
    masterVolume = settings["sound"]["masterVolume"];
    musicVolume = settings["sound"]["musicVolume"];
    soundVolume = settings["sound"]["soundVolume"];

    // WINDOW
    screenWidth = settings["window"]["screenWidth"];
    screenHeight = settings["window"]["screenHeight"];

    // PHYSICS
    gravity.y = settings["physics"]["gravity"];
    airResistance = settings["physics"]["airResistance"];
    collisionRetention = settings["physics"]["collisionRetention"];
    velocityThreshhold = settings["physics"]["velocityThreshhold"];
    onGroundResistance = settings["physics"]["onGroundResistance"];
    touchingDistance = settings["physics"]["touchingDistance"];
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
        gasTanks.emplace_back(item["name"], item["cost"], item["fuel"]);
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