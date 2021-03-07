#!/bin/sh

./geninput -A
sleep $1
./geninput -A -leftx $3 -lefty $4 -ZR
sleep $2
rm /run/input.txt
