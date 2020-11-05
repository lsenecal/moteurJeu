#include "graphscene.h"
#include <iostream>

GraphScene::GraphScene()
{
    std::cout << "GraphScene::GraphScene()" << std::endl;
}

GraphScene::~GraphScene()
{
    std::cout << "GraphScene::~GraphScene()" << std::endl;
}

void GraphScene::draw()
{
    for (GameObject* go : nodes)
        go->draw();
}

void GraphScene::addGameObject(GameObject* go)
{
    std::cout << "GraphScene::addGameObject()" << std::endl;
    nodes.push_back(go);
}
