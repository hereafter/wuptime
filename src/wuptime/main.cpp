#include "pch.h"
#include "Uptime.h"

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


int main(int argc, const char** argv)
{
    UptimeInfo info;
    wstringstream ss;
    ss << info.Uptime() << endl;
    ss << info.StartDateTime() << endl;
    
    cout << CW2A(ss.str().c_str(), CP_UTF8).m_psz;
    return 0;
}
