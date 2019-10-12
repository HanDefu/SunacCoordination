#pragma once

#include "../Object/PrototypeInfo.h"
#include "..\Object\Kitchen\AttrKitchen.h"
#include "..\Object\Bathroom\AttrBathroom.h"

using namespace std;

class CKitchenBathroomLocalData
{
public:
	CKitchenBathroomLocalData();
	virtual ~CKitchenBathroomLocalData();

	std::vector<AttrKitchen> GetKitchens(EKitchType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_hasPaiQiDao);
	std::vector<AttrKitchen> GetAllKitchens() { return m_allKitchens; }

	std::vector<AttrBathroom> GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir);
	std::vector<AttrBathroom> GetAllBathrooms() { return m_allBathrooms; }

protected:
	void InitKitchenLocalData();
	void InitBathroomLocalData();

protected:
	std::vector<AttrKitchen> m_allKitchens;
	std::vector<AttrBathroom> m_allBathrooms;
};

