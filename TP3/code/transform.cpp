#include "transform.h"

Transform::Transform(float s, QVector3D R, QVector3D T) : scale(s), Rotation(R), Translation(T)
{

}

Transform::~Transform()
{

}

QVector3D & Transform::getRotation()
{
    return this->Rotation;
}

QVector3D & Transform::getTranslation()
{
    return this->Translation;
}

float Transform::getScale()
{
    return scale;
}
