#pragma once
#include <vector>
#include <map>
#include "AttrWindow.h"
#include "..\..\Common\TYRect.h"

using namespace std;


//CWinClassify表示某个编号的门窗，及对应的id(多个)
class CWinClassify
{
public:
	CWinClassify();
	~CWinClassify();

	void AddObject(AcDbObjectId p_objId);
	void AddObject(vector<AcDbObjectId> p_objIds);
	bool RemoveObject(AcDbObjectId p_objId);
	void CheckAndRemoveObjectNotBelong(); //检查所含的AcDbObjectId是否有效，并移除已删除或者和此类型属性不一致的实体
	void ClearObjsects(); //清理所有的关联门窗

	bool IsEmpty()const { return m_winsInCad.size() == 0; }
	bool IsObjectIn(AcDbObjectId p_objId)const;

	void Rename(const CString p_newName);

	vector<AcDbObjectId> GetObjects() { return m_winsInCad; }

protected:
	bool IsObjectBelongThisClassify(AcDbObjectId p_obj); //判断是否和此门窗分类相同

public:
	AttrWindow m_winAtt;
	vector<AcDbObjectId> m_winsInCad; //在图上的CAD 1912
};

class CDocumentData;
//////////////////////////////////////////////////////////////////////////
//所有涉及门窗编号及编号修改的都通过CWindowAutoName类处理
//----------此类只管理门窗编号，不对门窗本身实体做任何处理和修改-------------
//移除原来的门窗时要调用RemoveObject， 
//新生成门窗时要调用AddWindowType
//修改门窗属性时存在两种情况
//    情况1：只修改当前门窗及关联门窗（由此门窗生成的立面图）,先调用RemoveObject，再调用AddWindowType
//    情况2：将此编号的门窗全部变更。可能会变更门窗编号，调用RenameWindows
class CWindowAutoName
{
	friend CDocumentData;
	CWindowAutoName();
public:
	~CWindowAutoName();


	//通过AttrWindow的属性值自动获取名字, 注意获取并没有添加到分类库中
	CString GetWindowName(const AttrWindow& p_att);

	//检查当前名称是否合理，如有同名的其它窗型则返回false
	bool IsNameValid(const AttrWindow& p_att, CString p_sName);

	//加入窗型列表中
	bool AddWindowType(const CString p_sInstanceCode, AcDbObjectId p_objId);
	bool AddWindowType(const AttrWindow& p_att, AcDbObjectId p_objId);
	bool AddWindowType(const AttrWindow& p_att, vector<AcDbObjectId> p_objIds);
	bool AddObject(AcDbObjectId p_id);
	void RemoveObject(AcDbObjectId p_id); //门窗移除时调用此函数更新
	void RemoveObjectsByInstantCode(CString p_instanceCode);//移除某个门窗编号的对象
	bool UpdateObject(AcDbObjectId p_id);//门窗参数变化时调用此函数更新，包括名称变化，属性变化
	bool UpdateObject(const AttrWindow& p_oldAtt, const AttrWindow& p_newAtt); //某个类型的门窗全部调整为新的类型
	bool RenameWindows(const CString p_preName, const CString p_newName);//将指定窗型重命名
	
	bool IsInstanceCodeExist(CString p_code);
	vector<AcDbObjectId> GetAllIdsByInstantCode(CString p_code);
	vector<AcDbObjectId> GetAllIds();


	void CheckAndRemoveObjectNotBelong(); //检查所含的AcDbObjectId是否有效，并移除已删除或者和此类型属性不一致的实体
	void RemoveAllObjects(); //移除所有的object，但是保留原来的名称库

	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);


	static CString GetMirrorInstanceCode(CString p_name);
	static bool IsSamePrototypeAndSize(CString p_instanceCode1, CString p_instanceCode2); //两个编号是否为相同的门窗原型和尺寸
protected:
	CWinClassify* FindWinClassifyByAtt(const AttrWindow& p_att);
	CWinClassify* FindWinClassifyByInstantCode(const CString p_sCode);
	CWinClassify* FindWinClassifyByObjectId(const AcDbObjectId& p_id);

	void RemoveEmptyClassify();

protected:
	vector<CWinClassify> m_allTypeWindows; //所有窗型
};

