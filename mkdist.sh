#!/bin/bash

DIR="$1-$2"

mkdir $DIR
cp -R src $DIR/.
cd $DIR
rm -fr `find . -name .svn`
cd ..
mkdir $DIR/build
cp configure $DIR/.
cp Makefile.in $DIR/.
cp mkdist.sh $DIR/.
cp mkdeb.sh $DIR/.
cp install.sh $DIR/.
cp COPYING $DIR/.
cp README $DIR/.
cp RELNOTES $DIR/.

tar -cjvf $DIR.tar.bz2 $DIR
rm -fr $DIR






