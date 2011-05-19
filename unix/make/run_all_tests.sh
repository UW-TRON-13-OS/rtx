#!/bin/sh

TOP_DIR=`dirname $0`/..
RUN_TEST_SH=$TOP_DIR/make/run_tests.sh
while [ $# -gt 0 ]
do
  if [ -d "$TOP_DIR/$1/test" ] ; then
    $RUN_TEST_SH $1
  fi
  shift
done
