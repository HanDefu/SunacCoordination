#pragma once
#include <vector>
#include "../Sunac_def.h"
#include "PrototypeCommonDef.h"

using namespace std;

//struct PrototypeSize
//{
//	int xLen;
//	int yLen;
//};

class CProBase
{
public:
	CProBase();
	virtual ~CProBase();

	virtual void AddSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step = 150);
	virtual void AddSize(int p_xLen, int p_yLen);
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
public:
	virtual void AddSize(int p_xLen, int p_yLen);
};

class CProKitchen : public CProBase
{
public:
	bool m_bHasPaiQiDao;
};

class CProMrg
{
public:
	static CProMrg* GetInstance();

	CProBase* GetPrototypeByFileName(CString p_sFileName);

	CProBathroom* GetProBathroomByFileName(CString p_sFileName);
	vector<CProBathroom> GetAllProBathrooms() { return m_allProBathrooms; }
	void AddProBathroom(const CProBathroom& p_proBathroom);

	CProKitchen* GetProKitchenByFileName(CString p_sFileName);
	vector<CProKitchen> GetAllProKitchens() { return m_allProKitchens; }
	void AddProKitchen(const CProKitchen& p_proKitchen);

protected:
	vector<CProBathroom> m_allProBathrooms;
	vector<CProKitchen> m_allProKitchens;
};