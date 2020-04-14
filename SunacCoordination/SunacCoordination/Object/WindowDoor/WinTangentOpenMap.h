#pragma once
#include <map>
using namespace std;


class CWinTangentOpenMap
{
public:
	void AddWindow(AcDbObjectId p_winId, AcDbObjectId p_tangentOpenId);
	AcDbObjectId GetTangentOpenId(AcDbObjectId p_winId);


protected:
	map<AcDbObjectId, AcDbObjectId> m_idsMap;

};