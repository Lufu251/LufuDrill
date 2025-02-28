#include <dataManager.hpp>

#include <stdexcept>
#include <filesystem>

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