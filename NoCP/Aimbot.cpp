#include <windows.h>
#include "stdafx.h"
#include "Hprocess.h"

// External Variables
extern DWORD dwClientBase;
extern DWORD o_LocalPlayer;
extern DWORD m_dwGlowObject;
extern DWORD m_iTeamNum;
extern DWORD dwPlayerBase;
extern DWORD m_iCrosshairId;
extern DWORD m_vecOrigin;
extern DWORD m_dwEntityList;
extern DWORD dwEngineBase;
extern DWORD o_ClientState;
extern DWORD o_ClientStateViewAngles;
extern DWORD m_vecViewOffset;
extern DWORD m_iTeamNum;

namespace Aimbot {
	// Struct for Angles
	struct VectorAngles {
		float x, y, z;
	}VectorStructAngles;

	// Variables
	VectorAngles LocalPlayerPosition;
	VectorAngles EnemyPosition;
	VectorAngles LocalPlayerViewAngles;
	VectorAngles AimAngles;
	VectorAngles EnemyHeadPosition;
	VectorAngles LocalPlayerPositionEyes;
	DWORD EnemyBoneMatrix;
	DWORD dwEntityList;
	DWORD dwCrosshairId;
	DWORD ClientState;
	float EnemyPositionEyes;
	float EnemyHeadBoneX;
	float EnemyHeadBoneY;
	float EnemyHeadBoneZ;
	float newPunchAngleX;
	float newPunchAngleY;
	int health;
	int ShotsFired;
	int TeamNum;
	int ClassId;

	// Prevent invalid angles being sent
	void Clamp() {
		// These are actually x values
		if (AimAngles.y > 89.f) {
			AimAngles.y -= 178.f;
		}
		else if (AimAngles.y < -89.f) {
			AimAngles.y += 178.f;
		}

		// These are actually y values
		if (AimAngles.x > 180.f) {
			AimAngles.x -= 360.f;
		}
		else if (AimAngles.x < -180.f) {
			AimAngles.x += 360.f;
		}

		AimAngles.z = 0;
	}

	// Vector of our new Aim Assited View Angles
	VectorAngles CalculateAngle(VectorAngles source, VectorAngles direction) {
		VectorAngles angle;
		VectorAngles newAngle;

		angle.x = direction.x - source.x;
		angle.y = direction.y - source.y;
		angle.z = direction.z - source.z;

		float magnitude = std::sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z);

		newAngle.x = std::atan2(angle.y, angle.x) * 180 / 3.14;
		newAngle.y = std::atan2(angle.z, magnitude) * 180 / 3.14;
		newAngle.z = 0;
		return newAngle;
	}

	// Read our Local Player
	void ReadLocalPlayer(HANDLE m_hProcess) {
		// Read our Local Player
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_LocalPlayer), &dwPlayerBase, sizeof(DWORD), 0);
		// Read our Crosshair position
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwPlayerBase + m_iCrosshairId), &dwCrosshairId, sizeof(DWORD), 0);
		// Read our Position
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwPlayerBase + m_vecOrigin), &LocalPlayerPosition, sizeof(LocalPlayerPosition), 0);
		// Read our Local Player Head Position
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwPlayerBase + m_vecViewOffset), &LocalPlayerPositionEyes, sizeof(LocalPlayerPositionEyes), 0);
	}

	// Read all our Local Entities and process them accordingly
	void ReadEntity(HANDLE m_hProcess) {
		// Loop through all Entities (0 is me, so ignore me)
		for (int i = 1; i < 64; i++) {
			// Read each entity
			ReadProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + m_dwEntityList + (i * 0x10)), &dwEntityList, sizeof(dwEntityList), 0);
			ReadProcessMemory(m_hProcess, (PBYTE*)(dwEntityList + 0xFC), &health, sizeof(health), 0);
			ReadProcessMemory(m_hProcess, (PBYTE*)(dwEntityList + m_iTeamNum), &TeamNum, sizeof(TeamNum), 0);		

			// WE HAVE A PLAYER :)
			if (health > 0 && health <= 100) {
				// If the entity !=0, it's a PLAYER! (weird bug if we don't do this check)
				if (dwEntityList != 0 && TeamNum != 2) {
					// Read the EnemyBoneMatrix
					ReadProcessMemory(m_hProcess, (PBYTE*)(dwEntityList + 0x2698), &EnemyBoneMatrix, sizeof(EnemyBoneMatrix), 0);
					// Read X
					ReadProcessMemory(m_hProcess, (PBYTE*)(EnemyBoneMatrix + 0x30 * 8 + 0x0C), &EnemyHeadBoneX, sizeof(EnemyHeadBoneX), 0);
					// Read Y
					ReadProcessMemory(m_hProcess, (PBYTE*)(EnemyBoneMatrix + 0x30 * 8 + 0x1C), &EnemyHeadBoneY, sizeof(EnemyHeadBoneY), 0);
					// Read Z
					ReadProcessMemory(m_hProcess, (PBYTE*)(EnemyBoneMatrix + 0x30 * 8 + 0x2C), &EnemyHeadBoneZ, sizeof(EnemyHeadBoneZ), 0);

					// Set the Vector
					EnemyHeadPosition.x = EnemyHeadBoneX;
					EnemyHeadPosition.y = EnemyHeadBoneY;
					EnemyHeadPosition.z = -EnemyHeadBoneZ + LocalPlayerPositionEyes.z;
					break;
				}
			}
		}
	}

	// Read from our Local Engine
	void ReadEngine(HANDLE m_hProcess) {
		// Read the Client State
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwEngineBase + o_ClientState), &ClientState, sizeof(ClientState), 0);
		// Read the Client's Angles
		ReadProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles), &LocalPlayerViewAngles, sizeof(LocalPlayerViewAngles), 0);
	}

	// Call the CalculateAngle Function
	void CalculateNewAngles() {
		AimAngles = CalculateAngle(LocalPlayerPosition, EnemyHeadPosition);
	}

	// Check if we're holding (mouse2)
	void TriggerAimBot(HANDLE m_hProcess) {
		if (GetAsyncKeyState(0x05)) {
			// Clamp our angles to prevent invalid angles being sent to the server
			Clamp();
			WriteProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles), &AimAngles.y, sizeof(float), 0);
			WriteProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles + 0x4), &AimAngles.x, sizeof(float), 0);
		}
	}

	// Call All Functions
	void CallAimFunctions(HANDLE m_hProcess) {
		// Read our Local Player Addresses
		ReadLocalPlayer(m_hProcess);
		// Read our Local Entity Addresses
		ReadEntity(m_hProcess);
		// Read our Local Engine Addresses
		ReadEngine(m_hProcess);
		// Call our CalculateAngle Function
		CalculateNewAngles();
		// Trigger the AimBot Function when pressing a key
		TriggerAimBot(m_hProcess);
	}

	// Create the Handle and Trigger Functions
	void Aimbot(CMemoryManager* MemoryManager) {
		HANDLE m_hProcess = MemoryManager->GetHandle();
		CallAimFunctions(m_hProcess);
	}
};