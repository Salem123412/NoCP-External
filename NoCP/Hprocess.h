#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>

class CMemoryManager {
private:
	// Handle Process
	HANDLE m_hProcess;
	// Process Id
	DWORD m_dwProcessId;
	// All modules taken from the process
	std::vector<MODULEENTRY32> m_Modules;
public:
	// Attach Process Function
	bool Attach(const std::string& strProcessName) {
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (hSnapshot == INVALID_HANDLE_VALUE) {
			return false;
		}

		PROCESSENTRY32 ProcEntry;

		ProcEntry.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hSnapshot, &ProcEntry)) {
			// If strings are the same, close the Snapshot Handle and open Handle to the process, then return it
			if (!strcmp(ProcEntry.szExeFile, strProcessName.c_str())) {
				// Close the Snapshot Handle
				CloseHandle(hSnapshot);
				
				// Open a new handle and set the HANDLE m_hProcess to that process
				m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwProcessId);

				// Set the processId
				m_dwProcessId = ProcEntry.th32ProcessID;

				// Return
				return true;
			}
		}
		// If we're placed in here, there's no process with that name open
		else {
			// Close the Snapshot Handle
			CloseHandle(hSnapshot);

			// Return
			return false;
		}

		while (Process32Next(hSnapshot, &ProcEntry)) {
			// If strings are the same, close the Snapshot Handle and open Handle to the process, then return it
			if (!strcmp(ProcEntry.szExeFile, strProcessName.c_str())) {
				// Close the Snapshot Handle
				CloseHandle(hSnapshot);

				// Open a new handle and set the HANDLE m_hProcess to that process
				m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwProcessId);

				// Set the processId
				m_dwProcessId = ProcEntry.th32ProcessID;

				// Return
				return true;
			}
		}

		// If we've been placed here, there are no more processes to scan, therefore we'll stop the program by returning false
		CloseHandle(hSnapshot);
		// Return
		return false;
	};

	// Grab Module Function (true/false)
	bool GetModule(const std::string& strModuleName) {
		// Define the Snapshot Handle
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);

		// If the module is invalid, return false
		if (hSnapshot == INVALID_HANDLE_VALUE) {
			return false;
		}

		// Define the Module Entry
		MODULEENTRY32 ModEntry;

		// ModuleEntry size == size of ModuleEntry32
		ModEntry.dwSize = sizeof(MODULEENTRY32);

		// Call Module32First with our params
		if (Module32First(hSnapshot, &ModEntry)) {
			// Check if they match
			if (!strcmp(ModEntry.szModule, strModuleName.c_str())) {
				// Close the Snapshot
				CloseHandle(hSnapshot);

				// Add Module Entries to the m_Modules vector
				m_Modules.push_back(ModEntry);

				// Return
				return true;
			}
		}
		// If there aren't any processes running in the first place, cancel the whole operation straight away
		else {
			// Close the Snapshot handle
			CloseHandle(hSnapshot);

			// Return
			return false;
		}

		// While there are multiple results, loop through them all
		while (Module32Next(hSnapshot, &ModEntry)) {
			// Check if they match
			if (!strcmp(ModEntry.szModule, strModuleName.c_str())) {
				// Close the Snapshot
				CloseHandle(hSnapshot);

				// Add the Module Entries to the m_Modules vector
				m_Modules.push_back(ModEntry);

				// Return
				return true;
			}
		}

		// If we're here, there aren't any more handles, therefore, close the Snapshot
		CloseHandle(hSnapshot);

		// Return
		return false;
	}

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
			std::cout << strProcessName.c_str() << " isn't a running process!";

			// Sleep for 1 second
			Sleep(1000);

			// Exit the program
			exit(0);
		}

		// If we're here, there were not errors and we're successfully hooked
		std::cout << strProcessName.c_str() << " has been successfully hooked!";
	}

	// Getters
	HANDLE GetHandle() {
		return m_hProcess;
	}

	DWORD GetProcId() {
		return m_dwProcessId;
	}

	std::vector<MODULEENTRY32> GetModules() {
		return m_Modules;
	}
};