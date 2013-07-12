QMAKE_CXXFLAGS += $$(CXXFLAGS)
QMAKE_CFLAGS += $$(CFLAGS)
QMAKE_LIBS += $$(LDFLAGS)
TEMPLATE      = app
HEADERS       = mainwindow.h \
                transactionthread.h \
                dev_storage.h \
                dev_db.h \
                exec_prog.h \
                qtmib_ip.h \
                qtmib_socket.h \
                qtmib_discover.h
SOURCES       = mainwindow.cpp \
                main.cpp \
                transactionthread.cpp \
                dev_db.cpp \
                exec_prog.c \
                qtmib_socket.c
RESOURCES = qtmib-discover.qrc
TARGET=../../build/qtmib-discover
