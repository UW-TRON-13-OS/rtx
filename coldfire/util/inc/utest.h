#ifndef _UTEST_H_
#define _UTEST_H_

#include "dbug.h"
#include "utils.h"

extern int num_tests;
extern int num_test_passed;

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

#define utest_test_results() \
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

#endif
