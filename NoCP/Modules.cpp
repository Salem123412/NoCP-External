#include <windows.h>
#include "stdafx.h"
#include "Hprocess.h"
#include "Offsets.h"

namespace Modules {
	void GetModules(CMemoryManager* MemoryManager) {
		// Manage the error of not finding the client.dll
		if (!MemoryManager->GetModule("client.dll")) {
			std::cout << "Client Module not found!" << std::endl;

			// Sleep for 1 second
			Sleep(1000);

			// Exit the program
			exit(0);
		}

		// Manage the error of not finding the client.dll
		if (!MemoryManager->GetModule("engine.dll")) {
			std::cout << "Engine Module not found!" << std::endl;

			// Sleep for 1 second
			Sleep(1000);

			// Exit the program
			exit(0);
		}

		// Loop through all the modules
		for (auto m : MemoryManager->GetModules()) {
			// check if client.dll matches one of the modules
			if (!strcmp(m.szModule, "client.dll")) {
				// Set dwClientBase to the current client
				dwClientBase = reinterpret_cast<DWORD>(m.modBaseAddr);
			}
			// check if engine.dll matches one of the modules
			if (!strcmp(m.szModule, "engine.dll")) {
				// Set dwClientBase to the current client
				dwEngineBase = reinterpret_cast<DWORD>(m.modBaseAddr);
				break;
			}
		}
	}
}
