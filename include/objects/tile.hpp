#pragma once

#include <aabb.hpp>
#include <block.hpp>

#include <nlohmann/json.hpp>

class Tile : public AABB{
public:
    int mType = -1;
    bool mBlocking;
    bool mBreakable = true;
    int mHardness;

    Tile(){}
    Tile(const Vector2 rPosition, const Vector2 rSize) : AABB(rPosition, rSize){}
    ~Tile(){}

    void setTileFromBlock(int id, Block& block){
        mType = id;
        mBlocking = block.mBlocking;
        mHardness = block.mHardness;
    }
};

// Convert Tile to JSON
inline void to_json(nlohmann::json& j, const Tile& t) {
    j = {
        {"type", t.mType},
        {"blocking", t.mBlocking},
        {"breakable", t.mBreakable},
        {"hardness", t.mHardness}
    };
}

// Convert JSON to Tile
inline void from_json(const nlohmann::json& j, Tile& t) {
    j.at("type").get_to(t.mType);
    j.at("blocking").get_to(t.mBlocking);
    j.at("breakable").get_to(t.mBreakable);
    j.at("hardness").get_to(t.mHardness);
}