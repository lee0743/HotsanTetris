#pragma once

#include "TickCounter.h"

void InitTickCounter()
{
	startMilliseconds = GetTickCount64();
}

DWORD GetElaspedMillisecond()
{
	ULONGLONG endTimeMilliseconds = GetTickCount64();
	return endTimeMilliseconds - startMilliseconds;
}