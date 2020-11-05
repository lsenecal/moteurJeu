#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QVector3D>
#include <QMatrix4x4>

struct Rotation {
    QVector3D axe = QVector3D(1.0f, 0.0f, 0.0f);
    float angle = 0.0f;
};

class Transform
{
public:
    Transform(float s = 1.0f, Rotation rotation = {}, QVector3D translation = QVector3D(0.0f, 0.0f, 0.0f));
    Rotation & getRotation();
    QVector3D & getTranslation();
    float & getScale();
    QMatrix4x4 getModelMatrix();
    ~Transform();
private:
    float scale;
    Rotation rotation;
    QVector3D translation;
};

#endif // TRANSFORM_H
