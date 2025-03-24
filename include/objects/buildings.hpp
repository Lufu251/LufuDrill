#pragma once

#include <aabb.hpp>
#include <guiContext.hpp>

enum BUILDING_TYPE{GAS_STATION, TRADER, TOOL_SHOP, EQUIPMENT_SHOP};

class Building : public AABB{
private:
    
public:
    GuiContext* mMenuToTrigger;
    int mType;

    Building(/* args */){}
    Building(const int type, const Vector2 rPosition, const Vector2 rSize) : AABB(rPosition, rSize), mType(type){}
    ~Building(){}

    void setMenuToTrigger(GuiContext& rMenuToTrigger){
        mMenuToTrigger = &rMenuToTrigger;
    }
};

// Serialize Vector2d to JSON
inline void to_json(nlohmann::json& j, const Vector2& v) {
    j = {{"x", v.x}, {"y", v.y}};
}

// Deserialize JSON to Vector2d
inline void from_json(const nlohmann::json& j, Vector2& v) {
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
}

// Convert Building to JSON
inline void to_json(nlohmann::json& j, const Building& b) {
    j = {
        {"type", b.mType},
        {"position", b.position},
        {"size", b.size}
    };
}

// Convert JSON to Building
inline void from_json(const nlohmann::json& j, Building& b) {
    j.at("type").get_to(b.mType);
    j.at("position").get_to(b.position);
    j.at("size").get_to(b.size);
}