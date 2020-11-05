QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp \
    camera.cpp \
    gameobject.cpp \
    geometryfactory.cpp \
    graphscene.cpp \
    mesh.cpp \
    transform.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
	BasicIO.h \
    camera.h \
    gameobject.h \
    geometryfactory.h \
    graphscene.h \
    mainwidget.h \
    geometryengine.h \
    mesh.h \
    transform.h

RESOURCES += \
    file.qrc \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[YOUR_PATH]
INSTALLS += target