#pragma once



class CWindTableGen
{
public:	
	static void SunacWindowsTable_AllView();//√≈¥∞±Ìold

	static void SunacWindowsTable_TopView();


protected:
	static void WriteDataToTable(AcDbTable *p_table, int p_dataStartRow, int p_floorColumnCount, 
		vector<CString> p_WinFloorColumns, vector<CString> p_floorColumns, const CWindowAndCount& p_winAndCount);

	static void AddXDataForWinTable(AcDbTable *p_table, vAcDbObjectId p_winIds);
	static void GetWinFloorColumns(CWindowAndCount windowAndCount, vector<CString>& floorColumns);

};

