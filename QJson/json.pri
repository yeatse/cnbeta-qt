INCLUDEPATH += ./QJson
DEPENDPATH  += ./QJson

HEADERS += \
    FlexLexer.h \
    json_parser.hh \
    json_scanner.h \
    location.hh \
    parser.h \
    parser_p.h \
    parserrunnable.h \
    position.hh \
    qjson_debug.h \
    qjson_export.h \
    qobjecthelper.h \
    serializer.h \
    serializerrunnable.h \
    stack.hh


SOURCES += \
    json_parser.cc \
    json_scanner.cc \
    json_scanner.cpp \
    parser.cpp \
    parserrunnable.cpp \
    qobjecthelper.cpp \
    serializer.cpp \
    serializerrunnable.cpp

OTHER_FILES += \
    json_parser.yy \
    json_scanner.yy
