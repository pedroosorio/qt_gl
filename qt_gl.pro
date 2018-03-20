#-------------------------------------------------
#
# Project created by QtCreator 2018-03-13T23:46:44
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_gl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lOpenGL32 -l"$$PWD/ThirdParty/freetype/win64/freetype"

INCLUDEPATH += "$$PWD/ThirdParty" \
               "$$PWD/ThirdParty/freetype/include"

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    glwidget.cpp \
    mesh.cpp \
    renderengine.cpp \
    scene.cpp \
    camera.cpp \
    model.cpp \
    objloader.cpp

HEADERS += \
        mainwindow.h \
    glwidget.h \
    mesh.h \
    renderengine.h \
    scene.h \
    camera.h \
    model.h \
    context.h \
    objloader.h \
    objloader.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    Shaders/vertex_shader.vert \
    Shaders/fragment_shader.frag

RESOURCES += \
    res.qrc

# using shell_path() to correct path depending on platform
# escaping quotes and backslashes for file paths
CONFIG(debug, debug|release): OUTDIR = debug
else: OUTDIR = release
copydata.commands = $(COPY_FILE) \"$$shell_path($$PWD\\ThirdParty\\freetype\\win64\\freetype.dll)\" \"$$shell_path($$OUT_PWD\\$$OUTDIR)\"
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
