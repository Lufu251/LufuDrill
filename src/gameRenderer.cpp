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

void GameRenderer::renderMapGrid(World& world){
    TextureAtlas& tileset = AssetManager::getInstance().getTextureAtlas("tileset");

    // Render game view with the camera ------------------------------------
    BeginMode2D(camera);

    // Render the visible blocks from the grid
    int xRenderAmount = (GetScreenWidth() / world.mBlockSize) / (2 * camera.zoom) +2; // Calculate how many tiles are viewed by the camera
    int yRenderAmount = (GetScreenHeight() / world.mBlockSize) / (2 * camera.zoom) +2; // Calculate how many tiles are viewed by the camera

    // Calculate the cameraTarget position on the grid
    int iCamera = camera.target.x / world.mBlockSize;
    int jCamera = camera.target.y / world.mBlockSize;

    // Loop over the amount of blocks that are visible
    for (int i = -xRenderAmount; i <= xRenderAmount; i++){
        for (int j = -yRenderAmount; j <= yRenderAmount; j++){
            size_t iBlock = iCamera + i;
            size_t jBlock = jCamera + j;
            // Clamp to grid size
            if(iBlock < 0 || iBlock >= world.mGrid.gridSizeX || jBlock < 0 || jBlock >= world.mGrid.gridSizeY){
                continue;
            }

            Block* block = &world.mGrid(iBlock, jBlock); // Set the current block
            switch (block->mType){
                case EMPTY: DrawRectangle(block->position.x, block->position.y, world.mBlockSize, world.mBlockSize, RAYWHITE); break;
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

    for (auto& building : DataManager::getInstance().world.buildings){
            switch (building.mType)
            {
            case GAS_STATION:
                DrawRectangleV(building.position, building.size, BLUE);
                break;
            case TRADER:
                DrawRectangleV(building.position, building.size, YELLOW);
                break;
            case SHOP:
                DrawRectangleV(building.position, building.size, BLUE);
                break;
            default:
                break;
            }
        }
    EndMode2D();
}

void GameRenderer::renderPlayer(){
    BeginMode2D(camera);

    DataManager::getInstance().player.render();

    EndMode2D();
}