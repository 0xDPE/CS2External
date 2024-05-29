#include "driver.h"

#include <tlhelp32.h>

DWORD driver::getPid(const std::string& processName)
{
	DWORD pidWithHighestThreadCount = 0;
	DWORD highestThreadCount = 0;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnap, &pe);

	while (Process32Next(hSnap, &pe))
	{
		if ((strcmp(pe.szExeFile, processName.c_str()) == 0) && (pe.cntThreads > highestThreadCount))
		{
			highestThreadCount = pe.cntThreads;
			pidWithHighestThreadCount = pe.th32ProcessID;
		}
	}

	pID = pidWithHighestThreadCount;
	return pidWithHighestThreadCount;
}


uintptr_t driver::getModuleBaseAddress(uintptr_t procID, const std ::string& module)
{
	HANDLE handle;
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

	MODULEENTRY32 mod;
	mod.dwSize = sizeof(MODULEENTRY32);
	Module32First(handle, &mod);

	do
	{
		if (strcmp(module.c_str(), mod.szModule) == 0)
		{
			CloseHandle(handle);
			return (uintptr_t)mod.modBaseAddr;
		}
	} while (Module32Next(handle, &mod));

	CloseHandle(handle);
	return 0;
}

HANDLE driver::getHandle(DWORD procId)
{
	if (procId == NULL)
		procId = pID;

	HANDLE Handle = OpenProcess(PROCESS_VM_READ, TRUE, procId);
	if (Handle == NULL)
		return 0;
	handle = Handle;

	return Handle;
}