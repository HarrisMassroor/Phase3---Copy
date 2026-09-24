#include "square.h"

long Square(long n, unsigned long *invocations)
{
    ++(*invocations);
    if (n == 0)
        return 0;
    return Square(n - 1, invocations) + n + n - 1;
}