
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include <rxmfcapi.h>
#include <dbgroup.h>
#include <geassign.h>
#include "accmd.h"
#include "dbtable.h"
#include "../Object/RCBlock.h"
#include "../Object/WindowDoor/RCWindow.h"
#include "../Common/ComFun_ACad.h"
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_Sunac.h"
#include "../Tool/DocLock.h"
#include "CommandWindowDetail.h"


CWindowDetailTemplate::CWindowDetailTemplate()
{
	pnt = AcGePoint3d(0, 0, 0);//插入点
}

bool CWindowDetailTemplate::InsertTemplates(int p_index, AcGePoint3d p_pnt)
{
	const int countPerCoulum = 3;
	int rowIndex = (int)(p_index / countPerCoulum);
	int columnIndex = p_index%countPerCoulum;

	pnt = GetInsertPnt(p_pnt, rowIndex, columnIndex);

	AcDbObjectId id = thisBlock.Insert(GetTemplateFileName(), pnt, 	0, L"Sunac_window",  0);

	return true;
}


//门窗详图
void CWindowDetail::DrawWindowDetail()
{
	CDocLock lockEnt;

	//第一步：选择需要统计的门窗
	eViewDir viewDir = E_VIEW_FRONT;
	bool bSuc1 = SelectViewDir(viewDir);
	if (bSuc1 == false)
	{
		return;
	}
	vAcDbObjectId winIds = SelectWindows(viewDir);
	if (winIds.size() == 0)
		return;

	//第二步  选择门窗表插入点
	AcGePoint3d pnt;
	bool bSuc = TY_GetPoint(pnt, L"请选择门窗详图插入点");
	if (bSuc == false)
		return;

	//第三步：读取门窗数据并且分类汇总
	CWindowCountArray winCountArray;
	winCountArray.InitByWindowIds(winIds);


	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < winCountArray.GetCount(); i++)
	{
		const AttrWindow& winAtt = winCountArray.GetWindow(i).winAtt;

		//4.1首先插入门窗表模板
		CWindowDetailTemplate winDetailTemplate;
		winDetailTemplate.InsertTemplates(i, pnt);

		AcGePoint3d winPnt = winDetailTemplate.GetInsertPntWindow(winAtt.GetW(), winAtt.GetH());

		AcDbObjectId idOut = GenerateWindow(winAtt, winPnt, E_VIEW_FRONT, E_DIR_BOTTOM, true, L"Sunac_Window");

		CreateDataText(winAtt, winDetailTemplate);

		CreateDetailDims(winAtt, idOut);
	}

	return;
}

bool CWindowDetail::CreateDataText(const AttrWindow& winAtt, CWindowDetailTemplate winDetailTemplate)
{
	CString str;
	////功能区
	//JHCOM_CreateText(AcGePoint3d(winDetailTemplate.pnt.x + winDetailTemplate.gongNengQuX,
	//	                      winDetailTemplate.pnt.y + winDetailTemplate.gongNengQuY,0),
	//	AcGeVector3d(0,0,1), 
	//	120, 0, L"厨房"); 

	//门窗编号
	JHCOM_CreateText(AcGePoint3d(winDetailTemplate.pnt.x + winDetailTemplate.idX,
		winDetailTemplate.pnt.y + winDetailTemplate.idY, 0),
		AcGeVector3d(0, 0, 1),
		120, 0,
		winAtt.GetInstanceCode());

	//窗面积
	str.Format(L"%.2fm", winAtt.GetWindowArea());
	JHCOM_CreateText(AcGePoint3d(winDetailTemplate.pnt.x + winDetailTemplate.areaX,
		winDetailTemplate.pnt.y + winDetailTemplate.areaY, 0),
		AcGeVector3d(0, 0, 1),
		120, 0,
		str);
	JHCOM_CreateText(AcGePoint3d(winDetailTemplate.pnt.x + winDetailTemplate.areaX + 470,
		winDetailTemplate.pnt.y + winDetailTemplate.areaY + 85, 0),
		AcGeVector3d(0, 0, 1),
		60, 0,
		_T("2")); //绘制㎡符号

	//开启面积
	str.Format(L"%.2fm", winAtt.GetTongFengQty());
	JHCOM_CreateText(AcGePoint3d(winDetailTemplate.pnt.x + winDetailTemplate.openAreaX,
		winDetailTemplate.pnt.y + winDetailTemplate.openAreaY, 0),
		AcGeVector3d(0, 0, 1),
		120, 0,
		str);
	JHCOM_CreateText(AcGePoint3d(winDetailTemplate.pnt.x + winDetailTemplate.openAreaX + 470,
		winDetailTemplate.pnt.y + winDetailTemplate.openAreaY + 85, 0),
		AcGeVector3d(0, 0, 1),
		60, 0,
		_T("2")); //绘制㎡符号

	//窗下墙高度
	str.Format(L"%dmm", (int)(winAtt.GetHeightUnderWindow()));
	JHCOM_CreateText(AcGePoint3d(winDetailTemplate.pnt.x + winDetailTemplate.heightUnderWindowX,
		winDetailTemplate.pnt.y + winDetailTemplate.heightUnderWindowY, 0),
		AcGeVector3d(0, 0, 1),
		120, 0,
		str);

	return true;
}

int CWindowDetail::CreateDetailDims(const AttrWindow& winAtt, AcDbObjectId m_id)
{
	CDocLock lockEnt;
	if (m_id == 0)
		return -1;

	const double W = winAtt.GetW();
	const double H = winAtt.GetH();
	const double A = winAtt.GetA();

	TYRect rect;
	DQ_GetBlockReferenceInsertPoint(m_id, rect.m_lb);
	rect.m_rt.x = rect.m_lb.x + W;
	rect.m_rt.y = rect.m_lb.y + H;
	rect.m_rt.z = 0;

	//JHCOM_GetObjectMinMaxPoint(m_id, rect.m_lb, rect.m_rt);
	double offset = 150;

	//----------------先标注竖向的--------------------//
	const double h2 = winAtt.GetH2();
	const double h1 = winAtt.GetH1();
	const AcGePoint3d  rightBottomPt = rect.GetRB();
	const AcGePoint3d  rightTopPt = rect.GetRT();

	AcGePoint3d  start = rightBottomPt;
	AcGePoint3d  end = rightBottomPt;
	AcGePoint3d  mid = rightBottomPt;

	int colorIndex = 2;
	int textHeight = 80;
	CString layer = L"0"; //TODO

	if (A > TOL)//如果A值存在 先标注两端的A
	{
		start = rightBottomPt;
		end = AcGePoint3d(start.x, start.y + A, 0);
		mid = AcGePoint3d(start.x + offset, (start.y + end.y) / 2, 0);
		MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);

		start = rightTopPt;
		end = AcGePoint3d(start.x, start.y - A, 0);
		mid = AcGePoint3d(start.x + offset, (start.y + end.y) / 2, 0);
		MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
	}

	if (h2 > TOL)
	{
		start = AcGePoint3d(rightBottomPt.x, rightBottomPt.y + A, 0);
		end = AcGePoint3d(start.x, start.y + h2, 0);
		mid = AcGePoint3d(start.x + offset, start.y + h2 / 2, 0);
		MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
	}

	//H1一定有
	start = AcGePoint3d(rightBottomPt.x, rightBottomPt.y + A + h2, 0);
	end = AcGePoint3d(start.x, start.y + h1, 0);
	mid = AcGePoint3d(start.x + offset, start.y + h1 / 2, 0);
	MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);

	//总高度一定有
	start = rightBottomPt;
	end = rightTopPt;
	mid = AcGePoint3d(start.x + offset * 2, (start.y + end.y) / 2, 0);
	MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);

	//////////////////////////////////////////////////////////////////////////
	//----------------标注横向的--------------------//

	double W1 = winAtt.GetW1();
	double W2 = winAtt.GetW2();
	double W3 = winAtt.GetW3();

	start = rect.GetLT();
	end = start;
	mid = start;
	if (A > TOL)//如果A值存在 先标注两端的A
	{
		end.x += A;
		mid.x += A / 2;
		mid.y += offset;
		MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);

		AcGePoint3d start1 = rect.GetRT();
		AcGePoint3d end1 = start1;
		AcGePoint3d mid1 = start1;
		end1.x -= A;
		mid1.x -= A / 2;
		mid1.y += offset;
		MD2010_AddAlignedDimension2(start1, end1, mid1, layer, colorIndex, textHeight);
	}

	//W1 都会存在
	start = end;
	mid = end;
	end.x += W1;
	mid.x += W1 / 2;
	mid.y += offset;
	MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);

	//标注W2
	if (W2 > TOL)
	{
		start = end;
		mid = end;
		end.x += W2;
		mid.x += W2 / 2;
		mid.y += offset;
		MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
	}

	//最后可能还有一个W1
	if (W - W1 - W2 - A * 2 > TOL)
	{
		start = end;
		mid = end;
		end.x += W1;
		mid.x += W1 / 2;
		mid.y += offset;
		MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
	}

	//标注总的宽度
	start = rect.GetLT();
	mid = start;
	end = start;
	end.x += W;
	mid.x += W / 2;
	mid.y += offset * 2;
	MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);

	return 0;
}
