#include <drillUnit.hpp>

#include <globals.hpp>

DrillUnit::DrillUnit(){}
DrillUnit::DrillUnit(const Vector2 rPosition, const Vector2 rSize, const Vector2 rVelocity) : AABB(rPosition, rSize, rVelocity){}
DrillUnit::~DrillUnit(){}

void DrillUnit::addForce(const Vector2& rVelocity){
    velocity += rVelocity;
}

void DrillUnit::updatePosition(){
    position += velocity;
}

void DrillUnit::setCargoSize(size_t size){
    cargoBay.mCargo.reserve(size);
    for (size_t i = 0; i < size; i++){
        cargoBay.mCargo.push_back(0);
    }
}

int DrillUnit::getWeight(){
    int weight = 0;
    for (size_t i = 0; i < cargoBay.mCargo.size(); i++){
        weight += gDM.ores[i].mWeight * cargoBay.mCargo[i]; // Add weights from the amount of cargo
    }
    return weight;
}

int DrillUnit::getValue(){

    return 0;
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