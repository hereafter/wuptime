#include "pch.h"
#include "UptimeInfo.h"
using namespace std;

constexpr const char* __HELP__ = "\n"
"Usage:\n"
" uptime [options]\n"
"\n"
"Options:\n"
" -p, --pretty    show uptime in pretty format\n"
" -h, --help      display this help and exit\n"
" -s, -since      system up since\n"
" -V, --version   output version information and exit\n\n";



int main()
{
    UptimeInfo info;
    return 0;
}
