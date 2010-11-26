#!/bin/sh

total=0
for d in `ls $root`
do
  if [ -d $d ]; then
    count=`find $d -name "*.[c,h,sh,mk]" | xargs wc -l | tail -n 1 | awk '{ print $1 }'`
    if [ $count -ne 0 ]; then
      echo "$d $count"
    fi
    total=`expr $count + $total`
  fi
done
echo "total $total"
