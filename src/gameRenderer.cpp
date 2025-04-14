#include "raylib.h"
#include "world.hpp"
#include <gameRenderer.hpp>

#include <globals.hpp>
#include <textureAtlas.hpp>

GameRenderer::GameRenderer(/* args */){}

GameRenderer::~GameRenderer(){}

void GameRenderer::initialize(){
    lightmap = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    camera.zoom = 2.0f;
    camera.rotation = 0.0f;
    camera.target = gDM.player.mPosition;
    setCameraOffset({GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f});
}

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
    int xRenderAmount = (static_cast<float>(GetScreenWidth()) / world.mBlockSize) / camera.zoom +2; // Calculate how many tiles are viewed by the camera
    int yRenderAmount = (static_cast<float>(GetScreenHeight()) / world.mBlockSize) / camera.zoom +2; // Calculate how many tiles are viewed by the camera

    // Calculate the cameraTarget position on the grid and substract half the amount of rendered pixels
    int iStartBlock = camera.target.x / world.mBlockSize - static_cast<float>(xRenderAmount) /2;
    int jStartBlock = camera.target.y / world.mBlockSize - static_cast<float>(yRenderAmount) /2;

    TextureAtlas& tileset = gAM.getTextureAtlas("tileset");

    // Loop over the amount of blocks that are visible
    for (int i = 0; i <= xRenderAmount; i++){
        for (int j = 0; j <= yRenderAmount; j++){
            size_t iBlock = iStartBlock + i;
            size_t jBlock = jStartBlock + j;
            // Clamp to grid size
            if(iBlock >= world.mGrid.gridSizeX || jBlock >= world.mGrid.gridSizeY){
                continue;
            }

            Tile* tile = &world.mGrid(iBlock, jBlock); // Set the current block

            if(gDM.blocks[tile->mType].mRenderID >= 0) DrawTextureRec(tileset.texture, tileset.sections[gDM.blocks[tile->mType].mRenderID].rect, tile->mPosition, WHITE);
        }
    }
    EndMode2D();
}

void GameRenderer::renderMapBuildings(World& world){
    TextureAtlas& buildingset = gAM.getTextureAtlas("buildingset");

    BeginMode2D(camera);
    for (auto& building : world.buildings){
        Rectangle buildingRec = {building.mPosition.x, building.mPosition.y, building.mSize.x, building.mSize.y};
        DrawTexturePro(buildingset.texture, buildingset.sections[building.mType].rect, buildingRec, {0,0}, 0, WHITE);
    }
    EndMode2D();
}

void GameRenderer::renderPlayer(DrillUnit& player){
    TextureAtlas& buildingset = gAM.getTextureAtlas("drillunitset");

    BeginMode2D(camera);
    switch (player.state)
    {
    case LEFT: DrawTextureRec(buildingset.texture, buildingset.sections[player.state].rect, {player.mPosition.x -6, player.mPosition.y}, WHITE); break;
    case RIGHT: DrawTextureRec(buildingset.texture, buildingset.sections[player.state].rect, {player.mPosition.x, player.mPosition.y}, WHITE); break;
    case DRILL_LEFT: DrawTextureRec(buildingset.texture, buildingset.sections[player.state].rect, {player.mPosition.x -6, player.mPosition.y}, WHITE); break;
    case DRILL_RIGHT: DrawTextureRec(buildingset.texture, buildingset.sections[player.state].rect, {player.mPosition.x, player.mPosition.y}, WHITE); break;
    case DRILL_DOWN: DrawTextureRec(buildingset.texture, buildingset.sections[player.state].rect, {player.mPosition.x, player.mPosition.y}, WHITE); break;
    }
    EndMode2D();
}

void GameRenderer::renderBackground(DrillUnit& player){
    BeginMode2D(camera);
    Texture2D  skyTexture = gAM.getTexture("sky");
    Texture2D  cloudTexture = gAM.getTexture("cloud");
    Texture2D  mountainTexture = gAM.getTexture("mountain");
    

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

void GameRenderer::drawLightmap(DrillUnit& player, World& world){
    // Light parameters
    Color lightColor = WHITE;
    Texture2D* lightTexture = &gAM.getTexture("light_texture");
    float scale = gDM.world.mBlockSize / static_cast<float>(lightTexture->height) * 3.5;


    // Render the visible blocks from the grid
    int xRenderAmount = (static_cast<float>(GetScreenWidth()) / world.mBlockSize) / camera.zoom +2; // Calculate how many tiles are viewed by the camera
    int yRenderAmount = (static_cast<float>(GetScreenHeight()) / world.mBlockSize) / camera.zoom +2; // Calculate how many tiles are viewed by the camera

    // Calculate the cameraTarget position on the grid and substract half the amount of rendered pixels
    int iStartBlock = camera.target.x / world.mBlockSize - static_cast<float>(xRenderAmount) /2;
    int jStartBlock = camera.target.y / world.mBlockSize - static_cast<float>(yRenderAmount) /2;

    // Start drawing to the lightmap
    BeginTextureMode(lightmap);
        ClearBackground(BLACK); // Darkness base

        BeginMode2D(camera); // Camera mode
            BeginBlendMode(BLEND_ADDITIVE); // Blend mode
            // ------------ START DRAW ------------
            // Draw player light
            DrawCircleGradient(player.mPosition.x + player.mSize.x / 2, player.mPosition.y + player.mSize.y / 2, gDM.world.mBlockSize * gDM.playerSight, lightColor, BLACK);

            // Draw grid lights
            for (int i = 0; i <= xRenderAmount; i++){
                for (int j = 0; j <= yRenderAmount; j++){
                    size_t iBlock = iStartBlock + i;
                    size_t jBlock = jStartBlock + j;
                    // Clamp to grid size
                    if(iBlock >= world.mGrid.gridSizeX || jBlock >= world.mGrid.gridSizeY){
                        continue;
                    }
                    Tile* tile = &world.mGrid(iBlock, jBlock); // Set the current block
                    if(gDM.blocks[tile->mType].mLight == true){
                        Vector2 tSize = {static_cast<float>(lightTexture->width * scale), static_cast<float>(lightTexture->height * scale)};
                        DrawTextureEx(*lightTexture, tile->mPosition + tile->mSize /2 - tSize /2, 0.0f, scale, lightColor);
                    }
                }
            }

            // ------------ END DRAW ------------
            EndBlendMode();
        EndMode2D();
    EndTextureMode();
}

void GameRenderer::renderLightmap(){
        // Overlay the lightmap with multiplicative blending
        BeginBlendMode(BLEND_MULTIPLIED);
            DrawTextureRec(lightmap.texture,
                            { 0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(-GetScreenHeight())}, // Flip Y
                            { 0, 0 },
                            WHITE);
        EndBlendMode();

}