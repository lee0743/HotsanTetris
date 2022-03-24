#pragma once

#include <Windows.h>

void InitTickCounter(ULONGLONG* startMilliseconds);
DWORD GetElaspedMillisecond(ULONGLONG* startMilliseconds);