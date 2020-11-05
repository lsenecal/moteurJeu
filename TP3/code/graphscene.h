#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <vector>
#include <gameobject.h>

class GraphScene
{
public:
    GraphScene();
    void draw();
    void addGameObject(GameObject* go);
    ~GraphScene();
private:
    std::vector<GameObject*> nodes;
};

#endif // GRAPHSCENE_H
