#include <gameHandler.hpp>

#include <iostream>
#include <algorithm>

#include <globalVariables.hpp>

GameHandler::GameHandler(/* args */){}

GameHandler::~GameHandler(){}

void GameHandler::handleInput(Vector2& rDirection){
    // Player input
    if(IsKeyDown(KEY_A)) rDirection.x += -0.5;
    if(IsKeyDown(KEY_D)) rDirection.x += 0.5;
    if(IsKeyDown(KEY_W)) rDirection.y += -1;
}

void GameHandler::generateTerrain(Grid& grid){
    for (size_t x = 0; x < grid.sizeX; x++){
            for (size_t y = 0; y < grid.sizeY; y++){
                if(y > 15 && x != 5){
                    grid(x,y) = Block(ROCK, {static_cast<float>(x * grid.s), static_cast<float>(y * grid.s)}, {static_cast<float>(grid.s), static_cast<float>(grid.s)});
                }
                else{
                    grid(x,y) = Block(EMPTY, {static_cast<float>(x * grid.s), static_cast<float>(y * grid.s)}, {static_cast<float>(grid.s), static_cast<float>(grid.s)});
                }
            }
        }

        grid(10,5).mType = ROCK; grid(15,15).mType = ROCK; grid(15,14).mType = ROCK;

        grid(0,15).mType = ROCK; grid(0,14).mType = ROCK; grid(0,13).mType = ROCK; grid(0,12).mType = ROCK;
        grid(0,11).mType = ROCK; grid(0,10).mType = ROCK; grid(0,9).mType = ROCK; grid(0,8).mType = ROCK; 

        grid(19,15).mType = ROCK; grid(19,14).mType = ROCK; grid(19,13).mType = ROCK; grid(19,12).mType = ROCK;
        
        grid(10,14).mType = ROCK;

        grid(12,14).mType = ROCK; grid(13,14).mType = ROCK;

        grid(8,10).mType = ROCK; grid(9,10).mType = ROCK;
}

std::vector<AABB> GameHandler::getPossibleCollisionsFromGrid(AABB& box, Grid& grid){
    std::vector<AABB> blocks;
    blocks.reserve(9);

    // Get the player position on the grid
    size_t iPlayer = box.getGridPosition(grid.s).x;
    size_t jPlayer = box.getGridPosition(grid.s).y;

    // Check how many tiles around the player need to be calculated
    int gridDistancex = Vector2Length(box.v) / static_cast<float>(grid.s) + std::ceil(box.s.x / grid.s); // amount of tiles that need to be checked
    int gridDistancey = Vector2Length(box.v) / static_cast<float>(grid.s) + std::ceil(box.s.y / grid.s); // amount of tiles that need to be checked

    // Loop blocks near player
    for (int i = -gridDistancex; i <= gridDistancex; i++){
        for (int j = -gridDistancey; j <= gridDistancey; j++){
            // Calculate the gridposition of the block that needs to be checked
            int iBlock = iPlayer + i;
            int jBlock = jPlayer + j;

            // Check if position is out of bound from grid and skip this loop
            if (iBlock < 0 || iBlock > grid.sizeX -1 || jBlock < 0 || jBlock > grid.sizeY -1){
                // Out of bound
                //std::cout << "Collision check: Grid position is out ofbound" << iBlock << " " << jBlock << std::endl;
                continue;
            }
            else if(grid(iBlock, jBlock).mType > 0){ // In bound block needs to be checked
                // Add block to the list
                blocks.push_back(grid(iBlock, jBlock));
            }
        }
    }
    return blocks;
}

void GameHandler::collisionResponse(AABB& box, Grid& grid){
    AABB boxClone = box;

    //std::cout << std::endl << "------------- Collision START -------------" << std::endl;
    // Loop a cloned Player with the collision and end loop when movement is done
    while(Vector2Length(boxClone.v) > 0){
        
        // Create a list with block that need to be checked
        std::vector<AABB> blocks = getPossibleCollisionsFromGrid(box, grid); // AABB from all blocks that can collide
        // Get the nearest collision from the List
        Hit hit = GetNearestCollisionFromList(boxClone, blocks);

        // When nearest is less then one a hit occured
        if(hit.collisionTime < 1.0f){
            // Hit occured
            
            // Round position to prevent floating point inside of a block
            if(hit.n.x < 0){
                // LEFT
                //std::cout << "Collision on LEFT" << " x " << hit.x << " y " << hit.y << " distance " << hit.collisionTime << std::endl;

                // Dampening everytime a hit occures
                boxClone.v.x *= ext_bounceDamping;
                box.v.x *= ext_bounceDamping;
            }
            else if(hit.n.x > 0){
                // RIGHT
                //std::cout << "Collision on RIGHT" << " x " << hit.x << " y " << hit.y << " distance " << hit.collisionTime << std::endl;

                // Dampening everytime a hit occures
                boxClone.v.x *= ext_bounceDamping;
                box.v.x *= ext_bounceDamping;
            }
            if(hit.n.y < 0){
                // TOP
                //std::cout << "Collision on TOP" << " x " << hit.x << " y " << hit.y << " distance " << hit.collisionTime << std::endl;

                // Dampening everytime a hit occures
                boxClone.v.y *= ext_bounceDamping;
                box.v.y *= ext_bounceDamping;

                // Slow when moving over ground
                boxClone.v.x *= ext_onGroundSlow;
                box.v.x *= ext_onGroundSlow;
            }
            else if(hit.n.y > 0){
                // DOWN
                //std::cout << "Collision on Bottom" << " x " << hit.x << " y " << hit.y << " distance " << hit.collisionTime << std::endl;

                // Dampening everytime a hit occures
                boxClone.v.y *= ext_bounceDamping;
                box.v.y *= ext_bounceDamping;
            }

            boxClone.p = hit.p; // Move to the point of collision

            // Set cPlayer velocity to the remaining velocity
            float remainingTime = 1.0f - hit.collisionTime; // Calculate remaining distance after the collision
            boxClone.v *= remainingTime;

            // reflect the velocity of cPlayer and player
            if (abs(hit.n.x) > 0.0001f) box.v.x = -box.v.x;
            if (abs(hit.n.y) > 0.0001f) box.v.y = -box.v.y;

            if (abs(hit.n.x) > 0.0001f) boxClone.v.x = -boxClone.v.x;
            if (abs(hit.n.y) > 0.0001f) boxClone.v.y = -boxClone.v.y;
        }
        else{
            // No hit occured do a regular move
            boxClone.p += boxClone.v; // regulary add velocity
            boxClone.v = {0 , 0}; // Reset velocity all movement has been done / END Loop

            //std::cout << "------------- Collision END -------------" << std::endl;
        }
    }
    box.p = boxClone.p; // Loop has ended player can now be set to the new position
}

void GameHandler::checkPlayerTouchingBlocks(AABB& box, Grid& grid){
    bool up = false, right = false, down = false, left = false;

    // Distance which is considered as touching
    float epsilon = 0.1;

    // Get the player position on the grid
    size_t iPlayer = box.getGridPosition(grid.s).x;
    size_t jPlayer = box.getGridPosition(grid.s).y;

    // Check how many tiles around the player need to be calculated
    int gridDistancex = Vector2Length(box.v) / static_cast<float>(grid.s) + std::ceil(box.s.x / grid.s); // amount of tiles that need to be checked
    int gridDistancey = Vector2Length(box.v) / static_cast<float>(grid.s) + std::ceil(box.s.y / grid.s); // amount of tiles that need to be checked

    // Loop blocks near player
    for (int i = -gridDistancex; i <= gridDistancex; i++){
        for (int j = -gridDistancey; j <= gridDistancey; j++){
            // Calculate the gridposition of the block that needs to be checked
            int iBlock = iPlayer + i;
            int jBlock = jPlayer + j;

            // Check if position is out of bound from grid and skip this loop
            if (iBlock < 0 || iBlock > grid.sizeX -1 || jBlock < 0 || jBlock > grid.sizeY -1){
                // Out of bound
                //std::cout << "Collision check: Grid position is out ofbound" << iBlock << " " << jBlock << std::endl;
                continue;
            }
            else if(grid(iBlock, jBlock).mType > 0){ // Block is not empty
                // Check block for touching side ---------------------------------------------------
                Block& block = grid(iBlock, jBlock);
            }
        }
    }
}