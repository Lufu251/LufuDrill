#pragma once

#include <raylib.h>
#include <raymath.h>

#include <tools.hpp>
#include <aabb.hpp>
#include <tile.hpp>
 

enum FACING{LEFT, RIGHT, DRILL_LEFT, DRILL_RIGHT, DRILL_DOWN};

class DrillUnit : public AABB{
public:
    Drill drill;
    GasTank gasTank;
    Hull hull;
    CargoBay cargoBay;
    Engine engine;

    // Players state
    int money = 0;
    int state = 0;
    float drillTime = 0;
    Tile* drillingBlock;

    // Values to track touching sides
    int top, right, bottom, left;

    DrillUnit();
    DrillUnit(const Vector2 rPosition, const Vector2 rSize, const Vector2 rVelocity);
    ~DrillUnit();
    
    void addForce(const Vector2& rVelocity);
    void updatePosition();
    void setCargoSize(size_t size);
    int getCargoWeight();
    int getCargoValue();
    void clearCargo();
    bool addCargo(int id);
    bool removeCargo(int id);
};