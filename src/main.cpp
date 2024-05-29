#include "Cheats/entityHealths.h"
#include "WinApiTomFoolery/driver.h"

int main()
{
	entityHealths cheat;
	driver Driver;

	DWORD procId = Driver.getPid("cs2.exe");
	std::cout << procId << std::endl;
	uintptr_t modBaseAddr = Driver.getModuleBaseAddress(procId, "client.dll");
	std::cout << std::hex << modBaseAddr << std::endl;

	if (Driver.getHandle(NULL) == 0)
	{
		std::cout << "wtf" << std::endl;
	}
	else
	{
		std::cout << ":)" << std::endl;
	}

	cheat.getEntityList(modBaseAddr);
	cheat.entityLoop();


	return -1;
}