#pragma once

#include "raylib.h"
#include <scene.hpp>

#include <iostream>

#include <gameHandler.hpp>
#include <gameRenderer.hpp>
#include <playerGui.hpp>
#include <gasStationMenu.hpp>
#include <traderMenu.hpp>
#include <toolShopMenu.hpp>
#include <equipmentShopMenu.hpp>
#include <inputHandler.hpp>
#include <globals.hpp>

class GameScene : public Scene{
private:
    GameHandler gameHandler;
    GameRenderer gameRenderer;

    // Gui Elements
    PlayerGui playerGui;
    GasStationMenu gasStationMenu;
    TraderMenu traderMenu;
    ToolShopMenu toolShopMenu;
    EquipmentShopMenu equipmentShopMenu;
 
public:
    GameScene(){}
    ~GameScene(){}

    void initialize() override{
        std::cout << "Initialize GameScene" << std::endl;
        // Load Assets
        gAM.loadTextureAtlas("tileset");
        gAM.loadTextureAtlas("buildingset");
        gAM.loadTextureAtlas("drillunitset");

        gAM.loadTexture("menuTemplate", "menuTemplate.png");
        gAM.loadTexture("sky", "sky.png");
        gAM.loadTexture("mountain", "mountain.png");
        gAM.loadTexture("cloud", "cloud.png");
        gAM.loadTexture("light_texture", "light_texture.png");
        
        gAM.loadMusic("nebula_run", "nebula_run.ogg");

        // Play music
        PlayMusicStream(gAM.getMusic("nebula_run"));

        // Init Player
        gDM.player = DrillUnit({200,300}, {24,24}, 200);
        gDM.player.drill = gDM.drills[0];
        gDM.player.gasTank = gDM.gasTanks[0];
        gDM.player.hull = gDM.hulls[0];
        gDM.player.cargoBay = gDM.cargoBays[0];
        gDM.player.engine = gDM.engines[0];

        // Set cargo size to the amount of blocks that where loaded from config
        gDM.player.setCargoSize(gDM.ores.size());

        // Set Hull and Gas to max
        gDM.player.gasTank.mGas = gDM.player.gasTank.mGasMax;
        gDM.player.hull.mHealth = gDM.player.hull.mHealthMax;

        playerGui = PlayerGui({0,0}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
        playerGui.initialize();
        playerGui.enable();

        gasStationMenu = GasStationMenu({100,100}, {800,700});
        gasStationMenu.initialize();

        traderMenu = TraderMenu({100,100}, {800,700});
        traderMenu.initialize();

        toolShopMenu = ToolShopMenu({100,100}, {800,700});
        toolShopMenu.initialize();

        equipmentShopMenu = EquipmentShopMenu({100,100}, {800,700});
        equipmentShopMenu.initialize();
        
        
        for (auto& building : gDM.world.buildings){
            switch (building.mType)
            {
            case GAS_STATION: building.setMenuToTrigger(gasStationMenu); break;
            case TRADER: building.setMenuToTrigger(traderMenu); break;
            case TOOL_SHOP: building.setMenuToTrigger(toolShopMenu); break;
            case EQUIPMENT_SHOP: building.setMenuToTrigger(equipmentShopMenu); break;
            }
        }
        
        // Camera initialise
        gameRenderer.initialize();
    }

    void update() override {
        float deltaTime = GetFrameTime();
        UpdateMusicStream(gAM.getMusic("nebula_run"));

        // Do updates on screen resize
        if(IsWindowResized()){
            gameRenderer.setCameraOffset({GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f});
            gameRenderer.lightmap = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        }

        // Update Input
        InputHandler::getInstance().updateMovementInput();

        // Add forces to DrillUnit
        gameHandler.applyForcesToDrillUnit(deltaTime);
        // General
        if(gDM.player.state == DRILL_DOWN){
            gDM.player.mVelocity.x = 0;
        }
        //gameHandler.checkCollisionAndMove(gDM.player, gDM.world); // Physics collision response and move player
        gameHandler.updatePlayerPosition(deltaTime); // Update player position
        gameHandler.clampToGrid(gDM.player, gDM.world); //Clamp player to grid

        // Update DrillUnit General
        gameHandler.updateDrillUnitStates(gDM.player, InputHandler::getInstance().movementInput);
        gameHandler.updateDrillUnitDrilling(gDM.player, gDM.world);
        gameHandler.checkPlayerTouchingSides(gDM.player, gDM.world);
        gameHandler.checkBuildingTriggers(gDM.player, gDM.world);
        gameHandler.drainGas(gDM.player, InputHandler::getInstance().movementInput);
        gameHandler.checkGameOverStates(gDM.player);

        // Update and pan camera
        gameRenderer.moveCameraToPosition(gDM.player.mPosition + gDM.player.mSize /2);
        gameRenderer.clampCameraToGrid(gDM.player, gDM.world);

        // Update Gui
        playerGui.update();
        gasStationMenu.update();
        traderMenu.update();
        toolShopMenu.update();
        equipmentShopMenu.update();
    }

    void render() override {
        // Clear Screen for the new render cycle
        ClearBackground({94, 131, 166, 255});

        gameRenderer.renderBackground(gDM.player);
        gameRenderer.renderMapGrid(gDM.world);
        gameRenderer.renderMapBuildings(gDM.world);
        gameRenderer.renderPlayer(gDM.player);
        gameRenderer.drawLightmap(gDM.player, gDM.world);
        gameRenderer.renderLightmap();

        gasStationMenu.render();
        traderMenu.render();
        toolShopMenu.render();
        equipmentShopMenu.render();

        playerGui.render();
    }
};