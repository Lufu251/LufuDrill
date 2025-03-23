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

void GameRenderer::clampCameraToGrid(AABB& box, World& world){
    camera.target.x = std::clamp(camera.target.x, camera.offset.x /camera.zoom, static_cast<float>(world.mGrid.gridSizeX * world.mBlockSize - camera.offset.x /camera.zoom));
    camera.target.y = std::clamp(camera.target.y, camera.offset.y /camera.zoom, static_cast<float>(world.mGrid.gridSizeY * world.mBlockSize - camera.offset.y /camera.zoom));
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
                case EMPTY: break;
                case DIRT: DrawTextureRec(tileset.texture, tileset.sections["DIRT"], block->position, WHITE); break;
                case STONE: DrawTextureRec(tileset.texture, tileset.sections["STONE"], block->position, WHITE); break;
                case COPPERORE: DrawTextureRec(tileset.texture, tileset.sections["COPPERORE"], block->position, WHITE); break;
                case GOLDORE: DrawTextureRec(tileset.texture, tileset.sections["GOLDORE"], block->position, WHITE); break;
                case PLATINUMORE: DrawTextureRec(tileset.texture, tileset.sections["PLATINUMORE"], block->position, WHITE); break;
            }
        }
    }
    EndMode2D();
}

void GameRenderer::renderMapBuildings(World& world){
    TextureAtlas& buildingset = AssetManager::getInstance().getTextureAtlas("buildingset");

    BeginMode2D(camera);

    for (auto& building : world.buildings){
            Rectangle buildingRec = {building.position.x, building.position.y, building.size.x, building.size.y};
            switch (building.mType)
            {
            case GAS_STATION: DrawTexturePro(buildingset.texture, buildingset.sections["GAS_STATION"], buildingRec, {0,0}, 0, WHITE); break;
            case TRADER: DrawTexturePro(buildingset.texture, buildingset.sections["TRADER"], buildingRec, {0,0}, 0, WHITE); break;
            case TOOL_SHOP: DrawTexturePro(buildingset.texture, buildingset.sections["TOOL_SHOP"], buildingRec, {0,0}, 0, WHITE); break;
            case EQUIPMENT_SHOP: DrawTexturePro(buildingset.texture, buildingset.sections["EQUIPMENT_SHOP"], buildingRec, {0,0}, 0, WHITE); break;
            }
        }
    EndMode2D();
}

void GameRenderer::renderPlayer(Player& player){
    BeginMode2D(camera);

    DrawRectangleV(player.position, player.size, RED);

    EndMode2D();
}

void GameRenderer::renderBackground(Player& player){
    BeginMode2D(camera);
    Texture2D  skyTexture = AssetManager::getInstance().getTexture("sky");
    Texture2D  cloudTexture = AssetManager::getInstance().getTexture("cloud");
    Texture2D  mountainTexture = AssetManager::getInstance().getTexture("mountain");
    

    float paralaxLayer1 = 0;
    float paralaxLayer2 = player.position.x / 15;
    float paralaxLayer3 = player.position.x / 40;

    float yOffset = 0;
    float scale = 1.5;

    int repeat = 12;
    for(float i = 0; i < repeat; i++){
        DrawTextureEx(skyTexture, {skyTexture.width * i * scale - paralaxLayer1, yOffset}, 0, scale, {255,255,255,255});
    }
    for(float i = 0; i < repeat; i++){
        DrawTextureEx(cloudTexture, {cloudTexture.width * i * scale  - paralaxLayer2, yOffset}, 0, scale, {255,255,255,255});
    }
    for(float i = 0; i < repeat; i++){
        DrawTextureEx(mountainTexture, {mountainTexture.width * i * scale  - paralaxLayer3, yOffset}, 0, scale, {255,255,255,255});
    }
    
    EndMode2D();
}