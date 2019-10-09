#include "stdafx.h"
#include "KitchenBathroomLocalData.h"


CKitchenBathroomLocalData::CKitchenBathroomLocalData()
{
	InitKitchenLocalData();
	InitBathroomLocalData();
}


CKitchenBathroomLocalData::~CKitchenBathroomLocalData()
{
}

void CKitchenBathroomLocalData::InitKitchenLocalData()
{
	vector<CProKitchen> proKitchens(7);

	proKitchens[0].m_sFileName = L"KUq_Dui.dwg";
	proKitchens[0].m_sType = L"KUq";
	proKitchens[0].m_doorPos = E_DIR_BOTTOM;
	proKitchens[0].m_windowPos = E_DIR_TOP;
	proKitchens[0].AddSizeRange(2450, 1700, 3500, 1850);
	proKitchens[0].DeleteSize(2450, 1700);

	proKitchens[1].m_sFileName = L"KUq_Dui_ZhuanJiao.dwg";
	proKitchens[1].m_sType = L"KUq_z";
	proKitchens[1].m_doorPos = E_DIR_BOTTOM;
	proKitchens[1].m_windowPos = E_DIR_TOP;
	proKitchens[1].AddSizeRange(2450, 2000, 3500, 2600);

	proKitchens[2].m_sFileName = L"KUq_Chui.dwg";
	proKitchens[2].m_sType = L"KUq_c";
	proKitchens[2].m_doorPos = E_DIR_BOTTOM;
	proKitchens[2].m_windowPos = E_DIR_RIGHT;
	proKitchens[2].AddSizeRange(2450, 1700, 3500, 1850);
	proKitchens[2].DeleteSize(2450, 1700);

	proKitchens[3].m_sFileName = L"KUq_Chui_ZhuanJiao.dwg";
	proKitchens[3].m_sType = L"KUq_c_z";
	proKitchens[3].m_doorPos = E_DIR_BOTTOM;
	proKitchens[3].m_windowPos = E_DIR_RIGHT;
	proKitchens[3].AddSizeRange(2450, 2000, 3500, 2450);

	proKitchens[4].m_sFileName = L"KUs.dwg";
	proKitchens[4].m_sType = L"KUs";
	proKitchens[4].m_doorPos = E_DIR_BOTTOM;
	proKitchens[4].m_windowPos = E_DIR_TOP;
	proKitchens[4].AddSizeRange(2300, 2450, 2450, 3200);

	proKitchens[5].m_sFileName = L"KL.dwg";
	proKitchens[5].m_sType = L"KL";
	proKitchens[5].m_doorPos = E_DIR_LEFT;
	proKitchens[5].m_windowPos = E_DIR_RIGHT;
	proKitchens[5].AddSizeRange(2600, 1700, 3950, 2000);

	proKitchens[6].m_sFileName = L"KI.dwg";
	proKitchens[6].m_sType = L"KI";
	proKitchens[6].m_doorPos = E_DIR_LEFT;
	proKitchens[6].m_windowPos = E_DIR_RIGHT;
	proKitchens[6].AddSizeRange(3050, 1700, 4100, 1700);

	for (UINT i = 0; i < proKitchens.size(); i++)
		proKitchens[i].m_bHasPaiQiDao = false;

	//创建包含排气道的原型
	vector<CProKitchen> proKitchens_P = proKitchens;

	for (UINT i = 0; i < proKitchens_P.size(); i++)
	{
		proKitchens_P[i].m_sFileName.Replace(L".dwg", L"_P.dwg");
		proKitchens_P[i].m_sType += L"_p";
		proKitchens_P[i].m_bHasPaiQiDao = true;
	}

	for (UINT i = 0; i < proKitchens.size(); i++)
	{
		CProMrg::GetInstance()->AddProKitchen(proKitchens[i]);
		CProMrg::GetInstance()->AddProKitchen(proKitchens_P[i]);
	}
}

void CKitchenBathroomLocalData::InitBathroomLocalData()
{
	vector<CProBathroom> proBathrooms(12);

	proBathrooms[0].m_sFileName = L"TI3.dwg";
	proBathrooms[0].m_sType = L"TI3";
	proBathrooms[0].m_doorPos = E_DIR_BOTTOM;
	proBathrooms[0].m_windowPos = E_DIR_TOP;
	proBathrooms[0].AddSizeRange(1600, 2450, 1600, 3050);
	proBathrooms[0].AddSizeRange(1700, 2450, 1850, 3050);

	proBathrooms[1].m_sFileName = L"TI3_g.dwg";
	proBathrooms[1].m_sType = L"TI3_g";
	proBathrooms[1].m_doorPos = E_DIR_BOTTOM;
	proBathrooms[1].m_windowPos = E_DIR_TOP;
	proBathrooms[1].AddSizeRange(1600, 2750, 1600, 3200);
	proBathrooms[1].AddSizeRange(1700, 2750, 1850, 3200);

	proBathrooms[2].m_sFileName = L"TI4.dwg";
	proBathrooms[2].m_sType = L"TI4";
	proBathrooms[2].m_doorPos = E_DIR_BOTTOM;
	proBathrooms[2].m_windowPos = E_DIR_TOP;
	proBathrooms[2].AddSizeRange(1600, 3050, 1600, 3500);
	proBathrooms[2].AddSizeRange(1700, 3050, 1850, 3500);

	proBathrooms[3].m_sFileName = L"TI4_g.dwg";
	proBathrooms[3].m_sType = L"TI4_g";
	proBathrooms[3].m_doorPos = E_DIR_BOTTOM;
	proBathrooms[3].m_windowPos = E_DIR_TOP;
	proBathrooms[3].AddSizeRange(1600, 3500, 1600, 3650);
	proBathrooms[3].AddSizeRange(1700, 3500, 1850, 3650);

	proBathrooms[4].m_sFileName = L"TL3.dwg";
	proBathrooms[4].m_sType = L"TL3";
	proBathrooms[4].m_doorPos = E_DIR_BOTTOM;
	proBathrooms[4].m_windowPos = E_DIR_RIGHT;
	proBathrooms[4].AddSizeRange(1700, 1850, 1850, 2300);
	//不支持以下尺寸
	proBathrooms[4].DeleteSize(1700, 1850);
	proBathrooms[4].DeleteSize(1700, 2000);

	proBathrooms[5].m_sFileName = L"TL3_标准淋浴房.dwg";
	proBathrooms[5].m_sType = L"TL3_b";
	proBathrooms[5].m_doorPos = E_DIR_BOTTOM;
	proBathrooms[5].m_windowPos = E_DIR_RIGHT;
	proBathrooms[5].AddSizeRange(2000, 1850, 2150, 2300);

	proBathrooms[6].m_sFileName = L"TL4.dwg";
	proBathrooms[6].m_sType = L"TL4";
	proBathrooms[6].m_doorPos = E_DIR_BOTTOM;
	proBathrooms[6].m_windowPos = E_DIR_RIGHT;
	proBathrooms[6].AddSizeRange(1700, 2450, 1850, 2750);

	proBathrooms[7].m_sFileName = L"TL4_标准淋浴房.dwg";
	proBathrooms[7].m_sType = L"TL4_b";
	proBathrooms[7].m_doorPos = E_DIR_BOTTOM;
	proBathrooms[7].m_windowPos = E_DIR_RIGHT;
	proBathrooms[7].AddSizeRange(2000, 2450, 2150, 2450);

	proBathrooms[8].m_sFileName = L"TU3-1600X2450.dwg";
	proBathrooms[8].m_sType = L"TU3";
	proBathrooms[8].m_doorPos = E_DIR_LEFT;
	proBathrooms[8].m_windowPos = E_DIR_RIGHT;
	proBathrooms[8].AddSize(1600, 2450);

	proBathrooms[9].m_sFileName = L"TU3-1850X2000.dwg";
	proBathrooms[9].m_sType = L"TU3";
	proBathrooms[9].m_doorPos = E_DIR_LEFT;
	proBathrooms[9].m_windowPos = E_DIR_TOP;
	proBathrooms[9].AddSize(1850, 2000);

	proBathrooms[10].m_sFileName = L"TU3-1850X2750.dwg";
	proBathrooms[10].m_sType = L"TU3";
	proBathrooms[10].m_doorPos = E_DIR_LEFT;
	proBathrooms[10].m_windowPos = E_DIR_TOP;
	proBathrooms[10].AddSize(1850, 2750);

	proBathrooms[11].m_sFileName = L"TU4-2000X2750.dwg";
	proBathrooms[11].m_sType = L"TU4";
	proBathrooms[11].m_doorPos = E_DIR_LEFT;
	proBathrooms[11].m_windowPos = E_DIR_TOP;
	proBathrooms[11].AddSize(2000, 2750);

	for (UINT i = 0; i < proBathrooms.size(); i++)
		CProMrg::GetInstance()->AddProBathroom(proBathrooms[i]);
}
