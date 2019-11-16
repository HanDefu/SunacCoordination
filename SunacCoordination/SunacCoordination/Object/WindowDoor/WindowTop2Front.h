#pragma once
#include "AttrWindow.h"

class CWindowTop2Front
{
public:
	static bool GenFrontFromTop();
	static AcDbObjectId  GenerateWindow(const AttrWindow& curWinAtt, AcGePoint3d pos, eViewDir p_view);

protected:
	static E_DIRECTION GetTopViewWindowDirection(const vector<AcDbExtents> &allExtents); //得到平面窗户的方位，上、下左右
	static vector<double> GetAllXValueInFrontView(const vector<AcDbExtents> &allExtents); //获取各窗户在立面视图上的x值

	static 	AcDbObjectIdArray CopyAllFloorByOneFloor(const AcDbObjectIdArray& oneFloorIds, const vector<AttrWindow> &winAtts);
	;
};

