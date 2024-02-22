const int MAXSIZE = 7984;
std::string ConvertDatetimeToString(const std::string& datetimeValue) {
	// Define the input and output formats
	const char* inputFormat = "%m/%d/%Y %I:%M:%S %p";
	const char* outputFormat = "%Y-%m-%d %H:%M:%S"; // Remove the %S for seconds

	// Prepare input stream to parse the input datetime
	std::istringstream input(datetimeValue);
	std::tm tm = {};
	input >> std::get_time(&tm, inputFormat);

	if (input.fail()) {
		// Parsing failed
		return "InvalidDatetime";
	}

	// Convert the parsed components to a formatted string
	char buffer[20];
	strftime(buffer, sizeof(buffer), outputFormat, &tm);

	return buffer;
}

int __fastcall DBProcess(void *Socket, void *edx, int Data)
{
	void *pPos = (void *)(Data + 3);
	int Type = *(BYTE*)(Data + 2);

	if (Output)
		Blue("%d", Type);
	
	if (Type == 1)
	{
		unsigned long PID = 0, Value = 0, UID = 0;
		Read((char *)pPos, (unsigned int)"ddd", &PID, &Value, &UID);
		DWORD IID = 0, ItemStat = 0;
		DWORD Count = 0;
		xCDB dbItem; CDB(&dbItem);

		if (PID)
		{
			Interface<ITools> Tools;

			BindDword(&dbItem, &Count);
			BindDword(&dbItem, &IID);
			BindDword(&dbItem, &ItemStat);

			std::string Expr = "FROM Item WHERE(ItemStat > 0) AND (PID = %d) AND (Info & %d = 0) OR (ItemStat > 0) AND(PID = %d) AND(Info & %d <> 0)";
			std::string Statement = "SELECT (SELECT COUNT(*) AS Expr1 " + Expr + "), [IID], [ItemStat] " + Expr;
			if (Execute((SQLHSTMT*)&dbItem, Statement.c_str(), PID, 16, UID, 16, PID, 16, UID, 16))
			{
				bool Entered = false;
				char* Packet;
				int Size = 0;
				while (Fetch(&dbItem))
				{
					if (!Entered) {
						Entered = true;
						Packet = new char[Count * 8];
					}

					if (Size < MAXSIZE) {
						Tools->Compile(Packet + Size, "dd", IID, ItemStat);
						Size += 8;
					}
				}

				if (Entered) {
					ServerSend((int)Socket, 55, "bm", Count, Packet, Size);
					delete[] Packet;
				}
			}

			CDBx(&dbItem);
			char Lock[17];
			DWORD PetLife = 0;
			IID = 0;
			Count = 0;

			BindDword(&dbItem, &Count);
			BindDword(&dbItem, &PetLife);
			BindDword(&dbItem, &IID);
			
			Expr = "FROM Item WHERE [PetTime] != 0 AND (([PID] = %d AND ([Info] & %d) = 0) OR ([PID] = %d AND ([Info] & %d) <> 0))";
			Statement = "SELECT (SELECT COUNT(*) AS Expr1 " + Expr + "), [PetTime],[IID] " + Expr;
			if (Execute((SQLHSTMT*)&dbItem, Statement.c_str(), PID, 16, UID, 16, PID, 16, UID, 16))
			{
				bool Entered = false;
				char* Packet;
				int Size = 0;
				while (Fetch(&dbItem))
				{
					if (!Entered) {
						Entered = true;
						Packet = new char[Count * 8];
					}

					if (Size < MAXSIZE) {
						Tools->Compile(Packet + Size, "dd", IID, PetLife);
						Size += 8;
					}
				}

				if (Entered) {
					ServerSend((int)Socket, 53, "bm", Count, Packet, Size);
					delete[] Packet;
				}
			}

			CDBx(&dbItem);
			IID = 0;

			BindString(&dbItem, Lock, 17);
			BindDword(&dbItem, &IID);
			if (Execute((SQLHSTMT*)&dbItem, "SELECT [Lock], [IID] FROM Item WHERE [Lock] != 'nopwd' AND (([PID] = %d AND ([Info] & %d) = 0) OR ([PID] = %d AND ([Info] & %d) <> 0))", PID, 16, UID, 16))
			{
				while (Fetch(&dbItem))
					ServerSend((int)Socket, 54, "ds", IID, Lock);
			}
		}

		FreeCDB(&dbItem);
	}

	if (Type == 3) {
		void *MypPos = pPos;
		unsigned long IID = 0, PID = 0;
		unsigned short index = 0;
		int a4 = 0;
		char a5 = 0;
		int a6 = 0, a7 = 0, a8 = 0;

		Read((char *)MypPos, (unsigned int)"ddwdbddd", &IID, &PID, &index, &a4, &a5, &a6, &a7, &a8);

		if(index && Ridings.count(index))
			ServerSend((int)Socket, 64, "dd", PID, index);
	}

	if (Type == 2) {
		void *MypPos = pPos;
		xCDB dbItem; CDB(&dbItem);
		int PID = 0;
		DWORD Index = 0, Amount = 0, Prefix = 0, Storage = 0, UID = 0;
		Read((char *)MypPos, (unsigned int)"d", &PID);

		BindDword(&dbItem, &UID);
		if (Execute((SQLHSTMT*)&dbItem, "SELECT [UID] FROM Player WHERE PID = %d", PID))
			Fetch(&dbItem);

		CDBx(&dbItem);

		BindDword(&dbItem, &Index);
		BindDword(&dbItem, &Amount);
		BindDword(&dbItem, &Prefix);
		BindDword(&dbItem, &Storage);
		
		if (Execute((SQLHSTMT*)&dbItem, "SELECT [Index], [Amount], [Prefix], [Storage] FROM SpecialItems WHERE (PID = %d AND Storage = 0) OR (PID = %d AND Storage = 1)",PID,UID))
		{
			while (Fetch(&dbItem))
			{
				if (Index && Amount)
					ServerSend((int)Socket, 59, "ddddd", PID, Index, Amount, Prefix,Storage);
			}
		}
		FreeCDB(&dbItem);
	}

	if (Type == 56) {
		Start = false;
		xCDB dbItem;
		CDB(&dbItem);

		Execute((SQLHSTMT*)&dbItem, "UPDATE SpecialBuffs SET [Value] = [Time] WHERE [BuffID] >= 1528 AND [BuffID] <= 1530 AND [Value] = 0");

		DWORD GID = 0;
		char GName[15];
		CDBx(&dbItem);
		BindDword(&dbItem, &GID);
		BindString(&dbItem, GName, 15);

		Execute((SQLHSTMT*)&dbItem, "SELECT GID, Name	FROM  Guild	WHERE (GID IN (SELECT  GID	FROM  GuildCastle))");
		Fetch(&dbItem);
		if (GID && GName)
			ServerSend((int)Socket, 60, "ds", GID, GName);
		CDBx(&dbItem);

		DWORD PID = 0;
		DWORD Amount = 0;
		GID = 0;
		char PName[21];
		BindDword(&dbItem, &PID);
		BindDword(&dbItem, &GID);
		BindDword(&dbItem, &Amount);
		BindString(&dbItem, PName, 21);

		Execute((SQLHSTMT*)&dbItem, "SELECT GuildBuff.PID, GuildBuff.GID, GuildBuff.Amount, Player.Name	FROM(	SELECT PID, GID, Amount, Rank()	over(Partition BY GuildBuff.GID		ORDER BY Amount DESC) AS Rank	FROM GuildBuff)   GuildBuff INNER JOIN	Player ON GuildBuff.PID = Player.PID	WHERE Rank <= 3");
		while (Fetch(&dbItem))
			ServerSend((int)Socket, 67, "ddds", PID, GID, Amount, PName);

		CDBx(&dbItem);
		char WName[15];
		BindString(&dbItem, WName, 15);

		Execute((SQLHSTMT*)&dbItem, "SELECT Name FROM  Player  WHERE (PID = (SELECT  PID	FROM   WorldCup	WHERE (CurrentWinner = 1)))");
		Fetch(&dbItem);
		if (WName)
			ServerSend((int)Socket, 61, "s", WName);

		CDBx(&dbItem);
		Execute((SQLHSTMT*)&dbItem, "DELETE FROM MonsterTrack");

		CDBx(&dbItem);
		Execute((SQLHSTMT*)&dbItem, "UPDATE ItemRestored SET PID = 0 WHERE (EXISTS (SELECT * FROM Item WHERE Item.IID = ItemRestored.IID AND Item.PID > 0))", (int)time(0));

		CDBx(&dbItem);
		Execute((SQLHSTMT*)&dbItem, "DELETE FROM Item FROM Item INNER JOIN ItemRestored ON Item.IID = ItemRestored.IID WHERE (ItemRestored.Val2 <= %d) AND (ItemRestored.PID > 0)", (int)time(0));

		CDBx(&dbItem);
		Execute((SQLHSTMT*)&dbItem, "DELETE FROM ItemRestored WHERE (Val2 <= %d AND PID > 0) OR (NOT EXISTS (SELECT * FROM Item WHERE Item.IID = ItemRestored.IID))", (int)time(0));

		FreeCDB(&dbItem);

		Start = true;
	}

	if (Type == 13) {
		int PID = 0;
		char Flag = 0, Clear = 0;
		int QuestTime = 0, QuestRepeat = 0, MonsterCount = 0;
		unsigned short Quest = 0;

		Read((char *)pPos, (unsigned int)"dwbbddd", &PID, &Quest, &Flag, &Clear, &QuestTime,&QuestRepeat,&MonsterCount);


		xCDB dbItem; CDB(&dbItem);

		Execute((SQLHSTMT*)&dbItem, "INSERT INTO Quest VALUES ( %d, %d, %d, %d, %d, %d , %d)", PID, Quest, Flag, Clear, QuestTime, QuestRepeat, MonsterCount);

		Logging(7, 16, PID, 0, 0, Quest, Flag, Clear, 0);
		return FreeCDB(&dbItem);
	}

	if (Type == 14) {
		int PID = 0;
		char Flag = 0, Clear = 0;
		int QuestTime = 0, QuestRepeat = 0, MonsterCount = 0;
		unsigned short Quest = 0;

		Read((char *)pPos, (unsigned int)"dwbbddd", &PID, &Quest, &Flag, &Clear, &QuestTime, &QuestRepeat, &MonsterCount);

		xCDB dbItem; CDB(&dbItem);

		Execute((SQLHSTMT*)&dbItem, "UPDATE Quest SET [Flag] = %d, [Clear] = %d, [QuestTime] = %d, [QuestRepeat] = %d, [MonsterCount] = %d WHERE [PID] = %d AND [Quest] = %d", Flag, Clear, QuestTime, QuestRepeat, MonsterCount, PID, Quest);

		Logging(7, 16, PID, 0, 0, Quest, Flag, Clear, 0);
		return FreeCDB(&dbItem);
	}

	if (Type == 85)
	{
		int PID = 0;
		char Lock[17];
		DWORD PetLife = 0, IID = 0;
		xCDB dbItem; CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"d", &PID);

		if (PID)
		{
			BindDword(&dbItem, &PetLife);
			BindString(&dbItem, Lock, 17);
			BindDword(&dbItem, &IID);

			if (Execute((SQLHSTMT*)&dbItem, "SELECT [PetTime],[Lock],[IID] FROM Item WHERE ([PID] = %d OR ([PID] = (SELECT UID FROM Player WHERE [PID] = %d) AND ([Info] & %d) = 0)) AND ([PetTime] != 0 OR [Lock] != 'nopwd')", PID, PID, 16))
			{
				while (Fetch(&dbItem))
					ServerSend((int)Socket, 53, "sddd", Lock, PetLife, IID, PID);
			}
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 25)
	{
		void *MypPos = pPos;
		DWORD RemainLeft;
		xCDB dbItem; CDB(&dbItem);
		int PID = 0, Type = 0, Remain = 0, Value = 0;
		Read((char *)MypPos, (unsigned int)"dbdb", &PID, &Type, &Remain, &Value);

		if (PID && (Type >= 119 && Type <= 155) && Value == 1)
		{
			BindDword(&dbItem, &RemainLeft);
			Execute((SQLHSTMT*)&dbItem, "SELECT [Remain] FROM BuffRemain WHERE [Type] = %d AND [PID] = %d", Type, PID);
			Fetch(&dbItem);

			if ((RemainLeft - time(0)) > 0)
			{
				ServerSend((int)Socket, 52, "ddd", PID, Type, RemainLeft - time(0));
				return FreeCDB(&dbItem);
			}
		}

		FreeCDB(&dbItem);
	}

	if (Type == 72){
		void *MypPos = pPos;
		xCDB dbItem;
		CDB(&dbItem);
		DWORD Name;
		int d1=0,d2=0;
		const char* OldName="",*NewName="";
		Read((char *)MypPos, (unsigned int)"ddss", &d1,&d2,&OldName,&NewName);
		BindDword(&dbItem, &Name);
		if (Execute((SQLHSTMT*)&dbItem, "SELECT [PID] FROM Player WHERE Name = '%s'", NewName))
				Fetch(&dbItem);

		FreeCDB(&dbItem);

		if (Name > 0) {
			ServerSend((int)Socket, 26, "dbs", d1, 0, NewName);
			return 0;
		}
	}

	if (Type == 81)
	{
		int PID = 0, HP = 0, HK = 0, HD = 0, DKPT = 0, DKPW = 0, PLT = 0, PLW = 0, SVT = 0, SVW = 0, RP = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"ddddddddddd", &PID, &HP, &HK, &HD, &DKPT, &DKPW, &PLT, &PLW, &SVT, &SVW, &RP);

		if (PID) {
			if (HP) {
				Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET [HonorPoint] = %d WHERE [PID] = %d", HP, PID);
				CDBx(&dbItem);
			}
			if (HK) {
				Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET [HonorKill] = %d WHERE [PID] = %d", HK, PID);
				CDBx(&dbItem);
			}
			if (HD) {
				Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET [HonorDeath] = %d WHERE [PID] = %d", HD, PID);
				CDBx(&dbItem);
			}
			if (DKPT) {
				Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET [DKPTotal] = %d WHERE [PID] = %d", DKPT, PID);
				CDBx(&dbItem);
			}
			if (DKPW) {
				Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET [DKPWin] = %d WHERE [PID] = %d", DKPW, PID);
				CDBx(&dbItem);
			}
			if (PLT) {
				Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET [PLTotal] = %d WHERE [PID] = %d", PLT, PID);
				CDBx(&dbItem);
			}
			if (PLW) {
				Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET [PLWin] = %d WHERE [PID] = %d", PLW, PID);
				CDBx(&dbItem);
			}
			if (SVT) {
				Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET [SVTotal] = %d WHERE [PID] = %d", SVT, PID);
				CDBx(&dbItem);
			}
			if (SVW) {
				Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET [SVWin] = %d WHERE [PID] = %d", SVW, PID);
				CDBx(&dbItem);
			}
			if (RP)
				Execute((SQLHSTMT*)&dbItem, "UPDATE Player SET [RewardPoint] = %d WHERE [PID] = %d", RP, PID);
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 82)
	{
		int PID = 0;
		DWORD HP,HK,HD,DKPT,DKPW,PLT,PLW,SVT,SVW,RP;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"d", &PID);

		if (PID)
		{
			BindDword(&dbItem, &HP);
			BindDword(&dbItem, &HK);
			BindDword(&dbItem, &HD);
			BindDword(&dbItem, &DKPT);
			BindDword(&dbItem, &DKPW);
			BindDword(&dbItem, &PLT);
			BindDword(&dbItem, &PLW);
			BindDword(&dbItem, &SVT);
			BindDword(&dbItem, &SVW);
			BindDword(&dbItem, &RP);

			if (Execute((SQLHSTMT*)&dbItem, "SELECT [HonorPoint], [HonorKill], [HonorDeath], [DKPTotal], [DKPWin], [PLTotal], [PLWin], [SVTotal], [SVWin], [RewardPoint] FROM Player WHERE [PID] = %d", PID))
			{
				Fetch(&dbItem);
				ServerSend((int)Socket, 50, "ddddddddddd", PID, HP, HK, HD, DKPT, DKPW, PLT, PLW, SVT, SVW, RP);
			}
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 83)
	{
		int Day = 0, Time = 0, PID = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"ddd", &PID, &Day, &Time);

		if (PID)
			Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET [EmokDay] = %d, [EmokTime] = %d WHERE [PID] = %d", Day, Time, PID);

		return FreeCDB(&dbItem);
	}

	if (Type == 84)
	{
		int PID = 0;
		DWORD Time, Day;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"d", &PID);

		if (PID)
		{
			BindDword(&dbItem, &Time);
			BindDword(&dbItem, &Day);

			if (Execute((SQLHSTMT*)&dbItem, "SELECT [EmokDay], [EmokTime] FROM Player WHERE [PID] = %d AND [EmokDay]!=0", PID))
			{
				Fetch(&dbItem);
				ServerSend((int)Socket, 51, "ddd", PID, Time, Day);
			}
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 86)
	{
		RETCODE rc; HENV henv; HDBC hdbc; HSTMT hstmt;
		int xType = 0; const char *MyID,*PWD2ND;
		Read((char *)pPos, (unsigned int)"ss", &MyID, &PWD2ND);
		std::stringstream strValue;
		strValue << PWD2ND << std::endl;
		int Value = 0;
		strValue >> Value;

		if (MyID != NULL && PWD2ND != NULL)
		{
			const char *db = "kal_auth";
			SQLAllocEnv(&henv);
			SQLAllocConnect(henv, &hdbc);
			rc = SQLConnect(hdbc, (unsigned char*)db, SQL_NTS, 0, 0, 0, 0);

			if ((rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO))
			{
				SQLFreeConnect(hdbc);
				SQLFreeEnv(henv);
				return 0;
			}

			rc = SQLAllocStmt(hdbc, &hstmt);
			std::stringstream query;
			query << "UPDATE Login SET [PWD2ND] = '" << Value << "' WHERE [ID] = '" << MyID << "'";
			std::string runquery = query.str();
			const char *sql =  runquery.c_str();
			rc = SQLExecDirect(hstmt, (unsigned char*)sql, SQL_NTS);
			if ((rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO))
			{
				SQLFreeStmt(hstmt, SQL_DROP);
				SQLDisconnect(hdbc);
				SQLFreeConnect(hdbc);
				SQLFreeEnv(henv);
				return 0;
			}

			SQLFreeStmt(hstmt, SQL_DROP);
			SQLDisconnect(hdbc);
			SQLFreeConnect(hdbc);
			SQLFreeEnv(henv);
		}

		return 0;
	}

	if (Type == 87)
	{
		int PID = 0, SetGem = 0, IID = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"ddd", &PID, &SetGem, &IID);

		if (PID && IID){
			Executex((SQLHSTMT*)&dbItem, "UPDATE Item SET [SetGem] = %d WHERE [PID] = %d AND [IID] = %d", SetGem, PID, IID);
			CDBx(&dbItem);
			Executex((SQLHSTMT*)&dbItem, "UPDATE Item SET [SetGem] = %d WHERE [PID] = %d AND [IID] = %d AND SetGem!=%d", SetGem, PID, IID,SetGem);
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 88)
	{
		RETCODE rc; HENV henv; HDBC hdbc; HSTMT hstmt;
		int PID = 0, IID = 0; const char *Lock;
		Read((char *)pPos, (unsigned int)"dsd", &PID, &Lock, &IID);

		if (Lock != NULL && IID)
		{
			const char *db = "kal_db";
			SQLAllocEnv(&henv);
			SQLAllocConnect(henv, &hdbc);
			rc = SQLConnect(hdbc, (unsigned char*)db, SQL_NTS, 0, 0, 0, 0);

			if ((rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO))
			{
				SQLFreeConnect(hdbc);
				SQLFreeEnv(henv);
				return 0;
			}

			rc = SQLAllocStmt(hdbc, &hstmt);
			std::stringstream query;
			query << "UPDATE Item SET [Lock] = '" << Lock << "' WHERE [IID] = '" << IID << "'";
			std::string runquery = query.str();
			const char *sql =  runquery.c_str();
			rc = SQLExecDirect(hstmt, (unsigned char*)sql, SQL_NTS);
			if ((rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO))
			{
				SQLFreeStmt(hstmt, SQL_DROP);
				SQLDisconnect(hdbc);
				SQLFreeConnect(hdbc);
				SQLFreeEnv(henv);
				return 0;
			}

			SQLFreeStmt(hstmt, SQL_DROP);
			SQLDisconnect(hdbc);
			SQLFreeConnect(hdbc);
			SQLFreeEnv(henv);
		}

		return 0;
	}

	if (Type == 89)
	{
		int PID = 0, IID = 0, PetLife = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"ddd", &PID, &PetLife, &IID);

		if (PID && IID && PetLife){
			Executex((SQLHSTMT*)&dbItem, "UPDATE Item SET [PetTime] = %d WHERE [PID] = %d AND [IID] = %d", PetLife, PID, IID); 
			CDBx(&dbItem);
			Executex((SQLHSTMT*)&dbItem, "UPDATE Item SET [PetTime] = %d WHERE [PID] = %d AND [IID] = %d", PetLife, PID, IID);
			CDBx(&dbItem);
			Executex((SQLHSTMT*)&dbItem, "UPDATE Item SET [PetTime] = %d WHERE [PID] = %d AND [IID] = %d", PetLife, PID, IID);
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 90)
	{
		int IID = 0, ItemStat = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dd", &ItemStat, &IID);

		if (IID){
			for (int i = 0; i < 30; i++)
				Executex((SQLHSTMT*)&dbItem, "UPDATE Item SET [ItemStat] = %d WHERE [IID] = %d", ItemStat, IID); 
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 91)
	{
		int PID = 0, Quest = 0, Time = 0, Repeat = 0, Count = 0, MonsterID = 0, CountEach = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dddd", &PID, &Quest ,&MonsterID, &CountEach);
		
		if (PID && Quest) {
			if (MonsterID > 0) {
				if (!Executex((SQLHSTMT*)&dbItem, "UPDATE DailyQuests SET [Amount] = %d WHERE [PID] = %d AND [Monster] = %d", CountEach, PID, MonsterID))
					KDBServer::CDB::Execute2((SQLHSTMT*)&dbItem, "INSERT INTO DailyQuests VALUES(%d, %d, %d, %d)", PID, Quest, MonsterID, CountEach);
			}
			else if (MonsterID == -1) 
				Executex((SQLHSTMT*)&dbItem, "DELETE FROM DailyQuests WHERE [PID] = %d AND [Quest] = %d", PID, Quest);
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 92)
	{
		int PID = 0; DWORD QuestID = 0, QuestClear = 0, QuestTime = 0, QuestRepeat = 0, MonsterCount = 0, QuestEach = 0, MonsterID = 0, MonsterEachAmount = 0;
		xCDB ydbItem;
		CDB(&ydbItem);
		Read((char *)pPos, (unsigned int)"d", &PID);
		bool DailyChecked = false;
		if (PID)
		{
			BindDword(&ydbItem, &QuestID);
			BindDword(&ydbItem, &QuestClear);
			BindDword(&ydbItem, &QuestTime);
			BindDword(&ydbItem, &QuestRepeat);
			BindDword(&ydbItem, &MonsterCount);

			if (Execute((SQLHSTMT*)&ydbItem, "SELECT [Quest], [Clear], [QuestTime], [QuestRepeat], [MonsterCount] FROM Quest WHERE [PID] = %d AND [MonsterCount] = 1", PID))
			{
				DailyChecked = true;
				while (Fetch(&ydbItem))
				{
					if (QuestID)
						ServerSend((int)Socket, 56, "dddddd", PID, QuestID, QuestClear, QuestTime, QuestRepeat, MonsterCount);
				}
			}

			if(!DailyChecked)
				return FreeCDB(&ydbItem);

			CDBx(&ydbItem);
			BindDword(&ydbItem, &QuestEach);
			BindDword(&ydbItem, &MonsterID);
			BindDword(&ydbItem, &MonsterEachAmount);
			if (Execute((SQLHSTMT*)&ydbItem, "SELECT [Quest], [Monster], [Amount] FROM DailyQuests WHERE [PID] = %d", PID))
			{
				while (Fetch(&ydbItem))
				{
					if (QuestEach)
						ServerSend((int)Socket, 62, "dddd", PID, QuestEach, MonsterID, MonsterEachAmount);
				}
			}
		}

		return FreeCDB(&ydbItem);
	}

	if (Type == 93)
	{
		int PID=0;
		DWORD BuffID = 0,BuffValue=0, SBKey=0,SBName=0,Time=0;
		xCDB dbItem; CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"d", &PID);
		BindDword(&dbItem, &BuffID);
		BindDword(&dbItem, &BuffValue);
		BindDword(&dbItem, &SBKey);
		BindDword(&dbItem, &SBName);
		BindDword(&dbItem, &Time);

		if (Execute((SQLHSTMT*)&dbItem, "SELECT BuffID, Value, SBKey, SBName, Time FROM SpecialBuffs WHERE (PID = %d) ORDER BY BuffID DESC", PID))
		{
			while (Fetch(&dbItem))
			{
				if (BuffID)
					ServerSend((int)Socket, 57, "dddddd", PID, BuffID, BuffValue, SBKey, SBName, Time);
			}
		}

		CDBx(&dbItem);


		return FreeCDB(&dbItem);
	}
	if (Type == 94)
	{
		int IID = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"d", &IID);

		if (IID)
			Execute((SQLHSTMT*)&dbItem, "UPDATE Item SET Num = 1 WHERE [IID] = %d", IID);

		return FreeCDB(&dbItem);
	}

	//if (Type == 69) {
	//	int PID = 0;
	//	DWORD HouseIndex = 0, Points = 0;
	//	xCDB dbItem;
	//	CDB(&dbItem);
	//	Read((char *)pPos, (unsigned int)"dd", &HouseIndex, &Points);

	//	if (HouseIndex != 0) {
	//		Execute((SQLHSTMT*)&dbItem, "UPDATE Houses SET HousePoints = HousePoints + %d WHERE [HouseIndex] = %d", Points, HouseIndex);
	//	}

	//	CDBx(&dbItem);
	//	return FreeCDB(&dbItem);
	//}

	//if (Type == 70) {
	//	int PID = 0;
	//	DWORD HouseIndex = 0, TotalPlayers = 0;
	//	xCDB dbItem;
	//	CDB(&dbItem);
	//	Read((char *)pPos, (unsigned int)"ddd", &PID, &HouseIndex, &TotalPlayers);
	//	if (HouseIndex != 0) {
	//		if (HouseIndex == 172)
	//			Execute((SQLHSTMT*)&dbItem, "UPDATE Player SET House = 'Slytherin' WHERE PID = %d", PID);
	//		else if (HouseIndex == 171)
	//			Execute((SQLHSTMT*)&dbItem, "UPDATE Player SET House = 'Gryffindor' WHERE PID = %d", PID);
	//		else if (HouseIndex == 175)
	//			Execute((SQLHSTMT*)&dbItem, "UPDATE Player SET House = 'Ravenclaw' WHERE PID = %d", PID);
	//		else if (HouseIndex == 174)
	//			Execute((SQLHSTMT*)&dbItem, "UPDATE Player SET House = 'Hufflepuff' WHERE PID = %d", PID);

	//		Execute((SQLHSTMT*)&dbItem, "UPDATE Houses SET TotalPlayers = TotalPlayers + %d WHERE [HouseIndex] = %d", TotalPlayers, HouseIndex);

	//	}

	//	CDBx(&dbItem);
	//	 FreeCDB(&dbItem);
	//}
//
//	int HouseRewardTime = 0;
//	if (Type == 71) {
////		int PID = 0;
//		DWORD FutureTime = 0;
//		xCDB dbItem;
//		CDB(&dbItem);
//		Read((char *)pPos, (unsigned int)"d", &FutureTime);
//		DWORD RewardTime = 0, HouseIndex = 0, HousePID = 0;
//		BindDword(&dbItem, &RewardTime);
//		BindDword(&dbItem, &HouseIndex);
//		BindDword(&dbItem, &HousePID);
//
//
//
//		if (Execute((SQLHSTMT*)&dbItem, "SELECT h.[RewardDelay], h.[HouseIndex], q.[PID] FROM Houses h JOIN Quest q ON h.[HouseIndex] = q.[Quest] WHERE h.[HouseIndex] = (SELECT TOP 1[HouseIndex] FROM Houses ORDER BY HousePoints DESC) AND q.[Quest] = (SELECT TOP 1[HouseIndex] FROM Houses ORDER BY HousePoints DESC) ORDER BY h.HousePoints DESC ")) {
//			while (Fetch(&dbItem)) {
//				
//				if (RewardTime) {
//		//			Black("Executed xx");
//					ServerSend((int)Socket, 72, "dd", HousePID, RewardTime);
//				}
//			}
//		}
//
//			CDB(&dbItem); // Reopen the connection
//			Execute((SQLHSTMT*)&dbItem, "UPDATE Houses SET RewardDelay = %d, HousePoints = 0", FutureTime);
//				CDBx(&dbItem); // Close the connection again
//			
//		 FreeCDB(&dbItem);
//	}
//



	if (Type == 95)
	{
		xCDB dbItem; CDB(&dbItem);
		int Type = 0, PID = 0,BuffID=0,Value=0,SBKey=0,SBName=0,Time=0;
		Read((char *)pPos, (unsigned int)"ddddddd", &Type,&PID,&BuffID,&Value,&SBKey,&SBName,&Time);
		if (Type && PID)
		{
			if(Type==1){
				Executex((SQLHSTMT*)&dbItem, "DELETE FROM SpecialBuffs WHERE (PID = %d) AND (BuffID = %d)",PID,BuffID);
				Executex((SQLHSTMT*)&dbItem, "INSERT INTO SpecialBuffs (PID, BuffID, Value, SBName, SBKey, Time) VALUES (%d, %d, %d, %d, %d, %d)",PID,BuffID,Value,SBName,SBKey, Time);
			}
			else if(Type==2)
				Executex((SQLHSTMT*)&dbItem, "DELETE FROM SpecialBuffs WHERE (PID = %d) AND (BuffID = %d)",PID,BuffID);
			else if(Type==3)
				Executex((SQLHSTMT*)&dbItem, "UPDATE SpecialBuffs SET Value=%d,SBKey=%d,SBName=%d,Time=%d WHERE [PID] = %d AND [BuffID]=%d", Value,SBKey,SBName,Time,PID,BuffID);
		}

		return FreeCDB(&dbItem);
	}
	
	if (Type == 96)
	{
		int PID = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"d", &PID);

		if (PID)
			Execute((SQLHSTMT*)&dbItem, "UPDATE Item SET SetGem = 0 WHERE [PID] = %d AND [Index]=8771 AND SetGem!=0", PID);

		return FreeCDB(&dbItem);
	}

	if (Type == 97)
	{
		xCDB dbItem; CDB(&dbItem);
		int PID = 0, Index = 0, Amount = 0, Prefix = 0;
		Read((char *)pPos, (unsigned int)"dddd", &PID, &Index, &Amount, &Prefix);
		if (PID && Index && Amount)
			Execute((SQLHSTMT*)&dbItem, "DELETE FROM SpecialItems WHERE (PID = %d) AND ([Index] = %d) AND (Amount = %d) AND (Prefix = %d)", PID, Index, Amount, Prefix);
		
		return FreeCDB(&dbItem);
	}

	if (Type == 98) {
		xCDB dbItem; CDB(&dbItem);
		DWORD PIDExists = 0;
		int PID = 0, Wins = 0, Finals = 0, CurrentWinner = 0;
		Read((char *)pPos, (unsigned int)"dddd", &PID, &Wins, &Finals, &CurrentWinner);

		BindDword(&dbItem, &PIDExists);
		if (Execute((SQLHSTMT*)&dbItem, "SELECT [PID] FROM WorldCup WHERE PID = %d", PID))
			Fetch(&dbItem);
		
		CDBx(&dbItem);

		if (CurrentWinner) {
			Execute((SQLHSTMT*)&dbItem, "UPDATE WorldCup SET CurrentWinner = 0");
			CDBx(&dbItem);
		}

		if (PIDExists)
			Execute((SQLHSTMT*)&dbItem, "UPDATE WorldCup SET Wins = Wins + %d, Finals = Finals + %d, CurrentWinner = %d WHERE [PID] = %d", Wins, Finals, CurrentWinner, PID);
		else
			Execute((SQLHSTMT*)&dbItem, "INSERT INTO WorldCup (PID, Wins, Finals, CurrentWinner) VALUES (%d, %d, %d, %d)", PID, Wins, Finals, CurrentWinner);

		return FreeCDB(&dbItem);
	}

	if (Type == 99) {
		int PID = 0, HP = 0, HK = 0, HD = 0, DKPT = 0, DKPW = 0, PLT = 0, PLW = 0, SVT = 0, SVW = 0, RP = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"ddddddddddd", &PID, &HP, &HK, &HD, &DKPT, &DKPW, &PLT, &PLW, &SVT, &SVW, &RP);

		if (PID)
			Execute((SQLHSTMT*)&dbItem, "UPDATE Player SET [HonorPoint] = HonorPoint+%d, [HonorKill] = HonorKill+%d, [HonorDeath] = HonorDeath+%d, [DKPTotal] = DKPTotal+ %d, [DKPWin] = DKPWin + %d, [PLTotal] = PLTotal + %d, [PLWin] = PLWin+%d, [SVTotal] = SVTotal+%d, [SVWin] = SVWin+%d, [RewardPoint] = RewardPoint+%d WHERE [PID] = %d", HP, HK, HD, DKPT, DKPW, PLT, PLW, SVT, SVW, RP, PID);
		
		return FreeCDB(&dbItem);
	}

	if (Type == 100)
	{
		int UID = 0, Month = 0, Day = 0, PID = 0, Limit = 0, IPCheck = 0, Type = 0;
		const char* IP = "", *HWID = "";
		DWORD Check = 0;
		xCDB dbItem; CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dddddddss", &Type,&UID,&PID,&Month,&Day,&Limit,&IPCheck,&IP,&HWID);

		if (Type == 1) {

			BindDword(&dbItem, &Check);
			if (Execute((SQLHSTMT*)&dbItem, "SELECT UID FROM DailyLogins WHERE (UID = %d AND Month = %d AND Day = %d)", UID, Month, Day))
				Fetch(&dbItem);

			if (Check)
				return FreeCDB(&dbItem);

			CDBx(&dbItem);
			const char* IP2 = IP;
			if (!IPCheck)
				IP2 = "";
			if (Limit) {
				BindDword(&dbItem, &Check);

				if (Execute((SQLHSTMT*)&dbItem, "SELECT COUNT(*) AS Expr1 FROM DailyLogins WHERE ((IP = '%s' OR HWID = '%s') AND Month = %d AND Day = %d)", IP2, HWID, Month, Day))
					Fetch(&dbItem);

				if ((int)Check >= Limit)
					return FreeCDB(&dbItem);

				CDBx(&dbItem);
			}
			Check = 0;

			BindDword(&dbItem, &Check);
			if (Execute((SQLHSTMT*)&dbItem, "SELECT COUNT(*) AS Expr1 FROM DailyLogins WHERE (UID = %d AND Month = %d)", UID, Month))
				Fetch(&dbItem);

			int Dasy = (int)Check + 1;

			ServerSend((int)Socket, 63, "dd", PID, Dasy);
		}
		else if(Type == 2)
			Execute((SQLHSTMT*)&dbItem, "INSERT INTO DailyLogins (UID, Month, Day, IP, HWID) VALUES (%d, %d, %d, '%s', '%s')", UID,Month,Day,IP,HWID);

		return FreeCDB(&dbItem);
	}

	if (Type == 101) {
		int PID = 0, Quest = 0, Daily = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"ddd", &PID, &Quest, &Daily);

		if (PID && Quest) { 
			if (!Daily)
				Execute((SQLHSTMT*)&dbItem, "DELETE FROM Quest WHERE [PID] = %d AND [Quest] = %d", PID, Quest);
			else
				Execute((SQLHSTMT*)&dbItem, "DELETE FROM DailyQuests WHERE [PID] = %d AND [Quest] = %d", PID, Quest);
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 102) {
		xCDB dbItem;
		CDB(&dbItem);

		Execute((SQLHSTMT*)&dbItem, "DELETE FROM SpecialBuffs WHERE BuffID >= %d AND BuffID <= %d", 5000, 5400);
		return FreeCDB(&dbItem);
	}

	if (Type == 103) {
		int PID = 0, Quest = 0;
		xCDB dbItem;
		CDB(&dbItem);

		Read((char *)pPos, (unsigned int)"dd", &PID, &Quest);
		
		Execute((SQLHSTMT*)&dbItem, "UPDATE Quest SET [MonsterCount] = 1 WHERE [PID] = %d AND [Quest] = %d", PID, Quest);
		return FreeCDB(&dbItem);
	}

	if (Type == 104) {
		int PID = 0, Index = 0, Amount = 0, Prefix = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dddd", &PID, &Index, &Amount, &Prefix);

		Execute((SQLHSTMT*)&dbItem, "INSERT INTO SpecialItems (PID,[Index],Amount,Prefix) VALUES(%d,%d,%d,%d)", PID, Index, Amount, Prefix);

		return FreeCDB(&dbItem);
	}

	if (Type == 105) {
		int AuctionType = 0;
		const char* Seller = "", *Bidder = "";
		int PID = 0, IID = 0, BidRemain = 0, CurrentBid = 0, Price = 0, Index = 0, Prefix = 0, Info = 0, Num = 0, MaxEnd = 0, CurEnd = 0, SetGem = 0, XAttack = 0, XMagic = 0, XDefense = 0, XHit = 0, XDodge = 0, Protect = 0, UpgrLevel = 0, UpgrRate = 0, x = 0, y = 0, z = 0, remaintime = 0, dsstype = 0, phyatk = 0, magatk = 0, def = 0, absorb = 0, eva = 0, otp = 0, hpinc = 0, mpinc = 0, str = 0, hp = 0, intel = 0, wis = 0, agi = 0, PerfShotCheck = 0, QIgongGrade = 0, dg1stat = 0, dg1type = 0, a = 0, dg2stat = 0, dg2type = 0, refill = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"sddsdwdbddbbbbbbbbwbbbbbdbwwwwbbbbbbbbbbdbbwbbdd", &Seller, &BidRemain, &Price, &Bidder, &CurrentBid, &Index, &IID, &Prefix, &Info, &Num, &MaxEnd, &CurEnd, &SetGem, &XAttack, &XMagic, &XDefense, &XHit, &XDodge, &Protect, &UpgrLevel, &UpgrRate, &x, &y, &z, &remaintime, &dsstype, &phyatk, &magatk, &def, &absorb, &eva, &otp, &hpinc, &mpinc, &str, &hp, &intel, &wis, &agi, &PerfShotCheck, &QIgongGrade, &dg1stat, &dg1type, &a, &dg2stat, &dg2type, &refill, &PID);

		KDBServer::CDB::Execute2((SQLHSTMT*)&dbItem, "INSERT INTO Auction (IID, Type, Seller, Bidder, BidRemain, CurrentBid, Price, [Index], Prefix, Info, Num, MaxEnd, CurEnd, SetGem, XAttack, XMagic, XDefense, XHit, XDodge, Protect, UpgrLevel, UpgrRate, x, y, z, remaintime, dsstype, phyatk, magatk, def, absorb, eva, otp, hpinc, mpinc, str, hp, intel, wis, agi, PerfShotCheck, QIgongGrade, dg1stat, dg1type, a, dg2stat, dg2type, refill, PID) VALUES(%d, %d, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)", IID, 0, Seller, Bidder, BidRemain, CurrentBid, Price, Index, Prefix, Info, Num, MaxEnd, CurEnd, SetGem, XAttack, XMagic, XDefense, XHit, XDodge, Protect, UpgrLevel, UpgrRate, x, y, z, !remaintime ? 1 : remaintime , dsstype, phyatk, magatk, def, absorb, eva, otp, hpinc, mpinc, str, hp, intel, wis, agi, PerfShotCheck, QIgongGrade, dg1stat, dg1type, a, dg2stat, dg2type, refill, PID);

		return FreeCDB(&dbItem);
	}

	if (Type == 106) {

		const char* Name = "";
		int IID = 0, BidPID = 0, BidAmount = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"ddds", &IID, &BidPID,&BidAmount, &Name);

		Execute((SQLHSTMT*)&dbItem, "UPDATE Auction SET [Bidder] = '%s', [RPID] = %d, [CurrentBid] = %d WHERE [IID] = %d", Name, BidPID, BidAmount, IID);

		return FreeCDB(&dbItem);
	}

	if (Type == 107) {

		int IID = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"d", &IID);

		Execute((SQLHSTMT*)&dbItem, "DELETE FROM Auction WHERE [IID] = %d", IID);

		return FreeCDB(&dbItem);
	}

	if (Type == 108) {
		int IID = 0, PID = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dd", &PID, &IID);

		Execute((SQLHSTMT*)&dbItem, "UPDATE Item SET PID = %d WHERE [IID] = %d", PID, IID);

		return FreeCDB(&dbItem);
	}

	if (Type == 109) {
		int Index = 0, Type = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dd", &Index, &Type);

		Execute((SQLHSTMT*)&dbItem, "IF NOT EXISTS (SELECT * FROM InventoryCheck WHERE [Index] = %d) INSERT INTO InventoryCheck([Index],Type) VALUES(%d, %d)", Index, Index, Type);

		if (!Type)
			Ridings.insert(Index);

		return FreeCDB(&dbItem);
	}

	if (Type == 110) {
		int PID = 0;
		Read((char *)pPos, (unsigned int)"d", &PID);

		if (NewPlayer.count(PID)) {
			ServerSend((int)Socket, 65, "d", PID);
			NewPlayer.erase(PID);
		}

		return 0;
	}

	if (Type == 111)
	{
		int Monster = 0, Value = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dd", &Monster, &Value);

		if (Monster) {
			if (!Execute((SQLHSTMT*)&dbItem, "UPDATE MonsterTrack SET Alive = Alive + %d WHERE [Index] = %d", Value, Monster)) {
				if (Value == -1)
					Value = 0;
				
				Execute((SQLHSTMT*)&dbItem, "INSERT INTO MonsterTrack VALUES(%d, %d)", Monster, Value);
			}
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 112)
	{
		int IID = 0;;
		Read((char *)pPos, (unsigned int)"d", &IID);

		if (IID)
		{
			xCDB ydbItem;
			CDB(&ydbItem);

			DWORD Index = 0, Prefix = 0, MaxEnd = 0, CurEnd = 0, Info = 0, Num = 0, XHit = 0, XDefense = 0, XAttack = 0, XMagic = 0, XDodge = 0, Protect = 0, UpgrLevel = 0, SetGem = 0, ItemStat = 0;

			BindDword(&ydbItem, &Index);
			BindDword(&ydbItem, &MaxEnd);
			BindDword(&ydbItem, &CurEnd);
			BindDword(&ydbItem, &Info);
			BindDword(&ydbItem, &Num);
			BindDword(&ydbItem, &XHit);
			BindDword(&ydbItem, &XDefense);
			BindDword(&ydbItem, &XAttack);
			BindDword(&ydbItem, &XMagic);
			BindDword(&ydbItem, &XDodge);
			BindDword(&ydbItem, &Protect);
			BindDword(&ydbItem, &UpgrLevel);
			BindDword(&ydbItem, &SetGem);
			BindDword(&ydbItem, &Prefix);
			BindDword(&ydbItem, &ItemStat);
			if (Execute((SQLHSTMT*)&ydbItem, "SELECT [Index],[MaxEnd],[CurEnd],[Info],[Num],[XHit],[XDefense],[XAttack],[XMagic],[XDodge],[Protect],[UpgrLevel],[SetGem],[Prefix],[ItemStat] FROM [Item] WHERE [IID] = %d", IID))
			{
				Fetch(&ydbItem);
				if (Index)
					ServerSend((int)Socket, 66, "dddddddddddddddd", IID, Index, MaxEnd, CurEnd, Info, Num, XHit, XDefense, XAttack, XMagic, XDodge, Protect, UpgrLevel, SetGem, Prefix, ItemStat);
			}
			FreeCDB(&ydbItem);
		}

		return 1;
	}

	if (Type == 113)
	{
		int PID = 0, GID = 0, Count = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"ddd", &PID, &GID, &Count);

		if (PID && GID) {
			if (!Executex((SQLHSTMT*)&dbItem, "UPDATE GuildBuff SET [Amount] = [Amount] + %d WHERE [PID] = %d AND [GID] = %d", Count, PID, GID))
				KDBServer::CDB::Execute2((SQLHSTMT*)&dbItem, "INSERT INTO GuildBuff VALUES(%d, %d, %d)", PID, GID, Count);
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 114)
	{
		xCDB dbItem;
		CDB(&dbItem);
		
		std::ifstream filenwu("./Configs/DailyQuest.txt");
		if (filenwu.good())
		{
			std::string line;
			while (getline(filenwu, line))
			{
				int DailyQuesty = 0, DailyLevel = 0, DailyMonster = 0, DailyAmount = 0, DailyRepeat = 0, DailyTime = 0, DailyItem = 0, DailyIAmount = 0, Honor = 0, RewardPt = 0, Party = 0, Resetable = 0;
				char monsters[BUFSIZ];
				char amounts[BUFSIZ];
				char items[BUFSIZ];
				char iamounts[BUFSIZ];
				signed __int64 DailyExp = 0;

				if (sscanf(line.c_str(), "(dailyquest (quest %d)(level %d)(party %d)(monster %[0-9\\,])(amount %[0-9\\,])(maxrepeat %d)(timelimit %d)(item %[0-9\\,])(amount %[0-9\\,])(exp %lld)(honor %d)(reward %d)(resetable %d))", &DailyQuesty, &DailyLevel, &Party, &monsters, &amounts, &DailyRepeat, &DailyTime, &items, &iamounts, &DailyExp, &Honor, &RewardPt, &Resetable) == 13)
				{
					if (Resetable == 1) {
						Execute((SQLHSTMT*)&dbItem, "UPDATE Quest SET [QuestTime] = 0, [QuestRepeat] = 0 WHERE [Quest] = %d", DailyQuesty);
						CDBx(&dbItem);
					}
				}
				else if (sscanf(line.c_str(), "(dailyquest (quest %d)(level %d)(party %d)(monsterGroup %[0-9\\,-])(amount %[0-9\\,])(maxrepeat %d)(timelimit %d)(item %[0-9\\,])(amount %[0-9\\,])(exp %lld)(honor %d)(reward %d)(resetable %d))", &DailyQuesty, &DailyLevel, &Party, &monsters, &amounts, &DailyRepeat, &DailyTime, &items, &iamounts, &DailyExp, &Honor, &RewardPt, &Resetable) == 13)
				{
					if (Resetable == 1) {
						Execute((SQLHSTMT*)&dbItem, "UPDATE Quest SET [QuestTime] = 0, [QuestRepeat] = 0 WHERE [Quest] = %d", DailyQuesty);
						CDBx(&dbItem);
					}
				}
			}
			filenwu.close();
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 115) {
		int PID = 0, Index = 0, Amount = 0, Class = 0, Prefix = 0, Info = 0, Attack = 0, Magic = 0, TOA = 0, Upgrade = 0, Defense = 0, Evasion = 0, Endurance = 0, ItemStat = 0;
		const char* Message = "";
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dddddddddddddds", &PID, &Index, &Amount, &Class, &Prefix, &Info, &Attack, &Magic, &TOA, &Upgrade, &Defense, &Evasion, &Endurance, &ItemStat, &Message);

		KDBServer::CDB::Execute2((SQLHSTMT*)&dbItem, "INSERT INTO Reward VALUES(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s')", PID, Index, Amount, Class, Prefix, Info, Attack, Magic, TOA, Upgrade, Defense, Evasion, Endurance, ItemStat, Message);

		return FreeCDB(&dbItem);
	}

	if (Type == 116) {
		int PID = 0;
		DWORD Index = 0, Amount = 0, Class = 0, Prefix = 0, Info = 0, Attack = 0, Magic = 0, TOA = 0, Upgrade = 0, Defense = 0, Evasion = 0, Endurance = 0, ItemStat = 0;
		char Message[550];
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"d", &PID);

		BindDword(&dbItem, &Index);
		BindDword(&dbItem, &Amount);
		BindDword(&dbItem, &Class);
		BindDword(&dbItem, &Prefix);
		BindDword(&dbItem, &Info);
		BindDword(&dbItem, &Attack);
		BindDword(&dbItem, &Magic);
		BindDword(&dbItem, &TOA);
		BindDword(&dbItem, &Upgrade);
		BindDword(&dbItem, &Defense);
		BindDword(&dbItem, &Evasion);
		BindDword(&dbItem, &Endurance);
		BindDword(&dbItem, &ItemStat);
		BindString(&dbItem, Message, 550);

		if (Execute((SQLHSTMT*)&dbItem, "SELECT [Index], [Amount], [Class], [Prefix], [Info], [Attack], [Magic], [TOA], [Upgrade], [Defense], [Evasion], [Endurance], [ItemStat], [Message] FROM Reward WHERE PID = %d", PID))
		{
			while (Fetch(&dbItem))
					ServerSend((int)Socket, 68, "dddddddddddddds", PID, Index, Amount, Class, Prefix, Info, Attack, Magic, TOA, Upgrade, Defense, Evasion, Endurance, ItemStat, Message);
			
			CDBx(&dbItem);
			Executex((SQLHSTMT*)&dbItem, "DELETE FROM Reward WHERE [PID] = %d", PID);
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 117)
	{
		Read((char *)pPos, (unsigned int)"d", &Output);

		return 0;
	}

	if (Type == 45)
	{
		int PID = 0, GuildTime = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dd", &PID, &GuildTime);

		if (PID)
			Executex((SQLHSTMT*)&dbItem, "DELETE FROM GuildBuff WHERE [PID] = %d", PID);

		FreeCDB(&dbItem);
	}

	if (Type == 118)
	{
		int IID = 0, type = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dd", &IID, &type);

		if (IID) {
			if (type == 1)
			{
				Executex((SQLHSTMT*)&dbItem, "DELETE FROM ItemRestored WHERE IID = %d", IID);
				CDBx(&dbItem);
				Executex((SQLHSTMT*)&dbItem, "DELETE FROM Item WHERE IID = %d", IID);
			}
			else
				Executex((SQLHSTMT*)&dbItem, "UPDATE ItemRestored SET PID = 0 WHERE IID = %d", IID);
		}

		return FreeCDB(&dbItem);
	}

	if (Type == 119) {
		int Reborn = 0, PID = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dd", &PID, &Reborn);

		Execute((SQLHSTMT*)&dbItem, "UPDATE Player SET Reborn = %d WHERE [PID] = %d", Reborn, PID);

		return FreeCDB(&dbItem);
	}

	if (Type == 120) {
		int PID = 0;
		Read((char *)pPos, (unsigned int)"d", &PID);
		DWORD Reborn = 0;
		xCDB dbItem;
		CDB(&dbItem);
		BindDword(&dbItem, &Reborn);

		if (Execute((SQLHSTMT*)&dbItem, "SELECT [Reborn] FROM Player WHERE PID = %d", PID))
		{
			Fetch(&dbItem);

			if (Reborn)
				ServerSend((int)Socket, 70, "dd", PID, Reborn);
		}

		return FreeCDB(&dbItem);
	}

	//if (Type == 121) {
	//	int PID = 0;
	//	Read((char *)pPos, (unsigned int)"d", &PID);
	//	DWORD GPosition = 0;
	//	xCDB dbItem;
	//	CDB(&dbItem);
	//	BindDword(&dbItem, &GPosition);

	//	if (Execute((SQLHSTMT*)&dbItem, "SELECT Class FROM GuildMember WHERE PID = %d", PID))
	//	{
	//		Fetch(&dbItem);

	//		if (GPosition)
	//			ServerSend((int)Socket, 73, "dd", PID, GPosition);
	//	}

	//	return FreeCDB(&dbItem);
	//}
	//	
	//if (Type == 122) {
	//	int PurchaseValue = 0;
	//	Read((char *)pPos, (unsigned int)"d", &PurchaseValue);
	//	DWORD Value = 0;
	//	xCDB dbItem;
	//	CDB(&dbItem);
	//	BindDword(&dbItem, &Value);

	//	Execute((SQLHSTMT*)&dbItem, "UPDATE Store SET TotalWithdraw = TotalWithdraw + %d", PurchaseValue);


	//	if (Execute((SQLHSTMT*)&dbItem, "SELECT [TotalWithdraw] FROM Store"))
	//	{
	//		Fetch(&dbItem);

	//		if (Value)
	//			ServerSend((int)Socket, 74, "d", Value);
	//	}
	//	return FreeCDB(&dbItem);
	//}


	//Delete item
	if (Type == 123)
	{
		int IID = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"d", &IID);

		if (IID)
			Execute((SQLHSTMT*)&dbItem, "DELETE FROM Item WHERE [Index] = %d", IID);

		return FreeCDB(&dbItem);
	}

	//Delete quest
	if (Type == 124)
	{
		int PID = 0;
		int QuestID = 0;
		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dd", &PID, &QuestID);

		if (PID)
			Execute((SQLHSTMT*)&dbItem, "DELETE FROM Quest WHERE [PID] = %d AND [Quest] = %d", PID, QuestID);

		return FreeCDB(&dbItem);
	}


	if (Type == 125)
	{
		int PID = 0, CurEnd = 0, IID = 0, TypeCheck = 0, Num = 0, Instances = 0;
		DWORD QuestD = 0, NormalD = 0, MiniD = 0, InstanceD = 0;

		xCDB dbItem;
		CDB(&dbItem);
		Read((char *)pPos, (unsigned int)"dddddd", &TypeCheck, &PID, &CurEnd, &IID, &Num, &Instances);
		if (TypeCheck == 1)
		{
			if (PID && IID){
				Executex((SQLHSTMT*)&dbItem, "UPDATE Item SET [CurEnd] = %d, [MaxEnd] = %d WHERE [PID] = %d AND [IID] = %d", CurEnd, CurEnd, PID, IID);
				CDBx(&dbItem);
				Executex((SQLHSTMT*)&dbItem, "UPDATE Item SET [CurEnd] = %d, [MaxEnd] = %d WHERE [PID] = %d AND [IID] = %d AND CurEnd!=%d", CurEnd, CurEnd, PID, IID, CurEnd);
			}
		}
		
		if (TypeCheck == 2)
		{
			if (PID && IID){
				Executex((SQLHSTMT*)&dbItem, "UPDATE Item SET [Num] = %d WHERE [PID] = %d AND [IID] = %d", Num, PID, IID);
				CDBx(&dbItem);
				Executex((SQLHSTMT*)&dbItem, "UPDATE Item SET [Num] = %d WHERE [PID] = %d AND [IID] = %d AND Num!=%d", Num, PID, IID, Num);
			}
		}

		if (TypeCheck == 3)
		{
				char Message[550];
				BindString(&dbItem, Message, 550);

				if (Execute((SQLHSTMT*)&dbItem, "SELECT [LastLogin] FROM Player WHERE (PID = %d)", PID)) {
					if (Fetch(&dbItem)) {

						std::string formattedTimestamp = Message;
						size_t dotPos = formattedTimestamp.find('.');
						if (dotPos != std::string::npos) {
							formattedTimestamp = formattedTimestamp.substr(0, dotPos);
						}
						ServerSend((int)Socket, 71, "ds", PID, formattedTimestamp.c_str());
					}
				}
				CDBx(&dbItem);
				Executex((SQLHSTMT*)&dbItem, "UPDATE Player SET LastLogin = getdate() WHERE PID = %d", PID);
		}
		int QuestID = CurEnd;

		if (TypeCheck == 4)
		{
			int NormalMobs = IID;
			int MiniBosses = Num;

			if (!Execute((SQLHSTMT*)&dbItem, "UPDATE DailyDuty SET Quest = %d WHERE PID = %d", QuestID, PID) && NormalMobs == 0 && MiniBosses == 0 && Instances == 0) {
				Execute((SQLHSTMT*)&dbItem, "INSERT INTO DailyDuty (Quest, PID, NormalMobs, MiniBosses, Instances) VALUES (%d, %d, 0, 0, 0)", QuestID, PID);
			}
			else
			{
				if (NormalMobs)
				Executex((SQLHSTMT*)&dbItem, "UPDATE DailyDuty SET NormalMobs = %d WHERE PID = %d AND Quest = %d", NormalMobs, PID, QuestID);
				else if (MiniBosses)
					Executex((SQLHSTMT*)&dbItem, "UPDATE DailyDuty SET MiniBosses = %d WHERE PID = %d AND Quest = %d", MiniBosses, PID, QuestID);
				else if (Instances)
					Executex((SQLHSTMT*)&dbItem, "UPDATE DailyDuty SET Instances = %d WHERE PID = %d AND Quest = %d", Instances, PID, QuestID);

			}
		}

		if (TypeCheck == 5)
		{
			Execute((SQLHSTMT*)&dbItem, "DELETE FROM DailyDuty WHERE [PID] = %d AND [Quest] = %d", PID, QuestID);

		}

		if (TypeCheck == 6)
		{
			BindDword(&dbItem, &QuestD);
			BindDword(&dbItem, &NormalD);
			BindDword(&dbItem, &MiniD);
			BindDword(&dbItem, &InstanceD);

			if (Execute((SQLHSTMT*)&dbItem, "SELECT [Quest], [NormalMobs], [MiniBosses], [Instances] FROM DailyDuty WHERE (PID = %d)", PID)) {
				while (Fetch(&dbItem))
					ServerSend((int)Socket, 73, "ddddd", PID, QuestD, NormalD, MiniD, InstanceD);
			}
		}

		return FreeCDB(&dbItem);
	}


	return Process(Socket,Data);
}