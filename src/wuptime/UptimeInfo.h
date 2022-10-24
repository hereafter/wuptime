#pragma once

class UptimeInfo
{
public:
    UptimeInfo();

private:
    int _weeks;
    int _days;
    int _hours;
    int _minutes;
    int _seconds;
    int _milliseconds;
};

