#pragma once
#include "AttrRailing.h"

class CRailingTop2Front
{
public:
	static bool GenFrontFromTop();

protected:
	static bool GetTopViewDirection(E_DIRECTION &windowDir); //得到平面窗户的方位，上、下左右
	static vector<double> GetAllXValueInFrontView(const vector<AcDbExtents> &allExtents, const E_DIRECTION windowDir); //获取各窗户在立面视图上的x值

	static 	AcDbObjectIdArray CopyAllFloorByOneFloor(const AcDbObjectId& oneFloorId, const AttrRailing* pRailingAtt);


	static bool GetBlockInsertPosAndExtent(const AcDbObjectId p_id, AcGePoint3d& p_blockInsertPos, AcDbExtents& p_blockExtent);
};

