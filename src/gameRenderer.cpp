#include <gameRenderer.hpp>

#include <assetManager.hpp>
#include <textureAtlas.hpp>

GameRenderer::GameRenderer(/* args */){}

GameRenderer::~GameRenderer(){}

void GameRenderer::setCameraOffset(const Vector2& offset){
    camera.offset = offset;
}

void GameRenderer::moveCameraToPosition(const Vector2& position){
    camera.target = Vector2Lerp(camera.target, position, 0.6f);
}

void GameRenderer::renderMapGrid(Grid& map){
    TextureAtlas& tileset = AssetManager::getInstance().getTextureAtlas("tileset");

    // Render game view with the camera ------------------------------------
    BeginMode2D(camera);

    // Render the visible blocks from the grid
    int xRenderAmount = (GetScreenWidth() / map.blockSize) / (2 * camera.zoom) +2; // Calculate how many tiles are viewed by the camera
    int yRenderAmount = (GetScreenHeight() / map.blockSize) / (2 * camera.zoom) +2; // Calculate how many tiles are viewed by the camera

    // Calculate the cameraTarget position on the grid
    int iCamera = camera.target.x / map.blockSize;
    int jCamera = camera.target.y / map.blockSize;

    // Loop over the amount of blocks that are visible
    for (int i = -xRenderAmount; i <= xRenderAmount; i++){
        for (int j = -yRenderAmount; j <= yRenderAmount; j++){
            size_t iBlock = iCamera + i;
            size_t jBlock = jCamera + j;
            // Clamp to grid size
            if(iBlock < 0 || iBlock >= map.gridSizeX || jBlock < 0 || jBlock >= map.gridSizeY){
                continue;
            }

            Block* block = &map(iBlock, jBlock); // Set the current block
            switch (block->mType){
                case EMPTY: DrawRectangle(block->position.x, block->position.y, map.blockSize, map.blockSize, RAYWHITE); break;
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

void GameRenderer::renderMapBuildings(){
    BeginMode2D(camera);

    // GasStation
    DrawRectangleV(DataManager::getInstance().gasStation.position, DataManager::getInstance().gasStation.size, BLUE);
    // Trader
    DrawRectangleV(DataManager::getInstance().trader.position, DataManager::getInstance().trader.size, YELLOW);
    // ToolShop
    DrawRectangleV(DataManager::getInstance().shop.position, DataManager::getInstance().shop.size, BLUE);

    EndMode2D();
}

void GameRenderer::renderPlayer(){
    BeginMode2D(camera);

    DataManager::getInstance().player.render();

    EndMode2D();
}