#!/bin/sh

while :; do
  for x in `seq 0 255`; do
    printf "00 00 0f %02x %02x 80 80 00" ${x} 0 > /run/input.txt
    sleep 0.0001
  done
  for y in `seq 0 255`; do
    printf "00 00 0f %02x %02x 80 80 00" 255 ${y} > /run/input.txt
    sleep 0.0001
  done
  for x in `seq 255 -1 0`; do
    printf "00 00 0f %02x %02x 80 80 00" ${x} 255 > /run/input.txt
    sleep 0.0001
  done
  for y in `seq 255 -1 0`; do
    printf "00 00 0f %02x %02x 80 80 00" 0 ${y} > /run/input.txt
    sleep 0.0001
  done
done
