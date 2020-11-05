#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "gameobject.h"
#include "mesh.h"

#include <QVector3D>
#include <QVector2D>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_1>

#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_1
{
public:
    explicit MainWidget(QWidget* parent = nullptr);
    ~MainWidget() override;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void initShaders();
private:
    QOpenGLShaderProgram program;

    QMatrix4x4 Model;
    QMatrix4x4 View;
    QMatrix4x4 Projection;

    GameObject GO;
    Mesh * mesh = nullptr;

    int width = 1280;
    int height = 720;
};

#endif // MAINWIDGET_H
