#!/bin/sh
for x in $(seq 0 9); do
	./client -c $x &
done
echo Client Running
