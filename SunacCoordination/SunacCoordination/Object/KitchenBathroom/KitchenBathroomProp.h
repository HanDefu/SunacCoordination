
#pragma once

#include "dbmain.h"
#include "../AttrObject.h"
#include "../PrototypeCommonDef.h"


class CKitchenBathroomProp
{
public:
	CKitchenBathroomProp();
	virtual ~CKitchenBathroomProp();

	virtual void AddSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step = 150);
	virtual void AddSize(int p_xLen, int p_yLen);
	void DeleteSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step = 150);
	void DeleteSize(int p_xLen, int p_yLen);
	void ClearSizeList() { m_sizeList.clear(); }

	bool MatchPrototype(int p_xLen, int p_yLen, E_DIRECTION p_doorPos, E_DIRECTION p_windowPos);
	eWindowDoorPos GetWindowDoorPos() const;
	bool GetRotateAngle(E_DIRECTION p_doorPos, E_DIRECTION p_windowPos, int& p_angle, bool& p_needMirror);

	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* filer);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* filer) const;
	
public:
	E_DIRECTION m_doorPos;
	E_DIRECTION m_windowPos;
	vector<PrototypeSize> m_sizeList; //改为排除列表

	int m_minX;
	int m_minY;
	int m_maxX;
	int m_maxY;
};


