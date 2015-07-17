#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

typedef struct {
    const char *name;
    int (*func)();
} test_case;

#include "tests_decl.h"

static test_case test_cases[] = {
#include "tests_mapping.inc"
    {NULL, NULL}
};

static int run_all()
{
    test_case *i = test_cases;
    int res = 0;
    for (; i->func != NULL; ++i)
    {
        if (i->func() != 0)
        {
            fprintf(stderr, "failed test: %s\n", i->name);
            ++res;
        }
    }
    return res;
}

static int run_one(char *name)
{
    test_case *i = test_cases;
    for (; i->func != NULL; ++i)
    {
        if (strcmp(name, i->name) != 0)
        {
            continue;
        }
        if (i->func() != 0)
        {
            fprintf(stderr, "failed test: %s\n", i->name);
            return 1;
        }
        return 0;
    }
    fprintf(stderr, "unknown test: %s\n", name);
    return -1;
}

int main(int argc, char **argv)
{
    int i;
    if (argc <= 1)
    {
        return run_all();
    }
    for (i=1; i<argc; ++i)
    {
        run_one(argv[i]);
    }
}
