#pragma once

#include "TickCounter.h"

void InitTickCounter(ULONGLONG* startMilliseconds)
{
	*startMilliseconds = GetTickCount64();
}

DWORD GetElaspedMillisecond(ULONGLONG* startMilliseconds)
{
	ULONGLONG endTimeMilliseconds = GetTickCount64();
	return endTimeMilliseconds - *startMilliseconds;
}