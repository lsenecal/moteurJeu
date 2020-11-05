#include "gameobject.h"
#include <iostream>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

GameObject::GameObject()
{
    std::cout << "GameObject::GameObject()" << std::endl;
}

GameObject::GameObject(Mesh* m, QOpenGLShaderProgram* s) : mesh(m), shaders(s)
{
    initializeOpenGLFunctions();
}

GameObject::GameObject(Transform t, Mesh* m, QOpenGLShaderProgram* s) : GameObject(m, s)
{
    this->transform = t;
}

GameObject::GameObject(Mesh* m, QOpenGLShaderProgram* s, QOpenGLTexture* tex) : GameObject(m, s)
{
    this->texture = tex;
}

GameObject::GameObject(Transform t, Mesh* m, QOpenGLShaderProgram* s, QOpenGLTexture* tex) : GameObject(t, m, s)
{
    this->texture = tex;
}

GameObject::~GameObject()
{
    std::cout << "GameObject::~GameObject()" << std::endl;
}

Transform & GameObject::getTransform()
{
    return this->transform;
}

Mesh * GameObject::getMesh()
{
    return this->mesh;
}

QOpenGLShaderProgram* GameObject::getShader()
{
    return this->shaders;
}

void GameObject::setMesh(Mesh * mesh)
{
    this->mesh = mesh;
}

void GameObject::addChild(GameObject* go)
{
    go->parent = this;
    childrens.push_back(go);
}

void GameObject::setScaleTransform(float s)
{
    transform.getScale() = s;
}

void GameObject::setTranslationTransform(QVector3D translation)
{
    transform.getTranslation() = translation;
}

void GameObject::setRotationTransform(Rotation rotation)
{
    transform.getRotation() = rotation;
}

QMatrix4x4 GameObject::getGlobalModelMatrix()
{
    if (parent)
        return parent->getGlobalModelMatrix() * transform.getModelMatrix();
    else
        return transform.getModelMatrix();
}

void GameObject::draw()
{
    texture->bind(0);
    shaders->bind();

    shaders->setUniformValue("texture_surface", 0);

    shaders->setUniformValue("model_matrix", getGlobalModelMatrix());

    this->mesh->getArrayBuf().bind();
    this->mesh->getIndexBuf().bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = shaders->attributeLocation("a_position");
    shaders->enableAttributeArray(vertexLocation);
    shaders->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL shadersmable pipeline how to locate vertex texture coordinate data
    //int texcoordLocation = shaders->attributeLocation("a_tbexcoord");
    //shaders->enableAttributeArray(texcoordLocation);
    //shaders->setAttributeBuffer(texcoordLocation, GL_FLOAT, static_cast<int>(offset), 2, sizeof(VertexData));

    // Draw geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, this->mesh->getSize(), GL_UNSIGNED_SHORT, nullptr);

    for (GameObject* c : childrens)
        c->draw();
}
