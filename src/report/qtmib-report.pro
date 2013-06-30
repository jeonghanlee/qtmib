TEMPLATE      = app
HEADERS       = mainwindow.h \
                exec_prog.h \
                qtmib_report.h \
                report.h \
                bundle.h
SOURCES       = mainwindow.cpp \
                exec_prog.c \
                main.cpp \
                report.cpp \
                report_sys.cpp \
                report_soft.cpp \
                report_proc.cpp \
                report_interface.cpp \
                bundle.cpp
RESOURCES = qtmib-report.qrc
TARGET=../../build/qtmib-report
