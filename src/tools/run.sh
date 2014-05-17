#!/bin/bash
./qtmibadd treemodel.txt sofaware ~/.config/qtmib/mibs/96xxmib31.txt  > t
mv t treemodel.txt
./qtmibadd treemodel.txt centillion ~/.config/qtmib/mibs/netapp.mib > t
mv t treemodel.txt
./qtmibadd treemodel.txt sonicwall ~/.config/qtmib/mibs/LANCOPE-SNMPV2-MIB.txt > t
mv t treemodel.txt
./qtmibadd treemodel.txt juniper-ive ~/.config/qtmib/mibs/fortinet-* > t
mv t treemodel.txt
./qtmibadd treemodel.txt "security(5)" ~/.config/qtmib/mibs/pica* > t
mv t treemodel.txt
./qtmibadd treemodel.txt entitySensorMIB ~/.config/qtmib/mibs/GSMP-MIB.txt > t
mv t treemodel.txt
