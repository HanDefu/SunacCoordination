#pragma once
#include <map>
using namespace std;


class CWinTangentOpenMap
{
public:
	//在生成门窗、和appand新门窗（复制、拷贝、镜像等操作)时调用此函数，对门窗和门洞关联
	void AddWindow(AcDbObjectId p_winId, AcDbObjectId p_tangentOpenId);
	void RemoveWindow(AcDbObjectId p_winId);
	AcDbObjectId GetTangentOpenId(AcDbObjectId p_winId);



	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

protected:
	map<AcDbObjectId, AcDbObjectId> m_idsMap;

};