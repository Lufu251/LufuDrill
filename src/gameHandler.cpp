#include <gameHandler.hpp>

#include <iostream>
#include <algorithm>

#include <globalVariables.hpp>

GameHandler::GameHandler(/* args */){}

GameHandler::~GameHandler(){}

void GameHandler::initialize(){
    player = Player({200,200}, {24,24}, {0,0});
    mapGrid = Grid(100,100, 32);
    fuelMenu = FuelMenu({100,100}, {600,600});
    playerGui = PlayerGui({0,0}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
    fuelMenu.initialize();
    playerGui.initialize();
    playerGui.enable();
    

    // Camera initialization
    zoom = 2.0f;
    cameraTarget = player.p;
    camera.rotation = 0.0f;
    camera.offset = (Vector2){ GetScreenWidth() / 2.0f + player.s.x/2, GetScreenHeight() / 2.0f + player.s.y/2}; // Center the view on the player
    camera.zoom = zoom;

    generateTerrain();

    std::cout << "Game initialized" << std::endl;
}

void GameHandler::update(float deltaTime){
    // Update and smooth camera movement
    cameraTarget = Vector2Lerp(cameraTarget, player.p, 0.2f);
    camera.target = cameraTarget;

    // Movement input
    Vector2 direction{0,0};
    handleInput(direction);
    direction = Vector2Scale(direction, ext_speed); // Multiply by speed

    Vector2 frameVelocityChange{0,0};
    frameVelocityChange = direction + ext_gravity; // Update frames velocity change that will be added to the players velocity

    player.addVelocity(frameVelocityChange); // Add gravity and player input to the players velocity
    
    player.v = Vector2Scale(player.v, ext_friction); // Apply air friction

    // Stop completely if below the threshold
    if (Vector2Length(player.v) < ext_velocityThreshold) {
            player.v = { 0.0f, 0.0f };
    }

    // Physics collision response
    int d  = collisionResponse(player, mapGrid);
    player.top = false;
    player.right = false;
    player.bottom = false;
    player.left = false;
    if(d == 1){
        player.top = true;
    }
    if(d == 2){
        player.right = true;
    }
    if(d == 3){
        player.bottom = true;
    }
    if(d== 4){
        player.left = true;
    }

    if(player.top && player.topLast){
        std::cout << "On Top" << std::endl;
    }
    if(player.right && player.rightLast){
        std::cout << "On Right" << std::endl;
    }
    if(player.bottom && player.bottomLast){
        std::cout << "On Bottom" << std::endl;
    }
    if(player.left && player.leftLast){
        std::cout << "On Left" << std::endl;
    }


    player.topLast = player.top;
    player.rightLast = player.right;
    player.bottomLast = player.bottom;
    player.leftLast = player.left;

    // Clamp player to grdMap
    player.p.x = std::clamp(player.p.x, 0.f, static_cast<float>(mapGrid.sizeX * mapGrid.s - player.s.x));
    player.p.y = std::clamp(player.p.y, 0.f, static_cast<float>(mapGrid.sizeY * mapGrid.s - player.s.y));

    playerGui.update();
    fuelMenu.update();
    
    if(IsWindowResized()){
        camera.offset = (Vector2){ GetScreenWidth() / 2.0f - player.s.x, GetScreenHeight() / 2.0f - player.s.y}; // Center the view
    }
}

void GameHandler::render(){

    // Render game view with the camera ------------------------------------
    BeginMode2D(camera);

    // Render the visible blocks from the grid
    int xRenderAmount = (GetScreenWidth() / mapGrid.s) / (2 * zoom) +1; // Calculate how many tiles are viewed by the camera
    int yRenderAmount = (GetScreenHeight() / mapGrid.s) / (2 * zoom) +1; // Calculate how many tiles are viewed by the camera
    // Calculate the cameraTarget position on the grid
    size_t iCamera = cameraTarget.x / mapGrid.s; 
    size_t jCamera = cameraTarget.y / mapGrid.s;

    // Loop over the amount of blocks that are visible
    for (int i = -xRenderAmount; i <= xRenderAmount; i++){
        for (int j = -yRenderAmount; j <= yRenderAmount; j++){
            int iBlock = iCamera + i;
            int jBlock = jCamera + j;
            // Clamp to grid size
            if(iBlock < 0 || iBlock > mapGrid.sizeX || jBlock < 0 || iBlock > mapGrid.sizeY){
                continue;
            }

            Block block = mapGrid(iBlock, jBlock); // get the current block

            switch (block.mType)
            {
                case EMPTY: DrawRectangle(block.p.x, block.p.y, mapGrid.s, mapGrid.s, YELLOW); break;
                case DIRT: DrawRectangle(block.p.x, block.p.y, mapGrid.s, mapGrid.s, BROWN); break;
                case ROCK: DrawRectangle(block.p.x, block.p.y, mapGrid.s, mapGrid.s, GRAY); break;
            }
            //DrawText(TextFormat("[%i,%i]", iBlock , jBlock), 2 + block.p.x, 6 + block.p.y, 8, LIGHTGRAY);

            DrawRectangleLines(block.p.x, block.p.y, mapGrid.s, mapGrid.s, WHITE);
        }
    }
    
    player.render();

    EndMode2D();

    // Render Gui view ------------------------------------

    playerGui.render();
    fuelMenu.render();

}

void GameHandler::handleInput(Vector2& rDirection){
    // Player input
    if(IsKeyDown(KEY_A)) rDirection.x += -0.5;
    if(IsKeyDown(KEY_D)) rDirection.x += 0.5;
    if(IsKeyDown(KEY_W)) rDirection.y += -1;
}

void GameHandler::generateTerrain(){
    for (size_t x = 0; x < mapGrid.sizeX; x++){
            for (size_t y = 0; y < mapGrid.sizeY; y++){
                if(y > 15 && x != 5){
                    mapGrid(x,y) = Block(ROCK, {static_cast<float>(x * mapGrid.s), static_cast<float>(y * mapGrid.s)}, {static_cast<float>(mapGrid.s), static_cast<float>(mapGrid.s)});
                }
                else{
                    mapGrid(x,y) = Block(EMPTY, {static_cast<float>(x * mapGrid.s), static_cast<float>(y * mapGrid.s)}, {static_cast<float>(mapGrid.s), static_cast<float>(mapGrid.s)});
                }
            }
        }

        mapGrid(10,5).mType = ROCK; mapGrid(15,15).mType = ROCK; mapGrid(15,14).mType = ROCK;

        mapGrid(0,15).mType = ROCK; mapGrid(0,14).mType = ROCK; mapGrid(0,13).mType = ROCK; mapGrid(0,12).mType = ROCK;
        mapGrid(0,11).mType = ROCK; mapGrid(0,10).mType = ROCK; mapGrid(0,9).mType = ROCK; mapGrid(0,8).mType = ROCK; 

        mapGrid(19,15).mType = ROCK; mapGrid(19,14).mType = ROCK; mapGrid(19,13).mType = ROCK; mapGrid(19,12).mType = ROCK;
        
        mapGrid(10,14).mType = ROCK;

        mapGrid(12,14).mType = ROCK; mapGrid(13,14).mType = ROCK;

        mapGrid(8,10).mType = ROCK; mapGrid(9,10).mType = ROCK;
}

std::vector<AABB> GameHandler::getPossibleCollisionsFromGrid(AABB& b, Grid& grid){
    std::vector<AABB> blocks;
    blocks.reserve(9);

    // Get the player position on the grid
    size_t iPlayer = b.getGridPosition(grid.s).x;
    size_t jPlayer = b.getGridPosition(grid.s).y;

    // Check how many tiles around the player need to be calculated
    int gridDistance = Vector2Length(b.v) / static_cast<float>(grid.s) +1; // amount of tiles that need to be checked

    // Loop blocks near player
    for (int i = -gridDistance; i <= gridDistance; i++){
        for (int j = -gridDistance; j <= gridDistance; j++){
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

int GameHandler::collisionResponse(AABB& b, Grid& grid){
    AABB cb = b;
    int d = -1;

    //std::cout << std::endl << "------------- Collision START -------------" << std::endl;
    // Loop a cloned Player with the collision and end loop when movement is done
    while(Vector2Length(cb.v) > 0){
        
        // Create a list with block that need to be checked
        std::vector<AABB> blocks = getPossibleCollisionsFromGrid(b, grid); // AABB from all blocks that can collide
        // Get the nearest collision from the List
        Hit hit = GetNearestCollisionFromList(cb, blocks);

        // When nearest is less then one a hit occured
        if(hit.collisionTime < 1.0f){
            // Hit occured
            
            // Round position to prevent floating point inside of a block
            if(hit.n.x < 0){
                // LEFT
                //std::cout << "Collision on LEFT" << " x " << hit.x << " y " << hit.y << " distance " << hit.collisionTime << std::endl;
                d = 4;

                // Dampening everytime a hit occures
                cb.v.x *= ext_bounceDamping;
                b.v.x *= ext_bounceDamping;
            }
            else if(hit.n.x > 0){
                // RIGHT
                //std::cout << "Collision on RIGHT" << " x " << hit.x << " y " << hit.y << " distance " << hit.collisionTime << std::endl;
                d = 2;

                // Dampening everytime a hit occures
                cb.v.x *= ext_bounceDamping;
                b.v.x *= ext_bounceDamping;
            }
            if(hit.n.y < 0){
                // TOP
                //std::cout << "Collision on TOP" << " x " << hit.x << " y " << hit.y << " distance " << hit.collisionTime << std::endl;
                d = 1;

                // Dampening everytime a hit occures
                cb.v.y *= ext_bounceDamping;
                b.v.y *= ext_bounceDamping;

                // Slow when moving over ground
                cb.v.x *= ext_onGroundSlow;
                b.v.x *= ext_onGroundSlow;
            }
            else if(hit.n.y > 0){
                // DOWN
                //std::cout << "Collision on Bottom" << " x " << hit.x << " y " << hit.y << " distance " << hit.collisionTime << std::endl;
                d = 3;

                // Dampening everytime a hit occures
                cb.v.y *= ext_bounceDamping;
                b.v.y *= ext_bounceDamping;
            }

            cb.p = hit.p; // Move to the point of collision

            // Set cPlayer velocity to the remaining velocity
            float remainingTime = 1.0f - hit.collisionTime; // Calculate remaining distance after the collision
            cb.v *= remainingTime;

            // reflect the velocity of cPlayer and player
            if (abs(hit.n.x) > 0.0001f) player.v.x = -player.v.x;
            if (abs(hit.n.y) > 0.0001f) player.v.y = -player.v.y;

            if (abs(hit.n.x) > 0.0001f) cb.v.x = -cb.v.x;
            if (abs(hit.n.y) > 0.0001f) cb.v.y = -cb.v.y;
        }
        else{
            // No hit occured do a regular move
            cb.p += cb.v; // regulary add velocity
            cb.v = {0 , 0}; // Reset velocity all movement has been done / END Loop

            //std::cout << "------------- Collision END -------------" << std::endl;
        }
    }
    b.p = cb.p; // Loop has ended player can now be set to the new position

    return d;
}