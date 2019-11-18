#include "StdAfx.h"
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

	AcGePoint3d insertPos;
	bool bSuc = TY_GetPoint(insertPos, L"请选择立面图插入点");
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
	
	vector<double> allXvalue = GetAllXValueInFrontView(allExtents);
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

			AcDbEntity*  pCopyObj = NULL;
			es = pEnt->getTransformedCopy(xform, pCopyObj);
			AcDbObjectId idOut = JHCOM_PostToModelSpace(pCopyObj);
			pCopyObj->close();

			windowObjIds.append(idOut);
		}

		pEnt->close();
	}

	return windowObjIds;
}

E_DIRECTION CWindowTop2Front::GetTopViewWindowDirection(const vector<AcDbExtents> &allExtents) //得到平面窗户的方位，上、下左右
{
	E_DIRECTION viewDir = E_DIR_BOTTOM;

	//TODO
	return viewDir;
}

vector<double> CWindowTop2Front::GetAllXValueInFrontView(const vector<AcDbExtents> &allExtents) //获取各窗户在立面视图上的x值
{
	//判断窗户的方向
	E_DIRECTION viewDir = GetTopViewWindowDirection(allExtents);

	vector<double> allXvalue;
	if (viewDir == E_DIR_BOTTOM)
	{
		for (UINT i = 0; i < allExtents.size(); i++)
		{
			allXvalue.push_back(allExtents[i].minPoint().x);
		}
	}
	else
	{
		//TODO
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
