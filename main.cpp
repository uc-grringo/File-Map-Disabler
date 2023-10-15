#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

std::vector<char*> AttackList =
{
	(char*)"Global\\EasyAntiCheat_EOSBin",
	(char*)"FrapsMQ"
};

bool IsRunAsAdmin() {
	BOOL fIsRunAsAdmin = FALSE;
	DWORD dwError = ERROR_SUCCESS;
	PSID pAdministratorsGroup = NULL;

	// Allocate and initialize a SID of the administrators group.
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if (!AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdministratorsGroup)) {
		dwError = GetLastError();
	}
	else {
		if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin)) {
			dwError = GetLastError();
		}
		FreeSid(pAdministratorsGroup);
	}

	if (dwError != ERROR_SUCCESS) {
		std::cout << "CheckTokenMembership failed with error code " << dwError << std::endl;
	}

	return (fIsRunAsAdmin != 0);
}

inline int performAttack(const char* Target) 
{
	HANDLE MappingHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, Target);
	if (MappingHandle == NULL)
	{
		MessageBoxA(0, "Could not open file mapping object", "ERROR", MB_OK | MB_ICONHAND);
		return -1;
	}

	LPVOID Handle = MapViewOfFile(MappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (Handle == NULL)
	{
		MessageBoxA(0, "Could not map view of file", "ERROR", MB_OK | MB_ICONHAND);
		CloseHandle(MappingHandle);
		return -1;
	}

	char* data = static_cast<char*>(Handle);
	int dataSize = 100; // Adjust the size as needed
	char* emptyData = new char[dataSize]; // Create an empty buffer

	// Clear the shared memory using CopyMemory
	CopyMemory(data, emptyData, dataSize);

	delete[] emptyData; // Clean up the buffer

	UnmapViewOfFile(Handle);
	CloseHandle(MappingHandle);

	return 1;
}

int main() 
{
	ShowWindow(GetConsoleWindow(), 0);

	if (IsRunAsAdmin() == NULL)
	{
		MessageBoxA(0, "START AS ADMINISTRATOR", "ERROR", MB_OK | MB_ICONHAND);
		return -1;
	}

	for (auto Target : AttackList)
	{
		int result = performAttack(Target);
		if (result < 1)
		{
			auto text = std::string("Failed destroying: ").append(std::string(Target)).c_str();
			MessageBoxA(0, text, "ERROR", MB_OK | MB_ICONHAND);
		}
	}

	MessageBoxA(0, "COMMUNICATION GOT CUT OFF", "SUCCESS", MB_OK | MB_ICONASTERISK);
	return 0;
}
