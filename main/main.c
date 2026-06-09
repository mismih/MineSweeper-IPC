#include <windows.h>
#include <stdio.h>

int main() {
	STARTUPINFO siServer = { sizeof(siServer) };
	PROCESS_INFORMATION piServer;
	STARTUPINFO siClient = { sizeof(siClient) };
	PROCESS_INFORMATION piClient;

	BOOL serverUspesno = CreateProcess(
		L"projekti\\MS_Generator.exe",
		NULL, NULL, NULL, FALSE,
		CREATE_NO_WINDOW,
		NULL,
		L"projekti",
		&siServer, &piServer
	);

	if (!serverUspesno) {
		printf("Greska: Ne mogu da pokrenem server iz foldera 'projekti'\n");
		return -1;
	}

	Sleep(500);

	BOOL klijentUspesno = CreateProcess(
		L"projekti\\MS_Client.exe",
		NULL, NULL, NULL, FALSE,
		0, NULL,
		L"projekti",
		&siClient, &piClient
	);

	if (!klijentUspesno) {
		printf("Greska: Ne mogu da pokrenem klijent iz foldera 'projekti'\n");
		TerminateProcess(piServer.hProcess, 0);
		return -1;
	}

	CloseHandle(piServer.hProcess);
	CloseHandle(piServer.hThread);
	CloseHandle(piClient.hProcess);
	CloseHandle(piClient.hThread);

	return 0;
}