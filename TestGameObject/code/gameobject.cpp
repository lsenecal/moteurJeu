#include "gameobject.h"

GameObject::GameObject()
{

}

GameObject::GameObject(Mesh* mesh, QOpenGLShaderProgram* shader) : GameObject()
{
    initializeOpenGLFunctions();
    this->mesh = mesh;
    this->shaders = shader;
}

void GameObject::init()
{
    initializeOpenGLFunctions();
}

Mesh * GameObject::getMesh()
{
    return this->mesh;
}

QOpenGLShaderProgram* GameObject::getShader()
{
    return this->shaders;
}

void GameObject::setMesh(Mesh* mesh)
{
    this->mesh = mesh;
}

void GameObject::setShaders(QOpenGLShaderProgram* shader)
{
    this->shaders = shader;
}

void GameObject::draw()
{
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
