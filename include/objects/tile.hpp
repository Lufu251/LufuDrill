#pragma once

#include <aabb.hpp>

#include <nlohmann/json.hpp>

class Tile : public AABB{
public:
    size_t mType = -1;
    bool blocking = true;
    bool discovered;
    bool breakable;
    int hardness;

    Tile(){}
    Tile(const Vector2 rPosition, const Vector2 rSize) : AABB(rPosition, rSize){}
    ~Tile(){}
};

// Convert Tile to JSON
inline void to_json(nlohmann::json& j, const Tile& t) {
    j = {
        {"type", t.mType},
        {"blocking", t.blocking},
        {"discovered", t.discovered},
        {"breakable", t.breakable},
        {"hardness", t.hardness}
    };
}

// Convert JSON to Tile
inline void from_json(const nlohmann::json& j, Tile& t) {
    j.at("type").get_to(t.mType);
    j.at("blocking").get_to(t.blocking);
    j.at("discovered").get_to(t.discovered);
    j.at("breakable").get_to(t.breakable);
    j.at("hardness").get_to(t.hardness);
}