#include "mesh.h"
#include <iostream>

Mesh::Mesh() : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    std::cout << "Mesh::Mesh()" << std::endl;
    arrayBuf.create();
    indexBuf.create();
}

QOpenGLBuffer & Mesh::getArrayBuf()
{
    return this->arrayBuf;
}

QOpenGLBuffer & Mesh::getIndexBuf()
{
    return this->indexBuf;
}

Mesh::~Mesh()
{
    std::cout << "Mesh::~Mesh()" << std::endl;
    arrayBuf.destroy();
    indexBuf.destroy();
}

unsigned int Mesh::getSize()
{
    return size;
}

void Mesh::setSize(unsigned int i)
{
    size = i;
}
