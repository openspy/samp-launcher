#include "main.h"

#include "Gamespy/serverbrowsing/sb_serverbrowsing.h"

extern bool g_debugMode;

void do_launch_samp() {
	init_connect_dialog();
}

void launch_samp(const char* ip, SHORT port, const char *password) {
	// Prepare to create a new process.
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;

	memset(&ProcessInfo, 0, sizeof(PROCESS_INFORMATION));
	memset(&StartupInfo, 0, sizeof(STARTUPINFO));

	StartupInfo.cb = sizeof(StartupInfo);

	// Get the user's gta_sa location
	char exeLocation[MAX_PATH], name[MAX_SAMP_NAME];
	DWORD buffer = sizeof(exeLocation);

	// Open registry key
	HKEY hKey;
	long lError = RegOpenKeyEx(HKEY_CURRENT_USER,
		"Software\\SAMP",
		0,
		KEY_READ,
		&hKey);

	// Get value
	DWORD dwRet = RegQueryValueEx(hKey, "gta_sa_exe", NULL, NULL, (LPBYTE)&exeLocation, &buffer);

	// Make sure we got a good value for the gta_sa path
	if (dwRet != ERROR_SUCCESS)
	{
		MessageBoxA(hMainWindow, "Could not get the location of your GTA:SA installation. Is SA-MP installed correctly?", "SA:MP Launcher", MB_ICONERROR);
		RegCloseKey(hKey);
		return;
	}

	// remove \gta_sa.exe in a new variable (leaving just the directory path)
	char path[MAX_PATH];
	strcpy_s(path, sizeof(path), exeLocation);
	path[strlen(path) - 11] = '\0';

	// Get the player name
	buffer = sizeof(name);
	dwRet = RegQueryValueEx(hKey, "PlayerName", NULL, NULL, (LPBYTE)&name, &buffer);

	// Close registry
	RegCloseKey(hKey);

	if (dwRet != ERROR_SUCCESS)
	{
		MessageBoxA(hMainWindow, "Could not get Player Name. Set the name in your preferences.", "SA:MP Launcher", MB_ICONERROR);
		return;
	}

	char launchChar = 'c';
	if (g_debugMode) {
		launchChar = 'd';
	}
	char commandLine[128];
	// Construct it all in one command line string.
	if (password != NULL) {
		sprintf_s(commandLine, sizeof(commandLine), "-%c -h %s -p %d -n %s -z %s", launchChar, ip, port, name, password);
	}
	else {
		sprintf_s(commandLine, sizeof(commandLine), "-%c -h %s -p %d -n %s", launchChar, ip, port, name);
	}
	

	if (CreateProcess(exeLocation, commandLine, NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_SUSPENDED, NULL, path, &StartupInfo, &ProcessInfo))
	{
		// Create a new string that will hold the path to the file samp.dll
		char szWithSampdll[MAX_PATH] = "";
		int sampdll_path_len = sprintf_s(szWithSampdll, sizeof(szWithSampdll) - 1, "%s\\samp.dll", path);

		// Get the module handle to kernal32.dll
		HMODULE hKernel32Mod = GetModuleHandle("kernel32.dll");

		// Create address variable to hold the address of the LoadLibrary function.
		void* procAddress = NULL;

		// If it was a valid handle.
		if (hKernel32Mod)
			// Get the address of the LoadLibrary function so we can load samp.dll
			procAddress = (void*)GetProcAddress(hKernel32Mod, "LoadLibraryA");
		else
		{
			MessageBoxA(hMainWindow, "Could not find kernel32.dll", "SA:MP Launcher", MB_ICONERROR);
			return;
		}

		// Allocate memory in the new process we just created to store the string of the samp.dll file path.
		void* remoteProcessMemory = (void*)VirtualAllocEx(ProcessInfo.hProcess, NULL, sampdll_path_len + 1, MEM_COMMIT, PAGE_READWRITE);

		// Make sure the space was allocated.
		if (remoteProcessMemory != NULL) {
			// Write to the memory we just allocated the file path to samp.dll including directory.
			if (!WriteProcessMemory(ProcessInfo.hProcess, remoteProcessMemory, szWithSampdll, sampdll_path_len + 1, NULL)) {
				MessageBoxA(hMainWindow, "Could not find write GTA process memory", "SA:MP Launcher", MB_ICONERROR);
				return;
			}
		}
		else
		{
			// arg is null, and we can't continue then.
			// Let the user know there was a problem and exit.
			MessageBoxA(hMainWindow, "Memory could not be allocated to inject samp.dll", "SA:MP Launcher", MB_ICONERROR);
			return;
		}

		// Create new handle to our remote thread.
		HANDLE id = NULL;

		// Make sure The address of LoadLibrary isn't NULL
		if (procAddress != NULL)
		{
			// Create a remote thread that calls LoadLibrary, and as the parameter, the memory location we just wrote the samp.dll path to.
			// also don't execute this thread, but just create.
			id = CreateRemoteThread(ProcessInfo.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)procAddress, remoteProcessMemory, CREATE_SUSPENDED, NULL);
		}
		else
		{
			MessageBoxA(hMainWindow, "Could not find the address of LoadLibraryA", "SA:MP Launcher", MB_ICONERROR);
			return;
		}

		// Make sure id is a valid handle
		if (id)
		{
			// Resume the remote thread.
			ResumeThread(id);

			// Wait for the remote thread to finish executing.
			WaitForSingleObject(id, INFINITE);
		}
		else
		{
			MessageBoxA(hMainWindow, "the ID returned from CreateRemoteThread was invalid.", "SA:MP Launcher", MB_ICONERROR);
			return;
		}

		// Free the memory we just allocated that stores the samp.dll file path since LoadLibrary has been called and it's not needed anymore.
		VirtualFreeEx(ProcessInfo.hProcess, remoteProcessMemory, 0, MEM_RELEASE);

		// Resume the process (It was suspended, remember?)
		ResumeThread(ProcessInfo.hThread);

		// Close the handle to the process we created.
		CloseHandle(ProcessInfo.hProcess);

	}
}