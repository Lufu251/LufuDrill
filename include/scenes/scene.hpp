#pragma once

class Scene{
private:
    bool active = false;
public:
    Scene(/* args */){}
    virtual ~Scene(){}

    virtual void initialize() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    void enable(){
        active = true;
    }

    void disable(){
        active = false;
    }
};
struct A{
    virtual ~A();
};
