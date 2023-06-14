#!/bin/bash
niveles=(3124 3125 3054 3056 3056 3056 3052 3028 4056 4092 1324 1625 1854 2356 2856 3456 3552 3728 4356 4592 1111 1333 1555 1888 2222 2444 2777 3111 3333 3666 4044 4144 4777)
p=(0.5 0.6 0.4 0.5 0.7 1 0.5 0.6 0.3 0.5)
d=(30 40 40 50 40 50 40 60 70 80)
t=(30 40 30 40 20 50 40 50 70 70)
r=(0.5 0.6 0.4 0.5 0.7 0.5 0.5 0.6 0.3 0.5)

for j in "${niveles[@]}"
do
	echo "nivel: $j"
    ../simulador/simulador -level $j -pases $p -defenses $d -defensetypes $t -defenserandomness $r
     #-v #comentar v
    #mv game.ppm $j.ppm #comentar linea si fuera necesario
done
