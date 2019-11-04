#pragma once

#include "AttrAirCon.h"

using namespace std;

class CAirConStatistic
{
public:
	int SelectAirCons();
	void InsertAirCon(AttrAirCon* pAttr);

	AcDbObjectId InsertTableToCAD(AcGePoint3d insertPos);

protected:
	vector<pair<double, int>> m_allAirCons;
};
