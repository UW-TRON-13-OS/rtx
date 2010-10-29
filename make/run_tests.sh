#!/bin/sh

if [ $# -lt 1 ] ; then
  echo "usage: `basename $0` <module name>"
  exit 1
fi

MODULE_NAME=$1
MODULE_DIR=`dirname $0`/../tests/$MODULE_NAME

if [ ! -d $MODULE_DIR ] ; then
  echo "Error : Tests at $MODULE_DIR has not built yet. please run 'make test'"
  exit 1
fi

echo ""
echo "Running test suite for $MODULE_NAME"


cd $MODULE_DIR
for test in `find . -executable -type f`
do
  $test
  RESULT=$?
  TESTS_PASSED=`expr $RESULT / 100`
  TESTS_TOTAL=`expr $RESULT % 100`
  if [ $TESTS_PASSED -eq $TESTS_TOTAL ] ; then
    printf "    test %10s ... \033[1;32m $TESTS_PASSED / $TESTS_TOTAL \033[0m\n" $test
  else
    printf "    test %10s ... \033[1;31m $TESTS_PASSED / $TESTS_TOTAL \033[0m\n" $test
    printf "         %10s \033[1;31mFAILED\033[0m . Aborting\n" $test
    exit 1
  fi
done
