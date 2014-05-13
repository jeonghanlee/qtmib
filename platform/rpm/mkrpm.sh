#!/bin/bash
rm -fr rpmbuild

mkdir -p ~/rpmbuild/{RPMS,SRPMS,BUILD,SOURCES,SPECS,tmp}
cat <<EOF >~/.rpmmacros
%_topdir   %(echo $HOME)/rpmbuild
%_tmppath  %{_topdir}/tmp
EOF

cd ~/rpmbuild

mkdir -p qtmib-1.0/usr/bin
install -m 755 /usr/bin/qtmib qtmib-1.0/usr/bin/.
install -m 755 /usr/bin/qtmib-discover qtmib-1.0/usr/bin/.
install -m 755 /usr/bin/qtmib-report qtmib-1.0/usr/bin/.
install -m 755 /usr/bin/qtmib-translate qtmib-1.0/usr/bin/.

mkdir -p qtmib-1.0/usr/share/man/man1
install -m 644 /usr/share/man/man1/qtmib.1.gz qtmib-1.0/usr/share/man/man1/.
install -m 644 /usr/share/man/man1/qtmib-discover.1.gz qtmib-1.0/usr/share/man/man1/.
install -m 644 /usr/share/man/man1/qtmib-report.1.gz qtmib-1.0/usr/share/man/man1/.
install -m 644 /usr/share/man/man1/qtmib-translate.1.gz qtmib-1.0/usr/share/man/man1/.

mkdir -p qtmib-1.0/usr/share/doc/packages/qtmib
install -m 644 /usr/share/doc/qtmib/COPYING qtmib-1.0/usr/share/doc/packages/qtmib/.
install -m 644 /usr/share/doc/qtmib/README qtmib-1.0/usr/share/doc/packages/qtmib/.
install -m 644 /usr/share/doc/qtmib/RELNOTES qtmib-1.0/usr/share/doc/packages/qtmib/.

mkdir -p qtmib-1.0/usr/share/applications/
install -m 644 /usr/share/applications/qtmib.desktop qtmib-1.0/usr/share/applications/.
mkdir -p  qtmib-1.0/usr/share/pixmaps
install -m 644 /usr/share/pixmaps/qtmib.png qtmib-1.0/usr/share/pixmaps/.

tar -czvf qtmib-1.0.tar.gz qtmib-1.0

cp qtmib-1.0.tar.gz SOURCES/.

cat <<EOF > SPECS/qtmib.spec
%define        __spec_install_post %{nil}
%define          debug_package %{nil}
%define        __os_install_post %{_dbpath}/brp-compress

Summary: SNMP MIB Browser
Name: qtmib
Version: 1.0
Release: 1
License: GPL+
Group: Development/Tools
SOURCE0 : %{name}-%{version}.tar.gz
URL: http://qtmib.sourceforege.net

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
Easy-to-use SNMP MIB Browser based on QT4 library. It is build as
a front-end for net-snmp tools, and it allows the user to query SNMP-enabled
devices. It implements SNMPv1 and SNMPv2c, and is released under GPL v2
license.

%prep
%setup -q

%build

%install
rm -rf %{buildroot}
mkdir -p  %{buildroot}

cp -a * %{buildroot}


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
%{_bindir}/*
%{_docdir}/*
%{_mandir}/*
/usr/share/applications/qtmib.desktop
/usr/share/pixmaps/qtmib.png
 
%post

%changelog
* Sat May 10 2014  rcpteam <rcpteam@yahoo.com> 1.0
 - First stable release

EOF

rpmbuild -ba SPECS/qtmib.spec
rpm -qpl RPMS/x86_64/qtmib-1.0-1.x86_64.rpm
cd ..
rm -f qtmib-1.0-1.x86_64.rpm
cp rpmbuild/RPMS/x86_64/qtmib-1.0-1.x86_64.rpm .
