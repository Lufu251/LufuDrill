#include <gameRenderer.hpp>

GameRenderer::GameRenderer(/* args */){}

GameRenderer::~GameRenderer(){}

void GameRenderer::setCameraTarget(Player& player){
    // Camera initialization
    zoom = 2.0f;
    camera.rotation = 0.0f;
    cameraTarget = player.position;
    camera.offset = (Vector2){ GetScreenWidth() / 2.0f - player.size.x, GetScreenHeight() / 2.0f - player.size.y}; // Center the view on the player
    camera.zoom = zoom;
}

void GameRenderer::updateCamera(Player& player){
    cameraTarget = Vector2Lerp(cameraTarget, player.position, 0.6f);
    camera.target = cameraTarget;
}

void GameRenderer::updateCameraOnWindowResize(Player& player){
    if(IsWindowResized()){
        camera.offset = (Vector2){ GetScreenWidth() / 2.0f - player.size.x, GetScreenHeight() / 2.0f - player.size.y}; // Center the view
    }
}

void GameRenderer::renderGrid(Grid& mapGrid){
    // Render game view with the camera ------------------------------------
    BeginMode2D(camera);

    // Render the visible blocks from the grid
    int xRenderAmount = (GetScreenWidth() / mapGrid.blockSize) / (2 * zoom) +2; // Calculate how many tiles are viewed by the camera
    int yRenderAmount = (GetScreenHeight() / mapGrid.blockSize) / (2 * zoom) +2; // Calculate how many tiles are viewed by the camera

    // Calculate the cameraTarget position on the grid
    size_t iCamera = cameraTarget.x / mapGrid.blockSize;
    size_t jCamera = cameraTarget.y / mapGrid.blockSize;

    // Loop over the amount of blocks that are visible
    for (int i = -xRenderAmount; i <= xRenderAmount; i++){
        for (int j = -yRenderAmount; j <= yRenderAmount; j++){
            int iBlock = iCamera + i;
            int jBlock = jCamera + j;
            // Clamp to grid size
            if(iBlock < 0 || iBlock >= mapGrid.sizeX || jBlock < 0 || jBlock >= mapGrid.sizeY){
                continue;
            }

            Block block = mapGrid(iBlock, jBlock); // get the current block

            switch (block.mType)
            {
                case EMPTY: DrawRectangle(block.position.x, block.position.y, mapGrid.blockSize, mapGrid.blockSize, YELLOW); break;
                case DIRT: DrawRectangle(block.position.x, block.position.y, mapGrid.blockSize, mapGrid.blockSize, BROWN); break;
                case ROCK: DrawRectangle(block.position.x, block.position.y, mapGrid.blockSize, mapGrid.blockSize, GRAY); break;
            }
            DrawText(TextFormat("[%i,%i]", iBlock , jBlock), 2 + block.position.x, 6 + block.position.y, 8, LIGHTGRAY);

            DrawRectangleLines(block.position.x, block.position.y, mapGrid.blockSize, mapGrid.blockSize, WHITE);
        }
    }
    EndMode2D();
}

void GameRenderer::renderPlayer(Player& player){
    BeginMode2D(camera);
    
    player.render();

    EndMode2D();
}