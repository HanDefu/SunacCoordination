#pragma once

#include "AttrRailing.h"

using namespace std;

class CRailingStatistic
{
public:
	int SelectRailings();

	AcDbObjectId InsertTableToCAD(AcGePoint3d insertPos);

protected:
	void RailingClassify(AttrRailing* pAttr);
	vector<pair<AttrRailing, int>> AllRailings(){ return m_allRailings; }

protected:
	vector<pair<AttrRailing, int>> m_allRailings;
};
