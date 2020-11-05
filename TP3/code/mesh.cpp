#include "mesh.h"
#include <iostream>

Mesh::Mesh() : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();
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
    arrayBuf.destroy();
    indexBuf.destroy();
}

int Mesh::getSize()
{
    return size;
}

void Mesh::setSize(int i)
{
    size = i;
}
