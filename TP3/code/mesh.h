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
    int getSize();
    void setSize(int i);
private:
    int size;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // MESH_H
