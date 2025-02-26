#pragma once

#include <aabb.hpp>

enum BLOCK_TYPE{EMPTY, DIRT, ROCK};

class Block : public AABB{
public:
    size_t mType = EMPTY;

    Block(){}
    Block(size_t type, const Vector2 rPosition, const Vector2 rSize) : mType(type), AABB(rPosition, rSize) {

    }
    ~Block(){}
};