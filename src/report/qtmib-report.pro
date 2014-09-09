QMAKE_CXXFLAGS += $$(CXXFLAGS) -fstack-protector-all -D_FORTIFY_SOURCE=2 -fPIE -pie -Wformat -Wformat-security
QMAKE_CFLAGS += $$(CFLAGS) -fstack-protector-all -D_FORTIFY_SOURCE=2 -fPIE -pie -Wformat -Wformat-security
QMAKE_LIBS += $$(LDFLAGS) -Wl,-z,relro -Wl,-z,now
TEMPLATE      = app
HEADERS       = mainwindow.h \
                exec_prog.h \
                qtmib_report.h \
                report.h \
                bundle.h \
                qtmib_ip.h \
                ../common/pref_dialog.h
SOURCES       = mainwindow.cpp \
                exec_prog.c \
                main.cpp \
                report.cpp \
                report_sys.cpp \
                report_soft.cpp \
                report_proc.cpp \
                report_interface.cpp \
                report_route.cpp \
                report_tcpconnection.cpp \
                report_udpconnection.cpp \
                bundle.cpp \
                ../common/pref_dialog.cpp
RESOURCES = qtmib-report.qrc
TARGET=../../build/qtmib-report
