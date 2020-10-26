#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "mesh.h"
#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>

class GameObject : protected QOpenGLFunctions_3_1
{
public:
    GameObject();
    GameObject(Mesh* m, QOpenGLShaderProgram* s);
    Mesh* getMesh();
    QOpenGLShaderProgram* getShader();
    void draw();
private:
    Mesh* mesh;
    QOpenGLShaderProgram* shaders;
};

#endif // GAMEOBJECT_H
