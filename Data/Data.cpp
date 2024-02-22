#include <Windows.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <ctime>
#include <set>
bool Start = false;
int Output = 0;
std::set<int> Ridings = std::set<int>();
std::set<int> NewPlayer = std::set<int>();

std::string Int2String(int value)
{
	std::stringstream ss;
	ss << value;
	std::string str = ss.str();
	return str;
}

int String2Int(std::string String)
{
	int Integer;
	std::istringstream iss(String);
	iss >> Integer;
	return Integer;
}

#pragma pack(1)
HINSTANCE hL = 0;
FARPROC p[806] = { 0 };
#pragma comment(lib,"Detours/enigma_ide.lib")
#pragma comment(lib,"Detours/detours.lib")
#include "Detours/enigma_ide.h"
#include "Detours/detours.h"
#include "Lisans.h"
#include "Exports.h"
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include "Interface.h"
#include "Memory.h"
#include "Tools.h"
#include "Functions.h"
#include "StorageFix.h"
#include "BuffRemain.h"
#include "Login.h"
#include "SetMaxPacket.h"
#include "Packet.h"
	

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
							   RemoteLisansCheck();
							   if (!ScaniaLicense)
								   return FALSE;

							   InitializeADVAPI32();
							   SetMaxPacket();
							   DetourTransactionBegin();
							   DetourAttach(&(PVOID&)StorageInfo, StorageFix);
							   //DetourAttach(&(PVOID&)Execute, ExecuteHook);
							   //DetourAttach(&(PVOID&)ServerSend, SendServer);
							   //DetourAttach(&(PVOID&)BuffRemain, BuffRemainExt);
							   DetourAttach(&(PVOID&)Process, DBProcess);
							   DetourTransactionCommit();
							   break;
	}
	case DLL_PROCESS_DETACH:
	{
							   FinalizeADVAPI32();
							   SetMaxPacket();
							   DetourTransactionBegin();
							   DetourDetach(&(PVOID&)StorageInfo, StorageFix);
							   //DetourDetach(&(PVOID&)Execute, ExecuteHook);
							   //DetourDetach(&(PVOID&)ServerSend, SendServer);
							   //DetourDetach(&(PVOID&)BuffRemain, BuffRemainExt);
							   DetourDetach(&(PVOID&)Process, DBProcess);
							   DetourTransactionCommit();
							   break;
	}
	}
	return TRUE;
}