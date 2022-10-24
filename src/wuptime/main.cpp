#include "pch.h"
#include "Uptime.h"
#include <map>
#include <algorithm>

#include <winrt/base.h>
#include <winrt/windows.applicationmodel.h>

using namespace std;



void usage();
void version();

int main(int argc, const char** argv)
{
    if (argc == 1)
    {
        UptimeInfo info;
        cout << CW2A(info.Uptime().c_str(), CP_UTF8);
        return 0;
    }

    map<string, int> flags{
        {"-p", 1},
        {"--pretty", 1},
        {"-s", 2},
        {"--since", 2},
        {"-h", 3},
        {"--help", 3},
        {"-v", 4},
        {"--version", 4},
    };


    for (int i = 1; i < argc; i++)
    {
        string opt = argv[i];
        transform(opt.begin(), opt.end(), opt.begin(), ::tolower);

        auto flag = flags.find(opt);
        if (flag == flags.end())
        {
            usage();
            return -1;
        }

        switch (flag->second)
        {
        case 4:
            version();
            return 0;
        case 3:
            usage();
            return 0;
        case 2:
        {
            UptimeInfo info;
            cout << CW2A(info.StartDateTime().c_str(), CP_UTF8);
        }
            return 0;
        case 1:
        {
            UptimeInfo info;
            cout << CW2A(info.PrettyUptime().c_str(), CP_UTF8);
        }
            return 0;

        }
    }

    {
        UptimeInfo info;
        cout << CW2A(info.Uptime().c_str(), CP_UTF8);
        return 0;
    }
}

void usage()
{
    cout << "\n"
        "Usage:\n"
        " uptime [options]\n"
        "\n"
        "Options:\n"
        " -p, --pretty    show uptime in pretty format\n"
        " -h, --help      display this help and exit\n"
        " -s, --since      system up since\n"
        " -V, --version   output version information and exit\n\n";
}

void version()
{
    using namespace winrt;
    using namespace winrt::Windows::ApplicationModel;
    cout << "wuptime ";
    try 
    {
        auto package=Package::Current();
        auto v = package.Id().Version();
        cout << v.Major << "." << v.Minor << "." << v.Build << "." << v.Revision;
    }
    catch(...)
    {
        cout << "unknown version";
    }
}