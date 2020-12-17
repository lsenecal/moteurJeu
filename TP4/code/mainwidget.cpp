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
    terrain(nullptr),
    object(nullptr),
    texture_grass(nullptr),
    texture_rock(nullptr),
    texture_snow(nullptr)
{
    camera.eye = QVector3D(32.0, 20.0, 32.0);
    QVector3D center = QVector3D(0.0, 0.0, 0.0);
    camera.lForward = (center - camera.eye).normalized();
    camera.lRight = QVector3D(1.0, 0.0, -1.0).normalized();
    camera.lUp = QVector3D(0.0, 1.0, 0.0);//QVector3D::crossProduct(lRight, lForward).normalized();

    this->resize(width, height);

    View.lookAt(camera.eye, camera.getCenter(), camera.lUp);
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture_grass;
    delete texture_rock;
    delete texture_snow;
    delete terrain;
    delete object;
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
    if (e->key() == Qt::Key_Z)
        fFlag = true;
    else if (e->key() == Qt::Key_S)
        bFlag = true;

    if (e->key() == Qt::Key_D)
        rFlag = true;
    else if (e->key() == Qt::Key_Q)
        lFlag = true;
}

void MainWidget::keyReleaseEvent(QKeyEvent* e) {

    if (e->key() == Qt::Key_Z)
        fFlag = false;
    else if (e->key() == Qt::Key_S)
        bFlag = false;

    if (e->key() == Qt::Key_D)
        rFlag = false;
    else if (e->key() == Qt::Key_Q)
        lFlag = false;
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
    /*QVector2D mousePos;
    {
        QPoint p = this->mapFromGlobal(QCursor::pos());
        mousePos = QVector2D(static_cast<float>(p.x()), static_cast<float>(p.y()));
    }

    moveCamera(fFlag, bFlag, rFlag, lFlag, camera, View, speed);
    rotateCamera(mousePos, camera, View);*/

    moveObject(fFlag, bFlag, rFlag, lFlag, objPos, objDir, speed);

    float size = terrain->getSize();
    float half_size = size / 2;
    float res = terrain->getRes();

    QVector2D p1 = QVector2D(ceil((objPos.x() + half_size)/res), floor((objPos.z() + half_size)/res));
    QVector2D p2 = QVector2D(floor((objPos.x() + half_size)/res), ceil((objPos.z() + half_size)/res));
    QVector2D p3;

    QVector2D q = QVector2D(floor(objPos.x()), floor(objPos.z()));
    QVector2D r = QVector2D(ceil(objPos.x()), ceil(objPos.z()));

    QVector2D objPos2D = QVector2D(objPos.x(), objPos.z());

    if (objPos2D.distanceToPoint(q) < objPos2D.distanceToPoint(r))
        p3 = QVector2D(floor((objPos.x() + half_size)/res), floor((objPos.z() + half_size)/res));
    else
        p3 = QVector2D(ceil((objPos.x() + half_size)/res), ceil((objPos.z() + half_size)/res));

    QVector3D P1 = terrain->vertices[static_cast<int>(p1.x() + (size/res - p1.y())*size/res)].position;
    QVector3D P2 = terrain->vertices[static_cast<int>(p2.x() + (size/res - p2.y())*size/res)].position;
    QVector3D P3 = terrain->vertices[static_cast<int>(p3.x() + (size/res - p3.y())*size/res)].position;

    QVector3D n = QVector3D::crossProduct(P2 - P1, P3 - P1);

    QVector3D P = QVector3D(objPos.x(), 0.0f, objPos.z());
    QVector3D PP1 = P1 - P;

    float y = QVector3D::dotProduct(PP1, n) / n.y();

    objPos.setY(y + object->getSize());

    camera.eye = objPos - 5.0*objDir + 5.0*QVector3D(0.0f, 1.0f, 0.0f);
    camera.lForward = objDir - QVector3D(0.0f, 1.0f, 0.0f);
    camera.lUp = QVector3D(0.0f, 1.0f, 0.0f);

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

    terrain = new GeometryEngine;
    terrain->initAsPlaneGeometry(128, 64.0f, ":heightmap.png");

    object = new GeometryEngine;
    object->initAsSphereGeometry();

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!programTerrain.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshaderTerrain.glsl"))
        close();

    // Compile fragment shader
    if (!programTerrain.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshaderTerrain.glsl"))
        close();

    // Link shader pipeline
    if (!programTerrain.link())
        close();

    // Compile vertex shader
    if (!programObj.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshaderObj.glsl"))
        close();

    // Compile fragment shader
    if (!programObj.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshaderObj.glsl"))
        close();

    // Link shader pipeline
    if (!programObj.link())
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
}

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
    Projection.perspective(fov, static_cast<float>(aspect), zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture_grass->bind(0);
    texture_rock->bind(1);
    texture_snow->bind(2);

//! [6]

    // Bind shader pipeline for use
    if (!programTerrain.bind())
        close();

    View.setToIdentity();
    View.lookAt(camera.eye, camera.getCenter(), camera.lUp);

    // Set modelview-projection matrix
    programTerrain.setUniformValue("model_matrix", Model);
    programTerrain.setUniformValue("view_matrix", View);
    programTerrain.setUniformValue("projection_matrix", Projection);
    //! [6]

    // Use texture unit 0 which contains cube.png
    programTerrain.setUniformValue("texture_grass", 0);
    programTerrain.setUniformValue("texture_rock", 1);
    programTerrain.setUniformValue("texture_snow", 2);

    // Draw plane geometry
    terrain->drawGeometry(&programTerrain);

    if (!programObj.bind())
        close();

    QMatrix4x4 obj_model;
    obj_model.setToIdentity();
    obj_model.translate(objPos);

    programObj.setUniformValue("model_matrix", obj_model);
    programObj.setUniformValue("view_matrix", View);
    programObj.setUniformValue("projection_matrix", Projection);

    object->drawGeometry(&programObj);
}

void moveCamera(bool fFlag, bool bFlag, bool rFlag, bool lFlag, Camera & cam, QMatrix4x4 & View, float speed)
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
    float angleX = atanf(diff.x() / 640.0f);

    if (angleX < 0.1f && angleX > -0.1f)
        angleX = 0.0f;

    QMatrix4x4 M;
    M.setToIdentity();
    M.rotate(-angleX, lUp);
    lRight = M*lRight;
    lForward = M*lForward;

    QVector3D center = cam.getCenter();
    lUp = QVector3D::crossProduct(lRight, lForward).normalized();

    View.setToIdentity();
    View.lookAt(eye, center, lUp);
}

void moveObject(bool fFlag, bool bFlag, bool rFlag, bool lFlag, QVector3D & objPos, QVector3D & objDir, float speed) {

    if (fFlag)
        objPos += speed * objDir;
    else if (bFlag)
        objPos -= speed * objDir;

    QMatrix4x4 M;
    M.setToIdentity();

    float angleSpeed = 0.0f;

    if (rFlag)
        angleSpeed = -1.0f;
    else if (lFlag)
        angleSpeed = 1.0f;

    if (rFlag || lFlag){
        M.rotate(angleSpeed, QVector3D(0.0, 1.0, 0.0));
        objDir = M * objDir;
    }
}
