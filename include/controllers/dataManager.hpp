#pragma once

#include <iostream>
#include <string>

#include <raylib.h>

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
    

    // Containers for data
    

public:
    // Variables
    float movementSpeed = 0.6f;
    const Vector2 gravity{0.0f,0.4f}; // Gravity
    float airResistance = 0.985f;

    // Access the singleton instance
    static DataManager& getInstance();

    // Search for a directory and set the path to this directory
    void searchDataDirectoryPath(std::string dir, int searchDepth);


};
