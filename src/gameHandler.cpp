#include <gameHandler.hpp>

#include <algorithm>
#include <random>

#include <globals.hpp>
#include <inputHandler.hpp>
#include <world.hpp>
#include <buildings.hpp>

GameHandler::GameHandler(/* args */){}

GameHandler::~GameHandler(){}

// Return vector in which player will be moved
void GameHandler::updateDrillUnitMovement(DrillUnit& drillUnit, World& world){
    // Values for player force
    Vector2 direction = Vector2Scale(InputHandler::getInstance().movementInput, gDM.thrustForce); // Multiply by speed
    direction.x = direction.x * gDM.sideThrustForce; // weaken side thruster
    if(direction.y > 0) direction.y = 0; // Stop down acceleration
    
    Vector2 airResistance = Vector2Negate(drillUnit.velocity - drillUnit.velocity * world.mAirDensity);
    // Add Forces
    drillUnit.addForce(direction); // Add movementInput to player velocity
    drillUnit.addForce(world.mGravity); // Add gravity to player velocity
    drillUnit.addForce(airResistance); // Add airResistance to player velocity

    if(drillUnit.state == DRILL_DOWN){
        drillUnit.velocity.x = 0;
    }
}

void GameHandler::updateDrillUnitStates(DrillUnit& player, Vector2& movementInput){
    switch (player.state){
    case LEFT:
        // StateChange triggers
        if(movementInput.x > 0) player.state = RIGHT; // Change facing direction
        if(player.left > gDM.startDrillThreshold && player.bottom && movementInput.x < 0) player.state = DRILL_LEFT; // Start drill
        if(player.bottom > gDM.startDrillThreshold && movementInput.y > 0 && movementInput.x == 0) player.state = DRILL_DOWN; // Start drill
        break;

    case RIGHT:
        // StateChange triggers
        if(movementInput.x < 0) player.state = LEFT; // Change facing direction
        if(player.right > gDM.startDrillThreshold && player.bottom && movementInput.x > 0) player.state = DRILL_RIGHT; // Start drill
        if(player.bottom > gDM.startDrillThreshold && movementInput.y > 0 && movementInput.x == 0) player.state = DRILL_DOWN; // Start drill
        break;

    case DRILL_DOWN:
        // StateChange triggers
        if(movementInput.x > 0) player.state = RIGHT; // Movement interupt
        if(movementInput.x < 0) player.state = LEFT; // Movement interupt
        if(movementInput.y < 0) player.state = RIGHT; // Movement interupt
        if(player.drillTime == 0) player.state = RIGHT; // Drill done
        
        break;

    case DRILL_LEFT:
        // StateChange triggers
        if(movementInput.x > 0) player.state = RIGHT; // Movement interupt
        if(movementInput.y < 0) player.state = LEFT; // Movement interupt
        if(movementInput.y > 0 && movementInput.x == 0) player.state = LEFT; // Movement interupt
        if(player.drillTime == 0) player.state = LEFT; // Drill done
        
        break;

    case DRILL_RIGHT:
        // StateChange triggers
        if(movementInput.x < 0) player.state = LEFT; // Movement interupt
        if(movementInput.y < 0) player.state = RIGHT; // Movement interupt
        if(movementInput.y > 0 && movementInput.x == 0) player.state = RIGHT; // Movement interupt
        if(player.drillTime == 0) player.state = RIGHT; // Drill done

        break;
    
    default:
        // Default state for the player
        player.state = RIGHT;
        break;
    }
}

void GameHandler::generateTerrain(World& world){
    world.mGravity = {0, 0.4f};
    world.mAirDensity = 0.985f;

    // Pre init
    for (size_t x = 0; x < world.mGrid.gridSizeX; x++){
        for (size_t y = 0; y < world.mGrid.gridSizeY; y++){
            world.mGrid(x,y) = Tile({static_cast<float>(x * world.mBlockSize), static_cast<float>(y * world.mBlockSize)}, {static_cast<float>(world.mBlockSize), static_cast<float>(world.mBlockSize)});
        }
    }

    // Place buildings
    world.buildings.push_back(Building(GAS_STATION ,{400, 416}, {64, 64}));
    world.buildings.push_back(Building(TRADER ,{800, 416}, {64, 64}));
    world.buildings.push_back(Building(TOOL_SHOP ,{1200, 416}, {64, 64}));
    world.buildings.push_back(Building(EQUIPMENT_SHOP ,{1600, 416}, {64, 64}));

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,15);

    for (size_t x = 0; x < world.mGrid.gridSizeX; x++){
        for (size_t y = 0; y < world.mGrid.gridSizeY; y++){

            // Create a tunnel to the bottom
            if(x == 20){
                world.mGrid(x,y).setTileFromBlock(0, gDM.blocks[0]);
                continue;
            }

            // Air pokets
            if(dist6(rng) == 2 && y != 15){
                world.mGrid(x,y).setTileFromBlock(0, gDM.blocks[0]);
                continue;
            }

            // empty_zone
            if(y < 15){
                world.mGrid(x,y).setTileFromBlock(0, gDM.blocks[0]);
                continue;
            }

            // dirt_zone
            if(y < 250){
                if(dist6(rng) == 10){
                    world.mGrid(x,y).setTileFromBlock(2, gDM.blocks[2]); // cuprium
                }
                else if(dist6(rng) == 11){
                    world.mGrid(x,y).setTileFromBlock(3, gDM.blocks[3]); // albium
                }
                else if(dist6(rng) == 12){
                    world.mGrid(x,y).setTileFromBlock(4, gDM.blocks[4]); // platinum
                }
                else if(dist6(rng) == 13){
                    world.mGrid(x,y).setTileFromBlock(5, gDM.blocks[5]); // goldium
                }
                else{
                    world.mGrid(x,y).setTileFromBlock(1, gDM.blocks[1]); // dirt
                }
                continue;
            }

            // stone_zone
            if(y < 600){
                if(dist6(rng) == 10){
                    world.mGrid(x,y).setTileFromBlock(7, gDM.blocks[7]); // celestium
                }
                else if(dist6(rng) == 11){
                    world.mGrid(x,y).setTileFromBlock(8, gDM.blocks[8]); // faerite
                }
                else if(dist6(rng) == 12){
                    world.mGrid(x,y).setTileFromBlock(9, gDM.blocks[9]); // florite
                }
                else if(dist6(rng) == 13){
                    world.mGrid(x,y).setTileFromBlock(10, gDM.blocks[10]); // prismarite
                }
                else{
                    world.mGrid(x,y).setTileFromBlock(6, gDM.blocks[6]); // stone
                }
                continue;
            }

            // magma_zone
            if(y < 800){
                if(dist6(rng) == 10){
                   world.mGrid(x,y).setTileFromBlock(12, gDM.blocks[12]); // pyrite
                }
                else if(dist6(rng) == 11){
                    world.mGrid(x,y).setTileFromBlock(13, gDM.blocks[13]); // infernium
                }
                else if(dist6(rng) == 12){
                    world.mGrid(x,y).setTileFromBlock(14, gDM.blocks[14]); // magmatite
                }
                else{
                    world.mGrid(x,y).setTileFromBlock(11, gDM.blocks[11]); // magmastone
                }
                continue;
            }

            // void_zone
            if(y < 1000){
                if(dist6(rng) == 10){
                    world.mGrid(x,y).setTileFromBlock(16, gDM.blocks[16]); // voidium
                }
                else if(dist6(rng) == 11){
                    world.mGrid(x,y).setTileFromBlock(17, gDM.blocks[17]); // singularium
                }
                else{
                    world.mGrid(x,y).setTileFromBlock(15, gDM.blocks[15]); // voidstone
                }
                continue;
            }
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
            else if(world.mGrid(iBlock, jBlock).mBlocking == true){ // In bound block needs to be checked
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
    DataManager& dataManager = gDM;
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
    DataManager& dataManager = gDM;
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
        gDM.gameOver = true;
    }
    // Check if hull is destroyed
    if(player.hull.mHealth <= 0){
        gDM.gameOver = true;
    }
}

void GameHandler::collisionDamageToPlayer(){
    float threshold = 8;
    // Check if vertical velocity is greater than
    if(gDM.player.velocity.y >= threshold){
        float damage = std::clamp( 6 * (gDM.player.velocity.y -threshold), threshold, 100.0f);
        gDM.player.hull.mHealth -= damage;
    }
}

void GameHandler::drainGasFromDrillUnit(DrillUnit& player, Vector2& movementInput){
    // Drain passiv fuel
    player.gasTank.mGas -= gDM.passivFuelUsage;

    // Drain gas while drilling
    if(player.state == DRILL_DOWN || player.state == DRILL_LEFT || player.state == DRILL_RIGHT){
        player.gasTank.mGas -= gDM.drillingFuelUsage;
        return;
    }
    // Drain gas while moving
    if(Vector2LengthSqr(movementInput) > 0){
        player.gasTank.mGas -= gDM.movingFuelUsage;
    }

    
}

void GameHandler::discoverWorldBlocks(DrillUnit& drillUnit, World& world){
    int radius = gDM.discoverRange;

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
                world.mGrid(iBlock, jBlock).mDiscovered = true;
            }
        }
    }
    
}

void GameHandler::updateDrillUnitDrilling(DrillUnit& drillUnit, World& world){
    // Get the player position on the grid
    size_t iPlayer = drillUnit.getGridPosition(world.mBlockSize).x;
    size_t jPlayer = drillUnit.getGridPosition(world.mBlockSize).y;

    float epsilon = 0.000005;
    if(fabs(drillUnit.drillTime - 0) <= epsilon){
        if(drillUnit.state == DRILL_DOWN){
            drillUnit.drillingBlock = &world.mGrid(iPlayer, jPlayer +1);
            // Set drillUnit time to drill to the hardness of the block
            drillUnit.drillTime = drillUnit.drillingBlock->mHardness;
        }
        else if(drillUnit.state == DRILL_LEFT){
            drillUnit.drillingBlock = &world.mGrid(iPlayer -1, jPlayer);
            // Set drillUnit time to drill to the hardness of the block
            drillUnit.drillTime = drillUnit.drillingBlock->mHardness;
        }
        else if(drillUnit.state == DRILL_RIGHT){
            drillUnit.drillingBlock = &world.mGrid(iPlayer +1, jPlayer);
            // Set drillUnit time to drill to the hardness of the block
            drillUnit.drillTime = drillUnit.drillingBlock->mHardness;
        }
    }
    else{
        if(drillUnit.drillingBlock == nullptr){
            // block is nullpointer
        }
        else{
            // Stop drilling when block can't be drilled
            if(drillUnit.drillingBlock->mBreakable == false){
                
                return;
            }

            // drillUnit is drilling a block reduce drillTime by drillpower
            drillUnit.drillTime -= drillUnit.drill.mPower;

            if(drillUnit.drillTime <= 0){
                // Block is mined
                drillUnit.drillTime = 0; // Reset drillTime
                if(gDM.blocks[drillUnit.drillingBlock->mType].mOreID >= 0) drillUnit.addCargo(gDM.blocks[drillUnit.drillingBlock->mType].mOreID); // Add the mined block to the cargo

                drillUnit.drillingBlock->setTileFromBlock(0, gDM.blocks[0]); // Replace block with air
                // Particles
            }
        }
    }

    // Reset drillingBlock when drillUnit is not in drilling mode
    if(!(drillUnit.state == DRILL_LEFT || drillUnit.state == DRILL_RIGHT || drillUnit.state == DRILL_DOWN)){
        drillUnit.drillingBlock = nullptr;
        drillUnit.drillTime = 0;
    }
    else{
        
    }
}