#include "pch.h"
#include "UptimeInfo.h"


#pragma region Constants
constexpr uint64_t kSecondsInWeeks = 7 * 24 * 3600;
constexpr uint64_t kSecondsInDays = 24 * 3600;
constexpr uint64_t kSecondsInHours = 3600;
constexpr uint64_t kSecondsInMinutes = 60;
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
	_weeks = static_cast<int>(seconds / kSecondsInWeeks);
	_days = static_cast<int>(seconds % kSecondsInWeeks / kSecondsInDays);
	_hours = static_cast<int>((seconds % kSecondsInDays) / kSecondsInHours);
	_minutes = static_cast<int>((seconds % kSecondsInHours) / kSecondsInMinutes);
	_seconds = static_cast<int>(seconds % kSecondsInMinutes);

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