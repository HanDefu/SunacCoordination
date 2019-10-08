#pragma once

#include "../Object/PrototypeInfo.h"
#include <vector>

using namespace std;

class CKitchenBathroomLocalData
{
public:
	CKitchenBathroomLocalData();
	virtual ~CKitchenBathroomLocalData();

	static CKitchenBathroomLocalData* GetInstance();

	void InitLocalData();

	CProBathroom* GetBathroomPrototypeByFileName(CString p_sFileName);
	vector<CProBathroom> GetAllBathroomPrototypes() { return m_proBathrooms; }

protected:
	vector<CProBathroom> m_proBathrooms;
};

