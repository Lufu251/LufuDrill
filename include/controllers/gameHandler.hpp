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
    Vector2 playerMovementInput(DrillUnit& player);
    void updatePlayerState(DrillUnit& player, Vector2& movementInput);
    void generateTerrain(World& world);
    void clampToGrid(AABB& box, World& world);
    std::vector<AABB> getPossibleCollisionsFromGrid(AABB& box, World& world);
    void checkCollisionAndMove(AABB& box, World& world);
    void checkPlayerTouchingSides(DrillUnit& player, World& world);
    void checkBuildingTriggers(AABB& box, World& world);
    void checkGameOverStates(DrillUnit& player);
    void collisionDamageToPlayer();
    void drainGasFromPlayer(DrillUnit& player, Vector2& movementInput);
    void discoverWorldBlocks(DrillUnit& player, World& world);
    void playerDrill(DrillUnit& player, World& world);
};