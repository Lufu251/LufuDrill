#include <drillUnit.hpp>

#include <iostream>

#include <globals.hpp>

DrillUnit::DrillUnit(){}
DrillUnit::DrillUnit(const Vector2 position, const Vector2 size) : AABB(position, size){}
DrillUnit::DrillUnit(const Vector2 position, const Vector2 size, const float mass) : AABB(position, size, mass){}

void DrillUnit::setCargoSize(size_t size){
    cargoBay.mCargo.reserve(size);
    for (size_t i = 0; i < size; i++){
        cargoBay.mCargo.push_back(0);
    }
}

int DrillUnit::getCargoWeight(){
    int weight = 0;
    for (size_t i = 0; i < cargoBay.mCargo.size(); i++){
        weight += gDM.ores[i].mWeight * cargoBay.mCargo[i]; // Add weights from the amount of cargo
    }

    return weight;
}

int DrillUnit::getCargoValue(){
    int value = 0;
    for (size_t i = 0; i < cargoBay.mCargo.size(); i++){
        value += gDM.ores[i].mValue * cargoBay.mCargo[i]; // Add values from the amount of cargo
    }

    return value;
}

void DrillUnit::clearCargo(){
    for (size_t i = 0; i < cargoBay.mCargo.size(); i++){
        cargoBay.mCargo[i] = 0;
    }
    cargoBay.mStored = 0;
}

bool DrillUnit::addCargo(int id){
    if(cargoBay.mStored < cargoBay.mMaxStored){
        cargoBay.mCargo[id] ++;
        cargoBay.mStored ++;
        std::cout << "Cargo added " << gDM.ores[id].mName << std::endl;
        return true;
    }
    return false;
}

bool DrillUnit::removeCargo(int id){
    if(cargoBay.mStored != 0 && cargoBay.mCargo[id] != 0){
        cargoBay.mCargo[id] --;
        cargoBay.mStored --;
        return true;
    }
    return false;
}