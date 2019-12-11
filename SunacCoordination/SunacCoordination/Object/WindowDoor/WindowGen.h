#pragma once

#include "AttrWindow.h"
#include "RCWindow.h"


struct CWinTranslationPara
{
	AcGePoint3d insertPos;
	double rotateAngle;
};

struct CWinInsertPara
{
	AcGePoint3d insertPos;  //实际的插入位置，由于旋转角度可能不是左下角点
	AcGePoint3d leftBottomPos;
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
	//生成新的门窗
	static AcDbObjectId  GenerateWindow(AttrWindow curWinAtt, const AcGePoint3d pos, E_DIRECTION p_winDir, bool p_bDetailWnd, const AcDbObjectId p_fromWinId, CString p_sLayerName);

	//更新门窗参数, bUpdateRelatedWin为true则同步更新关联的门窗，p_originalId为最开启发起修改更新的门窗，以便在后续关联修改时不重复修改
	static AcDbObjectId UpdateWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin);//

protected:
	static double GetBlockRotateAngle(const AttrWindow& curWinAtt, eViewDir p_view, E_DIRECTION p_winDir);
	static AcGePoint3d GetBlockInsertPos(const AcDbObjectId p_id, const AttrWindow& curWinAtt, const AcGePoint3d pos, eViewDir p_view, E_DIRECTION p_winDir);
	static void UpdateRcWindowPara(const AcDbObjectId p_id, const AttrWindow& curWinAtt, eViewDir p_view, bool p_bDetailWnd);
	static void AddWinAtt(const AcDbObjectId p_id, AttrWindow p_winAtt);
	static void UpdateWinAtt(const AcDbObjectId p_id, AttrWindow p_winAtt);

	static void ModifyOneWindow(const AcDbObjectId p_id, AttrWindow newWinAtt);

	static bool GetWindowInsertPos(AcDbObjectId p_id, AcGePoint3d &p_insertPos, double& p_angle);
	static AcGePoint3d GetWindowLeftBottomPos(AcDbObjectId p_id);
	static E_DIRECTION GetWindowInsertDir(AcDbObjectId p_id);
	static CWinInsertPara GetWindowInsertPara(AcDbObjectId p_id); //根据已插入的门窗获取其插入的信息
	static bool IsWindowMirror(AcDbObjectId p_id);
	static AttrWindow* GetWinAtt(AcDbObjectId p_id);

	static bool IsPrototypeCodeSame(const AcDbObjectId p_id, const AttrWindow& newWinAtt); //新的原型编号是否和之前的不同
	static bool SetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId p_fromWinId, AcDbObjectIdArray p_relatedIds);
	static bool GetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId& p_fromWinId, AcDbObjectIdArray& p_relatedIds);

	static bool MirrorObjectByCenter(const AcDbObjectId p_id, E_DIRECTION p_winDir);
	static bool DeleteWindowObj(const AcDbObjectId p_id);

	//插入门窗编号
	static AcDbObjectId InsertWindowDoorCode(eViewDir p_viewDir, CString p_number);
	static void MoveWindowDoorCode(AcDbObjectId p_id, double p_winWidth, double p_winHeight, AcGePoint3d p_origin, eViewDir p_viewDir, E_DIRECTION p_winDir);
	static double GetWinWidth(AcDbObjectId p_id);
	static double GetWinHeight(AcDbObjectId p_id);
};


