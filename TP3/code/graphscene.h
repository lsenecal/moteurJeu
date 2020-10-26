#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <vector>

template<class T>
class Node {
    std::vector<Node> nodes;
    T& element;
};

template<class T>
class GraphScene
{
public:
    GraphScene();
    void draw();
    ~GraphScene();
private:
    std::vector<Node<T>> nodes;
};

#endif // GRAPHSCENE_H
