#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "transform.h"
#include "mesh.h"

class GameObject : protected QOpenGLFunctions_3_1
{
public:
    GameObject();
    GameObject(Mesh* m, QOpenGLShaderProgram* s);
    GameObject(Transform t, Mesh* m, QOpenGLShaderProgram* s);
    ~GameObject();
    Transform& getTransform();
    Mesh* getMesh();
    QOpenGLShaderProgram* getShader();
    void setMesh(Mesh* mesh);
    void draw();
private:
    Transform transform;
    Mesh* mesh;
    QOpenGLShaderProgram* shaders;
};

#endif // GAMEOBJECT_H
