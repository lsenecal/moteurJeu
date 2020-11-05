#include "transform.h"

Transform::Transform(float s, struct Rotation R, QVector3D T) : scale(s), rotation(R), translation(T)
{

}

Transform::~Transform()
{

}

Rotation & Transform::getRotation()
{
    return this->rotation;
}

QVector3D & Transform::getTranslation()
{
    return this->translation;
}

float & Transform::getScale()
{
    return scale;
}

QMatrix4x4 Transform::getModelMatrix()
{
    QMatrix4x4 model;
    model.scale(scale);
    model.translate(translation);
    model.rotate(rotation.angle, rotation.axe);
    return model;
}
