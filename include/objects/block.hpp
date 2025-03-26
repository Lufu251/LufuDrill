#pragma once

#include <aabb.hpp>

#include <nlohmann/json.hpp>

enum BLOCK_TYPE{EMPTY, DIRT, STONE, COPPERORE, GOLDORE, PLATINUMORE};

class Block : public AABB{
public:
    size_t mType = EMPTY;
    bool blocking = true;
    bool discovered = false;
    int hardness = 30;

    Block(){}
    Block(size_t type) : mType(type){}
    Block(size_t type, const Vector2 rPosition, const Vector2 rSize) : AABB(rPosition, rSize), mType(type){}
    ~Block(){}
};

// Convert Block to JSON
inline void to_json(nlohmann::json& j, const Block& b) {
    j = {
        {"type", b.mType},
        {"blocking", b.blocking},
        {"discovered", b.discovered},
        {"hardness", b.hardness}
    };
}

// Convert JSON to Block
inline void from_json(const nlohmann::json& j, Block& b) {
    j.at("type").get_to(b.mType);
    j.at("blocking").get_to(b.blocking);
    j.at("discovered").get_to(b.discovered);
    j.at("hardness").get_to(b.hardness);
}