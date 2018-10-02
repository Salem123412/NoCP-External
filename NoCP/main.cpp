#pragma once
#include <Windows.h>
#include "stdafx.h"
#include "Hprocess.h"
#include "Modules.h"
#include "main.h"
#include "Bhop.h"
#include "Aimbot.h"
#include "Glow.h"
#include "CloseProgram.h"
#include "RCS.h"

int main () {
	CMemoryManager* MemoryManager = new CMemoryManager("csgo.exe");

	Modules::GetModules(MemoryManager);

	while (true) {
		Bhop::CreateJump(MemoryManager);

		Aimbot::Aimbot(MemoryManager);

		Glow::MakeGlow(MemoryManager);

		RCS::RecoilControl(MemoryManager);

		CloseProgram::Exit();

		Sleep(5);
	}
}
