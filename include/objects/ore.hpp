#pragma once

#include <nlohmann/json.hpp>

#include <string>

class Ore
{
public:
    std::string mName;
    int mValue;
    int mWeight;

    Ore(/* args */){}
    ~Ore(){}
};

// Convert JSON
inline void from_json(const nlohmann::json& j, Ore& o) {
    j.at("name").get_to(o.mName);
    j.at("value").get_to(o.mValue);
    j.at("weight").get_to(o.mWeight);
}