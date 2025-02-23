#include <string>

class Drill
{
private:
    std::string type;
    float hardness;
    
public:
    Drill(/* args */){}
    ~Drill(){}
};


class FuelTank{
private:
    std::string type;
    float fuel;
    float fuelMax;

public:
    FuelTank(/* args */){}
    ~FuelTank(){}
};

class Hull{
private:
    std::string type;
    float health;
    float healthMax;

public:
    Hull(/* args */){}
    ~Hull(){}
};

class Storage{
private:
    std::string type;

public:
    Storage(/* args */){}

    ~Storage(){}
};

class Engine{
private:
    std::string type;
    float power;

public:
    Engine(/* args */){}
    ~Engine(){}
};