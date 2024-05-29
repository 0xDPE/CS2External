#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

class driver
{
public:
	DWORD getPid(const std::string& processName);
	uintptr_t getModuleBaseAddress(uintptr_t procID, const std::string& module);
	template <typename T> T read(uintptr_t address)
	{
		T ret;
		ReadProcessMemory(handle, (LPCVOID)address, &ret, sizeof(T), NULL);
		return ret;
	}
	
	HANDLE getHandle(DWORD procId);

	DWORD pID;
	BYTE* ModuleBaseAddr;

	HANDLE handle;
};

