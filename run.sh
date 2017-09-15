#!/bin/bash

#clear && make && ./bin/driver.o
#clear && make &&
# ./bin/ReadWritePGMx.o ./data/ship.pgm ./bin/linux_ship_05x.pgm &&
# ./bin/ReadWritePGMx.o ./data/street.pgm ./bin/linux_street_05x.pgm &&
# ./bin/ReadWritePGMx.o ./data/woman.pgm ./bin/linux_woman_05x.pgm 


clear && make &&

for i in 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
do
  ./bin/ReadWritePGMx.o ./data/ship.pgm ./bin/linux_run/linux_ship_0_$i.pgm $i
done
 
for i in 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
do
  ./bin/ReadWritePGMx.o ./data/street.pgm ./bin/linux_run/linux_street_0_$i.pgm $i
done

for i in 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
do
  ./bin/ReadWritePGMx.o ./data/woman.pgm ./bin/linux_run/linux_woman_0_$i.pgm $i
done
 