#ifndef _UTEST_H_
#define _UTEST_H_

#include <stdio.h>

extern int num_tests;
extern int num_passed;

            //printf("%s:%s ", __FILE__, __LINE__ ); 
#define utest_assert(condition, errMsg) do { \
        if (!(condition)) { \
            printf(errMsg "\n"); \
        } else { \
            num_passed++; \
        } \
        num_tests++; \
    } while (0)

#define utest_test_results() \
    if (num_passed == num_tests)  \
    { \
        printf("    %-30s ... \033[1;32m %d / %d \033[0m\n", argv[0]+2, num_passed, num_tests); \
    } \
    else \
    { \
        printf("    %-30s ... \033[1;33m %d / %d \033[0m\n", argv[0]+2, num_passed, num_tests); \
    } \
    return -1;

#endif
