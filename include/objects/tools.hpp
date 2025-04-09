#pragma once

#include <string>
#include <vector>
#include <block.hpp>

// Base class
class Tool {
public:
    std::string mName;
    int mCost;

    Tool(){}
    Tool(std::string name, int cost) : mName(name), mCost(cost) {}
};

class Drill : public Tool{
public:
    float mPower;

    Drill(){}
    Drill(std::string name, int cost, float power) : Tool(name, cost), mPower(power) {}
    ~Drill(){}
};


class GasTank: public Tool{
public:
    float mGas;
    float mGasMax;

    GasTank(){}
    GasTank(std::string name, int cost, int gasMax) : Tool(name, cost), mGasMax(gasMax) {}
    ~GasTank(){}
};

class Hull: public Tool{
public:
    float mHealth;
    float mHealthMax;

    Hull(){}
    Hull(std::string name, int cost, int healthMax) : Tool(name, cost), mHealthMax(healthMax) {}
    ~Hull(){}
};

class CargoBay: public Tool{
public:
    int mStored = 0;
    int mMaxStored;
    std::vector<int> mCargo;

    CargoBay(){}
    CargoBay(std::string name, int cost, int capacity) : Tool(name, cost), mMaxStored(capacity) {}
    ~CargoBay(){}

    void setCargoSize(size_t size){
        mCargo.reserve(size);
        for (size_t i = 0; i < size; i++){
            mCargo.push_back(0);
        }
    }

    int getWeight(){
        int weight = 0;
        for (size_t i = 0; i < mCargo.size(); i++){
             // Add weights from the amount of cargo
        }
        return weight;
    }

    void clearCargo(){
        for (size_t i = 0; i < mCargo.size(); i++){
            mCargo[i] = 0;
        }
        mStored = 0;
    }

    bool addCargo(int id){
        if(mStored < mMaxStored){
            mCargo[id] ++;
            mStored ++;
            return true;
        }
        return false;
    }
    bool removeCargo(int id){
        if(mStored != 0 && mCargo[id] != 0){
            mCargo[id] --;
            mStored --;
            return true;
        }
        return false;
    }
};

class Engine: public Tool{
public:
    float mPower;

    Engine(){}
    Engine(std::string name, int cost, int power) : Tool(name, cost), mPower(power) {}
    ~Engine(){}
};

class Equipment: public Tool{
public:

    Equipment(){}
    Equipment(std::string name, int cost) : Tool(name, cost) {}
    ~Equipment(){}
};
