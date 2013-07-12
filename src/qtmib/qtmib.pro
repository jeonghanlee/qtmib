QMAKE_CXXFLAGS += $$(CXXFLAGS)
QMAKE_CFLAGS += $$(CFLAGS)
QMAKE_LIBS += $$(LDFLAGS)
HEADERS   = mainwindow.h qtmib.h clicked_label.h pref_dialog.h oid_translator.h \
	    exec_prog.h search_dialog.h
SOURCES   = main.cpp \
            mainwindow.cpp \
            exec_prog.c \
            clicked_label.cpp \
            pref_dialog.cpp \
            oid_translator.cpp \
            utils.cpp \
            search_dialog.cpp
RESOURCES = qtmib.qrc
TARGET=../../build/qtmib

