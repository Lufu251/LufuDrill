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
    // Access the singleton instance
    static DataManager& getInstance();

    // Search for a directory and set the path to this directory
    void searchDataDirectoryPath(std::string dir, int searchDepth);


};
