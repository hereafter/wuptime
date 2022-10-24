#include "pch.h"
#include "UptimeInfo.h"

#pragma region Constants
constexpr uint64_t kSecondsInWeeks = 7 * 24 * 3600;
constexpr uint64_t kSecondsInDays = 24 * 3600;
constexpr uint64_t kSecondsInHours = 3600;
constexpr uint64_t kSecondsInMinutes = 60;
#pragma endregion


UptimeInfo::UptimeInfo()
{
	auto ticks =GetTickCount64();
	uint64_t seconds = ticks / 1000ull;
	_milliseconds = ticks % 1000ull;
	_weeks = seconds / kSecondsInWeeks;
	_days = seconds % kSecondsInWeeks / kSecondsInDays;
	_hours = (seconds % kSecondsInDays) / kSecondsInHours;
	_minutes = (seconds % kSecondsInHours) / kSecondsInMinutes;
	_seconds = seconds % kSecondsInMinutes;
}