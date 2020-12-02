#include "server.h"

int saveData(string data)
{
	ofstream file("result.txt", ios_base::app);

	if (!file.is_open()) {
		cout << "Error open file output." << endl;
		return -1;
	}

	file << data << endl;
	file.close();

	return 0;
}

int handlerServer(SOCKET connection)
{
	char data[1024];

	if (recv(connection, data, sizeof(data), NULL) == -1) {
		cout << "Error read data from client." << endl;
		return -1;
	}
	string strData = string(data);

	if (strData == "show")
		ShellExecuteA(NULL, "open", "result.txt", NULL, NULL, SW_RESTORE);
	else if (strData == "exit")
		return -1;
	else {
		if (saveData(strData) == -1)
			return -1;
	}

	return 0;
}
