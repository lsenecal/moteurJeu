#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "mesh.h"
#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

class GameObject : protected QOpenGLFunctions_3_1
{
public:
    GameObject();
    GameObject(Mesh* mesh, QOpenGLShaderProgram* shader);
    void init();
    void setMesh(Mesh* mesh);
    void setShaders(QOpenGLShaderProgram* shader);
    Mesh* getMesh();
    QOpenGLShaderProgram* getShader();
    void draw();
private:
    Mesh* mesh;
    QOpenGLShaderProgram* shaders;
};

#endif // GAMEOBJECT_H
