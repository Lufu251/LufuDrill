#pragma once

#include <string>

class Drill{
public:
    std::string name;
    int cost;
    int power;

    Drill(/* args */){}
    ~Drill(){}
};


class FuelTank{
public:
    std::string name;
    int cost;
    float fuel;
    float fuelMax;

    FuelTank(/* args */){}
    ~FuelTank(){}
};

class Hull{
public:
    std::string name;
    int cost;
    float health;
    float healthMax;

    Hull(/* args */){}
    ~Hull(){}
};

class CargoBay{
public:
    std::string name;
    int cost;
    int capacity;

    CargoBay(/* args */){}
    ~CargoBay(){}
};

class Engine{
public:
    std::string name;
    int cost;
    float power;

    Engine(/* args */){}
    ~Engine(){}
};

class Equpiment{
public:
    std::string name;
    int cost;

    Equpiment(/* args */){}
    ~Equpiment(){}
};
