#include "pch.h"

int64_t get_uptime();


int main()
{
    auto times = get_uptime();
    return 0;
}


int64_t get_uptime()
{
    auto ticks=GetTickCount64();
    return 0;
}