#pragma once

#include <raylib.h>
#include <raymath.h>

#include <tools.hpp>
#include <aabb.hpp>
#include <tile.hpp>
//#include <globals.hpp>

enum FACING{LEFT, RIGHT, DRILL_LEFT, DRILL_RIGHT, DRILL_DOWN};

class DrillUnit : public AABB{
public:
    Drill drill;
    GasTank gasTank;
    Hull hull;
    CargoBay cargoBay;
    Engine engine;

    // Players state
    int state = 0;
    float drillTime = 0;
    Tile* drillingBlock;

    // Values to track touching sides
    int top, right, bottom, left;

    DrillUnit() : AABB(){}
    DrillUnit(const Vector2 rPosition, const Vector2 rSize, const Vector2 rVelocity) : AABB(rPosition, rSize, rVelocity){}
    ~DrillUnit(){}
    
    void addForce(const Vector2& rVelocity){
        velocity += rVelocity;
    }

    void updatePosition(){
        position += velocity;
    }

    void setCargoSize(size_t size){
        cargoBay.mCargo.reserve(size);
        for (size_t i = 0; i < size; i++){
            cargoBay.mCargo.push_back(0);
        }
    }

    int getWeight(){
        int weight = 0;
        for (size_t i = 0; i < cargoBay.mCargo.size(); i++){
            //weight += gDM.ores[i].mWeight * cargoBay.mCargo[i]; // Add weights from the amount of cargo
        }
        return weight;
    }

    int getValue(){

        return 0;
    }

    void clearCargo(){
        for (size_t i = 0; i < cargoBay.mCargo.size(); i++){
            cargoBay.mCargo[i] = 0;
        }
        cargoBay.mStored = 0;
    }

    bool addCargo(int id){
        if(cargoBay.mStored < cargoBay.mMaxStored){
            cargoBay.mCargo[id] ++;
            cargoBay.mStored ++;
            return true;
        }
        return false;
    }

    bool removeCargo(int id){
        if(cargoBay.mStored != 0 && cargoBay.mCargo[id] != 0){
            cargoBay.mCargo[id] --;
            cargoBay.mStored --;
            return true;
        }
        return false;
    }
};