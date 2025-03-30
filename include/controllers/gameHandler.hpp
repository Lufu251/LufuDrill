#pragma once

#include <vector>

#include <raylib.h>
#include <raymath.h>

#include <aabb.hpp>
#include <player.hpp>
#include <grid.hpp>
#include <gasStationMenu.hpp>
#include <playerGui.hpp>

class GameHandler
{
private:

public:
    GameHandler(/* args */);
    ~GameHandler();

    // Helper methods
    void updateDrillUnitMovement(DrillUnit& drillUnit, World& world);
    void updateDrillUnitStates(DrillUnit& player, Vector2& movementInput);
    void generateTerrain(World& world);
    void clampToGrid(AABB& box, World& world);
    std::vector<AABB> getPossibleCollisionsFromGrid(AABB& box, World& world);
    void checkCollisionAndMove(AABB& box, World& world);
    void checkPlayerTouchingSides(DrillUnit& player, World& world);
    void checkBuildingTriggers(AABB& box, World& world);
    void checkGameOverStates(DrillUnit& player);
    void collisionDamageToPlayer();
    void drainGasFromDrillUnit(DrillUnit& player, Vector2& movementInput);
    void discoverWorldBlocks(DrillUnit& player, World& world);
    void updateDrillUnitDrilling(DrillUnit& player, World& world);
};