#pragma once

#include <vector>
#include <map>
#include "AttrWindow.h"
#include "..\..\Common\TYRect.h"
#include "WindowSelect.h"

using namespace std;

class CDocumentData;

//门窗编号等管理类,通过门窗查找编号文字，或者通过编号文字查找门窗
class CInstanceCodeTextMrg
{
	friend CDocumentData;
	CInstanceCodeTextMrg();
public:
	~CInstanceCodeTextMrg();

	void AddInstanceCode(const CSunacObjInCad &p_sunacObj, AcDbObjectId p_textId);

	void RemoveInstanceCode(const CSunacObjInCad & p_sunacObj);
	void RemoveInstanceCodeByWinId(AcDbObjectId p_winId);
	void RemoveInstanceCodeText(AcDbObjectId p_textId);
	void RemoveInvalidInstanceCode();  //移除无效的门窗编号，无效的门窗编号为门窗本体被删除的编号
	void RemoveAll();

	AcDbObjectId FindTextId(const CSunacObjInCad &p_sunacObj);
	bool SetNewInstanceCode(const CSunacObjInCad &p_sunacObj, CString p_newTextCode);//更新编号值

	bool IsTextIdIn(AcDbObjectId p_textId);

	//得到当前图纸范围内的所有的门窗编号文字的id
	static vector<AcDbObjectId> GetAllInstanceCodeIds();
	static vector<AcDbObjectId> GetInstanceCodeIdsInRect(const TYRect p_rect);

	
	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

protected:
	map<CSunacObjInCad, AcDbObjectId> m_instanceMap;
};