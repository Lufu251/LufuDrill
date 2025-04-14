#pragma once

#include <nlohmann/json.hpp>

#include <string>

class Block{
public:
    std::string mName;
    int mOreID;
    int mRenderID;
    int mHardness;
    bool mLight;
    bool mBlocking;

    Block(/* args */){}
    ~Block(){}
};

// Convert JSON
inline void from_json(const nlohmann::json& j, Block& b) {
    j.at("name").get_to(b.mName);
    j.at("ore_id").get_to(b.mOreID);
    j.at("render_id").get_to(b.mRenderID);
    j.at("hardness").get_to(b.mHardness);
    j.at("light").get_to(b.mLight);
    j.at("blocking").get_to(b.mBlocking);
}