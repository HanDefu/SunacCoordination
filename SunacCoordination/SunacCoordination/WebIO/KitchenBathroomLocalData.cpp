#include "stdafx.h"
#include "KitchenBathroomLocalData.h"


CKitchenBathroomLocalData::CKitchenBathroomLocalData()
{
	InitLocalData();
}


CKitchenBathroomLocalData::~CKitchenBathroomLocalData()
{
}

CKitchenBathroomLocalData* CKitchenBathroomLocalData::GetInstance()
{
	static CKitchenBathroomLocalData instance;
	return &instance;
}

void CKitchenBathroomLocalData::InitLocalData()
{
	m_proBathrooms.resize(12);

	m_proBathrooms[0].m_sFileName = L"TI3.dwg";
	m_proBathrooms[0].m_sType = L"TI3";
	m_proBathrooms[0].m_doorPos = E_DIR_BOTTOM;
	m_proBathrooms[0].m_windowPos = E_DIR_TOP;
	m_proBathrooms[0].AddSizeRange(1600, 2450, 1600, 3050);
	m_proBathrooms[0].AddSizeRange(1700, 2450, 1850, 3050);

	m_proBathrooms[1].m_sFileName = L"TI3_g.dwg";
	m_proBathrooms[1].m_sType = L"TI3_g";
	m_proBathrooms[1].m_doorPos = E_DIR_BOTTOM;
	m_proBathrooms[1].m_windowPos = E_DIR_TOP;
	m_proBathrooms[1].AddSizeRange(1600, 2750, 1600, 3200);
	m_proBathrooms[1].AddSizeRange(1700, 2750, 1850, 3200);

	m_proBathrooms[2].m_sFileName = L"TI4.dwg";
	m_proBathrooms[2].m_sType = L"TI4";
	m_proBathrooms[2].m_doorPos = E_DIR_BOTTOM;
	m_proBathrooms[2].m_windowPos = E_DIR_TOP;
	m_proBathrooms[2].AddSizeRange(1600, 3050, 1600, 3500);
	m_proBathrooms[2].AddSizeRange(1700, 3050, 1850, 3500);

	m_proBathrooms[3].m_sFileName = L"TI4_g.dwg";
	m_proBathrooms[3].m_sType = L"TI4_g";
	m_proBathrooms[3].m_doorPos = E_DIR_BOTTOM;
	m_proBathrooms[3].m_windowPos = E_DIR_TOP;
	m_proBathrooms[3].AddSizeRange(1600, 3500, 1600, 3650);
	m_proBathrooms[3].AddSizeRange(1700, 3500, 1850, 3650);

	m_proBathrooms[4].m_sFileName = L"TL3.dwg";
	m_proBathrooms[4].m_sType = L"TL3";
	m_proBathrooms[4].m_doorPos = E_DIR_BOTTOM;
	m_proBathrooms[4].m_windowPos = E_DIR_RIGHT;
	m_proBathrooms[4].AddSizeRange(1700, 1850, 1850, 2300);
	//不支持以下尺寸
	m_proBathrooms[4].DeleteSize(1700, 1850);
	m_proBathrooms[4].DeleteSize(1700, 2000);

	m_proBathrooms[5].m_sFileName = L"TL3_标准淋浴房.dwg";
	m_proBathrooms[5].m_sType = L"TL3_b";
	m_proBathrooms[5].m_doorPos = E_DIR_BOTTOM;
	m_proBathrooms[5].m_windowPos = E_DIR_RIGHT;
	m_proBathrooms[5].AddSizeRange(2000, 1850, 2150, 2300);

	m_proBathrooms[6].m_sFileName = L"TL4.dwg";
	m_proBathrooms[6].m_sType = L"TL4";
	m_proBathrooms[6].m_doorPos = E_DIR_BOTTOM;
	m_proBathrooms[6].m_windowPos = E_DIR_RIGHT;
	m_proBathrooms[6].AddSizeRange(1700, 2450, 1850, 2750);

	m_proBathrooms[7].m_sFileName = L"TL4_标准淋浴房.dwg";
	m_proBathrooms[7].m_sType = L"TL4_b";
	m_proBathrooms[7].m_doorPos = E_DIR_BOTTOM;
	m_proBathrooms[7].m_windowPos = E_DIR_RIGHT;
	m_proBathrooms[7].AddSizeRange(2000, 2450, 2150, 2450);

	m_proBathrooms[8].m_sFileName = L"TU3-1600X2450.dwg";
	m_proBathrooms[8].m_sType = L"TU3";
	m_proBathrooms[8].m_doorPos = E_DIR_LEFT;
	m_proBathrooms[8].m_windowPos = E_DIR_RIGHT;
	m_proBathrooms[8].AddSize(1600, 2450);

	m_proBathrooms[9].m_sFileName = L"TU3-1850X2000.dwg";
	m_proBathrooms[9].m_sType = L"TU3";
	m_proBathrooms[9].m_doorPos = E_DIR_LEFT;
	m_proBathrooms[9].m_windowPos = E_DIR_TOP;
	m_proBathrooms[9].AddSize(1850, 2000);

	m_proBathrooms[10].m_sFileName = L"TU3-1850X2750.dwg";
	m_proBathrooms[10].m_sType = L"TU3";
	m_proBathrooms[10].m_doorPos = E_DIR_LEFT;
	m_proBathrooms[10].m_windowPos = E_DIR_TOP;
	m_proBathrooms[10].AddSize(1850, 2750);

	m_proBathrooms[11].m_sFileName = L"TU4-2000X2750";
	m_proBathrooms[11].m_sType = L"TU4";
	m_proBathrooms[11].m_doorPos = E_DIR_LEFT;
	m_proBathrooms[11].m_windowPos = E_DIR_TOP;
	m_proBathrooms[11].AddSize(2000, 2750);
}

CProBathroom* CKitchenBathroomLocalData::GetBathroomPrototypeByFileName(CString p_sFileName)
{
	for (UINT i = 0; i < m_proBathrooms.size(); i++)
	{
		if (m_proBathrooms[i].m_sFileName == p_sFileName)
		{
			return &m_proBathrooms[i];
			break;
		}
	}
	return NULL;
}
