#pragma once

using namespace std;

class UptimeInfo
{
public:
    UptimeInfo();

public:
	wstring CurrentTime() const;
	wstring StartDateTime() const;
	wstring Uptime() const;
	wstring PrettyUptime() const;

private:
    int _decades;
    int _years;
    int _weeks;
    int _days;
    int _daysTotal;
    int _hours;
    int _minutes;
    int _seconds;
    int _milliseconds;

    SYSTEMTIME _stamp;
    SYSTEMTIME _start;

};

