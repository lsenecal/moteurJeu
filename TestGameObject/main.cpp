#include "mainwidget.h"

#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#ifndef QT_NO_OPENGL
#include "mainwidget.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    a.setApplicationName("Test GameObject");
    a.setApplicationVersion("0.1");
    #ifndef QT_NO_OPENGL
        MainWidget widget;
        widget.show();
    #else
        QLabel note("OpenGL Support required");
        note.show();
    #endif
        return a.exec();
}
