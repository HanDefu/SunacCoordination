#include "StdAfx.h"
#include <dbidmap.h>
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Sunac.h"
#include "WindowTop2Front.h"
#include "AttrWindow.h"
#include "RCWindow.h"


bool CWindowTop2Front::GenFrontFromTop()
{
	//1.选择需要设置楼层的门窗
	vAcDbObjectId m_vids = SelectWindows(E_VIEW_TOP);
	if (m_vids.size() == 0)
	{
		return false;
	}


	E_DIRECTION windowDir = E_DIR_BOTTOM;
	bool bSuc1 = GetTopViewWindowDirection(windowDir);
	if (bSuc1 == false)
		return false;

	AcGePoint3d insertPos;
	bool bSuc = TY_GetPoint(insertPos, L"\n请选择立面图插入点");
	if (bSuc == false)
		return false;

	//////////////////////////////////////////////////////////////////////////
	AcDbObjectIdArray idsNoFloorInfo; //没有设置楼层的窗户
	vector<AttrWindow>  winAtts;
	vector<AcGePoint3d> allPos; //插入点
	vector<AcDbExtents> allExtents; //插入点
	for (UINT i = 0; i < m_vids.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = m_vids[i];
		oneWindow.InitParameters();

		AttrWindow* pAtt = oneWindow.GetAttribute();
		if (pAtt != NULL)
		{
			if (pAtt->GetFloorInfo().GetAllFloor().size() == 0)
			{
				idsNoFloorInfo.append(m_vids[i]);
			}

			AttrWindow attTemp(*pAtt);
			attTemp.m_viewDir = E_VIEW_FRONT;  //新生成的为立面图
			winAtts.push_back(attTemp);
			pAtt->close();

			allPos.push_back(oneWindow.m_blockInsertPos);
			allExtents.push_back(oneWindow.m_blockExtent);
		}
	}

	if (idsNoFloorInfo.length() > 0)
	{
		AfxMessageBox(_T("部分窗户未设置楼层和层高"));
		for (int i = 0; i < idsNoFloorInfo.length(); i++)
		{
			JHCOM_HilightObject(idsNoFloorInfo[i], true);
		}
		return false;
	}
	
	vector<double> allXvalue = GetAllXValueInFrontView(allExtents, windowDir);
	if (allXvalue.size()==0)
	{
		return false;
	}

	//对原来的窗户进行排序，找到最左侧的位置
	double minX = 1e10;
	for (UINT i = 0; i < allXvalue.size(); i++)
	{
		if (allXvalue[i] < minX)
		{
			minX = allXvalue[i];
		}
	}


	//////////////////////////////////////////////////////////////////////////
	AcDbObjectIdArray oneFloorIds;
	for (UINT i = 0; i < winAtts.size(); i++)
	{
		const AttrWindow& curWinAtt = winAtts[i];

		//当前列的插入点
		AcGePoint3d posColum = insertPos;
		posColum.x += allXvalue[i] - minX;

		const CFloorInfo floorInfo = curWinAtt.GetFloorInfo();
		const vector<int>  allFloos = floorInfo.GetAllFloor();
		if (allFloos.size() == 0)
			continue;

		const int curFloor = allFloos[0];
		AcGePoint3d pos = posColum;
		//yuan 选择左下角窗户时以下注释，Y方向选择楼层基准位置时放开
		//pos.y += floorInfo.GetFloorHeight()* (curFloor - 1);
		//if (curWinAtt.GetType() == WINDOW)
		//{
		//	pos.y += curWinAtt.GetHeightUnderWindow();
		//}

		AcDbObjectId idOut = GenerateWindow(curWinAtt, pos, E_VIEW_FRONT);
		oneFloorIds.append(idOut);
	}

	AcDbObjectIdArray windowObjIds = CopyAllFloorByOneFloor(oneFloorIds, winAtts);

	return true;
}

AcDbObjectIdArray CWindowTop2Front::CopyAllFloorByOneFloor(const AcDbObjectIdArray& oneFloorIds, const vector<AttrWindow> &winAtts)
{
	AcDbObjectIdArray windowObjIds;
	windowObjIds.append(oneFloorIds);


	Acad::ErrorStatus es;
	//其他楼层采用复制方式
	for (UINT i = 0; i < winAtts.size(); i++)
	{
		const AttrWindow& curWinAtt = winAtts[i];

		AcDbEntity* pEnt = NULL;
		es = acdbOpenObject(pEnt, oneFloorIds[i], AcDb::kForRead);
		if (es != Acad::eOk)
			continue;

		const CFloorInfo floorInfo = curWinAtt.GetFloorInfo();
		const vector<int>  allFloos = floorInfo.GetAllFloor();
		if (allFloos.size() == 0)
			continue;

		const int firstFloor = allFloos[0];
		for (UINT n = 1; n < allFloos.size(); n++)
		{
			AcGeMatrix3d xform;
			xform.setTranslation(AcGeVector3d(0, (allFloos[n] - firstFloor)*floorInfo.GetFloorHeight(), 0));

			AcDbEntity*  pCopyEntity = NULL;
			es = pEnt->getTransformedCopy(xform, pCopyEntity);
			if (pCopyEntity!=NULL)
			{
				AcDbObjectId idOut = JHCOM_PostToModelSpace(pCopyEntity);
				pCopyEntity->close();

				AttrWindow * pWindowAtt = new AttrWindow(curWinAtt);
				TY_AddAttributeData(idOut, pWindowAtt);
				pWindowAtt->close();

				windowObjIds.append(idOut);
			}
		}

		pEnt->close();
	}

	return windowObjIds;
}

bool CWindowTop2Front::GetTopViewWindowDirection(E_DIRECTION &windowDir) //得到平面窗户的方位，上、下左右
{
	windowDir = E_DIR_BOTTOM;

	CString sDir = _T("S");

	bool bSuc = false;
	do 
	{
		bSuc = GetStringInput(_T("\n请输入窗户朝向[东(E) 西(W) 南(S) 北(N)]<S>:"), sDir);
		if (bSuc == false)
			return false;

		sDir.Trim();
		sDir.MakeUpper();
		if (sDir.IsEmpty())
		{
			windowDir = E_DIR_BOTTOM;
			break;
		}

		if (sDir.Find(_T('E')) >= 0 || sDir.Find(_T('东')) >= 0)
		{
			windowDir = E_DIR_RIGHT;
			break;
		}
		else if (sDir.Find(_T('W')) >= 0 || sDir.Find(_T('西')) >= 0)
		{
			windowDir = E_DIR_LEFT;
			break;
		}
		else if (sDir.Find(_T('S')) >= 0 || sDir.Find(_T('南')) >= 0)
		{
			windowDir = E_DIR_BOTTOM;
			break;
		}
		else if (sDir.Find(_T('N')) >= 0 || sDir.Find(_T('北')) >= 0)
		{
			windowDir = E_DIR_TOP;
			break;
		}

	} while (bSuc);

	return bSuc;
}

vector<double> CWindowTop2Front::GetAllXValueInFrontView(const vector<AcDbExtents> &allExtents, const E_DIRECTION windowDir) //获取各窗户在立面视图上的x值
{
	vector<double> allXvalue;

	switch (windowDir)
	{
	case E_DIR_BOTTOM:
	{
						 for (UINT i = 0; i < allExtents.size(); i++)
						 {
							 allXvalue.push_back(allExtents[i].minPoint().x);
						 }
						 break;
	}

	case E_DIR_TOP:
	{
					  double maxX = -1e10;
					  for (UINT i = 0; i < allExtents.size(); i++)
					  {
						  double xValue = allExtents[i].maxPoint().x;
						  allXvalue.push_back(xValue);
						  if (maxX<xValue)
						  {
							  maxX = xValue;
						  }
					  }

					  for (UINT i = 0; i < allXvalue.size(); i++)
					  {
						  allXvalue[i] = maxX - allXvalue[i];
					  }
		break;
	}
	case E_DIR_LEFT:
	{
					   double maxY = -1e10;
					   for (UINT i = 0; i < allExtents.size(); i++)
					   {
						   double yValue = allExtents[i].minPoint().y;
						   allXvalue.push_back(yValue);
						   if (maxY < yValue)
						   {
							   maxY = yValue;
						   }
					   }
					   for (UINT i = 0; i < allXvalue.size(); i++)
					   {
						   allXvalue[i] = maxY - allXvalue[i];
					   }
					   break;
	}
	case E_DIR_RIGHT:
	{
						for (UINT i = 0; i < allExtents.size(); i++)
						{
							double valueY = allExtents[i].minPoint().y;
							allXvalue.push_back(valueY);
						}
						break;
	}
	default:
		assert(false);
		break;
	}

	return allXvalue;
}

AcDbObjectId  CWindowTop2Front::GenerateWindow(const AttrWindow& curWinAtt, AcGePoint3d pos, eViewDir p_view)
{
	RCWindow oneWindow;
	AcDbObjectId id = oneWindow.Insert(curWinAtt.GetPrototypeDwgFilePath(p_view), pos, 0, L"0", 256);

	oneWindow.InitParameters();

	oneWindow.SetParameter(L"H", (double)curWinAtt.GetH());
	oneWindow.SetParameter(L"W", (double)curWinAtt.GetW());
	oneWindow.SetParameter(L"W1", (double)curWinAtt.GetW1());
	if (curWinAtt.HasValue(_T("H2")))
		oneWindow.SetParameter(L"H2", (double)curWinAtt.GetH2());
	if (curWinAtt.HasValue(_T("W3")))
		oneWindow.SetParameter(L"W3", (double)curWinAtt.GetW3());
	if (curWinAtt.HasValue(_T("H3")))
		oneWindow.SetParameter(L"H3", (double)curWinAtt.GetH3());

	oneWindow.RunParameters();

	if (curWinAtt.m_isMirror && (curWinAtt.m_isMirrorWindow == false))
	{
		AcGePoint3d basePt(pos.x + curWinAtt.GetW() / 2, 0, 0);
		TYCOM_Mirror(oneWindow.m_id, basePt, AcGeVector3d(0, 1, 0));
	}


	//把UI的数据记录在图框的扩展字典中
	AttrWindow * pWindow = new AttrWindow(curWinAtt);
	pWindow->m_viewDir = p_view;
	oneWindow.AddAttribute(pWindow);
	pWindow->close();

	return id;
}
