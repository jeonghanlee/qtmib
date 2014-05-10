QMAKE_CXXFLAGS += $$(CXXFLAGS) -fstack-protector-all -D_FORTIFY_SOURCE=2 -fPIE -pie -Wformat -Wformat-security
QMAKE_CFLAGS += $$(CFLAGS) -fstack-protector-all -D_FORTIFY_SOURCE=2 -fPIE -pie -Wformat -Wformat-security
QMAKE_LIBS += $$(LDFLAGS) -Wl,-z,relro -Wl,-z,now
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
