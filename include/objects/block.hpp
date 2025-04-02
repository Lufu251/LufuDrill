#pragma once

#include <nlohmann/json.hpp>

#include <string>

class Block{
public:
    std::string mName;
    int mValue;
    float mWeight;
    int mHardness;
    bool mBlocking;

    Block(/* args */);
    ~Block();
};

// Convert Block to JSON
inline void to_json(nlohmann::json& j, const Block& b) {
    j = {
        {"name", b.mName},
        {"value", b.mValue},
        {"weight", b.mWeight},
        {"hardness", b.mHardness},
        {"blocking", b.mBlocking}
    };
}

// Convert JSON to Block
inline void from_json(const nlohmann::json& j, Block& b) {
    j.at("mName").get_to(b.mName);
    j.at("value").get_to(b.mValue);
    j.at("weight").get_to(b.mWeight);
    j.at("hardness").get_to(b.mHardness);
    j.at("blocking").get_to(b.mBlocking);
}