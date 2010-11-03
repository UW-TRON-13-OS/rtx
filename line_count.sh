#!/bin/sh

find . -name "*.[c,h,sh,mk]" | xargs wc -l | tail -n 1 | awk '{print $1}'
