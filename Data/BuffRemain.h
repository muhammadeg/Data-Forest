/*int __stdcall BuffRemainExt(int PID, int BuffID, int Time, int Type)
{
  if ( Type )
  {
    if ( Type == 1 ){
		xCDB dbItem;
		CDB(&dbItem);
		DWORD SBKey = 0,SBName=0,Value=0,Time=0;
		BindDword(&dbItem, &SBKey);
		BindDword(&dbItem, &SBName);
		BindDword(&dbItem, &Time);
		BindDword(&dbItem, &Value);

		if (Execute((SQLHSTMT*)&dbItem, "SELECT [SBKey], [SBName], [Time], [Value] FROM SpecialBuffs WHERE [PID] = %d AND [BuffID]=%d", PID,BuffID))
		{
			Fetch(&dbItem);
			PlayerBuffs[(PID*10000)+BuffID].SBKey = SBKey;
			PlayerBuffs[(PID*10000)+BuffID].SBName = SBName;
			PlayerBuffs[(PID*10000)+BuffID].Time = Time;
			PlayerBuffs[(PID*10000)+BuffID].Value = Value;
			PlayerBuffs[(PID*10000)+BuffID].Timestamp = GetTickCount()+500;

			ServerSend(PlayerSockets.find(PID)->second, 58, "ddd", PID, SBKey, SBName);
		}
		 FreeCDB(&dbItem);
			
		 CDB(&dbItem);
		 Execute((SQLHSTMT*)&dbItem, "DELETE FROM SpecialBuffs WHERE (PID = %d) AND (BuffID = %d)",PID,BuffID);
		 FreeCDB(&dbItem);
	}else if(Type == 2){
		if(PlayerBuffs.count((PID*10000)+BuffID))
			PlayerBuffs.erase((PID*10000)+BuffID);
	}
  }else{
	  if(PlayerBuffs.count((PID*10000)+BuffID)){
			if( PlayerBuffs.find((PID*10000)+BuffID)->second.Time >= (int)time(0) && PlayerBuffs.find((PID*10000)+BuffID)->second.Timestamp>=(int)GetTickCount()){
				xCDB dbItem;
				CDB(&dbItem);
				Execute((SQLHSTMT*)&dbItem, "INSERT INTO SpecialBuffs (PID, BuffID, Value, SBName, SBKey, Time) VALUES (%d, %d, %d, %d, %d, %d)",PID,BuffID,PlayerBuffs.find((PID*10000)+BuffID)->second.Value,PlayerBuffs.find((PID*10000)+BuffID)->second.SBKey,PlayerBuffs.find((PID*10000)+BuffID)->second.SBName, PlayerBuffs.find((PID*10000)+BuffID)->second.Time);
				ServerSend(PlayerSockets.find(PID)->second, 59, "dddd", PID, PlayerBuffs.find((PID*10000)+BuffID)->second.SBKey, PlayerBuffs.find((PID*10000)+BuffID)->second.SBName,PlayerBuffs.find((PID*10000)+BuffID)->second.Time);
				FreeCDB(&dbItem);
		  }
			 PlayerBuffs.erase((PID*10000)+BuffID);
	  }
  }

  return BuffRemain(PID,BuffID,Time,Type);
}
*/