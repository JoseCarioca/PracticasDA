#!/bin/bash
niveles=(1224 2225 2054 2056 2556 2452 2428 3256 3292 1324 1625 1854 2356 2856 3456 3552 3728 4356 4592 1111 1333 1555 1888 2222 2444 2777 3111 3333 3666 4044 4144 4777)
for j in "${niveles[@]}"
do
	#echo "nivel: $j"
    ../simulador/simulador -level $j #-v #comentar v
    #mv game.ppm $j.ppm #comentar linea si fuera necesario
done
