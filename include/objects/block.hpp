#pragma once

#include <aabb.hpp>

#include <nlohmann/json.hpp>

enum BLOCK_TYPE{EMPTY, DIRT, STONE, COPPERORE, GOLDORE, PLATINUMORE};

class Block : public AABB{
public:
    size_t mType = EMPTY;
    bool blocking = false;

    Block(){}
    Block(size_t type) : mType(type){}
    Block(size_t type, const Vector2 rPosition, const Vector2 rSize) : AABB(rPosition, rSize), mType(type){}
    ~Block(){}
};

// Convert Block to JSON
inline void to_json(nlohmann::json& j, const Block& b) {
    j = {
        {"type", b.mType},
        {"blocking", b.blocking}
    };
}

// Convert JSON to Block
inline void from_json(const nlohmann::json& j, Block& b) {
    j.at("type").get_to(b.mType);
    j.at("blocking").get_to(b.blocking);
}