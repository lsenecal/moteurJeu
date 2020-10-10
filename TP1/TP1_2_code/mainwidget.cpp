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

#include <QMouseEvent>

#include <math.h>
#include <QtMath>
#include <iostream>

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(nullptr),
    texture_grass(nullptr),
    texture_rock(nullptr),
    texture_snow(nullptr),
    texture_cube(nullptr),
    heightMap(nullptr),
    angularSpeed(0)
{
    for (size_t i = 0; i < 3; ++i)
    {
        eye = QVector3D(0.0, 10.0, 10.0);
        QVector3D center = QVector3D(0.0, 0.0, 0.0);
        lForward = (center - eye).normalized();
        lRight = QVector3D(1.0, 0.0, 0.0);
        lUp = QVector3D::crossProduct(lRight, lForward).normalized();

        cameras.push_back({eye, lForward, lRight, lUp});
    }

    this->resize(width, height);

    Camera & c = cameras.at(0);
    View.lookAt(c.eye, c.getCenter(), c.lUp);
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture_grass;
    delete texture_rock;
    delete texture_snow;
    delete texture_cube;
    delete heightMap;
    delete geometries;
    doneCurrent();
}

//! [0]
/*void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}*/

void MainWidget::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_C && freeFlag)
        freeFlag = false;
    else if (e->key() == Qt::Key_C && !freeFlag)
        freeFlag = true;

    if (freeFlag) {
        if (e->key() == Qt::Key_Z)
            fFlag = true;
        else if (e->key() == Qt::Key_S)
            bFlag = true;

        if (e->key() == Qt::Key_D)
            rFlag = true;
        else if (e->key() == Qt::Key_Q)
            lFlag = true;

        if (e->key() == Qt::Key_A)
            lrFlag = true;
        else if(e->key() == Qt::Key_E)
            rrFlag = true;

        if (e->key() == Qt::Key_Space)
            uFlag = true;
        else if (e->key() == Qt::Key_Control)
            dFlag = true;
    }

    if (e->key() == Qt::Key_1)
        sID = 0;

    if (e->key() == Qt::Key_2) {
        changeHeightMap(1);
        sID = 1;
    }

    if (e->key() == Qt::Key_3) {
        changeHeightMap(2);
        sID = 2;
    }
}

void MainWidget::keyReleaseEvent(QKeyEvent* e) {

    if (freeFlag) {
        if (e->key() == Qt::Key_Z)
            fFlag = false;
        else if (e->key() == Qt::Key_S)
            bFlag = false;

        if (e->key() == Qt::Key_D)
            rFlag = false;
        else if (e->key() == Qt::Key_Q)
            lFlag = false;

        if (e->key() == Qt::Key_A)
            lrFlag = false;
        else if(e->key() == Qt::Key_E)
            rrFlag = false;

        if (e->key() == Qt::Key_Space)
            uFlag = false;
        else if (e->key() == Qt::Key_Control)
            dFlag = false;
    }
}

std::ostream& operator<< (std::ostream& os, const QVector3D& v);

/*void MainWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (freeFlag) {

        QVector2D centralPos = QVector2D(640, 360);

        QVector2D currentMousePosition = QVector2D(e->localPos());

        QVector2D diff = currentMousePosition - centralPos;
        float angleX = 0.2f*atanf(diff.x());
        float angleY = 0.2f*atanf(diff.y());

        QMatrix4x4 M;
        M.setToIdentity();
        M.rotate(-angleX, lUp);
        M.rotate(-angleY, lRight);
        lRight = M*lRight;
        lForward = M*lForward;
        M.rotate(-angleY, lRight);

        QVector3D center = eye + lForward;
        lUp = QVector3D::crossProduct(lRight, lForward).normalized();

        View.setToIdentity();
        View.lookAt(eye, center, lUp);
    }
}*/

//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    QVector2D mousePos;
    {
        QPoint p = this->mapFromGlobal(QCursor::pos());
        mousePos = QVector2D(static_cast<float>(p.x()), static_cast<float>(p.y()));
    }

    if (freeFlag) {
        moveCamera(fFlag, bFlag, rFlag, lFlag, uFlag, dFlag, lrFlag, rrFlag, cameras.at(sID), View, speed);
    }

    float r = sqrtf((mousePos.x() - width/2.0f) * (mousePos.x() - width/2.0f) + (mousePos.y() - height/2.0f)*(mousePos.y() - height/2.0f));

    if (freeFlag && r > 20) {
        rotateCamera(mousePos, cameras.at(sID), View);
    }

    if (!freeFlag)
        Model.rotate(0.1f, QVector3D(0.0f, 1.0f, 0.0f));

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

    geometries = new GeometryEngine;

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!programCube.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshaderCube.glsl"))
        close();

    // Compile fragment shader
    if (!programCube.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshaderCube.glsl"))
        close();

    // Link shader pipeline
    if (!programCube.link())
        close();

    // Compile vertex shader
    if (!programPlane.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshaderPlane.glsl"))
        close();

    // Compile fragment shader
    if (!programPlane.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshaderPlane.glsl"))
        close();

    // Link shader pipeline
    if (!programPlane.link())
        close();

    // Compile vertex shader
    if (!programSphere.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshaderSphere.glsl"))
        close();

    // Compile fragment shader
    if (!programSphere.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshaderSphere.glsl"))
        close();

    // Link shader pipeline
    if (!programSphere.link())
        close();

    // Bind shader pipeline for use
    if (!programCube.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load cube.png image
    texture_grass = new QOpenGLTexture(QImage(":grass.png").mirrored());

    // Set nearest filtering mode for texture minification
    texture_grass->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture_grass->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture_grass->setWrapMode(QOpenGLTexture::Repeat);

    texture_rock = new QOpenGLTexture(QImage(":rock.png").mirrored());
    texture_rock->setMagnificationFilter(QOpenGLTexture::Nearest);
    texture_rock->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_rock->setWrapMode(QOpenGLTexture::Repeat);

    texture_snow = new QOpenGLTexture(QImage(":snowrocks.png").mirrored());
    texture_snow->setMagnificationFilter(QOpenGLTexture::Nearest);
    texture_snow->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_snow->setWrapMode(QOpenGLTexture::Repeat);

    texture_cube = new QOpenGLTexture(QImage(":cube.png").mirrored());
    texture_cube->setMagnificationFilter(QOpenGLTexture::Nearest);
    texture_cube->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_cube->setWrapMode(QOpenGLTexture::Repeat);

    heightMap = new QOpenGLTexture(QImage(":heightmap.png"));
    heightMap->setMinificationFilter(QOpenGLTexture::Nearest);
    heightMap->setMagnificationFilter(QOpenGLTexture::Linear);
    heightMap->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

void MainWidget::changeHeightMap(int sID) {

    if (HMisOriginal && sID == 2) {
        heightMap = new QOpenGLTexture(QImage(":water_2k.png"));
        HMisOriginal = false;

        heightMap->setMinificationFilter(QOpenGLTexture::Nearest);
        heightMap->setMagnificationFilter(QOpenGLTexture::Linear);
        heightMap->setWrapMode(QOpenGLTexture::Repeat);
    }
    else if (!HMisOriginal && sID ==1) {
        heightMap = new QOpenGLTexture(QImage(":heightmap.png"));
        HMisOriginal = true;

        heightMap->setMinificationFilter(QOpenGLTexture::Nearest);
        heightMap->setMagnificationFilter(QOpenGLTexture::Linear);
        heightMap->setWrapMode(QOpenGLTexture::Repeat);
    }
}

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1, zFar = 100.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, static_cast<float>(aspect), zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture_grass->bind(0);
    texture_rock->bind(1);
    texture_snow->bind(2);
    texture_cube->bind(3);
    heightMap->bind(4);

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    //matrix.translate(0.0, 0.0, -5.0);
    //matrix.rotate(rotation);

    if (sID == 0) {
        // Bind shader pipeline for use
        if (!programCube.bind())
            close();

        programCube.setUniformValue("model_matrix", Model);
        programCube.setUniformValue("view_matrix", View);
        programCube.setUniformValue("projection_matrix", projection);

        programCube.setUniformValue("texture_cube", 3);
    }
    else if (sID == 1) {

        // Bind shader pipeline for use
        if (!programPlane.bind())
            close();

        // Set modelview-projection matrix
        programPlane.setUniformValue("model_matrix", Model);
        programPlane.setUniformValue("view_matrix", View);
        programPlane.setUniformValue("projection_matrix", projection);
        //! [6]

        // Use texture unit 0 which contains cube.png
        programPlane.setUniformValue("texture_grass", 0);
        programPlane.setUniformValue("texture_rock", 1);
        programPlane.setUniformValue("texture_snow", 2);
        programPlane.setUniformValue("heightMap", 4);
    }
    else if (sID == 2) {

        // Bind shader pipeline for use
        if (!programSphere.bind())
            close();

        // Set modelview-projection matrix
        programSphere.setUniformValue("model_matrix", Model);
        programSphere.setUniformValue("view_matrix", View);
        programSphere.setUniformValue("projection_matrix", projection);
    //! [6]

        // Use texture unit 0 which contains cube.png
        programSphere.setUniformValue("texture_grass", 0);
        programSphere.setUniformValue("texture_rock", 1);
        programSphere.setUniformValue("texture_snow", 2);
        programSphere.setUniformValue("heightMap", 4);
    }

    if (sID == 0) {
        // Draw cube geometry
        geometries->drawCubeGeometry(&programCube);
    }
    else if (sID == 1) {
        // Draw cube geometry
        geometries->drawPlaneGeometry(&programPlane);
    }
    else if (sID == 2) {
        // Draw sphere geometry
        geometries->drawSphereGeometry(&programSphere);
    }
}

void moveCamera(bool fFlag, bool bFlag, bool rFlag, bool lFlag, bool uFlag, bool dFlag, bool lrFlag, bool rrFlag, Camera & cam, QMatrix4x4 & View, float speed)
{
    QVector3D & eye = cam.eye;
    QVector3D & lForward = cam.lForward;
    QVector3D & lRight = cam.lRight;
    QVector3D & lUp = cam.lUp;

    if (fFlag)
        eye += speed * lForward;
    else if (bFlag)
        eye -= speed * lForward;

    if (rFlag)
        eye += speed * lRight;
    else if (lFlag)
        eye -= speed * lRight;

     if (lrFlag) {
        QMatrix4x4 M;
        M.setToIdentity();
        M.rotate(-1, lForward);
        lUp = M*lUp;
        lRight = M*lRight;
     }
     else if(rrFlag) {
        QMatrix4x4 M;
        M.setToIdentity();
        M.rotate(1, lForward);
        lUp = M*lUp;
        lRight = M*lRight;
     }

     if (uFlag)
        eye += speed * lUp;
     else if (dFlag)
        eye -= speed * lUp;

    QVector3D center = cam.getCenter();
    lUp = QVector3D::crossProduct(lRight, lForward).normalized();
    View.setToIdentity();
    View.lookAt(eye, center, lUp);
}

void rotateCamera(QVector2D mousePos, Camera & cam, QMatrix4x4 & View) {

    QVector3D & eye = cam.eye;
    QVector3D & lForward = cam.lForward;
    QVector3D & lRight = cam.lRight;
    QVector3D & lUp = cam.lUp;

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

    QVector3D center = cam.getCenter();
    lUp = QVector3D::crossProduct(lRight, lForward).normalized();

    View.setToIdentity();
    View.lookAt(eye, center, lUp);
}
