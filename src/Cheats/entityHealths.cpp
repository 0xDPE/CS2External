#include "entityHealths.h"

#include "../Offsets/offsets.h"
#include "../Offsets/client.h"

#include "../WinApiTomFoolery/driver.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

uintptr_t entityHealths::getEntityList(uintptr_t modBaseAddr)
{
	entityListLoc = (modBaseAddr + cs2_dumper::offsets::client_dll::dwEntityList);
    std::cout << std::hex << entityListLoc << std::endl;
	return { modBaseAddr + cs2_dumper::offsets::client_dll::dwEntityList };
}

void entityHealths::entityLoop()
{
	driver Driver;

    for (int i = 0; i < 64; i++)
    {
        uintptr_t Entity = Driver.read<uintptr_t>(entityListLoc);
        if (Entity == 0) {
            std::cerr << "Failed to read entity at index " << i << std::endl;
            continue;
        }

        uintptr_t listEntity = Driver.read<uintptr_t>(Entity + ((8 * (i & 0x7FFF) >> 9) + 16));
        if (listEntity == 0) {
            std::cerr << "Failed to read listEntity at index " << i << std::endl;
            continue;
        }

        uintptr_t entityController = Driver.read<uintptr_t>(listEntity + (120) * (i & 0x1FF));
        if (entityController == 0) {
            std::cerr << "Failed to read entityController at index " << i << std::endl;
            continue;
        }

        uintptr_t entityControllerPawn = Driver.read<uintptr_t>(entityController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
        if (entityControllerPawn == 0) {
            std::cerr << "Failed to read entityControllerPawn at index " << i << std::endl;
            continue;
        }

        listEntity = Driver.read<uintptr_t>(Entity + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16));
        if (listEntity == 0) {
            std::cerr << "Failed to read listEntity (second read) at index " << i << std::endl;
            continue;
        }

        uintptr_t entityPawn = Driver.read<uintptr_t>(listEntity + (120) * (entityControllerPawn & 0x1FF));
        if (entityPawn == 0) {
            std::cerr << "Failed to read entityPawn at index " << i << std::endl;
            continue;
        }

        playerTeam = Driver.read<int>(entityPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
        playerHealth = Driver.read<int>(entityPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth);

        if (playerHealth < 0 || playerHealth > 100) {
            std::cerr << "Unreasonable health value: " << playerHealth << std::endl;
            continue;
        }

        uintptr_t entityNameAddress = Driver.read<uintptr_t>(entityController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName);

        entityName = Driver.read<char>(entityNameAddress);

        std::cout << playerHealth << std::endl;

    }
}
