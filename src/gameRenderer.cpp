#include <gameRenderer.hpp>

#include <assetManager.hpp>
#include <textureAtlas.hpp>

GameRenderer::GameRenderer(/* args */){}

GameRenderer::~GameRenderer(){}

void GameRenderer::setCameraOffset(const Vector2& offset){
    camera.offset = offset;
}

void GameRenderer::moveCameraToPlayer(){
    camera.target = Vector2Lerp(camera.target, dataManager.player.position, 0.6f);;
}

void GameRenderer::renderGrid(){
    TextureAtlas& tileset = assetManager.getTextureAtlas("tileset");

    // Render game view with the camera ------------------------------------
    BeginMode2D(camera);

    // Render the visible blocks from the grid
    int xRenderAmount = (GetScreenWidth() / dataManager.map.blockSize) / (2 * camera.zoom) +2; // Calculate how many tiles are viewed by the camera
    int yRenderAmount = (GetScreenHeight() / dataManager.map.blockSize) / (2 * camera.zoom) +2; // Calculate how many tiles are viewed by the camera

    // Calculate the cameraTarget position on the grid
    int iCamera = camera.target.x / dataManager.map.blockSize;
    int jCamera = camera.target.y / dataManager.map.blockSize;

    // Loop over the amount of blocks that are visible
    for (int i = -xRenderAmount; i <= xRenderAmount; i++){
        for (int j = -yRenderAmount; j <= yRenderAmount; j++){
            size_t iBlock = iCamera + i;
            size_t jBlock = jCamera + j;
            // Clamp to grid size
            if(iBlock < 0 || iBlock >= dataManager.map.gridSizeX || jBlock < 0 || jBlock >= dataManager.map.gridSizeY){
                continue;
            }

            Block* block = &dataManager.map(iBlock, jBlock); // Set the current block
            switch (block->mType){
                case EMPTY: DrawRectangle(block->position.x, block->position.y, dataManager.map.blockSize, dataManager.map.blockSize, RAYWHITE); break;
                case DIRT: DrawTextureRec(tileset.texture, tileset.sections["DIRT"], block->position, WHITE); break;
                case STONE: DrawTextureRec(tileset.texture, tileset.sections["STONE"], block->position, WHITE);; break;
                case COPPERORE: DrawTextureRec(tileset.texture, tileset.sections["COPPERORE"], block->position, WHITE);; break;
                case GOLDORE: DrawTextureRec(tileset.texture, tileset.sections["GOLDORE"], block->position, WHITE);; break;
                case PLATINUMORE: DrawTextureRec(tileset.texture, tileset.sections["PLATINUMORE"], block->position, WHITE);; break;
            }
        }
    }
    EndMode2D();
}

void GameRenderer::renderBuildings(){
    BeginMode2D(camera);

    dataManager.gasStation.render();
    dataManager.trader.render();
    dataManager.shop.render();

    EndMode2D();
}

void GameRenderer::renderPlayer(){
    BeginMode2D(camera);

    dataManager.player.render();

    EndMode2D();
}