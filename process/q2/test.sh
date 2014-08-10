#!/bin/sh
./$1 &
for x in $(seq 0 9); do
	./$2 -c $x &
done
echo Running
