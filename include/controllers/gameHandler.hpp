#pragma once

#include <world.hpp>
#include <vector>

#include <raylib.h>
#include <raymath.h>

#include <aabb.hpp>
#include <drillUnit.hpp>
#include <grid.hpp>
#include <gasStationMenu.hpp>
#include <playerGui.hpp>
#include <world.hpp>

class GameHandler
{
private:

public:
    GameHandler(/* args */);
    ~GameHandler();

    // Helper methods
    void applyForcesToDrillUnit(float& deltaTime);
    void updatePlayerPosition(float& deltaTime);
    void checkCollisionAndMove(AABB& box, World& world);

    void updateDrillUnitStates(DrillUnit& player, Vector2& movementInput);
    void generateTerrain(World& world);
    void clampToGrid(AABB& box, World& world);
    std::vector<AABB> getPossibleCollisionsFromGrid(AABB& box, World& world);
    void checkPlayerTouchingSides(DrillUnit& player, World& world);
    void checkBuildingTriggers(AABB& box, World& world);
    void checkGameOverStates(DrillUnit& player);
    void collisionDamage();
    void drainGas(DrillUnit& player, Vector2& movementInput);
    void updateDrillUnitDrilling(DrillUnit& player, World& world);
};