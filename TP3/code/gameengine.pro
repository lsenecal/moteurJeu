QT       += core gui widgets

TARGET = gameengine
TEMPLATE = app

SOURCES += main.cpp \
    gameobject.cpp \
    geometryfactory.cpp \
    graphscene.cpp \
    mesh.cpp \
    transform.cpp

SOURCES += \
    mainwidget.cpp

HEADERS += \
    gameobject.h \
    geometryfactory.h \
    graphscene.h \
    mainwidget.h \
    mesh.h \
    transform.h

RESOURCES += \
    file.qrc \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[YOUR_PATH]
INSTALLS += target