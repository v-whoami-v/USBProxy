#!/bin/sh

LL=0
LR=37
RL=219
RR=255

for x in `seq $LL $LR`; do
    for y in `seq 0 255`; do
        X=0
        Y=1
        echo ${x},${y},${X},${Y}
    done
done

for x in `seq $RL $RR`; do
    for y in `seq 0 255`; do
        X=255
        Y=1
        echo ${x},${y},${X},${Y}
    done
done
