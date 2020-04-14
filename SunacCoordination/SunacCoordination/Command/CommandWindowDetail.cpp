
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
#include "../Object/WindowDoor/WindowGen.h"
#include "../Object/WindowDoor/WindowSelect.h"
#include "../Common/ComFun_ACad.h"
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_Sunac.h"
#include "../Tool/DocLock.h"
#include "CommandWindowDetail.h"
#include "../Command/CommandHighlight.h"
#include "..\GlobalSetting.h"
#include "../Common/ComFun_Math.h"
#include "../Common/ComFun_Layer.h"
#include "../GlobalSetting.h"


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

	CCommandHighlight::GetInstance()->WindowDoorNoHighlight();

	//第一步：选择需要统计的门窗
	eViewDir viewDir = E_VIEW_FRONT;
	bool bSuc1 = SelectViewDir(viewDir);
	if (bSuc1 == false)
	{
		return;
	}

	const vector<CWinInCad> wins = CWindowSelect::SelectWindows(viewDir);
	if (wins.size() == 0)
		return;

	//第二步  选择门窗表插入点
	AcGePoint3d pnt;
	bool bSuc = TY_GetPoint(pnt, L"请选择门窗详图插入点");
	if (bSuc == false)
		return;

	//第三步：读取门窗数据并且分类汇总
	CWindowCountArray winCountArray;
	winCountArray.InitByWindowIds(wins);


	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < winCountArray.GetCount(); i++)
	{
		AttrWindow winAtt = winCountArray.GetWindow(i).winAtt;
		winAtt.m_viewDir = E_VIEW_EXTEND;
		winAtt.ClearWinsRelation();

		//4.1首先插入门窗表模板
		CWindowDetailTemplate winDetailTemplate;
		winDetailTemplate.InsertTemplates(i, pnt);

		AcGePoint3d winPnt = winDetailTemplate.GetInsertPntWindow(winAtt.GetW(), winAtt.GetH());

		AcDbObjectId idOut = CWindowGen::GenerateWindow(winAtt, winPnt, E_DIR_BOTTOM, true, AcDbObjectId::kNull);

		CreateDataText(winAtt, winDetailTemplate);

		if (winAtt.IsMirror() == true)
			CreateMirrorDetailDims(winAtt, idOut);
		else
			CreateDetailDims(winAtt, idOut);
	}

	//对选择的门窗高亮
	vAcDbObjectId winIds;
	for (UINT i = 0; i < wins.size(); i++)
	{
		winIds.push_back(wins[i].m_winId);
	}
	CCommandHighlight::GetInstance()->WindowDoorHighlight(winIds);

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
	str.Format(L"%d", (int)(winAtt.GetHeightUnderWindow()));
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

	CString oldLayerName;

	MD2010_GetCurrentLayer(oldLayerName);

	CString sWindowDoorLayerName = L"Sunac_dim";
	if (JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(sWindowDoorLayerName);
	}
	
	MD2010_SetCurrentLayer(sWindowDoorLayerName);

	const double W = winAtt.GetW();
	const double H = winAtt.GetH();
	const double A = winAtt.GetA();

	TYRect rect;
	DQ_GetBlockReferenceInsertPoint(m_id, rect.m_lb);
	rect.m_rt.x = rect.m_lb.x + W;
	rect.m_rt.y = rect.m_lb.y + H;
	rect.m_rt.z = 0;

	double offset = 250;

	//----------------先标注竖向的--------------------//
	const double h2 = winAtt.GetH2();
	const double h1 = winAtt.GetH1();
	const AcGePoint3d  rightBottomPt = rect.GetRB();
	const AcGePoint3d  rightTopPt = rect.GetRT();

	AcGePoint3d  start = rightTopPt;
	AcGePoint3d  end = rightTopPt;
	AcGePoint3d  mid = rightTopPt;

	if (A > TOL)//如果A值存在 先标注两端的A
	{
		start = rightBottomPt;
		
		end = AcGePoint3d(start.x, start.y + A, 0);
		mid = AcGePoint3d(start.x + offset, (start.y + end.y) / 2, 0);
		AddAlignedDimensionAndStyle(start, end, mid, A);

		start = rightTopPt;
		end = AcGePoint3d(start.x, start.y - A, 0);
		mid = AcGePoint3d(start.x + offset, (start.y + end.y) / 2, 0);
		AddAlignedDimensionAndStyle(start, end, mid, A);
	}

	//H1一定有
	start = end;
	mid = start;
	mid.x += offset;
	end.y -= h1;
	AddAlignedDimensionAndStyle(start, end, mid, h1);

	if (h2 > TOL)
	{
		start = end;
		mid = start;
		mid.x += offset;
		end.y -= h2;
		AddAlignedDimensionAndStyle(start, end, mid, h2);
	}

	//总高度一定有
	start = rightBottomPt;
	end = rightTopPt;
	mid = AcGePoint3d(start.x + offset * 2, (start.y + end.y) / 2, 0);
	AddAlignedDimensionAndStyle(start, end, mid, H);

	//////////////////////////////////////////////////////////////////////////
	//----------------标注横向的--------------------//
	bool m_isHasW3 = false;

	double W1 = winAtt.GetW1();
	double W2 = winAtt.GetW2();
	double W3 = winAtt.GetW3();

	start = rect.GetLT();
	end = start;
	mid = start;

	AcGePoint3d start1 = rect.GetRT();

	if (A > TOL)//如果A值存在 先标注两端的A
	{
		end.x += A;
		mid.x += A / 2;
		mid.y += offset;
		AddAlignedDimensionAndStyle(start, end, mid, A);

		AcGePoint3d end1 = start1;
		AcGePoint3d mid1 = start1;
		end1.x -= A;
		mid1.x -= A / 2;
		mid1.y += offset;
		AddAlignedDimensionAndStyle(start1, end1, mid1, A);
	}

	//先标注右侧的转角窗
	if (W3 > TOL)
	{
		AcGePoint3d start2 = AcGePoint3d(start1.x - A, start1.y, 0);
		AcGePoint3d mid2 = start2;
		AcGePoint3d end2 = start2;
		end2.x -= W3;
		mid2.x -= W3 / 2;
		mid2.y += offset;
		AddAlignedDimensionAndStyle(start2, end2, mid2, W3);
	}

	//若左侧也有转角窗，再标注左侧转角窗
	if (((W - W1 - W2 - W3 - A * 2 > W1) && (W - W1 - W2 - W3 - A * 2 > W3)) && winAtt.GetPrototypeCode().Find(L"Window") >= 0 
		|| (W - W1 - W2 - W3 - A * 2 == 500) && winAtt.GetPrototypeCode().Find(L"Window") >= 0)
	{
		m_isHasW3 = true;
		start = end;
		mid = end;
		end.x += W3;
		mid.x += W3 / 2;
		mid.y += offset;
		AddAlignedDimensionAndStyle(start, end, mid, W3);
	}

	//W1 都会存在
	start = end;
	mid = end;
	end.x += W1;
	mid.x += W1 / 2;
	mid.y += offset;
	AddAlignedDimensionAndStyle(start, end, mid, W1);

	//标注W2
	if (W2 > TOL)
	{
		start = end;
		mid = end;
		end.x += W2;
		mid.x += W2 / 2;
		mid.y += offset;
		AddAlignedDimensionAndStyle(start, end, mid, W2);
	}

	//最后可能还有一个W1
	if (m_isHasW3 == false)
	{
		if (W - W1 - W2 - W3 - A * 2 >= TOL)
		{
			start = end;
			mid = end;
			end.x += W1;
			mid.x += W1 / 2;
			mid.y += offset;
			AddAlignedDimensionAndStyle(start, end, mid, W1);
		}
		if (W - W1 * 2 - W2 - W3 - A * 2 >= TOL)
		{
			start = end;
			mid = end;
			end.x += W1;
			mid.x += W1 / 2;
			mid.y += offset;
			AddAlignedDimensionAndStyle(start, end, mid, W1);
		}
		if (W - W1*3 - W2 - W3 - A * 2 > TOL)
		{
			start = end;
			mid = end;
			end.x += W1;
			mid.x += W1 / 2;
			mid.y += offset;
			AddAlignedDimensionAndStyle(start, end, mid, W1);
		}
	}
	else
	{
		if (W - W1 - W2 - W3 * 2 - A * 2 > TOL)
		{
			start = end;
			mid = end;
			end.x += W1;
			mid.x += W1 / 2;
			mid.y += offset;
			AddAlignedDimensionAndStyle(start, end, mid, W1);
		}
	}

	//标注总的宽度
	start = rect.GetLT();
	mid = start;
	end = start;
	end.x += W;
	mid.x += W / 2;
	mid.y += offset * 2;
	AddAlignedDimensionAndStyle(start, end, mid, W);

	//标注窗下墙高
	double heightUnderWindow = winAtt.GetHeightUnderWindow();
	start = rect.GetRB();

	if (heightUnderWindow > 0 && winAtt.GetPrototypeCode().Find(_T("Window")) >= 0)
	{
		CreateHeightUnderWindow(start, heightUnderWindow, offset);
	}

	MD2010_SetCurrentLayer(oldLayerName);

	return 0;
}

int CWindowDetail::CreateMirrorDetailDims(const AttrWindow& winAtt, AcDbObjectId m_id)
{
	CDocLock lockEnt;

	if (m_id == 0)
		return -1;

	CString oldLayerName;

	MD2010_GetCurrentLayer(oldLayerName);

	CString sWindowDoorLayerName = L"Sunac_dim";
	if (JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(sWindowDoorLayerName);
	}

	MD2010_SetCurrentLayer(sWindowDoorLayerName);

	const double W = winAtt.GetW();
	const double H = winAtt.GetH();
	const double A = winAtt.GetA();

	TYRect rect;
	DQ_GetBlockReferenceInsertPoint(m_id, rect.m_lb);
	rect.m_lb.x -= W;
	rect.m_rt.x = rect.m_lb.x + W;
	rect.m_rt.y = rect.m_lb.y + H;
	rect.m_rt.z = 0;

	double offset = 250;

	//----------------先标注竖向的--------------------//
	const double h2 = winAtt.GetH2();
	const double h1 = winAtt.GetH1();
	const AcGePoint3d  rightBottomPt = rect.GetRB();
	const AcGePoint3d  rightTopPt = rect.GetRT();

	AcGePoint3d  start = rightTopPt;
	AcGePoint3d  end = rightTopPt;
	AcGePoint3d  mid = rightTopPt;

	if (A > TOL)//如果A值存在 先标注两端的A
	{
		start = rightBottomPt;
		end = AcGePoint3d(start.x, start.y + A, 0);
		mid = AcGePoint3d(start.x + offset, (start.y + end.y) / 2, 0);
		AddAlignedDimensionAndStyle(start, end, mid, A);

		start = rightTopPt;
		end = AcGePoint3d(start.x, start.y - A, 0);
		mid = AcGePoint3d(start.x + offset, (start.y + end.y) / 2, 0);
		AddAlignedDimensionAndStyle(start, end, mid, A);
	}

	//H1一定有
	start = end;
	mid = start;
	mid.x += offset;
	end.y -= h1;
	AddAlignedDimensionAndStyle(start, end, mid, h1);

	if (h2 > TOL)
	{
		start = end;
		mid = start;
		mid.x += offset;
		end.y -= h2;
		AddAlignedDimensionAndStyle(start, end, mid, h2);
	}

	//总高度一定有
	start = rightBottomPt;
	end = rightTopPt;
	mid = AcGePoint3d(start.x + offset * 2, (start.y + end.y) / 2, 0);
	AddAlignedDimensionAndStyle(start, end, mid, H);

	//////////////////////////////////////////////////////////////////////////
	//----------------标注横向的--------------------//
	bool m_isHasW3 = false;

	double W1 = winAtt.GetW1();
	double W2 = winAtt.GetW2();
	double W3 = winAtt.GetW3();

	start = rect.GetRT();
	end = start;
	mid = start;

	AcGePoint3d start1 = rect.GetLT();
	AcGePoint3d end1 = start1;
	AcGePoint3d mid1 = start1;

	if (A > TOL)//如果A值存在 先标注两端的A
	{
		end1.x += A;
		mid1.x += A / 2;
		mid1.y += offset;
		AddAlignedDimensionAndStyle(start1, end1, mid1, A);

		end.x -= A;
		mid.x -= A / 2;
		mid.y += offset;
		AddAlignedDimensionAndStyle(start, end, mid, A);
	}

	//先标注左侧的转角窗
	if (W3 > TOL)
	{
		start1 = end1;
		mid1 = start1;
		mid1.y += offset;
		end1.x += W3;
		AddAlignedDimensionAndStyle(start1, end1, mid1, W3);
	}

	//若右侧也有转角窗，再标注右侧转角窗
	if (((W - W1 - W2 - W3 - A * 2 > W1) && (W - W1 - W2 - W3 - A * 2 > W3)) && winAtt.GetPrototypeCode().Find(L"Window") >= 0
		|| (W - W1 - W2 - W3 - A * 2 == 500) && winAtt.GetPrototypeCode().Find(L"Window") >= 0)
	{
		m_isHasW3 = true;
		start = end;
		mid = end;
		end.x -= W3;
		mid.x -= W3 / 2;
		mid.y += offset;
		AddAlignedDimensionAndStyle(start, end, mid, W3);
	}

	//W1 都会存在
	start = end;
	mid = end;
	end.x -= W1;
	mid.x -= W1 / 2;
	mid.y += offset;
	AddAlignedDimensionAndStyle(start, end, mid, W1);

	//标注W2
	if (W2 > TOL)
	{
		start = end;
		mid = end;
		end.x -= W2;
		mid.x -= W2 / 2;
		mid.y += offset;
		AddAlignedDimensionAndStyle(start, end, mid, W2);
	}

	//最后可能还有一个W1
	if (m_isHasW3 == false)
	{
		if (W - W1 - W2 - W3 - A * 2 > TOL)
		{
			start = end;
			mid = end;
			end.x -= W1;
			mid.x -= W1 / 2;
			mid.y += offset;
			AddAlignedDimensionAndStyle(start, end, mid, W1);
		}
		if (W - W1 * 2 - W2 - W3 - A * 2 > TOL)
		{
			start = end;
			mid = end;
			end.x -= W1;
			mid.x -= W1 / 2;
			mid.y += offset;
			AddAlignedDimensionAndStyle(start, end, mid, W1);
		}
		if (W - W1 * 3 - W2 - W3 - A * 2 > TOL)
		{
			start = end;
			mid = end;
			end.x -= W1;
			mid.x -= W1 / 2;
			mid.y += offset;
			AddAlignedDimensionAndStyle(start, end, mid, W1);
		}
	}
	else
	{
		if (W - W1 - W2 - W3 * 2 - A * 2 > TOL)
		{
			start = end;
			mid = end;
			end.x -= W1;
			mid.x -= W1 / 2;
			mid.y += offset;
			AddAlignedDimensionAndStyle(start, end, mid, W1);
		}
	}

	//标注总的宽度
	start = rect.GetLT();
	mid = start;
	end = start;
	end.x += W;
	mid.x += W / 2;
	mid.y += offset * 2;
	AddAlignedDimensionAndStyle(start, end, mid, W);

	//标注窗下墙高
	double heightUnderWindow = winAtt.GetHeightUnderWindow();
	start = rect.GetRB();

	if (heightUnderWindow > 0 && winAtt.GetPrototypeCode().Find(_T("Window")) >= 0)
	{
		CreateHeightUnderWindow(start, heightUnderWindow, offset);
	}

	MD2010_SetCurrentLayer(oldLayerName);

	return 0;
}

AcDbObjectId CWindowDetail::AddAlignedDimensionAndStyle(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, double size)
{
	SetDetailTextStyle(L"_TCH_DIM_T3");

	//获取门窗详图模板中的标注尺寸名称
	CString sDimStyle = GetDimensionStyle();
	//获取所创建的标注尺寸的id
	AcDbObjectId dimStyleId = MD2010_GetDimstylerID(sDimStyle);

	if (JHCOM_PointDistance(start, end) <= TOL * 10000)//小于1的不标注
		return 0;

	CString entryName;
	entryName.Format(_T("%d"), (int)size);
	AcDbAlignedDimension  *pDim = new AcDbAlignedDimension(start, end, dimlinpnt, entryName, dimStyleId);

	AcDbObjectId dimID = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pDim);

	pDim->close();

	return dimID;
}

CString CWindowDetail::GetDimensionStyle()
{
	CString dimStyle;

	if (GlobalSetting::GetInstance()->m_winSetting.m_bWinDetailDimRate == 20)
	{
		dimStyle = L"_TCH_ARCH&&20";
	}
	else if (GlobalSetting::GetInstance()->m_winSetting.m_bWinDetailDimRate == 50)
	{
		dimStyle = L"_TCH_ARCH&&50";
	}
	else if (GlobalSetting::GetInstance()->m_winSetting.m_bWinDetailDimRate == 80)
	{
		dimStyle = L"_TCH_ARCH&&80";
	}
	else if (GlobalSetting::GetInstance()->m_winSetting.m_bWinDetailDimRate == 100)
	{
		dimStyle = L"_TCH_ARCH&&100";
	}
	else
	{
		dimStyle = L"_TCH_ARCH&&120";
	}

	return dimStyle;
}

int CWindowDetail::SetDetailTextStyle(CString dimname)
{
	AcDbTextStyleTable *pTextStylerTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStylerTbl, AcDb::kForRead);

	// 判断是否包含指定名称的层表记录
	if (!pTextStylerTbl->has(dimname))
	{
		pTextStylerTbl->close();
		CreateDetailTextStyle(dimname);
		return 0;
	}

	// 获得指定层表记录的指针
	AcDbTextStyleTableRecord *pTextStylerTblRcd = NULL;
	pTextStylerTbl->getAt(dimname, pTextStylerTblRcd, AcDb::kForWrite);

	pTextStylerTblRcd->setFileName(L"simplex.shx");
	pTextStylerTblRcd->setBigFontFileName(L"bigfont.shx");
	pTextStylerTblRcd->setXScale(0.5647);
	
	pTextStylerTblRcd->close();
	pTextStylerTbl->close();

	return 1;
}

void CWindowDetail::CreateDetailTextStyle(CString dimname)
{
	// 获得字体样式表
	AcDbTextStyleTable *pTextStyleTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl, AcDb::kForWrite);

	// 创建新的字体样式表记录
	AcDbTextStyleTableRecord *pTextStyleTblRcd = NULL;
	pTextStyleTblRcd = new AcDbTextStyleTableRecord();

	// 设置字体样式表记录的名称
	pTextStyleTblRcd->setName(dimname);

	// 设置字体文件名称
	pTextStyleTblRcd->setFileName(L"simplex.shx");
	pTextStyleTblRcd->setBigFontFileName(L"bigfont.shx");

	// 设置高宽比例
	pTextStyleTblRcd->setXScale(0.5647);

	// 将新的记录添加到字体样式表
	pTextStyleTbl->add(pTextStyleTblRcd);
	pTextStyleTblRcd->close();
	pTextStyleTbl->close();
}

void CWindowDetail::CreateHeightUnderWindow(AcGePoint3d start, double heightUnderWindow, double offset, const ACHAR* entryName)
{
	//画出倒三角下直线
	AcGePoint3d ptStart1(start.x + offset * 2, start.y, start.z);
	AcGePoint3d ptEnd1(ptStart1.x + 200, ptStart1.y, ptStart1.z);
	//画出倒三角
	AcGePoint3d pt1(ptStart1.x + 150, ptStart1.y, ptStart1.z);
	AcGePoint3d pt2(ptStart1.x + 100, ptStart1.y + 50, ptStart1.z);
	AcGePoint3d pt3(ptStart1.x + 200, ptStart1.y + 50, ptStart1.z);
	//画出倒三角上直线
	AcGePoint3d ptStart2(ptStart1.x + 200, ptStart1.y + 50, ptStart1.z);
	AcGePoint3d ptEnd2(ptStart1.x + 450, ptStart1.y + 50, ptStart1.z);
	
	AcDbLine *pLine1 = new AcDbLine(ptStart1, ptEnd1);
	AcDbLine *pLine2 = new AcDbLine(pt1, pt2);
	AcDbLine *pLine3 = new AcDbLine(pt2, pt3);
	AcDbLine *pLine4 = new AcDbLine(pt3, pt1);
	AcDbLine *pLine5 = new AcDbLine(ptStart2, ptEnd2);

	MD2010_PostModalToBlockTable(entryName, pLine1);
	MD2010_PostModalToBlockTable(entryName, pLine2);
	MD2010_PostModalToBlockTable(entryName, pLine3);
	MD2010_PostModalToBlockTable(entryName, pLine4);
	MD2010_PostModalToBlockTable(entryName, pLine5);

	pLine1->close();
	pLine2->close();
	pLine3->close();
	pLine4->close();
	pLine5->close();

	//创建窗下墙标注
	//AcDbObjectId heightUnderWindowDimID = AddAlignedDimensionAndStyle(ptStart2, ptEnd2, ptEnd2, heightUnderWindow);
	CString str;
	str.Format(L"%d", (int)heightUnderWindow);
	ptStart2.y += 50;
	AcDbText *pText = new AcDbText(ptStart2, str, AcDbObjectId::kNull, 148.75, 0);
	pText->setNormal(AcGeVector3d(0, 0, 1));
	pText->setPosition(ptStart2);
	AcDbObjectId textId = MD2010_GetTextStylerID(L"_TCH_DIM_T3");
	pText->setTextStyle(textId);
	pText->setWidthFactor(0.5647);
	pText->setColorIndex(7);
	JHCOM_PostToModelSpace(pText);
	pText->close();
}


