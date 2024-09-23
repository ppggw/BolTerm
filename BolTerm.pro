QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    frameupdater.cpp \
    main.cpp \
    mainwindow.cpp \
    udpclient.cpp

HEADERS += \
    frameupdater.h \
    mainwindow.h \
    udpclient.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += E:/opencv/build/build_debug/install/include
LIBS += -L"E:/opencv/build/build_debug/install/x64/vc16/lib" -lopencv_core451d
LIBS += -L"E:/opencv/build/build_debug/install/x64/vc16/lib" -lopencv_highgui451d
LIBS += -L"E:/opencv/build/build_debug/install/x64/vc16/lib" -lopencv_imgproc451d
LIBS += -L"E:/opencv/build/build_debug/install/x64/vc16/lib" -lopencv_features2d451d
LIBS += -L"E:/opencv/build/build_debug/install/x64/vc16/lib" -lopencv_calib3d451d
LIBS += -L"E:/opencv/build/build_debug/install/x64/vc16/lib" -lopencv_video451d
LIBS += -L"E:/opencv/build/build_debug/install/x64/vc16/lib" -lopencv_videoio451d
LIBS += -L"E:/opencv/build/build_debug/install/x64/vc16/lib" -lopencv_imgcodecs451d
LIBS += -L"E:/opencv/build/build_debug/install/x64/vc16/lib" -lopencv_dnn451d
LIBS += -L"E:/opencv/build/build_debug/install/x64/vc16/lib" -lopencv_tracking451d
