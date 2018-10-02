#include <Windows.h>
#include "stdafx.h"

namespace CloseProgram {
	void Exit() {
		if (GetAsyncKeyState(0x43) && GetAsyncKeyState(VK_CONTROL)) {
			exit(0);
		}
	}
};



