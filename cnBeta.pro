TEMPLATE = app
TARGET = cnBeta

VERSION = 2.0.0
DEFINES += VER=\\\"$$VERSION\\\"

QT += network webkit

INCLUDEPATH += src

HEADERS += \
    src/newslistretriever.h \
    src/articleretriever.h \
    src/commentretriever.h

SOURCES += main.cpp \
    src/newslistretriever.cpp \
    src/articleretriever.cpp \
    src/commentretriever.cpp

folder_symbian3.source = qml/cnBeta
folder_symbian3.target = qml

folder_js.source = qml/js
folder_js.target = qml

DEPLOYMENTFOLDERS = folder_js

symbian {
    contains(QT_VERSION, 4.7.3){
        DEFINES += Q_OS_S60V5
        INCLUDEPATH += $$[QT_INSTALL_PREFIX]/epoc32/include/middleware
        INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/Qt
    } else {
        CONFIG += qt-components
        MMP_RULES += "OPTION gcce -march=armv6 -mfpu=vfp -mfloat-abi=softfp -marm"
        DEPLOYMENTFOLDERS += folder_symbian3
    }

    TARGET.UID3 = 0xA0015BFD
    TARGET.CAPABILITY += \
        NetworkServices \
        ReadUserData \
        WriteUserData

    TARGET.EPOCHEAPSIZE = 0x40000 0x4000000

    vendorinfo = "%{\"Yeatse\"}" ":\"Yeatse\""
    my_deployment.pkg_prerules += vendorinfo
    DEPLOYMENT += my_deployment

    # Symbian have a different syntax
    DEFINES -= VER=\\\"$$VERSION\\\"
    DEFINES += VER=\"$$VERSION\"
}

include(QJson/json.pri)
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
