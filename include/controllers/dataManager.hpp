#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <raylib.h>

#include <tools.hpp>
#include <grid.hpp>
#include <player.hpp>

class DataManager
{
private:
    // Private constructor
    DataManager() = default;

    // Disable copy and assignment
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    // Private helper methods for loading
    

    // Private string for data folder path
    std::string dataPath;

    // Directory names
    std::string configDirectory = "config";
    std::string savesDirectory = "saves";
    
    // Containers for data
    // Equ containers
    std::vector<Drill> drills; // Stores all drills that can be bought
    std::vector<FuelTank> fuelTanks; // Stores all fuelTanks that can be bought
    std::vector<Hull> hulls; // Stores all huels that can be bought
    std::vector<CargoBay> cargoBays; // Stores all storages that can be bought
    std::vector<Engine> engines; // Stores all engines that can be bought
    std::vector<Equipment> equipments; // Stores all tools that can be bought

public:
    // GameState
    Player player;
    Grid map;

    // Window settings
    int screenWidth;
    int screenHeight;

    // Variables
    float movementSpeed = 0.6f;

    //Physics Variables
    const Vector2 gravity{0.0f,0.4f}; // Gravity of the world
    float airResistance = 0.985f; // 0 to 1; 0 is no resistance
    float collisionRetention = 0.1f; // Remaing velocity after collision
    float velocityThreshhold = 0.05f; // If velocity is below this value it will be set to zero
    float onGroundResistance = 0.8f; // Multiplication factor when moving on ground
    float touchingDistance = 1.0f; // Distance to check if a side should be considered as touching or not

    // Access the singleton instance
    static DataManager& getInstance();

    // Search for a directory and set the path to this directory
    void searchDataDirectoryPath(std::string dir, size_t searchDepth);

    // Load gamestate
    void loadGameState(const std::string& name);
    // Save gamestate
    void saveGameState(const std::string& name);

    // Loading setting configuration
    void loadSettingConfig(const std::string& name);

    // Loading data
    void loadToolConfig(const std::string& name);
};
