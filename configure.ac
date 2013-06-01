AC_PREREQ([2.68])
AC_INIT(qtmib, 0.8.1, rcpteam@yahoo.com, , http://qtmib.sourceforge.net)
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
AC_CHECK_LIB([QtCore], [main], [], AC_MSG_ERROR([*** QtCore library not installed - please install QT4 first ***]))
AC_CHECK_LIB([QtGui], [main], [], AC_MSG_ERROR([*** QtCore library not installed - please install QT4 first ***]))

# testing qmake
QMAKE=`which qmake`
if test "$QMAKE" == ""
then
	AC_MSG_ERROR([qmake is missing, please install QT4 development packages.])
fi
echo "Found $QMAKE"
qmake -v

cd src/qtmib && qmake && cd ../..

# testing net-snmp
NETSNMP=`which snmpget`
if test "$NETSNMP" == ""
then
	AC_MSG_WARN([net-snmp is requred for running qtmib, please install it (http://www.net-snmp.org/).])
else
	echo "Found $NETSNMP"
fi

cp confdefs.h qtmib_config.h
rm -f qtmib_prefix.h
AC_OUTPUT(Makefile)

