#ifndef _UTEST_H_
#define _UTEST_H_

#include "dbug.h"
#include "utils.h"

extern int num_tests;
extern int num_test_passed;

#define utest_start() do { \
        num_tests = 0; \
        num_test_passed = 0; \
    } while (0)

#define utest_assert(condition, errMsg) do { \
        if (!(condition)) { \
            dbug(errMsg "\n"); \
        } else { \
            num_test_passed++; \
        } \
        num_tests++; \
    } while (0)

#define utest_assert_str(str1,str2,errMsg) do { \
        if (!rtx_strcmp((str1), (str2)) == 0) { \
            printf(errMsg "\n"); \
        } else { \
            num_test_passed++; \
        } \
        num_tests++; \
    } while (0)

#define utest_test_results() do { \
        if (num_test_passed == num_tests)  \
        { \
            rtx_dbug_outs("    \033[1;32m"); \
            rtx_dbug_uint(num_test_passed); \
            rtx_dbug_outs(" / "); \
            rtx_dbug_uint(num_tests); \
            dbug("\033[0m"); \
        } \
        else \
        { \
            rtx_dbug_outs("    \033[1;31m"); \
            rtx_dbug_uint(num_test_passed); \
            rtx_dbug_outs(" / "); \
            rtx_dbug_uint(num_tests); \
            dbug("\033[0m"); \
        } \
        /* Store fake value of 0 for return value from main */ \
        asm("move.l #0, %d7"); \
        /* Get back to the monitor */ \
        asm("move.l #0,%d0"); \
        asm("trap #15"); \
    } while (0)

#endif
