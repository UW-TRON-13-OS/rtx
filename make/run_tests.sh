#!/bin/sh

if [ $# -lt 1 ] ; then
  echo "usage: `basename $0` <module name>"
  exit 1
fi

MODULE_NAME=$1
MODULE_DIR=`dirname $0`/../tests/$MODULE_NAME

if [ ! -d $MODULE_DIR ] ; then
  echo "Error : Tests for $MODULE_DIR have not built yet or do not exist."
  exit 1
fi

echo "Running test suite for $MODULE_NAME"


cd $MODULE_DIR
for test in `find . -executable -type f -name "test_*"`
do
  if [ "`basename $test`" = "gmon.out" ]; then
    continue
  fi

  $test
  RESULT=$?
  test_name=`basename $test`
  if [ $RESULT -ne 0 ] ; then
    printf "    %s \033[1;31mFAILED\033[0m . Aborting\n" $test_name
    exit 1
  fi
done

echo "\033[1;32mALL TESTS PASSED\033[0m"
