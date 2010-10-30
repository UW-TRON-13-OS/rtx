#ifndef _UTEST_H_
#define _UTEST_H_

#include <stdio.h>

extern int num_tests;
extern int num_passed;

            //printf("%s:%s ", __FILE__, __LINE__ ); 
#define utest_assert(condition, errMsg) do { \
        if (!(condition)) { \
            printf((errMsg)); \
        } else { \
            num_passed++; \
        } \
        num_tests++; \
    } while (0)

#define utest_test_results() (num_passed * 16 + num_tests)

#endif
