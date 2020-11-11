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

#include "mainwidget.h"
#include "geometryfactory.h"

#include <QMouseEvent>

#include <math.h>
#include <QtMath>
#include <iostream>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

static bool version2 = true;

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    sun(nullptr),
    earth(nullptr),
    moon(nullptr),
    sun_earth(nullptr),
    earth_moon(nullptr),
    texture_sun(nullptr),
    texture_earth(nullptr),
    texture_moon(nullptr)
{
    camera.eye = QVector3D(0, 10, 10);
    camera.lForward = (QVector3D(0.0f, 0.0f, 0.0f) - camera.eye).normalized();
    camera.lRight = QVector3D(1.0, 0.0, 0.0);
    camera.lUp = QVector3D::crossProduct(camera.lRight, camera.lForward);

    View.lookAt(camera.eye, QVector3D(0, 0, 0), camera.lUp);
    Model.setToIdentity();

    this->resize(1280, 720);
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete cubeMesh;
    delete sphereMesh;
    delete texture_sun;
    delete texture_earth;
    delete texture_moon;
    delete sun;
    delete earth;
    delete moon;
    delete sun_earth;
    delete earth_moon;
    doneCurrent();
}

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    // 1 year -> 6 min 5s = 365 s
    // 1 day = 24 hrs -> 1 s
    // Tmoon = 27 days -> 27 s
    // Rmoon = 27 dyas -> 27 s

    float PIf = 3.41592f;

    float Rsun = 2.0f*PIf / 27.0f;
    float Tearth = 2.0f*PIf / 365.25f;
    float Rearth = 2.0f*PIf / 1.0f;
    float Tmoon = 2.0f*PIf / 27.0f;
    float Rmoon = 2.0f*PIf / 27.0f;

    /*float Rsun = 0.1f;
    float Tearth = 0.2f;
    float Rearth = 0.5f;
    float Tmoon = 1.0f;
    float Rmoon = 1.0f;*/

    if(version2) {
        sun_earth->getTransform().getRotation().angle += Tearth;
        earth_moon->getTransform().getRotation().angle += Tmoon - Tearth;
    }

    sun->getTransform().getRotation().angle += Rsun - Tearth;
    earth->getTransform().getRotation().angle += Rearth - Tmoon - Tearth;
    moon->getTransform().getRotation().angle += Rmoon - Tmoon;

    QVector2D mousePos;
    {
        QPoint p = this->mapFromGlobal(QCursor::pos());
        mousePos = QVector2D(static_cast<float>(p.x()), static_cast<float>(p.y()));
    }

    rotateCamera(mousePos);

    update();
}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]

    cubeMesh = new Mesh();
    sphereMesh = new Mesh();
    GeometryFactory::addCubeGeometry(cubeMesh);
    GeometryFactory::addSphereGeometry(sphereMesh);

    GS = GraphScene();

    /*
     Version avec des périodes de rotation et de révolution différentes
     */

    if (version2) {
        sun_earth = new GameObject(nullptr, nullptr, nullptr);
        earth_moon = new GameObject(nullptr, nullptr, nullptr);

        sun_earth->setRotationTransform({QVector3D(0.0f, 1.0f, 0.0f), 0.0f});
        earth_moon->setRotationTransform({QVector3D(0.0f, 1.0f, 0.0f), 0.0f});
        earth_moon->setTranslationTransform(QVector3D(10.0f, 0.0f, 0.0f));

        sun = new GameObject(sphereMesh, &program, texture_sun);
        earth = new GameObject(sphereMesh, &program, texture_earth);
        moon = new GameObject(sphereMesh, &program, texture_moon);

        sun_earth->addChild(sun);
        sun_earth->addChild(earth_moon);

        earth_moon->addChild(earth);
        earth_moon->addChild(moon);

        sun->setScaleTransform(2.0f);
        sun->setRotationTransform({QVector3D(0.0f, 1.0f, 0.0f), 0.0f});

        earth->setScaleTransform(0.5f);

        //earth->setRotationTransform({QVector3D(0.0f, 1.0f, 0.0f), 0.0f});
        earth->setRotationTransform({QVector3D(tan(23/180*M_PI), 1.0f, 0.0f), 0.0f});

        moon->setScaleTransform(0.2f);
        moon->setTranslationTransform(QVector3D(10.0f, 0.0f, 0.0f));
        moon->setRotationTransform({QVector3D(0.0f, 1.0f, 0.0f), 0.0f});

        GS.addGameObject(sun_earth);
    }

    /*
     Version avec des périodes de rotation et de révolution identique
     */

    if (!version2) {
        sun = new GameObject(sphereMesh, &program, texture_sun);
        earth = new GameObject(sphereMesh, &program, texture_earth);
        moon = new GameObject(sphereMesh, &program, texture_moon);

        sun->setScaleTransform(2.0f);
        sun->setRotationTransform({QVector3D(0.0f, 1.0f, 0.0f), 0.0f});

        earth->setScaleTransform(0.5f);
        earth->setTranslationTransform(QVector3D(10.0f, 0.0f, 0.0f));
        earth->setRotationTransform({QVector3D(0.0f, 1.0f, 0.0f), 0.0f});
        earth->setRotationTransform({QVector3D(tan(23/180*M_PI), 1.0f, 0.0f), 0.0f});
        sun->addChild(earth);

        moon->setScaleTransform(0.2f);
        moon->setTranslationTransform(QVector3D(10.0f, 0.0f, 0.0f));
        earth->addChild(moon);

        GS.addGameObject(sun);
    }

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    texture_sun = new QOpenGLTexture(QImage(":sun.jpg").mirrored());
    texture_sun->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_sun->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_sun->setWrapMode(QOpenGLTexture::Repeat);

    texture_earth = new QOpenGLTexture(QImage(":earth.jpg").mirrored());
    texture_earth->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_earth->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_earth->setWrapMode(QOpenGLTexture::Repeat);

    texture_moon = new QOpenGLTexture(QImage(":moon.jpg").mirrored());
    texture_moon->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_moon->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_moon->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1, zFar = 100.0, fov = 45.0;

    // Reset projection
    Projection.setToIdentity();

    // Set perspective projection
    Projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//! [6]
    // Set modelview-projection matrix
    //program.setUniformValue("model_matrix", Model);
    program.setUniformValue("view_matrix", View);
    program.setUniformValue("projection_matrix", Projection);
//! [6]
//!
    GS.draw();
}

void MainWidget::rotateCamera(QVector2D mousePos) {

    QVector3D & eye = camera.eye;
    QVector3D & lForward = camera.lForward;
    QVector3D & lRight = camera.lRight;
    QVector3D & lUp = camera.lUp;

    QVector2D centralPos = QVector2D(640, 360);

    QVector2D & currentMousePosition = mousePos;

    QVector2D diff = currentMousePosition - centralPos;
    float angleX = atanf(diff.x()/640.0f);
    float angleY = atanf(diff.y() / 360.0f);

    if (angleX < 0.1f && angleX > -0.1f)
        angleX = 0.0f;

    if (angleY < 0.1f && angleY > -0.1f)
        angleY = 0.0f;

    QMatrix4x4 M;
    M.setToIdentity();
    M.rotate(-angleX, lUp);
    M.rotate(-angleY, lRight);
    lRight = M*lRight;
    lForward = M*lForward;
    M.rotate(-angleY, lRight);

    QVector3D center = camera.getCenter();
    lUp = QVector3D::crossProduct(lRight, lForward).normalized();

    View.setToIdentity();
    View.lookAt(eye, center, lUp);
}
