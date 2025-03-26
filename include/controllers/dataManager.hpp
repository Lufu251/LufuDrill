#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <raylib.h>

#include <tools.hpp>
#include <world.hpp>
#include <player.hpp>
#include <scene.hpp>
#include <buildings.hpp>

class DataManager
{
private:
    // Private constructor
    DataManager() = default;

    // Disable copy and assignment
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    // Private string for data folder path
    std::string dataPath;

    // Directory names
    std::string configDirectory = "config";
    std::string savesDirectory = "saves";
    
    // Containers for data
    

public:
    // Equipments
    std::vector<Drill> drills; // Stores all drills that can be bought
    std::vector<GasTank> gasTanks; // Stores all fuelTanks that can be bought
    std::vector<Hull> hulls; // Stores all huels that can be bought
    std::vector<CargoBay> cargoBays; // Stores all storages that can be bought
    std::vector<Engine> engines; // Stores all engines that can be bought
    std::vector<Equipment> equipments; // Stores all tools that can be bought

    // Scene
    std::unique_ptr<Scene> activeScene;

    // Tracking variables
    bool windowOpen = true;
    bool gameOver = false;

    // GameState
    DrillUnit player;
    World world;
    
    Building gasStation;
    Building trader;
    Building shop;

    // General

    // GAME
    float thrustForce;
    float sideThrustForce;
    int discoverRange;
    int startDrillThreshold;
    float passivFuelUsage;
    float movingFuelUsage;
    float drillingFuelUsage;

    // Sound
    float masterVolume;
    float musicVolume;
    float soundVolume;

    // Window settings
    int screenWidth;
    int screenHeight;

    // Physics Variables
    Vector2 gravity{0.0f,0.0f}; // Gravity of the world
    float airResistance; // 0 to 1; 0 is no resistance
    float collisionRetention; // Remaing velocity after collision
    float velocityThreshhold; // If velocity is below this value it will be set to zero
    float onGroundResistance; // Multiplication factor when moving on ground
    float touchingDistance; // Distance to check if a side should be considered as touching or not

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
