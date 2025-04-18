#pragma once

#include <string>
#include <vector>

#include <raylib.h>

#include <tools.hpp>
#include <world.hpp>
#include <block.hpp>
#include <drillUnit.hpp>
#include <scene.hpp>
#include <buildings.hpp>
#include <ore.hpp>

class DataManager
{
private:
    // Private string for data folder path
    std::string dataPath;

    // Directory names
    std::string configDirectory = "config";
    std::string savesDirectory = "saves";
    
public:
    // Private constructor
    DataManager() = default;

    // Equipments
    std::vector<Drill> drills; // Stores all drills that can be bought
    std::vector<GasTank> gasTanks; // Stores all fuelTanks that can be bought
    std::vector<Hull> hulls; // Stores all huels that can be bought
    std::vector<CargoBay> cargoBays; // Stores all storages that can be bought
    std::vector<Engine> engines; // Stores all engines that can be bought
    std::vector<Equipment> equipments; // Stores all tools that can be bought
    std::vector<Block> blocks; // Store all blocks from config
    std::vector<Ore> ores; // Store all ores from config


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
    int gasCost;
    float maxPlayerSpeed;
    float sideThrustForce;
    int discoverRange;
    int startDrillThreshold;
    float passivFuelUsage;
    float movingFuelUsage;
    float drillingFuelUsage;
    float playerSight;

    // Sound
    float masterVolume;
    float musicVolume;
    float soundVolume;

    // Window settings
    int screenWidth;
    int screenHeight;

    // Physics Variables
    float collisionRetention; // Remaing velocity after collision
    float velocityThreshhold; // If velocity is below this value it will be set to zero
    float onGroundResistance; // Multiplication factor when moving on ground
    float touchingDistance; // Distance to check if a side should be considered as touching or not

    // Search for a directory and set the path to this directory
    void searchDataDirectoryPath(std::string dir, size_t searchDepth);
    
    // Load gamestate
    void loadGameState(const std::string& name);
    // Save gamestate
    void saveGameState(const std::string& name);

    // Loading setting configuration
    void loadSettingConfig(const std::string& name); // Loading data
    void loadToolsConfig(const std::string& name);
    void loadBlocksConfig(const std::string& name); // Loading blocks
    void loadOresConfig(const std::string& name); // Load ores
};
