//https://github.com/M0V3/hazedumper -> Offsets

// Our client address (client.dll->address)
DWORD dwClientBase = 0;
// Our engine address (engine.dll->address)
DWORD dwEngineBase = 0;
// Our local player (player on the game)
DWORD dwPlayerBase = 0;
// Offset for forceJump
DWORD o_ForceJump = 0x4F1AAF4;
// Offset for Local Player
DWORD o_LocalPlayer = 0xAA6614;
// Offset for F_Flags
DWORD o_fFlags = 0x100;
// Offset for ClientState
DWORD o_ClientState = 0x57D894;
// Offset for ClientStateViewAngles
DWORD o_ClientStateViewAngles = 0x4D10;
// Offset for ForceMoveRight
DWORD o_ForceMoveRight = 0x2EC5C44;
// Offset for ForceMoveLeft
DWORD o_ForceMoveLeft = 0x2EC5C38;
// Offset for Local Player Health
DWORD m_iHealth = 0xFC;
// Offset for Dormancy
DWORD m_bDormant = 0xE9;
// Offset for TeamNumbers
DWORD m_iTeamNum = 0xF0;
// Offset for GlowObject
DWORD m_dwGlowObject = 0x4FA08E8;
// Offset for Entity List
DWORD m_dwEntityList = 0x4A8387C;
// Offset for Glow Index
DWORD m_iGlowIndex = 0xA310;
// Offset for Crosshair
DWORD m_iCrosshairId = 0xB2A4;
// Offset for our Vector Origin
DWORD m_vecOrigin = 0x134;
// Offset for our Vector View Offset
DWORD m_vecViewOffset = 0x104;

// Struct for Angles
struct Vector {
	float x, y, z;
}VectorStruct;

bool GlowBool = false;
