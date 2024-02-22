void __cdecl QuestFix(int DB, const char *Statement, int PID, int QuestID, int Flag, int Clear)
{
	Execute((void*)DB, "INSERT INTO Quest VALUES (%d, %d, %d, %d, %d, %d, %d)", PID, QuestID, Flag, Clear, 0, 0, 0);
}

void __cdecl NewCreatedPlayer(int DB, const char *Statement, int PID)
{
	NewPlayer.insert(PID);
	Execute((void*)DB, Statement, PID);
}

int __cdecl InvExtension(void* DB,const char *Query,signed int Size, int PID, signed int Info)
{
	xCDB dbItem; 
	CDB(&dbItem);

	if (Executex((SQLHSTMT*)&dbItem, "SELECT * FROM SpecialBuffs WHERE [PID] = %d AND [BuffID] >= 1528 AND [BuffID] <= 1530 AND [Value] > %d", PID, (int)time(0)))
	{
		while (Fetch(&dbItem))
			Size += 36;
	}

	FreeCDB(&dbItem);

	std::string SQLQuery1 = "SELECT TOP(%d) * FROM  Item AS a INNER JOIN InventoryCheck AS b ON a.[Index] = b.[Index] WHERE(a.PID = %d) AND (a.Info & 16 = 0) AND b.Type = 1 UNION SELECT TOP(48) * FROM  Item AS a INNER JOIN InventoryCheck AS b ON a.[Index] = b.[Index] WHERE(a.PID = %d) AND (a.Info & 16 = 0) AND b.Type = 0 UNION SELECT TOP(%d) * FROM Item AS a LEFT OUTER JOIN InventoryCheck AS b ON b.[Index] = a.[Index] WHERE (a.PID = %d) AND (a.Info & 16 = 0) AND (b.[Index] IS NULL)";

	return Executex(DB, SQLQuery1.c_str(), Size, PID, PID, Size, PID);
}

void __cdecl Send()
{
	if(!Start)
		Sleep(1);
	else
		Sleep(5);
	Random();
}

int __cdecl ExecuteHook(void* a1, const char *a2, ...)
{
	Blue("%s", a2); 
	va_list va; // [sp+668h] [bp+10h]@1

	va_start(va, a2);
	int retValue = Execute(a1, a2, va);
	va_end(va);
	return retValue;
}

void SetMaxPacket()
{
	Interface<IMemory> Memory;
	Interface<ITools> Tools;
	int NewPacket = 126;
	unsigned char MaxCharCount = 5;

	Memory->Copy((void*)0x0041adf9, &MaxCharCount, 1);
	Memory->Hook(0x4118C9, NS_CSocketLogin::Login, IMemory::_I_CALL);
	//Memory->Hook(0x410FE3, NS_CSocketLogin::LoadPlayer, IMemory::_I_CALL);
	Memory->Copy((void*)0x00410DD5, &NewPacket, 1);
	Memory->Hook(0x00414646, QuestFix, 0xe8, 5);
	Memory->Hook(0x00417215, NewCreatedPlayer, 0xe8, 5);
	Memory->Hook(0x004132AD, InvExtension, 0xe8, 5);
	//Memory->Hook(0x00410EAC, Send, 0xe8, 5); 
}