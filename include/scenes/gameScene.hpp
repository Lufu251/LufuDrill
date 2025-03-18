#pragma once

#include <scene.hpp>

#include <gameHandler.hpp>
#include <gameRenderer.hpp>
#include <playerGui.hpp>
#include <fuelMenu.hpp>
#include <dataManager.hpp>
#include <assetManager.hpp>
#include <buildings.hpp>

class GameScene : public Scene{
private:
    GameHandler gameHandler;
    GameRenderer gameRenderer;

    // Gui Elements
    PlayerGui playerGui;
    FuelMenu fuelMenu;

public:
    GameScene(/* args */){}
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

        DataManager::getInstance().gasStation = Building({400, 380}, {100, 100}, fuelMenu);
        DataManager::getInstance().trader = Building({1000, 380}, {100, 100}, fuelMenu);
        DataManager::getInstance().shop = Building({1600, 380}, {100, 100}, fuelMenu);

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
        gameRenderer.moveCameraToPlayer();

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
        gameHandler.checkCollisionAndMove(DataManager::getInstance().player, DataManager::getInstance().map);

        //Clamp player to grid
        gameHandler.clampToGrid(DataManager::getInstance().player, DataManager::getInstance().map);

        // Check if player is touching a block on any side and count for how long it is touching
        gameHandler.checkTouching(DataManager::getInstance().player, DataManager::getInstance().map);

        gameHandler.checkBuildingTriggers();

        // Print touching sides
        if(DataManager::getInstance().player.left) std::cout << "Player LEFT " << DataManager::getInstance().player.left << std::endl;
        if(DataManager::getInstance().player.right) std::cout << "Player RIGHT " << DataManager::getInstance().player.right << std::endl;
        if(DataManager::getInstance().player.top) std::cout << "Player TOP " << DataManager::getInstance().player.top << std::endl;
        if(DataManager::getInstance().player.bottom) std::cout << "Player BOTTOM " << DataManager::getInstance().player.bottom << std::endl;

        // Update Gui
        playerGui.update();
        fuelMenu.update();
    }

    void render() override {
        // Clear Screen for the new render cycle
        ClearBackground(PURPLE);

        gameRenderer.renderGrid();
        gameRenderer.renderBuildings();
        gameRenderer.renderPlayer();

        fuelMenu.render();
        playerGui.render();
    }
};