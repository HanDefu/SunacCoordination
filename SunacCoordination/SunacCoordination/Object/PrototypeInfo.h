#pragma once
#include <vector>
#include "../Sunac_def.h"
#include "PrototypeCommonDef.h"

using namespace std;

struct PrototypeSize
{
	int xLen;
	int yLen;
};

class CProBase
{
public:
	CProBase();
	virtual ~CProBase();

	void AddSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step = 150);
	void AddSize(int p_xLen, int p_yLen);
	void DeleteSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step = 150);
	void DeleteSize(int p_xLen, int p_yLen);
	void ClearSizeList() { m_sizeList.clear(); }

	bool MatchPrototype(int p_xLen, int p_yLen, E_DIRECTION p_doorPos, E_DIRECTION p_windowPos);
	eWindowDoorPos GetWindowDoorPos() const;
	bool GetRotateAngle(E_DIRECTION p_doorPos, E_DIRECTION p_windowPos, int& p_angle, bool& p_needMirror);

	CString m_sFileName;
	CString m_sType;

	E_DIRECTION m_doorPos;
	E_DIRECTION m_windowPos;

	bool m_bIsDynamic;

protected:
	vector<PrototypeSize> m_sizeList; //支持的尺寸列表
};

class CProBathroom : public CProBase
{

};

class CProKitchen : public CProBase
{

};