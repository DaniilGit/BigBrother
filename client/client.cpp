#include <winsock2.h>
#include <ShellAPI.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

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

	SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		cout << "Error: failed connect to server." << endl;
		return 0;
	}
	cout << "Connected to server!" << endl;
	char data[256] = "Hello!";
	char strExit[256] = "exit";

	while (1) {
		send(connection, data, sizeof(data), NULL);
		if (strcmp(data, strExit) == 0)
			break;
	}

	system("pause");
	return 0;
}