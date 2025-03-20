#pragma once

#include <scene.hpp>

#include <gameHandler.hpp>
#include <gameRenderer.hpp>
#include <playerGui.hpp>
#include <gasStationMenu.hpp>
#include <traderMenu.hpp>
#include <shopMenu.hpp>
#include <dataManager.hpp>
#include <assetManager.hpp>

class GameScene : public Scene{
private:
    // Access Singleton
    DataManager& dataManager = DataManager::getInstance();

    GameHandler gameHandler;
    GameRenderer gameRenderer;

    // Gui Elements
    PlayerGui playerGui;
    GasStationMenu gasStationMenu;
    TraderMenu traderMenu;
    ShopMenu shopMenu;

public:
    GameScene(){}
    ~GameScene(){}

    void initialize() override{
        

        // Preload textureAtlas
        AssetManager::getInstance().loadTextureAtlas("tileset");
        AssetManager::getInstance().loadTextureAtlas("particleset");

        // Load tools configuration
        dataManager.loadToolConfig("tools.json");

        // Init Player
        dataManager.player = Player({200,200}, {24,24}, {0,0});
        dataManager.player.drill = dataManager.drills[0];
        dataManager.player.fuelTank = dataManager.fuelTanks[0];
        dataManager.player.hull = dataManager.hulls[0];
        dataManager.player.cargoBay = dataManager.cargoBays[0];
        dataManager.player.engine = dataManager.engines[0];

        playerGui = PlayerGui({0,0}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
        playerGui.initialize();
        playerGui.enable();

        gasStationMenu = GasStationMenu({100,100}, {600,600});
        gasStationMenu.initialize();

        traderMenu = TraderMenu({100,100}, {600,600});
        traderMenu.initialize();

        shopMenu = ShopMenu({100,100}, {600,600});
        shopMenu.initialize();
        
        for (auto& building : dataManager.world.buildings){
            switch (building.mType)
            {
            case GAS_STATION:
                building.setMenuToTrigger(gasStationMenu);
                break;
            case TRADER:
                building.setMenuToTrigger(traderMenu);
                break;
            case SHOP:
                building.setMenuToTrigger(shopMenu);
                break;
            default:
                break;
            }
        }
        
        // Camera initialise
        gameRenderer.camera.zoom = 2.0f;
        gameRenderer.camera.rotation = 0.0f;
        gameRenderer.camera.target = dataManager.player.position;
        gameRenderer.setCameraOffset({GetScreenWidth() / 2.0f - dataManager.player.size.x, GetScreenHeight() / 2.0f - dataManager.player.size.y});
    }

    void update() override {
        //float deltaTime = GetFrameTime();
        // Do updates on screen resize
        if(IsWindowResized()){
            gameRenderer.setCameraOffset({GetScreenWidth() / 2.0f - dataManager.player.size.x, GetScreenHeight() / 2.0f - dataManager.player.size.y});
        }

        // Update and pan camera in the direction of the player
        gameRenderer.moveCameraToPosition(dataManager.player.position);

        // Movement input
        Vector2 movementInput = gameHandler.playerMovementInput();

        // Calculate air resistance
        Vector2 airResistance = Vector2Negate(dataManager.player.velocity - dataManager.player.velocity * dataManager.airResistance);

        // Add Forces
        dataManager.player.addForce(movementInput); // Add movementInput to player velocity
        dataManager.player.addForce(dataManager.gravity); // Add gravity to player velocity
        dataManager.player.addForce(airResistance); // Add airResistance to player velocity

        // Stop completely if below the threshold
        if (Vector2Length(dataManager.player.velocity) < dataManager.velocityThreshhold) dataManager.player.velocity = { 0.0f, 0.0f };

        // Physics collision response and move player
        gameHandler.checkCollisionAndMove(dataManager.player, dataManager.world);

        //Clamp player to grid
        gameHandler.clampToGrid(dataManager.player, dataManager.world);

        // Check if player is touching a block on any side and count for how long it is touching
        gameHandler.checkPlayerTouchingSides(dataManager.player, dataManager.world);
        gameHandler.checkBuildingTriggers(dataManager.player, dataManager.world);
        gameHandler.checkGameOverStates(dataManager.player);

        // Update Gas
        dataManager.player.fuelTank.mGas -= dataManager.passivFuelUsage;
        if(Vector2LengthSqr(movementInput) > 0){
            dataManager.player.fuelTank.mGas -= dataManager.activeFuelUsage;
        }

        // Update Gui
        playerGui.update();
        
        gasStationMenu.update();
        traderMenu.update();
        shopMenu.update();
    }

    void render() override {
        // Clear Screen for the new render cycle
        ClearBackground(PURPLE);

        gameRenderer.renderMapGrid(dataManager.world);
        gameRenderer.renderMapBuildings(dataManager.world);
        gameRenderer.renderPlayer(dataManager.player);

        gasStationMenu.render();
        traderMenu.render();
        shopMenu.render();

        playerGui.render();
    }
};