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

std::vector<AttrKitchen> CKitchenBathroomLocalData::GetKitchens(EKitchType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_hasPaiQiDao)
{
	vector<AttrKitchen> ret;

	int xLen = int(p_xLen + 0.5);
	int yLen = int(p_yLen + 0.5);
	int width = xLen;
	int height = yLen;
	if (p_doorDir == E_DIR_LEFT || p_doorDir == E_DIR_RIGHT)
		swap(width, height);

	CString sType;
	switch (p_type)
	{
	case E_KITCH_U:
		sType = L"KU";
		break;
	case E_KITCH_L:
		sType = L"KL";
		break;
	case E_KITCH_I:
		sType = L"KI";
		break;
	}

	for (UINT i = 0; i < m_allKitchens.size(); i++)
	{
		if ((p_type == E_KITCH_ALL || m_allKitchens[i].m_prototypeCode.Left(2) == sType) && m_allKitchens[i].m_prop.MatchPrototype(xLen, yLen, p_doorDir, p_windowDir) && p_hasPaiQiDao == m_allKitchens[i].m_hasPaiQiDao)
		{
			ret.push_back(m_allKitchens[i]);
			ret.back().m_width = width;
			ret.back().m_height = height;
		}
	}

	return ret;
}

std::vector<AttrBathroom> CKitchenBathroomLocalData::GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir)
{
	vector<AttrBathroom> ret;

	int xLen = int(p_xLen + 0.5);
	int yLen = int(p_yLen + 0.5);
	int width = xLen;
	int height = yLen;
	if (p_doorDir == E_DIR_LEFT || p_doorDir == E_DIR_RIGHT)
		swap(width, height);

	CString sType;
	switch (p_type)
	{
	case E_BATHROOM_I:
		sType = L"TI";
		break;
	case E_BATHROOM_L:
		sType = L"TL";
		break;
	case E_BATHROOM_U:
		sType = L"TU";
		break;
	}

	for (UINT i = 0; i < m_allBathrooms.size(); i++)
	{
		if ((p_type == E_BATHROOM_ALL || m_allBathrooms[i].m_prototypeCode.Left(2) == sType) && m_allBathrooms[i].m_prop.MatchPrototype(xLen, yLen/*, p_doorDir, p_windowDir*/))
		{
			ret.push_back(m_allBathrooms[i]);
			ret.back().m_width = width;
			ret.back().m_height = height;
		}
	}

	return ret;
}

void CKitchenBathroomLocalData::InitKitchenLocalData()
{
	m_allKitchens.resize(7);

	AttrKitchen* pKitchen = &m_allKitchens[0];
	CKitchenBathroomProp* pProp = &pKitchen->m_prop;
	//AttrObject基类属性
	pKitchen->m_prototypeCode = L"KUq";
	pKitchen->SetFileName(L"KUq_Dui.dwg");
	pKitchen->m_isDynamic = true;
	pKitchen->m_isJiTuan = true;
	//AttrBathroom类属性
	pKitchen->m_kitchenType = L"浅U型";
	pKitchen->m_windowDoorPos = DUIKAI;
	pKitchen->m_hasPaiQiDao = false;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_TOP;
	pProp->AddSizeRange(2450, 1700, 3500, 1850);
	pProp->DeleteSize(2450, 1700);

	pKitchen = &m_allKitchens[1];
	pProp = &pKitchen->m_prop;
	//AttrObject基类属性
	pKitchen->m_prototypeCode = L"KUq_z";
	pKitchen->SetFileName(L"KUq_Dui_ZhuanJiao.dwg");
	pKitchen->m_isDynamic = true;
	pKitchen->m_isJiTuan = true;
	//AttrBathroom类属性
	pKitchen->m_kitchenType = L"浅U型";
	pKitchen->m_windowDoorPos = DUIKAI;
	pKitchen->m_hasPaiQiDao = false;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_TOP;
	pProp->AddSizeRange(2450, 2000, 3500, 2600);

	pKitchen = &m_allKitchens[2];
	pProp = &pKitchen->m_prop;
	//AttrObject基类属性
	pKitchen->m_prototypeCode = L"KUq_c";
	pKitchen->SetFileName(L"KUq_Chui.dwg");
	pKitchen->m_isDynamic = true;
	pKitchen->m_isJiTuan = true;
	//AttrBathroom类属性
	pKitchen->m_kitchenType = L"浅U型";
	pKitchen->m_windowDoorPos = CHUIZHIKAI;
	pKitchen->m_hasPaiQiDao = false;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_RIGHT;
	pProp->AddSizeRange(2450, 1700, 3500, 1850);
	pProp->DeleteSize(2450, 1700);

	pKitchen = &m_allKitchens[3];
	pProp = &pKitchen->m_prop;
	//AttrObject基类属性
	pKitchen->m_prototypeCode = L"KUq_c_z";
	pKitchen->SetFileName(L"KUq_Chui_ZhuanJiao.dwg");
	pKitchen->m_isDynamic = true;
	pKitchen->m_isJiTuan = true;
	//AttrBathroom类属性
	pKitchen->m_kitchenType = L"浅U型";
	pKitchen->m_windowDoorPos = CHUIZHIKAI;
	pKitchen->m_hasPaiQiDao = false;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_RIGHT;
	pProp->AddSizeRange(2450, 2000, 3500, 2450);

	pKitchen = &m_allKitchens[4];
	pProp = &pKitchen->m_prop;
	//AttrObject基类属性
	pKitchen->m_prototypeCode = L"KUs";
	pKitchen->SetFileName(L"KUs.dwg");
	pKitchen->m_isDynamic = true;
	pKitchen->m_isJiTuan = true;
	//AttrBathroom类属性
	pKitchen->m_kitchenType = L"深U型";
	pKitchen->m_windowDoorPos = DUIKAI;
	pKitchen->m_hasPaiQiDao = false;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_TOP;
	pProp->AddSizeRange(2300, 2450, 2450, 3200);

	pKitchen = &m_allKitchens[5];
	pProp = &pKitchen->m_prop;
	//AttrObject基类属性
	pKitchen->m_prototypeCode = L"KL";
	pKitchen->SetFileName(L"KL.dwg");
	pKitchen->m_isDynamic = true;
	pKitchen->m_isJiTuan = true;
	//AttrBathroom类属性
	pKitchen->m_kitchenType = L"L型";
	pKitchen->m_windowDoorPos = DUIKAI;
	pKitchen->m_hasPaiQiDao = false;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_LEFT;
	pProp->m_windowPos = E_DIR_RIGHT;
	pProp->AddSizeRange(2600, 1700, 3950, 2000);

	pKitchen = &m_allKitchens[6];
	pProp = &pKitchen->m_prop;
	//AttrObject基类属性
	pKitchen->m_prototypeCode = L"KI";
	pKitchen->SetFileName(L"KI.dwg");
	pKitchen->m_isDynamic = true;
	pKitchen->m_isJiTuan = true;
	//AttrBathroom类属性
	pKitchen->m_kitchenType = L"I型";
	pKitchen->m_windowDoorPos = DUIKAI;
	pKitchen->m_hasPaiQiDao = false;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_LEFT;
	pProp->m_windowPos = E_DIR_RIGHT;
	pProp->AddSizeRange(3050, 1700, 4100, 1700);

	//创建包含排气道的原型
	int kitchenCount = (int)m_allKitchens.size();
	for (int i = 0; i < kitchenCount; i++)
	{
		AttrKitchen newKitchen(m_allKitchens[i]);
		newKitchen.m_prototypeCode += L"_p";

		CString sName = newKitchen.GetFileName();
		sName.Replace(L".dwg", L"_P.dwg");
		newKitchen.SetFileName(sName);

		newKitchen.m_hasPaiQiDao = true;
		m_allKitchens.push_back(newKitchen);
	}
}

void CKitchenBathroomLocalData::InitBathroomLocalData()
{
	m_allBathrooms.resize(12);

	AttrBathroom* pBathroom = &m_allBathrooms[0];
	CKitchenBathroomProp* pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TI3";
	pBathroom->SetFileName(L"TI3.dwg");
	pBathroom->m_isDynamic = true;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"I型";
	pBathroom->m_windowDoorPos = DUIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_TOP;
	pProp->AddSizeRange(1600, 2450, 1600, 3050);
	pProp->AddSizeRange(1700, 2450, 1850, 3050);

	pBathroom = &m_allBathrooms[1];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TI3_g";
	pBathroom->SetFileName(L"TI3_g.dwg");
	pBathroom->m_isDynamic = true;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"I型";
	pBathroom->m_windowDoorPos = DUIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_TOP;
	pProp->AddSizeRange(1600, 2750, 1600, 3200);
	pProp->AddSizeRange(1700, 2750, 1850, 3200);

	pBathroom = &m_allBathrooms[2];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TI4";
	pBathroom->SetFileName(L"TI4.dwg");
	pBathroom->m_isDynamic = true;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"I型";
	pBathroom->m_windowDoorPos = DUIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_TOP;
	pProp->AddSizeRange(1600, 3050, 1600, 3500);
	pProp->AddSizeRange(1700, 3050, 1850, 3500);

	pBathroom = &m_allBathrooms[3];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TI4_g";
	pBathroom->SetFileName(L"TI4_g.dwg");
	pBathroom->m_isDynamic = true;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"I型";
	pBathroom->m_windowDoorPos = DUIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_TOP;
	pProp->AddSizeRange(1600, 3500, 1600, 3650);
	pProp->AddSizeRange(1700, 3500, 1850, 3650);

	pBathroom = &m_allBathrooms[4];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TL3";
	pBathroom->SetFileName(L"TL3.dwg");
	pBathroom->m_isDynamic = true;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"L型";
	pBathroom->m_windowDoorPos = CHUIZHIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_RIGHT;
	pProp->AddSizeRange(1700, 2150, 1700, 2300);
	pProp->AddSizeRange(1850, 1850, 1850, 2300);

	pBathroom = &m_allBathrooms[5];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TL3_b";
	pBathroom->SetFileName(L"TL3_标准淋浴房.dwg");
	pBathroom->m_isDynamic = true;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"L型";
	pBathroom->m_windowDoorPos = CHUIZHIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_RIGHT;
	pProp->AddSizeRange(2000, 1850, 2150, 2300);

	pBathroom = &m_allBathrooms[6];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TL4";
	pBathroom->SetFileName(L"TL4.dwg");
	pBathroom->m_isDynamic = true;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"L型";
	pBathroom->m_windowDoorPos = CHUIZHIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_RIGHT;
	pProp->AddSizeRange(1700, 2450, 1850, 2750);

	pBathroom = &m_allBathrooms[7];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TL4_b";
	pBathroom->SetFileName(L"TL4_标准淋浴房.dwg");
	pBathroom->m_isDynamic = true;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"L型";
	pBathroom->m_windowDoorPos = CHUIZHIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_BOTTOM;
	pProp->m_windowPos = E_DIR_RIGHT;
	pProp->AddSizeRange(2000, 2450, 2150, 2450);

	pBathroom = &m_allBathrooms[8];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TU3";
	pBathroom->SetFileName(L"TU3-1600X2450.dwg");
	pBathroom->m_isDynamic = false;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"U型";
	pBathroom->m_windowDoorPos = CHUIZHIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_LEFT;
	pProp->m_windowPos = E_DIR_TOP;
	pProp->AddSize(1600, 2450);

	pBathroom = &m_allBathrooms[9];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TU3";
	pBathroom->SetFileName(L"TU3-1850X2000.dwg");
	pBathroom->m_isDynamic = false;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"U型";
	pBathroom->m_windowDoorPos = DUIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_LEFT;
	pProp->m_windowPos = E_DIR_RIGHT;
	pProp->AddSize(1850, 2000);

	pBathroom = &m_allBathrooms[10];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TU3";
	pBathroom->SetFileName(L"TU3-1850X2750.dwg");
	pBathroom->m_isDynamic = false;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"U型";
	pBathroom->m_windowDoorPos = CHUIZHIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_LEFT;
	pProp->m_windowPos = E_DIR_TOP;
	pProp->AddSize(1850, 2750);

	pBathroom = &m_allBathrooms[11];
	pProp = &pBathroom->m_prop;
	//AttrObject基类属性
	pBathroom->m_prototypeCode = L"TU4";
	pBathroom->SetFileName(L"TU4-2000X2750.dwg");
	pBathroom->m_isDynamic = false;
	pBathroom->m_isJiTuan = true;
	//AttrBathroom类属性
	pBathroom->m_sBathroomType = L"U型";
	pBathroom->m_windowDoorPos = CHUIZHIKAI;
	pBathroom->m_hasPaiQiDao = true;
	//原型尺寸与门窗初始位置
	pProp->m_doorPos = E_DIR_LEFT;
	pProp->m_windowPos = E_DIR_TOP;
	pProp->AddSize(2000, 2750);
}
