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

namespace RCS {
	// Struct for Angles
	struct VectorAngles {
		float x, y, z;
	} VectorStructAngles;

	// Variables
	VectorAngles ViewPunchAngles;
	VectorAngles OldAimPunch;
	VectorAngles LocalPlayerViewAngles;
	VectorAngles ViewPunchAnglesOld;
	VectorAngles newViewAngles;
	DWORD ClientState;
	int ShotsFired;

	void RecoilControl(CMemoryManager* MemoryManager) {
		HANDLE m_hProcess = MemoryManager->GetHandle();

		// OUR VIEW ANGLES

		// Read the Client State
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwEngineBase + o_ClientState), &ClientState, sizeof(ClientState), 0);
		// Read the Client's Angles
		ReadProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles), &LocalPlayerViewAngles, sizeof(LocalPlayerViewAngles), 0);

		// Read our Local Player
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_LocalPlayer), &dwPlayerBase, sizeof(DWORD), 0);

		// Read the amount of shots our Player has shot
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwPlayerBase + 0xA2B0), &ShotsFired, sizeof(ShotsFired), 0);

		// Read our Local Player's View Punch angles
		ReadProcessMemory(m_hProcess, (PBYTE*)(dwPlayerBase + 0x301C), &ViewPunchAngles, sizeof(ViewPunchAngles), 0);

		// Create our new view angles
		newViewAngles.x = (LocalPlayerViewAngles.x - (ViewPunchAngles.x - ViewPunchAnglesOld.x) * 2);
		newViewAngles.y = (LocalPlayerViewAngles.y - (ViewPunchAngles.y - ViewPunchAnglesOld.y) * 2);

		// Check if we're holding mouse 1 (shooting)
		if (ShotsFired > 1) {
			// Write back with our new Y view angle
			WriteProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles + 0x4), &newViewAngles.y, sizeof(float), 0);
			// Write back with our new X view angle
			WriteProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles), &newViewAngles.x, sizeof(float), 0);
		}

		// Set the old values
		ViewPunchAnglesOld.x = ViewPunchAngles.x;
		ViewPunchAnglesOld.y = ViewPunchAngles.y;
	}
}