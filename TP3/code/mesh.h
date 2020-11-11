#ifndef MESH_H
#define MESH_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_1>

class Mesh : public QOpenGLFunctions_3_1
{
public:
    Mesh();
    ~Mesh();
    QOpenGLBuffer & getArrayBuf();
    QOpenGLBuffer & getIndexBuf();
    unsigned long long getSize();
    void setSize(unsigned long long i);
private:
    unsigned long long size;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // MESH_H
