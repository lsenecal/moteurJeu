#include "mainwidget.h"

MainWidget::MainWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    View.lookAt(QVector3D(0, 0, 5), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    Model.setToIdentity();
}

MainWidget::~MainWidget()
{
    makeCurrent();
    delete mesh;
    doneCurrent();
}

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    mesh = new Mesh();

    GO = GameObject(mesh, &program);
}

void MainWidget::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    if (!program.link())
        close();

    if (!program.bind())
        close();
}

void MainWidget::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = 1, zFar = 100.0, fov = 45.0;

    Projection.setToIdentity();

    Projection.perspective(fov, aspect, zNear, zFar);
}

void MainWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.setUniformValue("model_matrix", Model);
    program.setUniformValue("view_matrix", View);
    program.setUniformValue("projection_matrix", Projection);

    // Test 1
    GO.draw();

    // Test 2
    /*GO.getMesh()->getArrayBuf().bind();
    GO.getMesh()->getIndexBuf().bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = GO.getShader()->attributeLocation("a_position");
    GO.getShader()->enableAttributeArray(vertexLocation);
    GO.getShader()->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL shadersmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = GO.getShader()->attributeLocation("a_tbexcoord");
    GO.getShader()->enableAttributeArray(texcoordLocation);
    GO.getShader()->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, GO.getMesh()->getSize(), GL_UNSIGNED_SHORT, 0);*/
}


