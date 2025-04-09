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
