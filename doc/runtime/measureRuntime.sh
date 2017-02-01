#!/bin/bash
for i in {100000000..6000000000..100000000}
do
    echo -n "$i," >> runtimes.csv
    gcc -std=c11 -DLIMIT=$i sieveRuntime.c -o sieveRuntime -lm && ./sieveRuntime >> runtimes.csv
done
