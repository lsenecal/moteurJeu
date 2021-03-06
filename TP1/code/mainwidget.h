/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "geometryengine.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_1>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QWheelEvent>

class GeometryEngine;

struct Camera {
    QVector3D eye;
    QVector3D lForward;
    QVector3D lRight;
    QVector3D lUp;

    inline QVector3D getCenter()
    {
        return eye + lForward;
    }
};

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_1
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:
    //void mousePressEvent(QMouseEvent *e) override;
    //void mouseReleaseEvent(QMouseEvent *e) override;
    //void mouseMoveEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initTextures();

    void changeHeightMap(int sID);

private:
    QBasicTimer timer;
    QOpenGLShaderProgram programCube;
    QOpenGLShaderProgram programPlane;
    QOpenGLShaderProgram programSphere;
    GeometryEngine *geometries;

    QOpenGLTexture* texture_grass;
    QOpenGLTexture* texture_rock;
    QOpenGLTexture* texture_snow;
    QOpenGLTexture* texture_cube;
    QOpenGLTexture* heightMap;

    std::vector<Camera> cameras;

    QVector3D eye;
    QVector3D lForward;
    QVector3D lRight;
    QVector3D lUp;

    QMatrix4x4 Model;
    QMatrix4x4 View;
    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;

    int width = 1280;
    int height = 720;

    // Flags
    bool fFlag = false;
    bool bFlag = false;
    bool rFlag = false;
    bool lFlag = false;
    bool uFlag = false;
    bool dFlag = false;
    bool rrFlag = false;
    bool lrFlag = false;

    bool freeFlag = false;

    // Kinematic
    float speed = 0.02f;

    // Other
    bool HMisOriginal = true;
    unsigned int sID = 0;

};

void moveCamera(bool fFlag, bool bFlag, bool rFlag, bool lFlag, bool uFlag, bool dFlag, bool lrFlag, bool rrFlag, Camera & cam, QMatrix4x4 & View, float speed);
void rotateCamera(QVector2D mousePos, Camera & cam, QMatrix4x4 & View);

#endif // MAINWIDGET_H
