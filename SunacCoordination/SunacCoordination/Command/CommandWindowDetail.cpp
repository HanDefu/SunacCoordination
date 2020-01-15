
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
		winAtt.m_relatedWinIds.removeAll();
		winAtt.m_fromWinId = AcDbObjectId::kNull;

		//4.1首先插入门窗表模板
		CWindowDetailTemplate winDetailTemplate;
		winDetailTemplate.InsertTemplates(i, pnt);

		AcGePoint3d winPnt = winDetailTemplate.GetInsertPntWindow(winAtt.GetW(), winAtt.GetH());

		AcDbObjectId idOut = CWindowGen::GenerateWindow(winAtt, winPnt, E_DIR_BOTTOM, true, AcDbObjectId::kNull);

		CreateDataText(winAtt, winDetailTemplate);

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
	//double offset = 150;
	double offset = 250;

	//----------------先标注竖向的--------------------//
	const double h2 = winAtt.GetH2();
	const double h1 = winAtt.GetH1();
	const AcGePoint3d  rightBottomPt = rect.GetRB();
	const AcGePoint3d  rightTopPt = rect.GetRT();

	AcGePoint3d  start = rightBottomPt;
	AcGePoint3d  end = rightBottomPt;
	AcGePoint3d  mid = rightBottomPt;

	//int colorIndex = 7;
	//int textHeight = 80;
	CString layer = L"0"; //TODO

	if (A > TOL)//如果A值存在 先标注两端的A
	{
		start = rightBottomPt;
		end = AcGePoint3d(start.x, start.y + A, 0);
		mid = AcGePoint3d(start.x + offset, (start.y + end.y) / 2, 0);
		//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
		AddAlignedDimensionAndStyle(start, end, mid, A, layer);

		start = rightTopPt;
		end = AcGePoint3d(start.x, start.y - A, 0);
		mid = AcGePoint3d(start.x + offset, (start.y + end.y) / 2, 0);
		//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
		AddAlignedDimensionAndStyle(start, end, mid, A, layer);
	}

	if (h2 > TOL)
	{
		start = AcGePoint3d(rightBottomPt.x, rightBottomPt.y + A, 0);
		end = AcGePoint3d(start.x, start.y + h2, 0);
		mid = AcGePoint3d(start.x + offset, start.y + h2 / 2, 0);
		//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
		AddAlignedDimensionAndStyle(start, end, mid, h2, layer);
	}

	//H1一定有
	start = AcGePoint3d(rightBottomPt.x, rightBottomPt.y + A + h2, 0);
	end = AcGePoint3d(start.x, start.y + h1, 0);
	mid = AcGePoint3d(start.x + offset, start.y + h1 / 2, 0);
	//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
	AddAlignedDimensionAndStyle(start, end, mid, h1, layer);

	//总高度一定有
	start = rightBottomPt;
	end = rightTopPt;
	mid = AcGePoint3d(start.x + offset * 2, (start.y + end.y) / 2, 0);
	//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
	AddAlignedDimensionAndStyle(start, end, mid, H, layer);

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
		//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
		AddAlignedDimensionAndStyle(start, end, mid, A, layer);

		AcGePoint3d end1 = start1;
		AcGePoint3d mid1 = start1;
		end1.x -= A;
		mid1.x -= A / 2;
		mid1.y += offset;
		//MD2010_AddAlignedDimension2(start1, end1, mid1, layer, colorIndex, textHeight);
		AddAlignedDimensionAndStyle(start1, end1, mid1, A, layer);
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
		//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
		AddAlignedDimensionAndStyle(start2, end2, mid2, W3, layer);
	}

	//若左侧也有转角窗，再标注左侧转角窗
	if (((W - W1 - W2 - W3 - A * 2 > W1) && (W - W1 - W2 - W3 - A * 2 > W3)) || (W - W1 - W2 - W3 - A * 2 == 500))
	{
		m_isHasW3 = true;
		start = end;
		mid = end;
		end.x += W3;
		mid.x += W3 / 2;
		mid.y += offset;
		//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
		AddAlignedDimensionAndStyle(start, end, mid, W3, layer);
	}

	//W1 都会存在
	start = end;
	mid = end;
	end.x += W1;
	mid.x += W1 / 2;
	mid.y += offset;
	//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
	AddAlignedDimensionAndStyle(start, end, mid, W1, layer);

	//标注W2
	if (W2 > TOL)
	{
		start = end;
		mid = end;
		end.x += W2;
		mid.x += W2 / 2;
		mid.y += offset;
		//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
		AddAlignedDimensionAndStyle(start, end, mid, W2, layer);
	}

	//最后可能还有一个W1
	if (m_isHasW3 == false)
	{
		if (W - W1 - W2 - W3 - A * 2 > TOL)
		{
			start = end;
			mid = end;
			end.x += W1;
			mid.x += W1 / 2;
			mid.y += offset;
			//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
			AddAlignedDimensionAndStyle(start, end, mid, W1, layer);
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
			//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
			AddAlignedDimensionAndStyle(start, end, mid, W1, layer);
		}
	}

	//标注总的宽度
	start = rect.GetLT();
	mid = start;
	end = start;
	end.x += W;
	mid.x += W / 2;
	mid.y += offset * 2;
	//MD2010_AddAlignedDimension2(start, end, mid, layer, colorIndex, textHeight);
	AddAlignedDimensionAndStyle(start, end, mid, W, layer);

	return 0;
}


void CWindowDetail::CreateDimensionStyle(CString styleName)
{
	// 获得当前图形的标注样式表
	AcDbDimStyleTable *pDimStyleTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pDimStyleTbl, AcDb::kForWrite);
	if (pDimStyleTbl->has(styleName))
	{
		pDimStyleTbl->close();//已经存在
		return;
	}

	// 创建新的标注样式表记录
	AcDbDimStyleTableRecord *pDimStyleTblRcd = NULL;
	pDimStyleTblRcd = new AcDbDimStyleTableRecord();

	// 设置标注样式的特性
	pDimStyleTblRcd->setName(styleName); //样式名称
	pDimStyleTblRcd->setDimscale(50); //全局比例
	pDimStyleTblRcd->setDimasz(1); // 箭头大小
	pDimStyleTblRcd->setDimexo(3); //尺寸界线偏移
	pDimStyleTblRcd->setDimdli(0); //尺寸线间距
	pDimStyleTblRcd->setDimexe(1); //超出尺寸线的距离
	//pDimStyleTblRcd->setDimzin(0); //消零
	//pDimStyleTblRcd->setDimtzin(2); //角度消零
	pDimStyleTblRcd->setDimtad(1); // 文字位于标注线的上方
	pDimStyleTblRcd->setDimtxt(2); // 标注文字的高度
	pDimStyleTblRcd->setDimgap(1.25); //设置标注文字周围的距离
	pDimStyleTblRcd->setDimtmove(2);  //设置标注文字的移动规则
	pDimStyleTblRcd->setDimtih(false); //文字方向
	pDimStyleTblRcd->setDimtix(true); //文字方向
	pDimStyleTblRcd->setDimblk(L"_ARCHTICK");//设置箭头的形状为建筑标记,设置尺寸线末尾的阴影部分显示
	pDimStyleTblRcd->setDimtxsty(MD2010_GetTextStylerID(L"_TCH_DIM_T3")); //文字样式

	// 将标注样式表记录添加到标注样式表中
	pDimStyleTbl->add(pDimStyleTblRcd);
	pDimStyleTblRcd->close();
	pDimStyleTbl->close();
}

AcDbObjectId CWindowDetail::AddAlignedDimensionAndStyle(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, double size, const ACHAR* newLayer)
{
	//创建一个标注尺寸(全局比例1:50)
	CreateDimensionStyle(_T("Z50"));
	//获取所创建的标注尺寸的id
	AcDbObjectId dimStyleId = MD2010_GetDimstylerID(_T("Z50"));

	if (JHCOM_PointDistance(start, end) <= TOL * 10000)//小于1的不标注
		return 0;

	CString entryName;
	entryName.Format(_T("%d"), (int)size);
	AcDbAlignedDimension  *pDim = new AcDbAlignedDimension(start, end, dimlinpnt, entryName, dimStyleId);

	AcDbObjectId dimID = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pDim);

	pDim->setLayer(newLayer);
	pDim->close();

	return dimID;
}


