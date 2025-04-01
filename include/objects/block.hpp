#pragma once

#include <aabb.hpp>

#include <nlohmann/json.hpp>

class Block : public AABB{
public:
    size_t mType = -1;
    bool blocking = true;
    bool discovered = false;
    bool breakable = true;
    int hardness = 30;


    Block(){}
    Block(const Vector2 rPosition, const Vector2 rSize) : AABB(rPosition, rSize){}
    ~Block(){}
};

// Convert Block to JSON
inline void to_json(nlohmann::json& j, const Block& b) {
    j = {
        {"type", b.mType},
        {"blocking", b.blocking},
        {"discovered", b.discovered},
        {"breakable", b.breakable},
        {"hardness", b.hardness}
    };
}

// Convert JSON to Block
inline void from_json(const nlohmann::json& j, Block& b) {
    j.at("type").get_to(b.mType);
    j.at("blocking").get_to(b.blocking);
    j.at("discovered").get_to(b.discovered);
    j.at("breakable").get_to(b.breakable);
    j.at("hardness").get_to(b.hardness);
}