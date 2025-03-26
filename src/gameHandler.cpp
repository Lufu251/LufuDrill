#include <gameHandler.hpp>

#include <iostream>
#include <algorithm>
#include <random>

#include <dataManager.hpp>
#include <world.hpp>
#include <buildings.hpp>

GameHandler::GameHandler(/* args */){}

GameHandler::~GameHandler(){}

// Return vector in which player will be moved
Vector2 GameHandler::playerMovementInput(DrillUnit& player){
    Vector2 direction{0,0};
    // Player input
    if(IsKeyDown(KEY_A)) direction.x += -1.f;
    if(IsKeyDown(KEY_D)) direction.x += 1.f;
    if(IsKeyDown(KEY_W)) direction.y += -1.f;
    if(IsKeyDown(KEY_S)) direction.y += 1.f;

    return direction;
}

void GameHandler::updatePlayerState(DrillUnit& player, Vector2& movementInput){
    switch (player.state){
    case LEFT:
        // StateChange triggers
        if(movementInput.x > 0) player.state = RIGHT; // Change facing direction
        if(player.left > 20 && player.bottom && movementInput.x < 0) player.state = DRILL_LEFT; // Start drill
        if(player.bottom > 20 && movementInput.y > 0 && movementInput.x == 0) player.state = DRILL_DOWN; // Start drill
        break;

    case RIGHT:
        // StateChange triggers
        if(movementInput.x < 0) player.state = LEFT; // Change facing direction
        if(player.right > 20 && player.bottom && movementInput.x > 0) player.state = DRILL_RIGHT; // Start drill
        if(player.bottom > 20 && movementInput.y > 0 && movementInput.x == 0) player.state = DRILL_DOWN; // Start drill
        break;

    case DRILL_DOWN:
        // StateChange triggers
        if(movementInput.x > 0) player.state = RIGHT; // Movement interupt
        if(movementInput.x < 0) player.state = LEFT; // Movement interupt
        if(movementInput.y < 0) player.state = RIGHT; // Movement interupt
        if(player.drillTime == 0) player.state = RIGHT; // Drill over
        
        break;

    case DRILL_LEFT:
        // StateChange triggers
        if(movementInput.x > 0) player.state = RIGHT; // Movement interupt
        if(movementInput.y < 0) player.state = LEFT; // Movement interupt
        if(player.drillTime == 0) player.state = LEFT; // Drill over
        
        break;

    case DRILL_RIGHT:
        // StateChange triggers
        if(movementInput.x < 0) player.state = LEFT; // Movement interupt
        if(movementInput.y < 0) player.state = RIGHT; // Movement interupt
        if(player.drillTime == 0) player.state = RIGHT; // Drill over

        break;
    
    default:
        // Default state for the player
        player.state = RIGHT;
        break;
    }
}

void GameHandler::generateTerrain(World& world){
    // Pre init
    for (size_t x = 0; x < world.mGrid.gridSizeX; x++){
        for (size_t y = 0; y < world.mGrid.gridSizeY; y++){
            world.mGrid(x,y) = Block(EMPTY, {static_cast<float>(x * world.mBlockSize), static_cast<float>(y * world.mBlockSize)}, {static_cast<float>(world.mBlockSize), static_cast<float>(world.mBlockSize)});
        }
    }

    // Place buildings
    world.buildings.push_back(Building(GAS_STATION ,{400, 416}, {64, 64}));
    world.buildings.push_back(Building(TRADER ,{800, 416}, {64, 64}));
    world.buildings.push_back(Building(TOOL_SHOP ,{1200, 416}, {64, 64}));
    world.buildings.push_back(Building(EQUIPMENT_SHOP ,{1600, 416}, {64, 64}));

    for (size_t x = 0; x < world.mGrid.gridSizeX; x++){
        for (size_t y = 0; y < world.mGrid.gridSizeY; y++){
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(1,10);

            // Create a tunnel to the bottom
            if(x == 20){
                world.mGrid(x,y).mType = EMPTY;
                world.mGrid(x,y).blocking = false;
                continue;
            }

            // Generate some air pockets
            if(y > 15 && dist6(rng) >= 10){
                world.mGrid(x,y).mType = EMPTY;
                world.mGrid(x,y).blocking = false;
                continue;
            }

            // Normal generation
            // Air layer
            if(y < 15){
                world.mGrid(x,y).mType = EMPTY;
                world.mGrid(x,y).blocking = false;
                continue;
            }
            // Dirt layer
            if(y < 100){
                if(dist6(rng) == 10){
                    world.mGrid(x,y).mType = COPPERORE;
                    continue;
                }
                world.mGrid(x,y).mType = DIRT;
                continue;
            }
            // Upper stone layer
            if(y < 500){
                if(dist6(rng) == 10){
                    world.mGrid(x,y).mType = GOLDORE;
                    continue;
                }
                world.mGrid(x,y).mType = STONE;
                continue;
            }
            // Lower stone layer
            if(y < 1000){
                if(dist6(rng) == 10){
                    world.mGrid(x,y).mType = PLATINUMORE;
                    continue;
                }
                world.mGrid(x,y).mType = STONE;
                continue;
            }
            // Anything Deeper Layer
            world.mGrid(x,y).mType = STONE;
        }
    }
}

std::vector<AABB> GameHandler::getPossibleCollisionsFromGrid(AABB& box, World& world){
    std::vector<AABB> blocks;
    blocks.reserve(9);

    // Get the player position on the grid
    size_t iPlayer = box.getGridPosition(world.mBlockSize).x;
    size_t jPlayer = box.getGridPosition(world.mBlockSize).y;

    // Check how many tiles around the player need to be calculated
    int gridDistanceX = Vector2Length(box.velocity) / static_cast<float>(world.mBlockSize) + std::ceil(box.size.x / world.mBlockSize); // amount of tiles that need to be checked
    int gridDistanceY = Vector2Length(box.velocity) / static_cast<float>(world.mBlockSize) + std::ceil(box.size.y / world.mBlockSize); // amount of tiles that need to be checked

    // Loop blocks near player
    for (int i = -gridDistanceX; i <= gridDistanceX; i++){
        for (int j = -gridDistanceY; j <= gridDistanceY; j++){
            // Calculate the gridposition of the block that needs to be checked
            size_t iBlock = iPlayer + i;
            size_t jBlock = jPlayer + j;

            // Check if position is out of bound from grid and skip this loop
            if (iBlock < 0 || iBlock >= world.mGrid.gridSizeX || jBlock < 0 || jBlock >= world.mGrid.gridSizeY){
                // Out of bound
                continue;
            }
            else if(world.mGrid(iBlock, jBlock).blocking == true){ // In bound block needs to be checked
                // Add block to the list
                blocks.push_back(world.mGrid(iBlock, jBlock));
            }
        }
    }
    return blocks;
}

void GameHandler::clampToGrid(AABB& box, World& world){
    // Clamp box to grid
    Vector2 positionBeforeClamp = box.position;
    box.position.x = std::clamp(box.position.x, 0.f, static_cast<float>(world.mGrid.gridSizeX * world.mBlockSize - box.size.x));
    box.position.y = std::clamp(box.position.y, 0.f, static_cast<float>(world.mGrid.gridSizeY * world.mBlockSize - box.size.y));
    // If position was clamped set velocity to 0 in this axis
    if(positionBeforeClamp.x != box.position.x) box.velocity.x = 0;
    if(positionBeforeClamp.y != box.position.y) box.velocity.y = 0;
}

void GameHandler::checkCollisionAndMove(AABB& box, World& world){
    DataManager& dataManager = DataManager::getInstance();
    AABB boxClone = box;

    // Loop a cloned Player with the collision and end loop when movement is done
    while(Vector2Length(boxClone.velocity) > 0){
        // Create a list with blocks that need to be checked
        std::vector<AABB> blocks = getPossibleCollisionsFromGrid(box, world); // AABB from all blocks that can collide
        // Get the nearest collision from the List
        Hit hit = GetNearestCollisionFromList(boxClone, blocks);

        // When nearest is less then one a hit occured
        if(hit.collisionTime < 1.0f){
            // A hit occured
            if(hit.n.x < 0){
                // RIGHT
                // Dampening everytime a hit occures
                boxClone.velocity.x *= dataManager.collisionRetention;
                box.velocity.x *= dataManager.collisionRetention;
            }
            else if(hit.n.x > 0){
                // LEFT
                // Dampening everytime a hit occures
                boxClone.velocity.x *= dataManager.collisionRetention;
                box.velocity.x *= dataManager.collisionRetention;
            }
            if(hit.n.y < 0){
                // BOTTOM
                // Apply damage to Hull when hitting the bottom
                collisionDamageToPlayer();

                // Dampening everytime a hit occures
                boxClone.velocity.y *= dataManager.collisionRetention;
                box.velocity.y *= dataManager.collisionRetention;

                // Slow when moving over ground
                boxClone.velocity.x *= dataManager.onGroundResistance;
                box.velocity.x *= dataManager.onGroundResistance;
            }
            else if(hit.n.y > 0){
                // TOP
                // Dampening everytime a hit occures
                boxClone.velocity.y *= dataManager.collisionRetention;
                box.velocity.y *= dataManager.collisionRetention;
            }

            boxClone.position = hit.p; // Move to the point of collision

            // Set cPlayer velocity to the remaining velocity
            float remainingTime = 1.0f - hit.collisionTime; // Calculate remaining distance after the collision
            boxClone.velocity *= remainingTime;

            // reflect the velocity of cPlayer and player
            if (abs(hit.n.x) > 0.0001f) box.velocity.x = -box.velocity.x;
            if (abs(hit.n.y) > 0.0001f) box.velocity.y = -box.velocity.y;

            if (abs(hit.n.x) > 0.0001f) boxClone.velocity.x = -boxClone.velocity.x;
            if (abs(hit.n.y) > 0.0001f) boxClone.velocity.y = -boxClone.velocity.y;
        }
        else{
            // No hit occured do a regular move
            boxClone.position += boxClone.velocity; // regulary add velocity
            boxClone.velocity = {0 , 0}; // Reset velocity all movement has been done / END Loop
        }
    }
    box.position = boxClone.position; // Loop has ended player can now be set to the new position
}

// Check if AABB is touching blocks on any side
void GameHandler::checkPlayerTouchingSides(DrillUnit& player, World& world){
    DataManager& dataManager = DataManager::getInstance();
    std::vector<AABB> blocks = getPossibleCollisionsFromGrid(player, world); // AABB from all blocks that can collide

    bool left = false, right = false, bottom = false, top = false;

    // Iterate the possible blocks
    for(auto & block : blocks){
        // Bottom
        if(AABBIntersection(player, AABB(Vector2Add(block.position, {0, -dataManager.touchingDistance}), block.size))){
            bottom = true;
        }
        // Top
        if(AABBIntersection(player, AABB(Vector2Add(block.position, {0, dataManager.touchingDistance}), block.size))){
            top = true;
        }
        // Right
        if(AABBIntersection(player, AABB(Vector2Add(block.position, {-dataManager.touchingDistance, 0}), block.size))){
            right = true;
        }
        // Top
        if(AABBIntersection(player, AABB(Vector2Add(block.position, {dataManager.touchingDistance, 0}), block.size))){
            left = true;
        }
    }

    // Update touching sides.
    // Bottom
    if(bottom){ player.bottom ++; }
    else{ player.bottom = 0; }

    // Top
    if(top){ player.top ++; }
    else{ player.top = 0; }

    // Right
    if(right){ player.right ++; }
    else{ player.right = 0; }

    // Left
    if(left){ player.left ++; }
    else{ player.left = 0; }

    // Clamp values to prevent them from climbing to infinity
    player.bottom = std::clamp(player.bottom, 0, 1000);
    player.top = std::clamp(player.top, 0, 1000);
    player.right = std::clamp(player.right, 0, 1000);
    player.left = std::clamp(player.left, 0, 1000);
}

void GameHandler::checkBuildingTriggers(AABB& box, World& world){
    for (auto & building : world.buildings){
        if(AABBIntersection(box, building)){
            building.mMenuToTrigger->enable();
        }
        else{
            building.mMenuToTrigger->disable();
        }
    }
}

void GameHandler::checkGameOverStates(DrillUnit& player){
    // Check if no gas is left
    if(player.gasTank.mGas <= 0){
        DataManager::getInstance().gameOver = true;
    }
    // Check if hull is destroyed
    if(player.hull.mHealth <= 0){
        DataManager::getInstance().gameOver = true;
    }
}

void GameHandler::collisionDamageToPlayer(){
    float threshold = 8;
    // Check if vertical velocity is greater than
    if(DataManager::getInstance().player.velocity.y >= threshold){
        float damage = std::clamp( 6 * (DataManager::getInstance().player.velocity.y -threshold), threshold, 100.0f);
        DataManager::getInstance().player.hull.mHealth -= damage;
    }
}

void GameHandler::drainGasFromPlayer(DrillUnit& player, Vector2& movementInput){
    // Drain passiv fuel
    player.gasTank.mGas -= DataManager::getInstance().passivFuelUsage;

    // Drain gas while drilling
    if(player.state == DRILL_DOWN || player.state == DRILL_LEFT || player.state == DRILL_RIGHT){
        player.gasTank.mGas -= DataManager::getInstance().drillingFuelUsage;
        return;
    }
    // Drain gas while moving
    if(Vector2LengthSqr(movementInput) > 0){
        player.gasTank.mGas -= DataManager::getInstance().movingFuelUsage;
    }

    
}

void GameHandler::discoverWorldBlocks(DrillUnit& drillUnit, World& world){
    int radius = 5;

    // Get the player position on the grid
    size_t iPlayer = drillUnit.getGridPosition(world.mBlockSize).x;
    size_t jPlayer = drillUnit.getGridPosition(world.mBlockSize).y;

    // Loop blocks near player
    for (int i = -radius; i <= radius; i++){
        for (int j = -radius; j <= radius; j++){
            // Calculate the gridposition of the block that needs to be checked
            size_t iBlock = iPlayer + i;
            size_t jBlock = jPlayer + j;

            // Check if position is out of bound from grid and skip this loop
            if (iBlock < 0 || iBlock >= world.mGrid.gridSizeX || jBlock < 0 || jBlock >= world.mGrid.gridSizeY){
                // Out of bound
                continue;
            }
            else{
                // Set block to discovered
                world.mGrid(iBlock, jBlock).discovered = true;
            }
        }
    }
    
}

void GameHandler::playerDrill(DrillUnit& drillUnit, World& world){
    // Get the player position on the grid
    size_t iPlayer = drillUnit.getGridPosition(world.mBlockSize).x;
    size_t jPlayer = drillUnit.getGridPosition(world.mBlockSize).y;

    // Set drillTime to the time the block needs to be destroyed
    if(drillUnit.drillTime == 0){
        if(drillUnit.state == DRILL_DOWN){
            drillUnit.drillingBlock = &world.mGrid(iPlayer, jPlayer +1);
            // Set drillUnit time to drill to the hardness of the block
            drillUnit.drillTime = drillUnit.drillingBlock->hardness;
        }
        else if(drillUnit.state == DRILL_LEFT){
            drillUnit.drillingBlock = &world.mGrid(iPlayer -1, jPlayer);
            // Set drillUnit time to drill to the hardness of the block
            drillUnit.drillTime = drillUnit.drillingBlock->hardness;
        }
        else if(drillUnit.state == DRILL_RIGHT){
            drillUnit.drillingBlock = &world.mGrid(iPlayer +1, jPlayer);
            // Set drillUnit time to drill to the hardness of the block
            drillUnit.drillTime = drillUnit.drillingBlock->hardness;
        }
    }
    else{
        if(drillUnit.drillingBlock == nullptr){
            // block is nullpointer
            //std::cout << "NULLPTR" << std::endl;
        }
        else{
            // drillUnit is drilling a block reduce drillTime by drillpower
            drillUnit.drillTime -= drillUnit.drill.mPower;

            if(drillUnit.drillTime <= 0){
                // Block is mined reset
                drillUnit.drillTime = 0;

                // Do something to the block
                drillUnit.drillingBlock->mType = EMPTY;
                drillUnit.drillingBlock->blocking = false;
            }
        }
    }

    // Reset drillingBlock when drillUnit is not in drilling mode
    if(drillUnit.state == DRILL_LEFT || drillUnit.state == DRILL_RIGHT || drillUnit.state == DRILL_DOWN){
        
    }
    else{
        drillUnit.drillingBlock = nullptr;
        drillUnit.drillTime = 0;
    }
}