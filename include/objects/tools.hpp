#pragma once

#include <string>

// Base class
class Tool {
public:
    std::string name;
    int cost;

    Tool(){}
    Tool(std::string name, int cost) : name(name), cost(cost) {}
};

class Drill : public Tool{
public:
    int power;

    Drill(){}
    Drill(std::string name, int cost, int power) : Tool(name, cost), power(power) {}
    ~Drill(){}
};


class FuelTank: public Tool{
public:
    std::string name;
    int cost;
    float fuel;
    float fuelMax;

    FuelTank(){}
    FuelTank(std::string name, int cost, int fuel) : Tool(name, cost), fuel(fuel) {}
    ~FuelTank(){}
};

class Hull: public Tool{
public:
    std::string name;
    int cost;
    float health;
    float healthMax;

    Hull(){}
    Hull(std::string name, int cost, int health) : Tool(name, cost), health(health) {}
    ~Hull(){}
};

class CargoBay: public Tool{
public:
    std::string name;
    int cost;
    int capacity;

    CargoBay(){}
    CargoBay(std::string name, int cost, int capacity) : Tool(name, cost), capacity(capacity) {}
    ~CargoBay(){}
};

class Engine: public Tool{
public:
    std::string name;
    int cost;
    float power;

    Engine(){}
    Engine(std::string name, int cost, int power) : Tool(name, cost), power(power) {}
    ~Engine(){}
};

class Equipment: public Tool{
public:
    std::string name;
    int cost;

    Equipment(){}
    Equipment(std::string name, int cost) : Tool(name, cost) {}
    ~Equipment(){}
};
