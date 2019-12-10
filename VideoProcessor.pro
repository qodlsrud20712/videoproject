QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoProcessor
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
	record.cpp

HEADERS += \
        function.h \
        mainwindow.h \
	record.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv4

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_freetype -lopencv_videoio -lopencv_objdetect -lwiringPi


