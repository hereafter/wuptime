#include "pch.h"
#include "Uptime.h"
#include <map>
#include <algorithm>
#include <conio.h>
#include <wil/resource.h>
#include <tlhelp32.h>
#include <Psapi.h>

#include <winrt/base.h>
#include <winrt/windows.applicationmodel.h>


using namespace std;


void usage();
void version();
int execute(int argc, const char** argv);
bool is_launched_from_ui();

int main(int argc, const char** argv)
{    
    auto noshell = is_launched_from_ui();
    if (noshell)
    {
        SetConsoleTitle(L"uptime");
    }

    auto code = execute(argc, argv);

    if (noshell)
    {
        cout << endl;
        cout << "Press any key to continue" << endl;
        while (_kbhit() == 0)
        {
            Sleep(100);
        }
    }
    return code;
}

int execute(int argc, const char** argv)
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
    cout << "uptime from wuptime ";
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

bool is_launched_from_ui()
{
    using namespace wil;

    DWORD ppid = 0;
    auto pid = ::GetCurrentProcessId();
    unique_handle snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

    PROCESSENTRY32 pe{ 0 };
    pe.dwSize = sizeof(pe);

    auto success = Process32First(snapshot.get(), &pe);
    if (!success) return false;
    do
    {
        if (pid == pe.th32ProcessID)
        {
            ppid = pe.th32ParentProcessID;
            break;
        }
    } while (Process32Next(snapshot.get(), &pe));

    if (ppid == 0) return false;

    unique_process_handle h{ ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ppid) };
    TCHAR szFilePath[MAX_PATH] = { 0 };
    DWORD filePathSize = MAX_PATH;
    ::QueryFullProcessImageName(h.get(), 0, szFilePath, &filePathSize);

    auto fileName = ::PathFindFileName(szFilePath);
    TCHAR szFileName[MAX_PATH] = { 0 };
    StrCpy(szFileName, fileName);
    PathRemoveExtension(szFileName);
    wstring fn = szFileName;
    std::transform(fn.begin(), fn.end(), fn.begin(), ::towlower);

    if (fn == L"explorer" || fn == L"dllhost") return true;
    return false;
}