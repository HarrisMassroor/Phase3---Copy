#include "square.h"

int Square(int N)
{
    if (N == 0)
        return 0;
    return Square(N - 1) + N + N - 1;
}
