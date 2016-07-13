QT += \
    xml

TARGET = \
    nclstats

VERSION = \
    0.1.2

DEFINES += \
    VERSION='\\"$${VERSION}-$$system(git log -n 1 --format="%h")\\"'

INCLUDEPATH += \
    src/ \
    src/core/ \
    src/ui \
    src/util/


SOURCES += \
    src/main.cpp \
    src/core/qnstanalyser.cpp \
    src/core/qnstdocument.cpp \
    src/core/qnstentity.cpp \
    src/util/qnstlanguage.cpp \
    src/util/qnstsettings.cpp \
    src/util/qnstutil.cpp \
    src/util/qnstregister.cpp

HEADERS += \
    src/core/qnstdocument.h \
    src/core/qnstentity.h \
    src/util/qnstlanguage.h \
    src/util/qnstsettings.h \
    src/util/qnstutil.h \
    src/core/qnstanalyser.h \
    src/util/qnstregister.h
