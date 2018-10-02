// 4 -> No action, 5 -> Hold down action, 6 -> single action

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

void AutoStrafe(CMemoryManager* MemoryManager) {
	HANDLE m_hProcess = MemoryManager->GetHandle();
	// Moving Left
	int moveLeft;

	ReadProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_LocalPlayer), &dwPlayerBase, sizeof(int), 0);
	ReadProcessMemory(m_hProcess, (PBYTE*)(dwPlayerBase + o_ForceMoveLeft), &moveLeft, sizeof(int), 0);



	// VIEW ANGLES
	// Get the process memory of the local player
	ReadProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_LocalPlayer), &dwPlayerBase, sizeof(int), 0);
	DWORD ClientState;
	ReadProcessMemory(m_hProcess, (PBYTE*)(dwEngineBase + o_ClientState), &ClientState, sizeof(int), 0);
	// Grab the views angles from the ClientState
	ReadProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles), &viewAngles, sizeof(viewAngles), 0);

	Clamp();

	WriteProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles), &viewAngles, sizeof(viewAngles), 0);

	Clamp();

	WriteProcessMemory(m_hProcess, (PBYTE*)(ClientState + o_ClientStateViewAngles), &viewAngles, sizeof(viewAngles), 0);
	
	std::cout << viewAngles.y << std::endl;

	// END VIEW ANGLES




	moveLeft = 5;
	WriteProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_ForceMoveLeft), &moveLeft, sizeof(int), 0);

	moveLeft = 4;
	WriteProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_ForceMoveLeft), &moveLeft, sizeof(int), 0);

	Sleep(2);

	// Moving Right
	int moveRight;

	ReadProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_LocalPlayer), &dwPlayerBase, sizeof(int), 0);
	ReadProcessMemory(m_hProcess, (PBYTE*)(dwPlayerBase + o_ForceMoveRight), &moveRight, sizeof(int), 0);

	moveRight = 5;
	WriteProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_ForceMoveRight), &moveRight, sizeof(int), 0);

	moveRight = 4;
	WriteProcessMemory(m_hProcess, (PBYTE*)(dwClientBase + o_ForceMoveRight), &moveRight, sizeof(int), 0);
}