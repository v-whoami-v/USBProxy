#!/bin/sh

LL=0
LR=38
RL=218
RR=255

for x in `seq $LL $LR`; do
    for y in `seq 0 255`; do
        X=$1
        Y=$2
        echo ${x},${y},${X},${Y}
    done
done

for x in `seq $RL $RR`; do
    for y in `seq 0 255`; do
        X=$3
        Y=$4
        echo ${x},${y},${X},${Y}
    done
done
