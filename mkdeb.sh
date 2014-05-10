#!/bin/bash

# a code archive should already be available

TOP=`pwd`
CODE_ARCHIVE="$1-$2.tar.bz2"
CODE_DIR="$1-$2"
INSTALL_DIR=$TOP
INSTALL_DIR+="/debian/usr"
DEBIAN_CTRL_DIR=$TOP
DEBIAN_CTRL_DIR+="/debian/DEBIAN"

echo "*****************************************"
echo "code archive: $CODE_ARCHIVE"
echo "code directory: $CODE_DIR"
echo "install directory: $INSTALL_DIR"
echo "debian control directory: $DEBIAN_CTRL_DIR"
echo "*****************************************"
tar -xjvf $CODE_ARCHIVE
mkdir -p $INSTALL_DIR
cd $CODE_DIR
./configure --prefix=$INSTALL_DIR
make && make install
cd ..
echo "*****************************************"
SIZE=`du -s debian/usr`
echo "install size $SIZE"
echo "*****************************************"

mv $INSTALL_DIR/share/doc/qtmib/RELNOTES $INSTALL_DIR/share/doc/qtmib/changelog.Debian
gzip -9 $INSTALL_DIR/share/doc/qtmib/changelog.Debian
rm $INSTALL_DIR/share/doc/qtmib/COPYING
cp platform/deb/copyright $INSTALL_DIR/share/doc/qtmib/.
mkdir -p $DEBIAN_CTRL_DIR
sed "s/QTMIBVER/$2/g"  platform/deb/control > $DEBIAN_CTRL_DIR/control
find ./debian -type d | xargs chmod 755
dpkg-deb --build debian
lintian debian.deb
mv debian.deb qtmib_$2_1_amd64.deb
rm -fr debian
rm -fr $CODE_DIR







