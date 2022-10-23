#include "pch.h"

#include <iostream>

using namespace std;

constexpr uint64_t kSecondsInWeeks = 7 * 24 * 3600;
constexpr uint64_t kSecondsInDays = 24 * 3600;
constexpr uint64_t kSecondsInHours = 3600;
constexpr uint64_t kSecondsInMinutes = 60;

struct UPTIMES 
{
    int weeks;
    int days;
    int hours;
    int minutes;
    int seconds;
    int milliseconds;
};



UPTIMES get_uptime();
wstring format_uptime(UPTIMES const& times, bool weeks=true);


int main()
{
    auto times = get_uptime();
    auto display = format_uptime(times);
    
    return 0;
}


UPTIMES get_uptime()
{
    UPTIMES times = { 0 };
    auto ticks=GetTickCount64(); //ms 1E-3

	uint64_t seconds = ticks / 1000ull;
	times.milliseconds = ticks % 1000ull;
	times.weeks = seconds / kSecondsInWeeks;
	times.days = seconds % kSecondsInWeeks / kSecondsInDays;
	times.hours = (seconds % kSecondsInDays) / kSecondsInHours;
	times.minutes = (seconds % kSecondsInHours) / kSecondsInMinutes;
	times.seconds = seconds % kSecondsInMinutes;
	return times;
}


wstring format_uptime(UPTIMES const& times, bool weeks)
{
    wstringstream ss;
    
    if (weeks)
    {
        if (times.weeks > 0)
        {
            ss << times.weeks << " week";
            if (times.weeks > 1) { ss << "s"; }
        }

        if (times.days > 0)
        {
            ss << times.days << " day";
            if (times.days > 1) { ss << "s"; }
        }
    }
    else
    {
        auto days = times.days + times.weeks * 7;
        if (days > 0)
        {
            ss << days << " day";
            if (days > 1) { ss << "s"; }
        }
    }


    return ss.str();
}