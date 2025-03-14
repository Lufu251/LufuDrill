#include <gameRenderer.hpp>

#include <assetManager.hpp>
#include <textureAtlas.hpp>

GameRenderer::GameRenderer(/* args */){}

GameRenderer::~GameRenderer(){}

void GameRenderer::setCameraOffset(Vector2 offset){
    camera.offset = offset;
}

void GameRenderer::moveCameraToPlayer(Player& player){
    camera.target = Vector2Lerp(camera.target, player.position, 0.6f);;
}

void GameRenderer::renderGrid(Grid& mapGrid){
    AssetManager& assetManager = AssetManager::getInstance();
    TextureAtlas& tileset = assetManager.getTextureAtlas("tileset");

    // Render game view with the camera ------------------------------------
    BeginMode2D(camera);

    // Render the visible blocks from the grid
    int xRenderAmount = (GetScreenWidth() / mapGrid.blockSize) / (2 * camera.zoom) +2; // Calculate how many tiles are viewed by the camera
    int yRenderAmount = (GetScreenHeight() / mapGrid.blockSize) / (2 * camera.zoom) +2; // Calculate how many tiles are viewed by the camera

    // Calculate the cameraTarget position on the grid
    int iCamera = camera.target.x / mapGrid.blockSize;
    int jCamera = camera.target.y / mapGrid.blockSize;

    // Loop over the amount of blocks that are visible
    for (int i = -xRenderAmount; i <= xRenderAmount; i++){
        for (int j = -yRenderAmount; j <= yRenderAmount; j++){
            int iBlock = iCamera + i;
            int jBlock = jCamera + j;
            // Clamp to grid size
            if(iBlock < 0 || iBlock >= mapGrid.gridSizeX || jBlock < 0 || jBlock >= mapGrid.gridSizeY){
                continue;
            }

            Block* block = &mapGrid(iBlock, jBlock); // gSet the current block
            switch (block->mType){
                case EMPTY: DrawRectangle(block->position.x, block->position.y, mapGrid.blockSize, mapGrid.blockSize, RAYWHITE); break;
                case DIRT: DrawTextureRec(tileset.texture, tileset.sections["DIRT"], block->position, WHITE); break;
                case STONE: DrawTextureRec(tileset.texture, tileset.sections["STONE"], block->position, WHITE);; break;
                case COPPERORE: DrawTextureRec(tileset.texture, tileset.sections["COPPERORE"], block->position, WHITE);; break;
                case GOLDORE: DrawTextureRec(tileset.texture, tileset.sections["GOLDORE"], block->position, WHITE);; break;
                case PLATINUMORE: DrawTextureRec(tileset.texture, tileset.sections["PLATINUMORE"], block->position, WHITE);; break;
            }
            //DrawText(TextFormat("[%i,%i]", iBlock , jBlock), 2 + block->position.x, 6 + block->position.y, 6, LIGHTGRAY);

            //DrawRectangleLines(block.position.x, block.position.y, mapGrid.blockSize, mapGrid.blockSize, WHITE);
        }
    }
    EndMode2D();
}

void GameRenderer::renderPlayer(Player& player){
    BeginMode2D(camera);
    
    player.render();

    EndMode2D();
}