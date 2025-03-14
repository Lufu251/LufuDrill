class Scene
{
private:
    bool active = false;
public:
    Scene(/* args */){}
    ~Scene(){}

    void load(){
        active = true;
    }

    void unload(){
        active = false;
    }

    void update(){
    }
};