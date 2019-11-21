#pragma once
#include "../Common/ComFun_ACad.h"
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/WindowDoor/RCWindow.h"


//改结构体代表插入进来的一个门窗详图的块
//Sunac2019\Data\Template\WindowDetail.dwg
class CWindowDetailTemplate
{
public:
	CWindowDetailTemplate();

	bool InsertTemplates(int p_index, AcGePoint3d p_pnt);

public:
	AcGePoint3d pnt;//外窗插入点

	//总宽度高度
	const static int width = 4000;
	const static int height = 4500;

	//原型编号
	const static int idX = 130;
	const static int idY = 650;

	//面积
	const static int areaX = 1760;
	const static int areaY = 690;

	//开启面积
	const static int openAreaX = 1960;
	const static int openAreaY = 190;

	//功能区
	const static int gongNengQuX = 3220;
	const static int gongNengQuY = 690;

	//窗下墙高度
	const static int heightUnderWindowX = 3310;
	const static int heightUnderWindowY = 190;


	//标注宽度
	const static int biaoZhuWid = 300;

	RCBlock thisBlock;

	//得到模板的插入点
	AcGePoint3d GetInsertPnt(AcGePoint3d basePnt, int rowIndex, int columnIndex)
	{
		return AcGePoint3d(basePnt.x + columnIndex*width, basePnt.y - (rowIndex + 1) * height, 0);
	}

	static CString GetTemplateFileName()
	{
		return MD2010_GetAppPath() + L"\\Sunac2019\\Data\\Template\\WindowDetail.dwg";;
	}

	//计算门窗的插入点
	AcGePoint3d GetInsertPntWindow(double winwid, double winHei)
	{
		return AcGePoint3d(pnt.x + (width - winwid - biaoZhuWid * 2) / 2,//宽度两边平分
			pnt.y + 4500 - 900 - winHei,//高度顶端一致
			0);
	}
};

typedef std::vector<CWindowDetailTemplate> vSWindowDetailTemplate;


class CWindowDetail
{
public:
	static void DrawWindowDetail();

protected:
	static bool CreateDataText(const AttrWindow& winAtt, CWindowDetailTemplate winDetailTemplate);

	static int CreateDetailDims(const AttrWindow& winAtt, AcDbObjectId m_id);
};

