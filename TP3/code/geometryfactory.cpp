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

#include "geometryfactory.h"

#include <QVector2D>
#include <QVector3D>
#include <QImage>
#include <QtMath>
#include <iostream>
#include <fstream>
#include <QFile>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryFactory::GeometryFactory()
{
}

GeometryFactory::~GeometryFactory()
{

}
//! [0]

void GeometryFactory::addCubeGeometry(Mesh *mesh)
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.

    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(1.0f, 1.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.0f)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.0f, 1.0f)}, // v4
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(1.0f, 1.0f)}, // v5
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v6
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(1.0f, 0.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.0f, 1.0f)}, // v8
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v9
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.0f, 0.0f)}, // v10
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 0.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.0f, 1.0f)}, // v12
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 1.0f)},  // v13
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.0f, 0.0f)}, // v14
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.0f, 1.0f)}, // v16
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(1.0f, 1.0f)}, // v17
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)}, // v18
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)}, // v19

        // Vertex data for face 5
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)}, // v20
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(1.0f, 1.0f)}, // v21
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.0f, 0.0f)}, // v22
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(1.0f, 0.0f)}  // v23
    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

    mesh->setSize(34);

//! [1]
    // Transfer vertex data to VBO 0
    mesh->getArrayBuf().bind();
    mesh->getArrayBuf().allocate(vertices, 24 * sizeof(VertexData));

    // Transfer index data to VBO 1
    mesh->getIndexBuf().bind();
    mesh->getIndexBuf().allocate(indices, 34 * sizeof(GLushort));
//! [1]
}

void GeometryFactory::addPlaneGeometry(Mesh *mesh)
{
    QVector<VertexData> planeVertices;

    unsigned int n = 64;
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j){
            planeVertices.push_back({ QVector3D((1.0f*j - n/2.0), 0, -(1.0f*i - n/2.0)), QVector2D(1.0f*j/(n-1), 1.0f*i/(n-1)) });
        }

    QVector<GLushort> planeIndices;

    for (size_t j = 0; j < n-1; ++j) {
        if (j != 0){
            planeIndices.push_back(static_cast<int>(j)*n + n);
        }
        for (size_t i = 0; i < n; ++i) {
            planeIndices.push_back(static_cast<int>(j)*n + static_cast<int>(i) + n);
            planeIndices.push_back(static_cast<int>(j)*n + static_cast<int>(i));
        }
        if(j != n-2){
            planeIndices.push_back((static_cast<int>(j)+1)*n - 1);
        }
    }

    mesh->setSize(planeIndices.size());

    mesh->getArrayBuf().bind();
    mesh->getArrayBuf().allocate(planeVertices.data(), planeVertices.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    mesh->getIndexBuf().bind();
    mesh->getIndexBuf().allocate(planeIndices.data(), planeIndices.size() * sizeof(GLushort));
}

void GeometryFactory::addSphereGeometry(Mesh * mesh)
{
    std::vector<QVector3D> vertices;
    std::vector<GLushort> indices;

    QFile qFile(":sphere.off");
    if (!qFile.open(QIODevice::ReadOnly | QIODevice::Text))
           assert(false);

    QString line = qFile.readLine();
    line = line.chopped(1);

    assert(line == "OFF");

    line = qFile.readLine();
    line = line.chopped(1);
    QStringList fields = line.split(" ");

    int vsize = fields[0].toInt();
    int isize = fields[1].toInt();

    for (int i = 0; i < vsize; ++i)
    {
        line = qFile.readLine();
        line = line.chopped(1);
        fields = line.split(" ");
        float x, y, z;
        x = fields[0].toFloat();
        y = fields[1].toFloat();
        z = fields[2].toFloat();
        QVector3D v = QVector3D(x, y, z);
        vertices.push_back(v);
    }

    for (int i = 0; i < isize; ++i)
    {
        line = qFile.readLine();
        line = line.chopped(1);
        fields = line.split(" ");
        indices.push_back(fields[0].toInt());
        indices.push_back(fields[1].toInt());
        indices.push_back(fields[2].toInt());
    }

    qFile.close();


    /*QFile qFile(":sphere.obj");
        if (!qFile.open(QIODevice::ReadOnly | QIODevice::Text))
               assert(false);

    QString line = qFile.readLine();
    line = qFile.readLine();
    line = qFile.readLine();
    line = qFile.readLine();

    int i = 0;

    while(!qFile.atEnd()) {
        line = qFile.readLine();
        line = line.chopped(1);
        QStringList fields = line.split(" ");

        if (fields[0] == 'v') {
            VertexData v;
            v.position = QVector3D(fields[1].toFloat(), fields[2].toFloat(), fields[3].toFloat());
            vertices.push_back(v);
        }
        else if (fields[0] == 'vt') {
            QVector2D tex = QVector2D(fields[1].toFloat(), fields[2].toFloat());
            vertices[i].texCoord = tex;
            i++;
        }
        else if (fields[0] == 'vn') {

        }
        else if (fields[0] == 'f') {
            for (int j = 1; j < fields.size(); ++j) {
                QString tri = fields[j];
                qDebug() << tri;
                QStringList indices = tri.split('/');
                indices.push_back(indices[0]);
                indices.push_back(indices[1]);
                indices.push_back(indices[2]);
            }
        }
    }

    qFile.close();

    */

    mesh->setSize(indices.size());

    mesh->getArrayBuf().bind();
    mesh->getArrayBuf().allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

    // Transfer index data to VBO 1
    mesh->getIndexBuf().bind();
    mesh->getIndexBuf().allocate(indices.data(), indices.size() * sizeof(GLushort));
}
