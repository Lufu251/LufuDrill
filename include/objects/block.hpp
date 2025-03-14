#pragma once

#include <aabb.hpp>

#include <nlohmann/json.hpp>

enum BLOCK_TYPE{EMPTY, DIRT, STONE, COPPERORE, GOLDORE, PLATINUMORE};

class Block : public AABB{
public:
    size_t mType = EMPTY;

    Block(){}
    Block(size_t type) : mType(type){}
    Block(size_t type, const Vector2 rPosition, const Vector2 rSize) : AABB(rPosition, rSize), mType(type){}
    ~Block(){}
};