#pragma once
#include "Hprocess.h"

using namespace CMemoryManager;

// Call this Function with our actual information
CMemoryManager(const std::string& strProcessName) {
	// Initialise Members and make sure they're all empty BEFORE we attempt the attach
	m_hProcess = INVALID_HANDLE_VALUE;
	m_dwProcessId = 0;
	m_Modules.clear();

	Attach(strProcessName);

	// Check if the attach process failed
	if (!Attach(strProcessName)) {
		// If it did, throw an error
		throw;
	}

	// If we're here, there were not errors and we're successfully hooked
	std::cout << strProcessName.c_str();
	std::cout << " was Successfully Hooked!";

	// Read The Memory
	template <class T>
	inline bool Read(DWORD dwAddress, T& Value) {
		return ReadProcessMemory(m_hProcess, reinterpret_cast<LPVOID>(dwAddress), Value, sizeof(T), NULL) == TRUE;
	}

	// Write to the Memory
	template <class T>
	inline bool Write(DWORD dwAddress, T& Value) {
		return WriteProcessMemory(m_hProcess, reinterpret_cast<LPVOID>(dwAddress), Value, sizeof(T), NULL) == TRUE;
	}
}