#ifndef _UTEST_H_
#define _UTEST_H_

#include <stdio.h>
#include <string.h>

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

#define utest_assert_str(str1,str2,errMsg) do { \
        if (!strcmp((str1), (str2)) == 0) { \
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
        return 0; \
    } \
    else \
    { \
        printf("    %-30s ... \033[1;31m %d / %d \033[0m\n", argv[0]+2, num_passed, num_tests); \
        return -1; \
    } \

#endif
