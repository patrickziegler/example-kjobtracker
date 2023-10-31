#!/bin/sh

for i in $(seq 5); do
    ./example-kde-jobtracker &
    sleep 1
done
