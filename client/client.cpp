#include <winsock2.h>
#include <ShellAPI.h>
#include <iostream>
#include "Interceptor.h"

using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

constexpr auto FILE_PATH = "log.txt";

SOCKET connection;

DWORD WINAPI hookJob(LPVOID lpParm);

int main(int argc, char* argv[]) {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error load lib" << endl;
		exit(1);
	}

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
	char data[256] = "Hello!";
	char strExit[256] = "exit";

	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hookJob, NULL, NULL, NULL);
	if (hThread)
		while (true)
		{
			;
		}
	else
		exit(EXIT_FAILURE);
	
// 	while (1) {
// 		send(connection, data, sizeof(data), NULL);
// 		if (strcmp(data, strExit) == 0)
// 			break;
// 	}

	system("pause");
	return 0;
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
