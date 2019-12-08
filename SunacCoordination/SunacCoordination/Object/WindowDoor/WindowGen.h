#pragma once

#include "AttrWindow.h"
#include "RCWindow.h"


struct CWindowInsertPara
{
	AcGePoint3d insertPos;
	AcGePoint3d mirrorBasePt;
	AcGeVector3d mirrorAxis;
	double rotateAngle;
};

class CWindowGen
{
public:
	//生成新的门窗
	static AcDbObjectId  GenerateWindow(const AttrWindow& curWinAtt, const AcGePoint3d pos, E_DIRECTION p_winDir, bool p_bDetailWnd, const AcDbObjectId p_fromWinId, CString p_sLayerName);

	//更新门窗参数, bUpdateRelatedWin为true则同步更新关联的门窗，p_originalId为最开启发起修改更新的门窗，以便在后续关联修改时不重复修改
	static AcDbObjectId UpdateWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin, const AcDbObjectId p_originalId );//

protected:
	static CWindowInsertPara InitInsertPara(const AttrWindow& curWinAtt, const AcGePoint3d pos, eViewDir p_view, E_DIRECTION p_winDir);
	static void UpdateRcWindowPara(RCWindow &oneWindow, const AttrWindow& curWinAtt, eViewDir p_view, bool p_bDetailWnd);

};


