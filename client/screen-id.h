#pragma once

#ifndef SCREENID_H
#define SCREENID_H

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <cstring>
#include <iostream>
#include <time.h>
#include <thread>

DWORD WINAPI do_screen(LPVOID lpParam);
void x(int count);
BOOL SaveBitmap(HDC hDC, HBITMAP hBitmap, int count);
int GetHDD();

#endif 

