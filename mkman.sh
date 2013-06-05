#!/bin/bash

sed "s/VERSION/$1/g" $2 > qtmib.1
MONTH=`date +%b`
sed -i "s/MONTH/$MONTH/g" qtmib.1
YEAR=`date +%Y`
sed -i "s/YEAR/$YEAR/g" qtmib.1
