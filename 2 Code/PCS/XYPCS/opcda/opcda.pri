INCLUDEPATH += $$PWD
SOURCES += \
    $$PWD/OPCWidget.cpp \
    $$PWD/OPCDefine.cpp \
    $$PWD/OPCServerList.cpp \
    $$PWD/OPCServer.cpp \
    $$PWD/OPCGroup.cpp \
    $$PWD/OPCCallback.cpp \
    $$PWD/OPCServerNewDialog.cpp \
    $$PWD/OPCServerPropDialog.cpp \
    $$PWD/OPCGroupNewDialog.cpp \
    $$PWD/OPCGroupEditDialog.cpp \
    $$PWD/OPCItemNewDialog.cpp \
    $$PWD/OPCItemPropDialog.cpp \
    $$PWD/OPCManager.cpp
HEADERS += \
    $$PWD/OPCWidget.h \
    $$PWD/opccomn.h \
    $$PWD/opcerror.h \
    $$PWD/opcguid.h \
    $$PWD/opcda.h \
    $$PWD/OPCDefine.h \
    $$PWD/OPCServerList.h \
    $$PWD/OPCServer.h \
    $$PWD/OPCGroup.h \
    $$PWD/OPCCallback.h \
    $$PWD/OPCServerNewDialog.h \
    $$PWD/OPCServerPropDialog.h \
    $$PWD/OPCGroupNewDialog.h \
    $$PWD/OPCGroupEditDialog.h \
    $$PWD/OPCItemNewDialog.h \
    $$PWD/OPCItemPropDialog.h \
    $$PWD/OPCManager.h
FORMS += \
    $$PWD/OPCWidget.ui \
    $$PWD/OPCServerNewDialog.ui \
    $$PWD/OPCServerPropDialog.ui \
    $$PWD/OPCGroupNewDialog.ui \
    $$PWD/OPCGroupEditDialog.ui \
    $$PWD/OPCItemNewDialog.ui \
    $$PWD/OPCItemPropDialog.ui

LIBS += -lole32 -loleaut32 -luuid
QMAKE_CXXFLAGS +=  -Wno-unknown-pragmas -Wno-unused-parameter -Wno-old-style-cast

