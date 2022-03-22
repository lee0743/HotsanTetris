#pragma once

#include <Windows.h>

static ULONGLONG startMilliseconds;

void InitTickCounter();
DWORD GetElaspedMillisecond();