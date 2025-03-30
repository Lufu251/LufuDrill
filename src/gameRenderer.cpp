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
    // Render game view with the camera ------------------------------------
    BeginMode2D(camera);

    // Render the visible blocks from the grid
    int xRenderAmount = (GetScreenWidth() / world.mBlockSize) / camera.zoom +2; // Calculate how many tiles are viewed by the camera
    int yRenderAmount = (GetScreenHeight() / world.mBlockSize) / camera.zoom +2; // Calculate how many tiles are viewed by the camera

    // Calculate the cameraTarget position on the grid and substract half the amount of rendered pixels
    int iStartBlock = camera.target.x / world.mBlockSize - xRenderAmount /2;
    int jStartBlock = camera.target.y / world.mBlockSize - yRenderAmount /2;

    TextureAtlas& tileset = AssetManager::getInstance().getTextureAtlas("tileset");
    Rectangle* rec_DIRT = &tileset.sections["DIRT"];
    Rectangle* rec_STONE = &tileset.sections["STONE"];
    Rectangle* rec_COPPERORE = &tileset.sections["COPPERORE"];
    Rectangle* rec_GOLDORE = &tileset.sections["GOLDORE"];
    Rectangle* rec_PLATINUMORE = &tileset.sections["PLATINUMORE"];

    // Loop over the amount of blocks that are visible
    for (int i = 0; i <= xRenderAmount; i++){
        for (int j = 0; j <= yRenderAmount; j++){
            size_t iBlock = iStartBlock + i;
            size_t jBlock = jStartBlock + j;
            // Clamp to grid size
            if(iBlock >= world.mGrid.gridSizeX || jBlock >= world.mGrid.gridSizeY){
                continue;
            }

            Block* block = &world.mGrid(iBlock, jBlock); // Set the current block
            Color color = BLACK;
            if(!block->blocking || block->discovered) color = WHITE;
            switch (block->mType){
                case EMPTY: break;
                case DIRT: DrawTextureRec(tileset.texture, *rec_DIRT, block->position, color); break;
                case STONE: DrawTextureRec(tileset.texture, *rec_STONE, block->position, color); break;
                case COPPERORE: DrawTextureRec(tileset.texture, *rec_COPPERORE, block->position, color); break;
                case GOLDORE: DrawTextureRec(tileset.texture, *rec_GOLDORE, block->position, color); break;
                case PLATINUMORE: DrawTextureRec(tileset.texture, *rec_PLATINUMORE, block->position, color); break;
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

void GameRenderer::renderPlayer(DrillUnit& player){
    TextureAtlas& buildingset = AssetManager::getInstance().getTextureAtlas("drillunitset");

    BeginMode2D(camera);
    switch (player.state)
    {
    case LEFT: DrawTextureRec(buildingset.texture, buildingset.sections["LEFT"], {player.position.x -6, player.position.y}, WHITE); break;
    case RIGHT: DrawTextureRec(buildingset.texture, buildingset.sections["RIGHT"],player.position, WHITE); break;
    case DRILL_DOWN: DrawTextureRec(buildingset.texture, buildingset.sections["DOWN"],player.position, GRAY); break;
    case DRILL_LEFT: DrawTextureRec(buildingset.texture, buildingset.sections["LEFT"], {player.position.x -6, player.position.y}, GRAY); break;
    case DRILL_RIGHT:DrawTextureRec(buildingset.texture, buildingset.sections["RIGHT"],player.position, GRAY); break;
    }
    EndMode2D();
}

void GameRenderer::renderBackground(DrillUnit& player){
    BeginMode2D(camera);
    Texture2D  skyTexture = AssetManager::getInstance().getTexture("sky");
    Texture2D  cloudTexture = AssetManager::getInstance().getTexture("cloud");
    Texture2D  mountainTexture = AssetManager::getInstance().getTexture("mountain");
    

    float paralax3 = camera.target.x - (camera.target.x + camera.offset.x) * 0.3;
    float paralax2 = camera.target.x - (camera.target.x + camera.offset.x) * 0.5;
    float paralax1 = camera.target.x - (camera.target.x + camera.offset.x) * 0.7;

    float yOffset = 0;
    float xOffset = -1000;
    float scale = 1.5;

    int repeat = 12;

    for(float i = 0; i < repeat; i++){
        DrawTextureEx(skyTexture, {mountainTexture.width * i * scale + paralax3 + xOffset, yOffset}, 0, scale, {255,255,255,255});
    }
    for(float i = 0; i < repeat; i++){
        DrawTextureEx(cloudTexture, {mountainTexture.width * i * scale + paralax2 + xOffset, yOffset}, 0, scale, {255,255,255,255});
    }
    for(float i = 0; i < repeat; i++){
        DrawTextureEx(mountainTexture, {mountainTexture.width * i * scale + paralax1 + xOffset, yOffset}, 0, scale, {255,255,255,255});
    }

    
    EndMode2D();
}