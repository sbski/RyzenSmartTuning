QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    ryzensmarttuningutility.cpp

HEADERS += \
    ../RyzenSmartTuning/RyzenSmartTuning.h \
    ../RyzenSmartTuning/UProfData.h \
    ../third_party/libryzenadj/OlsApi.h \
    ../third_party/libryzenadj/OlsDef.h \
    ../third_party/libryzenadj/nb_smu_ops.h \
    ../third_party/libryzenadj/ryzenadj.h \
    ../third_party/uprof/AMDProfileController.h \
    ../third_party/uprof/AMDTDefinitions.h \
    ../third_party/uprof/AMDTPowerProfileApi.h \
    ../third_party/uprof/AMDTPowerProfileDataTypes.h \
    ryzensmarttuningutility.h

FORMS += \
    ryzensmarttuningutility.ui

TRANSLATIONS += \
    RyzenSmartTuningUtility_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/../third_party/libryzenadj/ -llibryzenadj

INCLUDEPATH += $$PWD/../third_party/libryzenadj
DEPENDPATH += $$PWD/../third_party/libryzenadj

win32: LIBS += -L$$PWD/../third_party/libryzenadj/ -lWinRing0x64

INCLUDEPATH += $$PWD/../third_party/libryzenadj
DEPENDPATH += $$PWD/../third_party/libryzenadj

win32: LIBS += -L$$PWD/../third_party/uprof/ -lAMDPowerProfileAPI

INCLUDEPATH += $$PWD/../third_party/uprof
DEPENDPATH += $$PWD/../third_party/uprof

win32: LIBS += -L$$PWD/../third_party/uprof/ -lAMDProfileController

INCLUDEPATH += $$PWD/../third_party/uprof
DEPENDPATH += $$PWD/../third_party/uprof
