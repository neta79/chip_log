#ifndef __TEST_SUPPORT_H__
#define __TEST_SUPPORT_H__ 1

#include <stdlib.h>
#include <stdio.h>

#define check(cond) if (!(cond)) { \
    fprintf(stderr, "failed check: " #cond " (in " __FILE__ ", line %d)\n", __LINE__ ); \
    exit(1); \
}

#endif /* __TEST_SUPPORT_H__ */
