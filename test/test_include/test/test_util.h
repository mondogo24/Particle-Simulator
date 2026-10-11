#ifndef TEST_TEST_UTIL_H
#define TEST_TEST_UTIL_H

#include <stdio.h>

#define ASSERT(expresion, fail_message) if (!(expresion)) \
    printf("Assert failed at: %s, %s, %d\nExpresion: %s\nFail message: %s\n\n", \
    __FILE__, __func__, __LINE__, #expresion, fail_message)

#define FINALIZE_TESTING printf("Testing finalized in %s.\n", __FILE__)

#endif
