QT += network

HEADERS += \
    server.h \
    serverlogic.h

SOURCES += \
    server.cpp \
    main.cpp \
    serverlogic.cpp

QMAKE_CXXFLAGS += -std=c++0x
