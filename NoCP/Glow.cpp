#include <windows.h>
#include "stdafx.h"
#include "Hprocess.h"

// External Variables
extern DWORD dwClientBase;
extern DWORD o_LocalPlayer;
extern DWORD m_dwGlowObject;
extern DWORD m_dwEntityList;
extern DWORD m_iTeamNum;
extern bool GlowBool;

namespace Glow {
	// Variables
	DWORD GlowObject = 0;
	DWORD LocalPlayer = 0;
	DWORD Entity = 0;
	DWORD Health = 0;
	DWORD IsDormant = 0;
	DWORD GlowIndex = 0;
	DWORD TeamNum = 0;
	DWORD ClassId = 0;
	DWORD gBuffer = 0;
	DWORD LocalPlayerTeam = 0;
	DWORD dwEntityList = 0;
	int health;

	// GlowStruct
	struct GlowStruct {
		float red = 0;
		float green = 0;
		float blue = 0;
		float alpha = 0.75;
		byte padding[8];
		float buffer3 = 1;
		int buffer4 = 0;
		bool on = true;
	}GlowEntity;

	// Read the Local Player
	void ReadLocalPlayer(HANDLE m_hProcess) {
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_LocalPlayer), &LocalPlayer, sizeof(int), 0);
	}

	// Read the Glow Objects
	void ReadGlowObjects(HANDLE m_hProcess) {
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + m_dwGlowObject), &GlowObject, sizeof(int), 0);
	}

	// Read the Entity
	void ReadEntity(HANDLE m_hProcess, int i) {
		ReadProcessMemory(m_hProcess, (PBYTE*)(GlowObject + 0x38 * i), &Entity, sizeof(int), 0);
	}

	// Create a Buffer for the Entity
	void CreateBuffer(HANDLE m_hProcess) {
		ReadProcessMemory(m_hProcess, (PBYTE*)(Entity + 0x8), &gBuffer, sizeof(int), 0);
		ReadProcessMemory(m_hProcess, (PBYTE*)(gBuffer + 0x8), &gBuffer, sizeof(int), 0);
		ReadProcessMemory(m_hProcess, (PBYTE*)(gBuffer + 0x1), &gBuffer, sizeof(int), 0);
	}

	// Read the ClassId from the Entity
	void ReadClassId(HANDLE m_hProcess) {
		ReadProcessMemory(m_hProcess, (PBYTE*)(gBuffer + 0x14), &ClassId, sizeof(int), 0);
	}

	// Read the Team Number (Grab from the buffer we've created)
	void ReadTeamNumber(HANDLE m_hProcess) {
		ReadProcessMemory(m_hProcess, (PBYTE*)(Entity + m_iTeamNum), &TeamNum, sizeof(int), 0);
	}

	// Validate the Entity
	void ValidateEntity(HANDLE m_hProcess, int i) {
		// Make sure the Entity is a Player (35)
		if (ClassId == 35) {
			if (health == 100) {
				GlowEntity.red = 1;
				GlowEntity.blue = 0;
				GlowEntity.green = 0;
				WriteProcessMemory(m_hProcess, (PBYTE*)(GlowObject + 0x38 * i + 0x4), &GlowEntity, sizeof(GlowEntity), 0);
			}
			else if (health < 100 && health > 50) {
				GlowEntity.red = 1;
				GlowEntity.blue = 0;
				GlowEntity.green = 1;
				WriteProcessMemory(m_hProcess, (PBYTE*)(GlowObject + 0x38 * i + 0x4), &GlowEntity, sizeof(GlowEntity), 0);
			}
			else {
				GlowEntity.red = 1;
				GlowEntity.blue = 0;
				GlowEntity.green = 0;
				WriteProcessMemory(m_hProcess, (PBYTE*)(GlowObject + 0x38 * i + 0x4), &GlowEntity, sizeof(GlowEntity), 0);
			}
		}
	}

	// Read the Singular Entity
	void ReadSingularObject(HANDLE m_hProcess) {
		for (int i = 0; i < 200; i++) {

			ReadEntity(m_hProcess, i);

			CreateBuffer(m_hProcess);

			ReadClassId(m_hProcess);

			ReadTeamNumber(m_hProcess);

			ValidateEntity(m_hProcess, i);
		}
	}

	// Toggle Glow
	void ToggleGlow(CMemoryManager* MemoryManager) {
		// Wait for B Key Press
		if (GetAsyncKeyState(0x42) & 1) {
			GlowBool == false ? GlowBool = true : GlowBool = false;
		}

		if (GlowBool) {
			HANDLE m_hProcess = MemoryManager->GetHandle();

			ReadLocalPlayer(m_hProcess);

			ReadGlowObjects(m_hProcess);

			ReadSingularObject(m_hProcess);
		}
	}

	// Call all our Defined Functions
	void MakeGlow(CMemoryManager* MemoryManager) {
		ToggleGlow(MemoryManager);
	}
};