#pragma once
#include <vector>
#include "AttrWindow.h"

using namespace std;


class CWinClassify  //1912
{
public:
	CWinClassify();
	~CWinClassify();

	void AddObject(AcDbObjectId p_objId);
	void AddObject(vector<AcDbObjectId> p_objIds);
	bool RemoveObject(AcDbObjectId p_objId);
	void CheckObjectValid(); //检查AcDbObjectId是否有效，针对用户在CAD中直接删除了门窗的情况
	void ClearObjsects(); //清理所有的关联门窗

	bool IsEmpty()const { return m_winsInCad.size() == 0; }
	bool IsObjectIn(AcDbObjectId p_objId)const;

	void Rename(const CString p_newName);

protected:
	bool IsObjectBelongThisClassify(AcDbObjectId p_obj); //判断是否和此门窗分类相同

public:
	AttrWindow m_winAtt;
	vector<AcDbObjectId> m_winsInCad; //在图上的CAD 1912
};


//////////////////////////////////////////////////////////////////////////
//所有涉及门窗编号及编号修改的都通过CWindowAutoName类处理
class CWindowAutoName
{
	CWindowAutoName();
public:
	~CWindowAutoName();

	static CWindowAutoName* GetInstance();

	//自动获取名字, 注意获取并没有添加到分类库中
	CString GetWindowName(const AttrWindow& p_att);

	//检查当前名称是否合理，如有同名的其它窗型则返回false
	bool IsNameValid(const AttrWindow& p_att, CString p_sName);

	//加入窗型列表中
	void AddWindowType(const AttrWindow& p_att, AcDbObjectId p_objId);
	void AddWindowType(const AttrWindow& p_att, vector<AcDbObjectId> p_objIds);


	//将所有窗型重新自动命名
	void AutoNameAllWindow();
	void AutoNameWindows(const vector<AcDbObjectId>& p_ids);

	//将指定窗型重命名
	bool RenameWindow(const CString p_preName, const CString p_newName);

	void CheckObjectValid(); //对现有的检查有效性，用于移除变化后的实体
	void RemoveObject(AcDbObjectId p_id); //门窗参数变化时调用此函数更新
	vector<AcDbObjectId> GetAllIdsByInstantCode(CString p_code);

	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

protected:
	CWinClassify* FindWinClassifyByAtt(const AttrWindow& p_att);
	CWinClassify* FindWinClassifyByInstantCode(const CString p_sCode);

protected:
	vector<CWinClassify> m_allTypeWindows; //所有窗型

};
