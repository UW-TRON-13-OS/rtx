#ifndef _UTEST_H_
#define _UTEST_H_

extern int num_tests;
extern int num_passed;

#define utest_assert(condition, errMsgFmt, ...) do { \
        if (!(condition)) { \
            printf("%s:%s %s", __FILE__, __LINE__, #condition); \
            printf((errMsg), __VA_ARGS__); \
        } \
    } while (0)

#define utest_test_results() (num_passed * 100 + num_tests)

#endif
