#pragma once

#include <scene.hpp>

#include <gameHandler.hpp>
#include <gameRenderer.hpp>
#include <playerGui.hpp>
#include <fuelMenu.hpp>
#include <dataManager.hpp>
#include <assetManager.hpp>

class GameScene : public Scene{
private:
    GameHandler gameHandler;
    GameRenderer gameRenderer;

    // Gui Elements
    PlayerGui playerGui;
    FuelMenu fuelMenu;

public:
    GameScene(){}
    ~GameScene(){}

    void initialize() override{
        // Preload textureAtlas
        AssetManager::getInstance().loadTextureAtlas("tileset");
        AssetManager::getInstance().loadTextureAtlas("particleset");

        // Load tools configuration
        DataManager::getInstance().loadToolConfig("tools.json");

        playerGui = PlayerGui({0,0}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
        playerGui.initialize();
        playerGui.enable();

        fuelMenu = FuelMenu({100,100}, {600,600});
        fuelMenu.initialize();
        fuelMenu.enable();
        
        for (auto& building : DataManager::getInstance().world.buildings){
            switch (building.mType)
            {
            case GAS_STATION:
                building.setMenuToTrigger(fuelMenu);
                break;
            case TRADER:
                building.setMenuToTrigger(fuelMenu);
                break;
            case SHOP:
                building.setMenuToTrigger(fuelMenu);
                break;
            default:
                break;
            }
        }
        
        // Camera initialise
        gameRenderer.camera.zoom = 2.0f;
        gameRenderer.camera.rotation = 0.0f;
        gameRenderer.camera.target = DataManager::getInstance().player.position;
        gameRenderer.setCameraOffset({GetScreenWidth() / 2.0f - DataManager::getInstance().player.size.x, GetScreenHeight() / 2.0f - DataManager::getInstance().player.size.y});

        DataManager::getInstance().player = Player({200,200}, {24,24}, {0,0});
    }

    void update() override {
        //float deltaTime = GetFrameTime();
        // Do updates on screen resize
        if(IsWindowResized()){
            gameRenderer.setCameraOffset({GetScreenWidth() / 2.0f - DataManager::getInstance().player.size.x, GetScreenHeight() / 2.0f - DataManager::getInstance().player.size.y});
        }

        // Update and pan camera in the direction of the player
        gameRenderer.moveCameraToPosition(DataManager::getInstance().player.position);

        // Movement input
        Vector2 movementInput{0,0};
        gameHandler.handleInput(movementInput);
        movementInput = Vector2Scale(movementInput, DataManager::getInstance().movementSpeed); // Multiply by speed

        // Calculate air resistance
        Vector2 airResistance = DataManager::getInstance().player.velocity - DataManager::getInstance().player.velocity * DataManager::getInstance().airResistance;
        DataManager::getInstance().player.addForce(movementInput); // Add movementInput to player velocity
        DataManager::getInstance().player.addForce(DataManager::getInstance().gravity); // Add gravity to player velocity
        DataManager::getInstance().player.addForce(Vector2Negate(airResistance)); // Add airResistance to player velocity

        // Stop completely if below the threshold
        if (Vector2Length(DataManager::getInstance().player.velocity) < DataManager::getInstance().velocityThreshhold) DataManager::getInstance().player.velocity = { 0.0f, 0.0f };

        // Physics collision response and move player
        gameHandler.checkCollisionAndMove(DataManager::getInstance().player, DataManager::getInstance().world);

        //Clamp player to grid
        gameHandler.clampToGrid(DataManager::getInstance().player, DataManager::getInstance().world);

        // Check if player is touching a block on any side and count for how long it is touching
        gameHandler.checkPlayerTouchingSides(DataManager::getInstance().player, DataManager::getInstance().world);
        gameHandler.checkBuildingTriggers(DataManager::getInstance().player, DataManager::getInstance().world);

        // Update Gui
        playerGui.update();
        fuelMenu.update();
    }

    void render() override {
        // Clear Screen for the new render cycle
        ClearBackground(PURPLE);

        gameRenderer.renderMapGrid(DataManager::getInstance().world);
        gameRenderer.renderMapBuildings(DataManager::getInstance().world);
        gameRenderer.renderPlayer(DataManager::getInstance().player);

        fuelMenu.render();
        playerGui.render();
    }
};