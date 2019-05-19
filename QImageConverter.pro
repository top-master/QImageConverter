#
# Ready project using QImageReader, QImageWriter classes and XD framework
#

TEMPLATE = app
TARGET = QImageConverter

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}
load(extras)

DESTDIR = bin

RC_FILE = $$PWD/assets/main.rc

INCLUDEPATH += $$PWD/src

copyModules()

copyImageDriver(svg)
copyImageDriver(webp)

SOURCES += \
    src/component/mainview.cpp \
    src/feature/convert-thread.cpp \
    src/main-tool.cpp \
    src/service/apptool.cpp \
    $$extras/tools/crash-report.cpp \
    src/component/resize-mode.cpp

HEADERS  += \
    src/component/mainview.h \
    src/feature/convert-thread.h \
    src/service/apptool.h \
    src/main-tool.h \
    src/component/resize-mode.h

FORMS    += \
    src/component/mainview.ui \
    src/component/resize-mode.ui

RESOURCES += \
    assets/main.qrc

