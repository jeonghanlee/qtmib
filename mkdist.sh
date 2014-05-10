#!/bin/bash

DIR="$1-$2"

mkdir $DIR
cp -R src $DIR/.
cd $DIR
rm -fr `find . -name .svn`
rm -fr src/tools src/art
cd ..
mkdir $DIR/build
cp configure $DIR/.
cp Makefile.in $DIR/.
cp mkdist.sh $DIR/.
cp mkdeb.sh $DIR/.
cp mkman.sh $DIR/.
cp install.sh $DIR/.
cp COPYING $DIR/.
cp README $DIR/.
cp RELNOTES $DIR/.
cp platform $DIR/.

tar -cjvf $DIR.tar.bz2 $DIR
rm -fr $DIR






