#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class entityHealths
{
public:
	uintptr_t getEntityList(uintptr_t modBaseAddr);
	void entityLoop();

	uintptr_t entityListLoc;
	int playerTeam;
	int playerHealth;
	char entityName;
};

