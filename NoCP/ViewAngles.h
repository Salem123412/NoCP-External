#pragma once

// Define viewAngles as a Vector
Vector viewAngles;

void Clamp() {
	viewAngles.z = 0;

	// If y >= 180, set angle to 180
	if (viewAngles.x >= 89) {
		viewAngles.x = 89;
	}
	// If y <= -180, set angle to -180
	else if (viewAngles.x <= -89) {
		viewAngles.x = -89;
	}

	// If y >= 180, set angle to 180
	if (viewAngles.y >= 180) {
		viewAngles.y = 180;
	}
	// If y <= -180, set angle to -180
	else if (viewAngles.y <= -180) {
		viewAngles.y = -180;
	}
}

void ViewAngles(CMemoryManager* MemoryManager) {
	// Handler
	HANDLE m_hProcess = MemoryManager->GetHandle();

	// Get the process memory of the local player
	ReadProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_LocalPlayer), &dwPlayerBase, sizeof(int), 0);

	// Define ClientState as a DWORD
	DWORD ClientState;
	// Grab the ClientState (pointed value)
	ReadProcessMemory(m_hProcess, (PBYTE*)(dwEngineBase + o_ClientState), &ClientState, sizeof(int), 0);
	// Grab the views angles from the ClientState
	ReadProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles), &viewAngles, sizeof(viewAngles), 0);

	// X -> MAX ANGLE -> 89, X -> MIN ANGLE -> -89
	// Y -> MAX ANGLE -> 180, Y -> MIN ANGLE -> -180
	Clamp();

	//WriteProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles), &viewAngles, sizeof(viewAngles), 0);

	std::cout << viewAngles.x << "," << viewAngles.y << "," << viewAngles.z << std::endl;
}
