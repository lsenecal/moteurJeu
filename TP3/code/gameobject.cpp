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
    //initializeOpenGLFunctions();
}

GameObject::GameObject(Mesh* m, QOpenGLShaderProgram* s) : GameObject()
{
    this->mesh = m;
    this->shaders = s;
}

GameObject::GameObject(Transform t, Mesh* m, QOpenGLShaderProgram* s) : GameObject()
{
    this->transform = t;
    this->mesh = m;
    this->shaders = s;
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

void GameObject::draw()
{
    std::cout << "GameObject::draw()" << std::endl;

    shaders->bind();

    std::cout << &mesh->getArrayBuf() << std::endl;
    std::cout << &mesh->getIndexBuf() << std::endl;

    this->mesh->getArrayBuf().bind();
    this->mesh->getIndexBuf().bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = shaders->attributeLocation("a_position");
    shaders->enableAttributeArray(vertexLocation);
    shaders->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL shadersmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = shaders->attributeLocation("a_tbexcoord");
    shaders->enableAttributeArray(texcoordLocation);
    shaders->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, this->mesh->getSize(), GL_UNSIGNED_SHORT, 0);
}
