#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QVector3D>

class Transform
{
public:
    Transform(float s = 1.0f, QVector3D Rotation = QVector3D(0.0f, 0.0f, 0.0f), QVector3D Translation = QVector3D(0.0f, 0.0f, 0.0f));
    QVector3D & getRotation();
    QVector3D & getTranslation();
    float getScale();
    ~Transform();
private:
    float scale;
    QVector3D Rotation;
    QVector3D Translation;
};

#endif // TRANSFORM_H
