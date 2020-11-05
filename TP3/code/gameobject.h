#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include "transform.h"
#include "mesh.h"

class GameObject : protected QOpenGLFunctions_3_1
{
public:
    GameObject();
    GameObject(Mesh* m, QOpenGLShaderProgram* s);
    GameObject(Transform t, Mesh* m, QOpenGLShaderProgram* s);
    GameObject(Mesh* m, QOpenGLShaderProgram* s, QOpenGLTexture* tex);
    GameObject(Transform t, Mesh* m, QOpenGLShaderProgram* s, QOpenGLTexture* tex);
    ~GameObject();
    Transform& getTransform();
    Mesh* getMesh();
    QOpenGLShaderProgram* getShader();
    void setMesh(Mesh* mesh);
    void setShader(QOpenGLShaderProgram* shader);
    void addChild(GameObject* go);
    void setScaleTransform(float s);
    void setTranslationTransform(QVector3D transaltion);
    void setRotationTransform(Rotation rotation);
    void draw();
private:
    Transform transform;
    Mesh* mesh;
    QOpenGLShaderProgram* shaders;
    std::vector<GameObject*> childrens;
    GameObject* parent = nullptr;
    QOpenGLTexture* texture = nullptr;
    QMatrix4x4 getGlobalModelMatrix();
};

#endif // GAMEOBJECT_H
