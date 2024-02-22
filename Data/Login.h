namespace KDBServer
{
	namespace Console {
		static BOOL(__cdecl* WriteBlack)(char* text, ...) = (BOOL(__cdecl*)(char* text, ...))0x40DCB0;
		static BOOL(__cdecl* WriteRed)(char* text, ...) = (BOOL(__cdecl*)(char* text, ...))0x40DCE0;
		static BOOL(__cdecl* WriteBlue)(const char* text, ...) = (BOOL(__cdecl*)(const char* text, ...))0x40DC80;
		static BOOL(__cdecl* Write)(signed int a1, const char *a2, va_list a3) = (BOOL(__cdecl*)(signed int a1, const char *a2, va_list a3))0x40D990;
	}
	namespace CSocket
	{
		static void(__thiscall *Process)(void *Socket, char *packet) = (void(__thiscall*)(void*, char*))0x00410F10;
		static void(__thiscall *Login)(void *Socket, int nId, int UID) = (void(__thiscall*)(void*, int, int))0x004168c0;
		static void(__cdecl *Write)(void* Socket, unsigned char Type, const char* Format, ...) = (void(__cdecl*)(void*, unsigned char, const char*, ...))0x410EA0;
		//int __thiscall (void *this, char a2, int a3, int a4, int a5, int a6)
		//static void(__thiscall *LoadPlayer)(void *Socket, int nId, int UID) = (void(__thiscall*)(void*, int, int))0x004168c0;
	}
	struct _CDB
	{
		int Data[4];
	};
	namespace CDB
	{
		static void(__thiscall *CDB)(void *DB) = (void(__thiscall*)(void*))0x004026c0;
		static void(__thiscall *_CDB)(void *DB) = (void(__thiscall*)(void*))0x004026f0;
		static int(__cdecl *Execution)(int a1, int a2, int a3) = (int(__cdecl*)(int,int,int))0x0041BC20;
		static int(__cdecl *Prepare)(const char* Statement, ...) = (int(__cdecl*)(const char*, ...))0x0040dce0;
		static int(__cdecl *Execute)(void *DB, const char* Statement, ...) = (int(__cdecl*)(void*, const char*, ...))0x00402730;
		static int(__cdecl *Execute2)(void *DB, const char* Statement, ...) = (int(__cdecl*)(void*, const char*, ...))0x00402960;
		static int(__thiscall *Fetch)(void *DB) = (int(__thiscall*)(void*))0x00402ce0;
		static int(__thiscall *Reset)(void *DB) = (int(__thiscall*)(void*))0x00402DA0;
		static int(__thiscall *Expand)(int DB) = (int(__thiscall*)(int))0x0041B150;
		static SQLRETURN(__thiscall *GetReturn)(int DB, SQLPOINTER TargetValue) = (SQLRETURN(__thiscall*)(int, SQLPOINTER))0x0041B210;

		static void(__thiscall *Bind_String)(void *DB, char* String, int Length) = (void(__thiscall*)(void*, char*, int))0x0041b260;
		static void(__thiscall *Bind_Dword)(void *DB, unsigned long* Value) = (void(__thiscall*)(void*, unsigned long*))0x0041b1c0;
		static void(__thiscall *Bind_Word)(void *DB, unsigned short* Value) = (void(__thiscall*)(void*, unsigned short*))0x0041b390;
		static void(__thiscall *Bind_Byte)(void *DB, unsigned char* Value) = (void(__thiscall*)(void*, unsigned char*))0x0041b170;


		static void(__thiscall *Bind_DwordX)(int DB, int* Value) = (void(__thiscall*)(int, int*))0x0041b1c0;
		static void(__thiscall *Bind_WordX)(int DB, short* Value) = (void(__thiscall*)(int, short*))0x0041b390;
		static void(__thiscall *Bind_ByteX)(int DB, char* Value) = (void(__thiscall*)(int, char*))0x0041b170;
	}
	namespace CIOBuffer
	{
		static void * (__cdecl *Alloc)() = (void * (__cdecl*)())0x0040b500;
	}
	namespace CIOSocket
	{
		static void(__thiscall *Write)(void *Socket, char *Buffer) = (void(__thiscall*)(void*, char*))0x0040bd10;
	}
	static char*(__cdecl *ReadPacket)(char *Packet, const char *format, ...) = (char* (__cdecl*)(char *p, const char *format, ...))0x41CB40;
	static char * (__cdecl *WritePacket)(char *Packet, const char* Type, ...) = (char * (__cdecl*)(char*, const char*, ...))0x0041c830;

	static int (__stdcall *SendPack)(int a1, int a2, int a3) = (int (__stdcall*)(int,int,int))0x00418BA0;

	static char * (__stdcall *PutString)(char *Packet, const char* String) = (char * (__stdcall*)(char*, const char*))0x0041d0b0;
	static char * (__cdecl *PutDword)(char *Packet, unsigned long Value) = (char * (__cdecl*)(char*, unsigned long))0x0041bc40;
	static char * (__cdecl *PutWord)(char *Packet, unsigned short Value) = (char * (__cdecl*)(char*, unsigned short))0x0041bc00;
	static char * (__cdecl *PutByte)(char *Packet, unsigned char Value) = (char * (__cdecl*)(char*, unsigned char))0x0041bbe0;
	static void(__cdecl *InsertLog)(BYTE MainType, BYTE Type, int Player1, int Player2, int IID, int Value1, int Value2, int Value3, int Value4) = (void(__cdecl*)(BYTE, BYTE, int, int, int, int, int, int, int))0x0040DD10;
}


#ifndef _CSOCKETLOGIN_H
#define _CSOCKETLOGIN_H

namespace NS_CSocketLogin
{
	void __fastcall Login(void *Socket, void* _edx, int nId, int nUID) {
#ifdef DEBUG
		g_fileLog.AddLog(LOG_INF, "(NS_CSocketLogin::Login(void *Socket, void* _edx, int nId, int nUID) call)\n");
#endif // DEBUG
		char *end;

		char delName[17];
		DWORD delPID;
		unsigned __int8 delLevel;
		unsigned __int8 delClass;
		void *delPacket;
		unsigned __int8 delDayLeft;

		DWORD PID;
		char Name[17];
		unsigned char Class;
		unsigned char Specialty;
		unsigned char Level;
		DWORD GID;
		unsigned short Strength;
		unsigned short Health;
		unsigned short Intelligence;
		unsigned short Wisdom;
		unsigned short Dexterity;
		unsigned char Face;
		unsigned char Hair;

		char *pBuffer = (char*)KDBServer::CIOBuffer::Alloc();
		char *dbPacket = pBuffer;
		*((unsigned char*)pBuffer + 2) = 0;
		end = (char *)dbPacket + 10;

		KDBServer::_CDB db;
		KDBServer::CDB::CDB(&db);
		KDBServer::CDB::Bind_Dword(&db, &PID);
		KDBServer::CDB::Bind_String(&db, Name, 17);
		KDBServer::CDB::Bind_Byte(&db, &Class);
		KDBServer::CDB::Bind_Byte(&db, &Specialty);
		KDBServer::CDB::Bind_Byte(&db, &Level);
		KDBServer::CDB::Bind_Dword(&db, &GID);
		KDBServer::CDB::Bind_Word(&db, &Strength);
		KDBServer::CDB::Bind_Word(&db, &Health);
		KDBServer::CDB::Bind_Word(&db, &Intelligence);
		KDBServer::CDB::Bind_Word(&db, &Wisdom);
		KDBServer::CDB::Bind_Word(&db, &Dexterity);
		KDBServer::CDB::Bind_Byte(&db, &Face);
		KDBServer::CDB::Bind_Byte(&db, &Hair);

		int nCount = 0;
		KDBServer::CDB::Prepare("SELECT TOP %d [PID], [Name], [Class], [Specialty], [Level], [GID], [Strength], [Health], [Intelligence], [Wisdom], [Dexterity], [Face], [Hair] FROM Player WHERE UID = %d ORDER BY [Level]", 5, nUID);
		if (KDBServer::CDB::Execute(&db, "SELECT TOP %d [PID], [Name], [Class], [Specialty], [Level], [GID], [Strength], [Health], [Intelligence], [Wisdom], [Dexterity], [Face], [Hair] FROM Player WHERE UID = %d ORDER BY [Level]", 5, nUID))
		{
			char *d;
			while (KDBServer::CDB::Fetch(&db))
			{
				end = KDBServer::PutDword(end, PID);
				end = KDBServer::PutString(end, Name);
				end = KDBServer::PutByte(end, Class);
				end = KDBServer::PutByte(end, Specialty);
				end = KDBServer::PutByte(end, Level);
				end = KDBServer::PutDword(end, GID);
				end = KDBServer::PutWord(end, Strength);
				end = KDBServer::PutWord(end, Health);
				end = KDBServer::PutWord(end, Intelligence);
				end = KDBServer::PutWord(end, Wisdom);
				end = KDBServer::PutWord(end, Dexterity);
				end = KDBServer::PutByte(end, Face);
				d = KDBServer::PutByte(end, Hair);
				end = d + 1;
				*d = 0;

				KDBServer::_CDB dbItem;
				unsigned __int16 wIndex;
				KDBServer::CDB::CDB(&dbItem);
				KDBServer::CDB::Bind_Word(&dbItem, &wIndex);
				KDBServer::CDB::Prepare("SELECT TOP 36 [Index] FROM Item WHERE [PID] = %d AND ( [Info] & %d) = %d", PID, 17, 1);
				if (KDBServer::CDB::Execute(&dbItem, "SELECT TOP 36 [Index] FROM Item WHERE [PID] = %d AND ( [Info] & %d) = %d", PID, 17, 1))
				{
					while (KDBServer::CDB::Fetch(&dbItem))
					{
						end = KDBServer::PutWord(end, wIndex);
						++*d;
					}
				}
				
				nCount++; 
				/*KDBServer::CDB::Reset(&dbItem);

				DWORD IID = 0, ItemStat = 0;
				BindDword(&dbItem, &IID);
				BindDword(&dbItem, &ItemStat);

				KDBServer::CDB::Prepare("SELECT [IID], [ItemStat] FROM Item WHERE ItemStat > 0 AND (([PID] = %d AND ([Info] & %d) = 0 ) OR ([PID] = %d AND ([Info] & %d) <> 0))", PID,16,nUID,16);
				if (Execute((SQLHSTMT*)&dbItem, "SELECT [IID], [ItemStat] FROM Item WHERE ItemStat > 0 AND (([PID] = %d AND ([Info] & %d) = 0 ) OR ([PID] = %d AND ([Info] & %d) <> 0))", PID, 16, nUID, 16))
				{
					while (Fetch(&dbItem))
					{
						if (IID && ItemStat)
							ServerSend((int)Socket, 55, "dd", IID, ItemStat);
					}
				}*/

				KDBServer::CDB::_CDB(&dbItem);
			}
		}

		*(WORD *)dbPacket = (WORD)end - (WORD)dbPacket;
		*((DWORD *)pBuffer + 2000) = *(WORD *)dbPacket;
		KDBServer::WritePacket(dbPacket + 3, "dbw", nId, nCount, (unsigned short)(end - dbPacket - 10));
		KDBServer::CIOSocket::Write(Socket, pBuffer);

		KDBServer::CDB::_CDB(&db);


		char *delBuffer = (char*)KDBServer::CIOBuffer::Alloc();
		delPacket = delBuffer;
		*((unsigned char *)delBuffer + 2) = 29;
		end = (char *)delPacket + 10;

		int delCount = 0;
		KDBServer::_CDB dbDel;
		KDBServer::CDB::CDB(&dbDel);
		KDBServer::CDB::Bind_Dword(&dbDel, &delPID);
		KDBServer::CDB::Bind_String(&dbDel, delName, 17);
		KDBServer::CDB::Bind_Byte(&dbDel, &delLevel);
		KDBServer::CDB::Bind_Byte(&dbDel, &delClass);
		KDBServer::CDB::Bind_Byte(&dbDel, &delDayLeft);
		KDBServer::CDB::Prepare("SELECT TOP 5 PlayerDeleted.PID, Name, [Level], Class, DayLeft = 14-datediff(dd, DeletedTime, getdate()) FROM PlayerDeleted, Player WHERE PlayerDeleted.UID = %d AND PlayerDeleted.PID= Player.PID ORDER BY DayLeft DESC", nUID);
		if (KDBServer::CDB::Execute(&dbDel, "SELECT TOP 5 PlayerDeleted.PID, Name, [Level], Class, DayLeft = 14-datediff(dd, DeletedTime, getdate()) FROM PlayerDeleted, Player WHERE PlayerDeleted.UID = %d AND PlayerDeleted.PID= Player.PID ORDER BY DayLeft DESC", nUID))
		{
			while (KDBServer::CDB::Fetch(&dbDel) && delCount <= 5)
			{
				end = KDBServer::PutDword(end, delPID);
				end = KDBServer::PutString(end, delName);
				end = KDBServer::PutByte(end, delLevel);
				end = KDBServer::PutByte(end, delClass);
				end = KDBServer::PutByte(end, delDayLeft);
				delCount++;
			}
		}
		KDBServer::CDB::_CDB(&dbDel);

		*(unsigned short *)delPacket = (unsigned short)end - (unsigned short)delPacket;
		*((unsigned long *)delBuffer + 2000) = *(unsigned short *)delPacket;

		KDBServer::WritePacket((char *)delPacket + 3, "dbw", nId, delCount, end - ((char *)delPacket + 10));
		KDBServer::CIOSocket::Write(Socket, delBuffer);
	}

	void __fastcall LoadPlayer(void *Socket, void* _edx, int a2, int a3, int a4, int a5, int a6) {
		void *v7; // [sp+0h] [bp-E8h]@1
		char v8; // [sp+7h] [bp-E1h]@22
		int v9; // [sp+8h] [bp-E0h]@22
		char *v10; // [sp+Ch] [bp-DCh]@9
		int v11; // [sp+10h] [bp-D8h]@28
		char v12; // [sp+17h] [bp-D1h]@6
		__int16 v13; // [sp+18h] [bp-D0h]@12
		char v14; // [sp+1Ch] [bp-CCh]@18
		char v15; // [sp+1Dh] [bp-CBh]@12
		char v16; // [sp+1Eh] [bp-CAh]@12
		char v17; // [sp+1Fh] [bp-C9h]@12
		int v18; // [sp+20h] [bp-C8h]@12
		int v19; // [sp+24h] [bp-C4h]@12
		char v20; // [sp+2Ah] [bp-BEh]@12
		char v21; // [sp+2Bh] [bp-BDh]@12
		char v22; // [sp+2Ch] [bp-BCh]@25
		char v23; // [sp+2Dh] [bp-BBh]@25
		char v24; // [sp+2Eh] [bp-BAh]@12
		char v25; // [sp+2Fh] [bp-B9h]@28
		char v26; // [sp+30h] [bp-B8h]@12
		char v27; // [sp+31h] [bp-B7h]@12
		char v28; // [sp+32h] [bp-B6h]@12
		char v29; // [sp+33h] [bp-B5h]@25
		int v30; // [sp+34h] [bp-B4h]@25
		char v31; // [sp+3Ah] [bp-AEh]@12
		char v32; // [sp+3Bh] [bp-ADh]@12
		__int16 v33; // [sp+3Ch] [bp-ACh]@18
		int v34; // [sp+40h] [bp-A8h]@25
		char v35; // [sp+47h] [bp-A1h]@18
		int v36; // [sp+48h] [bp-A0h]@12
		char v37; // [sp+4Ch] [bp-9Ch]@9
		char v38; // [sp+4Dh] [bp-9Bh]@12
		char v39; // [sp+4Eh] [bp-9Ah]@1
		char v40; // [sp+4Fh] [bp-99h]@1
		int v41; // [sp+50h] [bp-98h]@1
		__int16 v42; // [sp+54h] [bp-94h]@1
		int v43; // [sp+58h] [bp-90h]@1
		int v44; // [sp+5Ch] [bp-8Ch]@1
		int v45; // [sp+60h] [bp-88h]@1
		char *v46; // [sp+64h] [bp-84h]@1
		char v47; // [sp+6Bh] [bp-7Dh]@1
		__int16 v48; // [sp+6Ch] [bp-7Ch]@1
		__int16 v49; // [sp+70h] [bp-78h]@1
		__int16 v50; // [sp+74h] [bp-74h]@1
		char v51; // [sp+7Bh] [bp-6Dh]@1
		int v52; // [sp+7Ch] [bp-6Ch]@1
		char v53; // [sp+81h] [bp-67h]@1
		char v54; // [sp+82h] [bp-66h]@1
		char v55; // [sp+83h] [bp-65h]@1
		__int16 v56; // [sp+84h] [bp-64h]@1
		char v57; // [sp+8Bh] [bp-5Dh]@1
		char v58; // [sp+8Ch] [bp-5Ch]@1
		char v59; // [sp+A6h] [bp-42h]@1
		char v60; // [sp+A7h] [bp-41h]@1
		void *v61; // [sp+A8h] [bp-40h]@1
		int v62; // [sp+ACh] [bp-3Ch]@1
		int v63; // [sp+B0h] [bp-38h]@1
		int v64; // [sp+B4h] [bp-34h]@1
		int v65 = 0; // [sp+B8h] [bp-30h]@5
		char v66; // [sp+C1h] [bp-27h]@1
		char v67; // [sp+C2h] [bp-26h]@1
		char TargetValue; // [sp+C3h] [bp-25h]@1
		KDBServer::_CDB v69; // [sp+C4h] [bp-24h]@1
		void *v70; // [sp+D4h] [bp-14h]@1
		char v71; // [sp+DAh] [bp-Eh]@1
		char v72; // [sp+DBh] [bp-Dh]@1
		int v73; // [sp+E4h] [bp-4h]@1
		v7 = Socket;
		v70 = (void *)KDBServer::CIOBuffer::Alloc();
		v61 = v70;
		*((BYTE *)v70 + 2) = 4;
		v46 = (char *)v61 + 8;
		v62 = 0;

		KDBServer::_CDB temp;
		KDBServer::CDB::CDB(&temp);
		int Size = 72;
		if (Execute((void*)&temp, "SELECT * FROM SpecialBuffs WHERE [PID] = %d AND [BuffID] >= 1528 AND [BuffID] <= 1530 AND [Time] > %d", a2, (int)time(0)))
		{
			while (KDBServer::CDB::Fetch(&temp))
				Size += 36;
		}

		/*KDBServer::CDB::Reset(&temp);
		Execute((SQLHSTMT*)&temp, "UPDATE Player SET [Exp] = Exp/10 WHERE [PID] = %d AND [Exp] < -10000000000", a2);
		KDBServer::CDB::Reset(&temp);
		Execute((SQLHSTMT*)&temp, "UPDATE Player SET [Exp] = Exp*-1 WHERE [PID] = %d AND [Exp] < 0", a2);
		*/

		KDBServer::CDB::_CDB(&temp);

		KDBServer::CDB::CDB(&v69);
		v73 = 0;
		KDBServer::CDB::Bind_DwordX((int)&v69,&v44);
		KDBServer::CDB::Expand((int)&v69);
		KDBServer::CDB::Bind_ByteX((int)&v69, &TargetValue);
		KDBServer::CDB::Bind_String((void*)&v69, &v58, 17);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v54);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v71);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v47);
		KDBServer::CDB::Bind_WordX((int)&v69, &v56);
		KDBServer::CDB::GetReturn((int)&v69, &v64);
		KDBServer::CDB::Bind_DwordX((int)&v69, &v41);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v53);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v66);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v59);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v57);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v39);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v40);
		KDBServer::CDB::Bind_WordX((int)&v69, &v48);
		KDBServer::CDB::Bind_WordX((int)&v69, &v42);
		KDBServer::CDB::Bind_WordX((int)&v69, &v49);
		KDBServer::CDB::Bind_WordX((int)&v69, &v50);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v72);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v55);
		KDBServer::CDB::Bind_DwordX((int)&v69, &v63);
		KDBServer::CDB::Bind_DwordX((int)&v69, &v43);
		KDBServer::CDB::Bind_DwordX((int)&v69, &v45);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v60);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v51);
		KDBServer::CDB::Bind_ByteX((int)&v69, &v67);
		KDBServer::CDB::Bind_DwordX((int)&v69, &v52);
		if (Execute((void*)&v69, "SELECT * FROM Player WHERE [PID] = %d", a2))
		{
			if (KDBServer::CDB::Fetch(&v69))
			{
				if (a4 == v44)
				{
					v46 = (char *)KDBServer::PutByte(v46, TargetValue);
					if (TargetValue != 1)
					{
						v46 = (char *)KDBServer::PutString(v46, &v58);
						v46 = (char *)KDBServer::PutByte(v46, v54);
						v46 = (char *)KDBServer::PutByte(v46, v71);
						v46 = (char *)KDBServer::PutByte(v46, v47);
						v46 = (char *)KDBServer::PutWord(v46, v56);
						v46 = (char *)KDBServer::CDB::Execution((int)v46, v64, v65);
						v46 = (char *)KDBServer::PutDword(v46, v41);
						v46 = (char *)KDBServer::PutByte(v46, v53);
						v46 = (char *)KDBServer::PutByte(v46, v66);
						v46 = (char *)KDBServer::PutByte(v46, v59);
						v46 = (char *)KDBServer::PutByte(v46, v57);
						v46 = (char *)KDBServer::PutByte(v46, v39);
						v46 = (char *)KDBServer::PutByte(v46, v40);
						v46 = (char *)KDBServer::PutWord(v46, v48);
						v46 = (char *)KDBServer::PutWord(v46, v42);
						v46 = (char *)KDBServer::PutWord(v46, v49);
						v46 = (char *)KDBServer::PutWord(v46, v50);
						v46 = (char *)KDBServer::PutByte(v46, v72);
						v46 = (char *)KDBServer::PutByte(v46, v55);
						v46 = (char *)KDBServer::PutDword(v46, v63);
						v46 = (char *)KDBServer::PutDword(v46, v43);
						v46 = (char *)KDBServer::PutDword(v46, v45);
						v46 = (char *)KDBServer::PutByte(v46, v60);
						v46 = (char *)KDBServer::PutByte(v46, v51);
						v46 = (char *)KDBServer::PutByte(v46, v67);
						v46 = (char *)KDBServer::PutDword(v46, v52);
						if (!(int)*(DWORD**)0x00448E20)
						{
							KDBServer::CDB::Reset(&v69);
							KDBServer::CDB::Bind_ByteX((int)&v69, &v12);
							if (!Execute((void*)&v69, "SELECT [Index] FROM Skill WHERE [PID] = %d AND [Index] = 62", a2)
								|| !KDBServer::CDB::Fetch(&v69))
							{
								KDBServer::CDB::Reset(&v69);
								Execute((void*)&v69, "INSERT INTO Skill VALUES(%d, 62, 1)", a2);
							}
						}
						v10 = v46++;
						*v10 = 0;
						KDBServer::CDB::Reset(&v69);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v12);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v37);
						if (Execute((void*)&v69, "SELECT TOP 84 [Index], [Level] FROM Skill WHERE [PID] = %d", a2))
						{
							while (KDBServer::CDB::Fetch(&v69))
							{
								v46 = (char *)KDBServer::PutByte(v46, v12);
								v46 = (char *)KDBServer::PutByte(v46, v37);
								++*v10;
							}
						}
						v10 = v46++;
						*v10 = 0;
						KDBServer::CDB::Reset(&v69);
						KDBServer::CDB::Expand((int)&v69);
						KDBServer::CDB::Bind_DwordX((int)&v69, &v36);
						KDBServer::CDB::Bind_WordX((int)&v69, &v13);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v24);
						KDBServer::CDB::Bind_DwordX((int)&v69, &v18);
						KDBServer::CDB::Bind_DwordX((int)&v69, &v19);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v20);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v26);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v31);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v21);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v15);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v27);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v28);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v16);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v38);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v32);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v17);
						if (Execute((void*)&v69, "SELECT TOP(%d) * FROM  Item AS a INNER JOIN InventoryCheck AS b ON a.[Index] = b.[Index] WHERE(a.PID = %d) AND (a.Info & 16 = 0) AND b.Type = 1 UNION SELECT TOP(48) * FROM  Item AS a INNER JOIN InventoryCheck AS b ON a.[Index] = b.[Index] WHERE(a.PID = %d) AND (a.Info & 16 = 0) AND b.Type = 0 UNION SELECT TOP(%d) * FROM Item AS a LEFT OUTER JOIN InventoryCheck AS b ON b.[Index] = a.[Index] WHERE (a.PID = %d) AND (a.Info & 16 = 0) AND (b.[Index] IS NULL)", Size, a2, a2, Size, a2))
						{
							while (KDBServer::CDB::Fetch(&v69))
							{
								v46 = (char *)KDBServer::PutWord(v46, v13);
								v46 = (char *)KDBServer::PutDword(v46, v36);
								v46 = (char *)KDBServer::PutByte(v46, v24);
								v46 = (char *)KDBServer::PutDword(v46, v18);
								v46 = (char *)KDBServer::PutDword(v46, v19);
								v46 = (char *)KDBServer::PutByte(v46, v20);
								v46 = (char *)KDBServer::PutByte(v46, v26);
								v46 = (char *)KDBServer::PutByte(v46, v31);
								v46 = (char *)KDBServer::PutByte(v46, v21);
								v46 = (char *)KDBServer::PutByte(v46, v15);
								v46 = (char *)KDBServer::PutByte(v46, v27);
								v46 = (char *)KDBServer::PutByte(v46, v28);
								v46 = (char *)KDBServer::PutByte(v46, v16);
								v46 = (char *)KDBServer::PutByte(v46, v38);
								v46 = (char *)KDBServer::PutByte(v46, v32);
								v46 = (char *)KDBServer::PutByte(v46, v17); 
								++*v10;
							}
						}
						v10 = v46++;
						*v10 = 0;
						KDBServer::CDB::Reset(&v69);
						KDBServer::CDB::Expand((int)&v69);
						KDBServer::CDB::Bind_DwordX((int)&v69, &v36);
						KDBServer::CDB::Bind_WordX((int)&v69, &v13);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v24);
						KDBServer::CDB::Bind_DwordX((int)&v69, &v18);
						KDBServer::CDB::Bind_DwordX((int)&v69, &v19);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v20);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v26);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v31);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v21);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v15);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v27);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v28);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v16);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v38);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v32);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v17);
						if (Execute((void*)&v69, "SELECT TOP %d * FROM Item WHERE [PID] = %d AND ( [Info] & %d) <> 0", 100, a4, 16))
						{
							while (KDBServer::CDB::Fetch(&v69))
							{
								v46 = (char *)KDBServer::PutWord(v46, v13);
								v46 = (char *)KDBServer::PutDword(v46, v36);
								v46 = (char *)KDBServer::PutByte(v46, v24);
								v46 = (char *)KDBServer::PutDword(v46, v18);
								v46 = (char *)KDBServer::PutDword(v46, v19);
								v46 = (char *)KDBServer::PutByte(v46, v20);
								v46 = (char *)KDBServer::PutByte(v46, v26);
								v46 = (char *)KDBServer::PutByte(v46, v31);
								v46 = (char *)KDBServer::PutByte(v46, v21);
								v46 = (char *)KDBServer::PutByte(v46, v15);
								v46 = (char *)KDBServer::PutByte(v46, v27);
								v46 = (char *)KDBServer::PutByte(v46, v28);
								v46 = (char *)KDBServer::PutByte(v46, v16);
								v46 = (char *)KDBServer::PutByte(v46, v38);
								v46 = (char *)KDBServer::PutByte(v46, v32);
								v46 = (char *)KDBServer::PutByte(v46, v17);
								++*v10;
							}
						}
						v10 = v46++;
						*v10 = 0;
						KDBServer::CDB::Reset(&v69);
						KDBServer::CDB::Expand((int)&v69);
						KDBServer::CDB::Bind_WordX((int)&v69, &v33);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v14);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v35);
						if (Execute((void*)&v69, "SELECT * FROM Quest WHERE [PID] = %d", a2))
						{
							while (KDBServer::CDB::Fetch(&v69))
							{
								v46 = (char *)KDBServer::PutWord(v46, v33);
								v46 = (char *)KDBServer::PutByte(v46, v14);
								v46 = (char *)KDBServer::PutByte(v46, v35);
								++*v10;
							}
						}
						if ((int)*(DWORD**)0x0043D970 & 4)
						{
							v10 = v46++;
							*v10 = 0;
							KDBServer::CDB::Reset(&v69);
							KDBServer::CDB::Bind_ByteX((int)&v69, &v8);
							KDBServer::CDB::Bind_DwordX((int)&v69, &v9);
							if (Execute((void*)&v69, "SELECT [Type], [nF1] FROM Event WHERE [PID] = %d", a2))
							{
								while (KDBServer::CDB::Fetch(&v69))
								{
									v46 = (char *)KDBServer::PutByte(v46, v8);
									v46 = (char *)KDBServer::PutDword(v46, v9);
									++*v10;
								}
							}
						}
						v10 = v46++;
						*v10 = 0;
						KDBServer::CDB::Reset(&v69);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v22);
						KDBServer::CDB::Bind_String((void*)&v69, &v58, 17);
						KDBServer::CDB::Bind_DwordX((int)&v69, &v30);
						KDBServer::CDB::Bind_DwordX((int)&v69, &v34);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v29);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v23);
						if (Execute(
							(void*)&v69,
							"SELECT [Relation], [Name], [RPID], [RUID], [Class], [Level] FROM MLM WHERE [PID] = %d ORDER BY [Relation]",
							a2))
						{
							while (KDBServer::CDB::Fetch(&v69))
							{
								v46 = (char *)KDBServer::PutByte(v46, v22);
								v46 = (char *)KDBServer::PutString(v46, &v58);
								v46 = (char *)KDBServer::PutDword(v46, v30);
								v46 = (char *)KDBServer::PutDword(v46, v34);
								v46 = (char *)KDBServer::PutByte(v46, v29);
								v46 = (char *)KDBServer::PutByte(v46, v23);
								++*v10;
							}
						}
						v10 = v46++;
						*v10 = 0;
						KDBServer::CDB::Reset(&v69);
						KDBServer::CDB::Bind_ByteX((int)&v69, &v25);
						KDBServer::CDB::Bind_DwordX((int)&v69, &v11);
						if (Execute((void*)&v69, "SELECT [Type],[Remain] FROM BuffRemain WHERE [PID] = %d", a2))
						{
							while (KDBServer::CDB::Fetch(&v69))
							{
								v46 = (char *)KDBServer::PutByte(v46, v25);
								v46 = (char *)KDBServer::PutDword(v46, v11);
								++*v10;
							}
						}
						v46 = (char *)KDBServer::PutDword(v46, a3);
						if (a3)
						{
							if (v41 != a3)
							{
								Black("CSocket::LoadPlayer() Load Guild Error (Load GID %d) (Curr. GID %d)");
								v62 = 2;
								goto LABEL_36;
							}
							v46 = (char *)KDBServer::SendPack(v41, a6, (int)v46);
						}
						v62 = 1;
						goto LABEL_36;
					}
				}
			}
		}
	LABEL_36:
		KDBServer::WritePacket((char *)v61 + 3, "db", a5, (unsigned __int8)v62);
		*(WORD *)v61 = (WORD)v46 - (WORD)v61;
		*((DWORD *)v70 + 2000) = *(WORD *)v61;
		KDBServer::CIOSocket::Write(v7, (char*)v70);
		v73 = -1;
		return KDBServer::CDB::_CDB(&v69);
	}

}


#endif