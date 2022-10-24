#include "pch.h"
#include "Uptime.h"


#pragma region Constants
constexpr uint64_t kSecondsInMinutes = 60;
constexpr uint64_t kSecondsInHours = 60* kSecondsInMinutes;
constexpr uint64_t kSecondsInDays = 24 * kSecondsInHours;
constexpr uint64_t kSecondsInWeeks = 7 * kSecondsInDays;
constexpr uint64_t kSecondsInYears = 365 * kSecondsInDays;
constexpr uint64_t kSecondsInDecades = 10 * kSecondsInYears;
#pragma endregion

using namespace std;
using namespace std::chrono;

UptimeInfo::UptimeInfo()
{

	SYSTEMTIME st{ 0 }, st2{ 0 };
	FILETIME ft = { 0 };
	::GetLocalTime(&st);

	auto ticks =GetTickCount64();
	uint64_t seconds = ticks / 1000ull;
	_milliseconds = static_cast<int>(ticks % 1000ull);
	_decades = static_cast<int>(seconds / kSecondsInDecades);
	_years = static_cast<int>(seconds / kSecondsInYears) % 10;
	_weeks = static_cast<int>(seconds / kSecondsInWeeks) % 52;
	_days = static_cast<int>(seconds / kSecondsInDays) % 7;
	_daysTotal = static_cast<int>(seconds / kSecondsInDays);
	_hours = static_cast<int>((seconds / kSecondsInHours) % 24);
	_minutes = static_cast<int>((seconds / kSecondsInMinutes) % 60);
	_seconds = static_cast<int>(seconds % 60);


	SystemTimeToFileTime(&st, &ft);
	_stamp = st;

	ULARGE_INTEGER tmp;
	tmp.LowPart = ft.dwLowDateTime;
	tmp.HighPart = ft.dwHighDateTime;

	tmp.QuadPart -= ticks*10000; //100ns
	ft.dwLowDateTime = tmp.LowPart;
	ft.dwHighDateTime = tmp.HighPart;

	FileTimeToSystemTime(&ft, &st2);
	_start = st2;
}


wstring UptimeInfo::CurrentTime() const
{
	wstringstream ss; 
	ss << setfill(L'0');
	ss << setw(2) << _stamp.wHour << ":" 
		<< setw(2) << _stamp.wMinute << ":" 
		<< setw(2) << _stamp.wSecond;
	return ss.str();
}

wstring UptimeInfo::StartDateTime() const
{
	wstringstream ss; 
	ss << setfill(L'0');
	auto&& t = _start;
	ss << t.wYear;
	ss << "-" 
		<< setw(2) << t.wMonth << "-" 
		<< setw(2) << t.wDay;
	ss << " ";
	ss << setw(2) << t.wHour << ":" 
		<< setw(2) << t.wMinute << ":" 
		<< setw(2) << t.wSecond;
	return ss.str();
}

wstring UptimeInfo::Uptime() const
{
	wstringstream ss;
	ss << setfill(L'0');
	ss << this->CurrentTime().c_str() << " up ";

	if (_daysTotal > 0)
	{
		ss << _daysTotal << (_daysTotal > 1 ? " days, " : " day, ");
	}

	if (_hours > 0)
	{
		ss << _hours << ":" << setw(2) << _minutes;
	}
	else
	{
		ss << _minutes << " min";
	}

	return ss.str();
}

wstring UptimeInfo::PrettyUptime() const
{
	wstringstream ss;
	ss << "up ";

	int values = 0;
	if (_decades > 0)
	{
		ss << _decades << (_decades > 1?" decades":" decade");
		values++;
	}

	if (_weeks > 0)
	{
		if (values > 0) ss << ", ";
		ss << _weeks << (_weeks > 1 ? " weeks" : " week");
		values++;
	}

	if (_days > 0)
	{
		if (values > 0) ss << ", ";
		ss << _days << (_days > 1 ? " days" : " day");
		values++;
	}

	if (_hours > 0)
	{
		if (values > 0) ss << ", ";
		ss << _hours << (_hours > 1 ? " hours" : " hour");
		values++;
	}

	if (_minutes > 0)
	{
		if (values > 0) ss << ", ";
		ss << _minutes << (_minutes > 1 ? " minutes" : " minute");
		values++;
	}

	if (values == 0)
	{
		ss << "seconds";
	}
	return ss.str();
}