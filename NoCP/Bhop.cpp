#include <windows.h>
#include "stdafx.h"
#include "Hprocess.h"

// External DWORDS
extern DWORD dwClientBase;
extern DWORD o_LocalPlayer;
extern DWORD dwPlayerBase;
extern DWORD o_fFlags;
extern DWORD o_ForceJump;

namespace Bhop {
	void CreateJump(CMemoryManager* MemoryManager) {
		HANDLE m_hProcess = MemoryManager->GetHandle();
		int ForceJump;
		int GroundInfo;

		// Get the process memory of the local player
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_LocalPlayer), &dwPlayerBase, sizeof(int), 0);

		// Get the process memory of whether the player's on the ground or not
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwPlayerBase + o_fFlags), &GroundInfo, sizeof(int), 0);

		// If we're not on the ground and we're pressing space, jump
		if (GroundInfo == 257 && GetAsyncKeyState(VK_SPACE)) {
			// Set Force jump to 5 (jump)
			ForceJump = 5;
			WriteProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_ForceJump), &ForceJump, sizeof(int), 0);
		}
		// If we're not on the ground, not crounching and we're pressing space, we're in the air.
		else if (GroundInfo != 257 && GetAsyncKeyState(VK_SPACE) && GroundInfo != 263) {
			// Set Force jump to 4 (don't jump)
			ForceJump = 4;
			WriteProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_ForceJump), &ForceJump, sizeof(int), 0);

			//AutoStrafe(MemoryManager);

		}
		// If we're doing something other than jumping, don't spam space
		else {
			ForceJump = 4;
			WriteProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_ForceJump), &ForceJump, sizeof(int), 0);
		}
	}
};