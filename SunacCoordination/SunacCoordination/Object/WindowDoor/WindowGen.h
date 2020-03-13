#pragma once

#include "AttrWindow.h"
#include "RCWindow.h"
#include "WindowSelect.h"


struct CWinTranslationPara
{
	AcGePoint3d insertPos;
	double rotateAngle;
};

struct CWinInsertPara
{
	AcGePoint3d insertPos;  //实际的插入位置，由于旋转角度可能不是左下角点
	AcGePoint3d leftBottomPos;
	AcGePoint3d rightTopPos;
	eViewDir viewDir;
	E_DIRECTION insertDir;
	bool bDetailWnd;
	CString sLayerName;

	AcDbObjectId fromWinId;
	AcDbObjectIdArray relatedWinIds;
};



class CWindowGen
{
public:
	//生成新的门窗 pos为窗户左下角点
	static AcDbObjectId  GenerateWindow(AttrWindow curWinAtt, const AcGePoint3d pos, E_DIRECTION p_winDir, bool p_bDetailWnd, const AcDbObjectId p_fromWinId); 

	//更新门窗参数, bUpdateRelatedWin为true则同步更新关联的门窗，p_originalId为最开启发起修改更新的门窗，以便在后续关联修改时不重复修改, bUpdateSameInstanceCode为是否更新同编号的门窗实例
	static AcDbObjectId UpdateWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin, bool bUpdateSameInstanceCode);//
	
	static bool RenameWindow(const AcDbObjectId p_id, CString p_sNewName, const bool bUpdateRelatedWin); //只是重命名，不更改其他

	static void AutoNameAllWindow();

protected:
	static AcDbObjectId UpdateOneWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin);//

	static double GetBlockRotateAngle(const AttrWindow& curWinAtt, eViewDir p_view, E_DIRECTION p_winDir);
	static AcGePoint3d GetBlockInsertPos(const AcDbObjectId p_id, const AttrWindow& curWinAtt, const AcGePoint3d pos, eViewDir p_view, E_DIRECTION p_winDir);
	static void UpdateRcWindowPara(const AcDbObjectId p_id, const AttrWindow& curWinAtt, eViewDir p_view, bool p_bDetailWnd);
	static void AddWinAtt(const AcDbObjectId p_id, const AttrWindow&  p_winAtt);
	static void UpdateWindowsAttribute(const AcDbObjectId p_id, const AttrWindow&  p_winAtt);

	static void ModifyOneWindow(const AcDbObjectId p_id, AttrWindow newWinAtt);

	static bool GetWindowInsertPos(AcDbObjectId p_id, AcGePoint3d &p_insertPos, double& p_angle);
	static AcGePoint3d GetWindowLeftBottomPos(AcDbObjectId p_id);
	static bool GetWindowInsertDir(AcDbObjectId p_id, E_DIRECTION & p_dirOut);
	static CWinInsertPara GetWindowInsertPara(AcDbObjectId p_id); //根据已插入的门窗获取其插入的信息

	static bool IsPrototypeCodeSame(const AcDbObjectId p_id, const AttrWindow& newWinAtt); //新的原型编号是否和之前的不同
	static bool SetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId p_fromWinId, AcDbObjectIdArray p_relatedIds);
	static bool GetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId& p_fromWinId, AcDbObjectIdArray& p_relatedIds);

	static bool MirrorObjectByCenter(const AcDbObjectId p_id, E_DIRECTION p_winDir);
	static bool DeleteWindowObj(const AcDbObjectId p_id);

	//插入门窗编号
	static AcDbObjectId InsertWindowDoorCode(eViewDir p_viewDir, CString p_number, AcGePoint3d p_pos);
	static void CreateWindowDoorCode(eViewDir p_viewDir, CWinInCad p_win, CString p_Code);
	static double GetWinWidth(AcDbObjectId p_id);
	static double GetWinHeight(AcDbObjectId p_id);


	//////////////////////////////////////////////////////////////////////////
	static bool SelectWindows(vector<CWinInCad>& p_winsOut, vector<AcDbObjectId>& p_textIdsOut, bool &p_bAllOut);
};


