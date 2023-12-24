QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    drawpad28x28.cpp \
    main.cpp \
    mainwindow.cpp \
    predictionboard.cpp

HEADERS += \
    drawpad28x28.h \
    mainwindow.h \
    numberClassifier.hpp \
    predictionboard.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



RESOURCES += \
    model_resource.qrc

INCLUDEPATH += \
    "C:\\Coding\\libs\\libtensorflow-cpu-windows-x86_64-2.11.0\\include"

LIBS += \
    -l"opengl32" \
    -L"C:\\Coding\\libs\\libtensorflow-cpu-windows-x86_64-2.11.0\\lib" \
    -l"tensorflow"
