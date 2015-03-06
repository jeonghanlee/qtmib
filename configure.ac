AC_PREREQ([2.68])
AC_INIT(qtmib, 1.1.1, rcpteam@yahoo.com, , http://qtmib.sourceforge.net)
AC_CONFIG_SRCDIR([src/qtmib/main.cpp])
#AC_CONFIG_HEADERS([config.h])


AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB

# checking pthread library
AC_CHECK_LIB([pthread], [main], [], AC_MSG_ERROR([*** POSIX thread support not installed - please install first ***]))
AC_CHECK_HEADER(pthread.h,,AC_MSG_ERROR([*** POSIX thread support not installed - please install first ***]))

# checking qt4 library
# Gentoo 64bit
if test -f /usr/lib64/qt4/libQtCore.so
then
	echo "Found qt4 library in /usr/lib64/qt4 directory"
# Gentoo 32bit
elif test -f /usr/lib/qt4/libQtCore.so
then
	echo "Found qt4 library in /usr/lib/qt4 directory"
else
	AC_CHECK_LIB([QtCore], [main], [], AC_MSG_ERROR([*** QtCore library not installed - please install QT4 first ***]))
	AC_CHECK_LIB([QtGui], [main], [], AC_MSG_ERROR([*** QtCore library not installed - please install QT4 first ***]))
fi

# testing qmake
QMAKE=`which qmake`
if test -z "$QMAKE"
then
	# fix for Fedora
	if test -f /usr/lib64/qt4/bin/qmake; then
		QMAKE=/usr/lib64/qt4/bin/qmake
	fi
	
	if test -z "$QMAKE"
	then
		AC_MSG_ERROR([qmake is missing, please install QT4 development packages.])
	fi
fi
echo "Found $QMAKE"
$QMAKE -v

cd src/qtmib && $QMAKE && cd ../..
cd src/discover && $QMAKE && cd ../..
cd src/report && $QMAKE && cd ../..

# build sanitizer.sh file
echo "#!/bin/bash" >sanitizer.sh
echo "export CFLAGS=-fsanitize=address" >> sanitizer.sh
echo "export CXXFLAGS=-fsanitize=address" >> sanitizer.sh
echo "export LDFLAGS=-lasan" >> sanitizer.sh
echo "cd src/qtmib && $QMAKE && cd ../.." >> sanitizer.sh
echo "cd src/discover && $QMAKE && cd ../.." >> sanitizer.sh
echo "cd src/report && $QMAKE && cd ../.." >> sanitizer.sh
chmod +x sanitizer.sh

# testing net-snmp
NETSNMP=`which snmpget`
if test -z "$NETSNMP"
then
	AC_MSG_WARN([net-snmp is requred for running qtmib, please install it (http://www.net-snmp.org/).])
else
	echo "Found $NETSNMP"
fi

cp confdefs.h qtmib_config.h
rm -f qtmib_prefix.h
AC_OUTPUT(Makefile)

