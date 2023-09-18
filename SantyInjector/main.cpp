#include <Windows.h>
#include <string>
#include <conio.h>
#include <iostream>

HANDLE hProcess;
HANDLE hThread;
DWORD pID;

std::string chooseDLL() {
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	if (GetOpenFileName(&ofn)) {
		return fileName;
	}
}

int main() {

	std::cout << "[Info] This is Genshin Impact Injector. Press enter to choose your DLL" << std::endl;
	_getch();

	std::string stringpath = chooseDLL();
	const char* dllPath = stringpath.c_str();
	std::cout << "[Info] You Chose " << dllPath << std::endl;
	Sleep(1000);
	std::cout << "[Info] Press Enter to Inject your Dll" << std::endl;
	_getch();

	GetWindowThreadProcessId(FindWindowA(NULL, "Genshin Impact"), &pID);

	if (!pID) {

		std::cout << "[Error] Genshin Impact process not found" << std::endl;
		return 0;
	}

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pID);
	if (hProcess == INVALID_HANDLE_VALUE) {
		return 0;
	}

	void* allocmemory = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(hProcess, allocmemory, dllPath, MAX_PATH, 0);

	hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, allocmemory, 0, 0);

	if (hThread) {
		std::cout << "[+] Inject Successfulled. " << std::endl;
		VirtualFreeEx(hProcess, allocmemory, NULL, MEM_RELEASE);
		CloseHandle(hThread);
		CloseHandle(hProcess);
	}

	return 0;
}