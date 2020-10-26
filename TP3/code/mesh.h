#ifndef MESH_H
#define MESH_H

#include <QOpenGLBuffer>

class Mesh
{
public:
    Mesh();
    ~Mesh();
    QOpenGLBuffer & getArrayBuf();
    QOpenGLBuffer & getIndexBuf();
    unsigned int getSize();
    void setSize(unsigned int i);
private:
    unsigned int size;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // MESH_H
