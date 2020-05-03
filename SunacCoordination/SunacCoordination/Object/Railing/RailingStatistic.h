#pragma once

#include "AttrRailing.h"
#include <dbtable.h>

using namespace std;

class CRailingStatistic
{
public:
	int SelectRailings();

	AcDbObjectId InsertTableToCAD(AcGePoint3d insertPos);
	void InsertRailingTableToCAD();
	void WriteDataToRailingTable(AcDbTable *p_table, int p_dataStartRow, int p_floorColumnCount, vector<CString> p_floorColumns, const CRailingAndCount& p_railingAndCount);
	void SpliteRailingByType(const CRailingAndCount& railingAndCount);

protected:
	void RailingClassify(const AttrRailing* pAttr);
	vector<pair<AttrRailing, int>> AllRailings(){ return m_allRailings; }

protected:
	vector<pair<AttrRailing, int>> m_allRailings;
	vector<CRailingAndCount> m_tyRailings; //Ã˙“’¿∏∏À
	vector<CRailingAndCount> m_blRailings; //≤£¡ß¿∏∏À
};
