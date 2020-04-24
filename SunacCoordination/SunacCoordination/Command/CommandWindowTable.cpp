
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include <rxmfcapi.h>
#include <dbgroup.h>
#include <geassign.h>
#include <algorithm>
#include <afxdisp.h>
#include "accmd.h"
#include "dbtable.h"
#include "Command.h"
#include "../Command/CommandHighlight.h"
#include "../Common/ComFun_Sunac.h"
#include "../Common/ComFun_ACad.h"
#include "../Common/ComFun_String.h"
#include "../Object/WindowDoor/RCWindow.h"
#include "../Object/WindowDoor/WindowSelect.h"
#include "../Object/WindowDoor/WindowTable.h"
#include "../Common/ComFun_Convert.h"
#include "../Tool/DocLock.h"

//门窗表
void CMD_SunacWindowsTable()
{
	CString str;

	CCommandHighlight::GetInstance()->SunacNoHighlight();

	//第一步：选择需要统计的门窗
	eViewDir viewDir = E_VIEW_FRONT;
	bool bSuc1 = SelectViewDir(viewDir);
	if (bSuc1 == false)
		return;

	const vector<CSunacObjInCad> wins = CSunacSelect::SelectSunacObjs(S_WINDOW, viewDir);
	if (wins.size() == 0)
		return;

	//第二步  选择门窗表插入点
	AcGePoint3d pnt;
	bool bSuc = TY_GetPoint(pnt, L"请选择门窗表插入点");
	if (bSuc == false)
		return;

	//第三步：读取门窗数据并且分类汇总
	CWindowCountArray winCountArray;
	bSuc = winCountArray.InitByWindowIds(wins);
	if (bSuc==false)
	{
		AfxMessageBox(_T("统计失败，请给平面图门窗设置楼层信息"));
		return;
	}

	//第四步 开始输出数据
	int numWindow = (int)winCountArray.GetCount();
	Acad::ErrorStatus es;
	AcDbTable *table = new AcDbTable();
	int dataStartRow = 3;
	AcCmColor redColor,yellowColor,greenColor;
	double allArea1 = 0;//总面积部分
	double allArea2 = 0;//通风部分
	redColor.setColorIndex(1);
	yellowColor.setColorIndex(2);
	greenColor.setColorIndex(3);
	//table->setTextHeight(100);//设置普遍文字高度，貌似不起作用？

	//t通用设置
	table->setPosition(pnt);
	table->setAlignment(AcDb::kMiddleCenter);


	//1.设置行数列数, 说明：3 是标题栏， +1是合计
	int allRowNum = 3+(numWindow+1);//+(doorNum+1)
	table->setNumRows(allRowNum);
	table->setNumColumns(17);
	table->setColumnWidth(1000);
	table->setRowHeight(500);

	//1.1 特殊列宽度设置
	table->setColumnWidth(0,500);
	table->setColumnWidth(1,500);
	table->setColumnWidth(5,1700);
	table->setColumnWidth(16,2000);
	table->setRowHeight(0,1000);

	//2. 合并单元格
	table->mergeCells(1,1,7,8);

	   //2.1 第一行合并
	for (int i = 0; i < 7; i++)
	{
		table->mergeCells(1,2,i,i);
	}

	for (int i = 9; i < 17; i++)
	{
		table->mergeCells(1,2,i,i);
	}

	//----3.设置字体高度----//
	table->setTextHeight(0,0,300);
	//其他所有的设置100高
	for (int i = 1; i < allRowNum; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			table->setTextHeight(i,j,100);
			table->setAlignment(i,j,AcDb::kMiddleCenter);
		}
	}

	//------4. 写Title 并设置字体高度-----//
	es = table->setTextString(0,0,L"门窗表");//0,0起头

	//-------5. 写标题栏------//
	table->setTextString(1,0,L"分类");
	table->setTextString(1,1,L"序号");
	table->setTextString(1,2,L"门窗编号");
	table->setTextString(1,3,L"图纸索引编号");
	table->setTextString(1,4,L"型材种类");
	table->setTextString(1,5,L"玻璃构造");
	table->setTextString(1,6,L"传热系数设计值K([w/(m²xk)]");
	table->setTextString(1,7,L"门窗洞口尺寸");
	table->setTextString(2,7,L"宽（mm)");
	table->setAlignment(2,7,AcDb::kMiddleCenter);
	table->setTextString(2,8,L"高（mm)");
	table->setAlignment(2,8,AcDb::kMiddleCenter);
	table->setTextString(1,9,L"樘数");
	table->setTextString(1,10,L"单樘门窗展开面积（m2)");
	table->setTextString(1,11,L"标准化窗框展开面积小计(m2)");
	table->setTextString(1,12,L"开启方式");
	table->setTextString(1,13,L"通风开启面积(m2)");
	table->setTextString(1,14,L"通风开启面积(%)");
	table->setTextString(1,15,L"应用部位");
	table->setTextString(1,16,L"备注");

	//--------外窗数据写出------//

	//合并左侧的一竖列
	table->mergeCells(dataStartRow,dataStartRow+numWindow-1,0,0);
		//合并合计的那一个
	table->mergeCells(dataStartRow+numWindow,dataStartRow+numWindow,0,1);

	table->setTextString(dataStartRow,0,L"外窗");
	table->setTextString(dataStartRow+numWindow, 0, L"合计");
	//table->setAlignment(dataStartRow,0,AcDb::CellAlignment::kMiddleCenter);

	//-------6.逐个写入外窗的数据------//
	for (int i = 0; i < numWindow; i++)
	{
		const CWindowAndCount& winAndCount = winCountArray.GetWindow(i);
		const AttrWindow * pWinAtt = &(winAndCount.winAtt);
		int W = (int)(pWinAtt->GetW());
		int H = (int)(pWinAtt->GetH());

		//序号
		str.Format(L"%d",i+1);
		table->setTextString(dataStartRow+i,1,str);

		//门窗编号
		table->setTextString(dataStartRow + i, 2, pWinAtt->GetInstanceCode());
		table->setContentColor(dataStartRow+i,2,0,redColor);

		//型材种类
		str = L"铝合金";
		table->setTextString(dataStartRow+i,4,str);
		table->setContentColor(dataStartRow+i,4,0,yellowColor);

		//玻璃构造
		table->setTextString(dataStartRow + i, 5, pWinAtt->m_material.sGlassSerial);
		table->setContentColor(dataStartRow+i,5,0,yellowColor);


		//传热系数设计值
		str.Format(_T("%.2f"), pWinAtt->m_material.heatCoeff);
		table->setTextString(dataStartRow+i,6,str);
		table->setContentColor(dataStartRow+i,6,0,yellowColor);

		//洞口宽
		str.Format(L"%d", W);
		table->setTextString(dataStartRow+i,7,str);
		table->setContentColor(dataStartRow+i,7,0,redColor);

		//洞口高
		str.Format(L"%d", H);
		table->setTextString(dataStartRow+i,8,str);
		table->setContentColor(dataStartRow+i,8,0,redColor);

		//樘数
		str.Format(L"%d", winAndCount.nCount);
		table->setTextString(dataStartRow+i,9,str);
		table->setContentColor(dataStartRow+i,9,0,redColor);

		//面积1
		double area = pWinAtt->GetWindowArea();
		str.Format(L"%.2f", area);
		table->setTextString(dataStartRow+i,10,str);
		table->setContentColor(dataStartRow+i,10,0,yellowColor);
		allArea1 += area * winAndCount.nCount;

		//面积2
		str.Format(L"%.2f", area);
		table->setTextString(dataStartRow+i,11,str);
		table->setContentColor(dataStartRow+i,11,0,yellowColor);

		//开启方式
		str = pWinAtt->m_openType;
		table->setTextString(dataStartRow+i,12,str);
		table->setContentColor(dataStartRow+i,12,0,redColor);
		
		//通风开启面积			
		double area2 = pWinAtt->GetTongFengQty();
		//double area2 = allWindowsTypes[i].GetW1() *(allWindowsTypes[i].GetH()-allWindowsTypes[i].GetH2())/1000000;// W1*(H-H2)/1000000;
		str.Format(L"%.2f", area2);
		table->setTextString(dataStartRow+i,13,str);
		table->setContentColor(dataStartRow+i,13,0,yellowColor);

		allArea2 += area2 * winAndCount.nCount;

		//通风开启面积 百分比
		//area2/area
		str.Format(L"%.2f", 100*area2/area);
		table->setTextString(dataStartRow+i,14,str);
		table->setContentColor(dataStartRow+i,14,0,yellowColor);

		//使用位置
		//str = L"阳台";
		//table->setTextString(dataStartRow+i,15,str);
		//table->setContentColor(dataStartRow+i,15,0,redColor);
	}

	//-----窗最后一个合计---//
	for (int kk = 2; kk <= 9; kk++ )
	{
		table->setTextString(dataStartRow+numWindow,kk,L"-");
	}
	//总面积1
	str.Format(L"%.2f", allArea1);
	table->setTextString(dataStartRow+numWindow,10,str);
	table->setContentColor(dataStartRow+numWindow,10,0,yellowColor);
		
	//总面积2
	table->setTextString(dataStartRow+numWindow,11,str);
	table->setContentColor(dataStartRow+numWindow,11,0,yellowColor);

	table->setTextString(dataStartRow+numWindow,12,L"-");

	//合计通风面积
	str.Format(L"%.2f", allArea2);
	table->setTextString(dataStartRow+numWindow,13,str);
	table->setContentColor(dataStartRow+numWindow,13,0,yellowColor);

	//合计通风面积%
	str.Format(L"%.2f", allArea2/allArea1 * 100);
	table->setTextString(dataStartRow+numWindow,14,str);
	table->setContentColor(dataStartRow+numWindow,14,0,yellowColor);

	table->setTextString(dataStartRow+numWindow,15,L"-");


	AcDbObjectId tableId = JHCOM_PostToModelSpace(table);


	//对选择的门窗高亮
	vAcDbObjectId winIds;
	for (UINT i = 0; i < wins.size(); i++)
	{
		winIds.push_back(wins[i].m_winId);
	}
	CCommandHighlight::GetInstance()->SunacHighlight(winIds);

	return;
}

//p_dataStartRow为开始行号, p_floorColumnCount为楼层列的数量，p_floorColumns为楼层信息(如：1,2-5,7,10-16)
void WriteDataToTable(AcDbTable *p_table, int p_dataStartRow, int p_floorColumnCount, vector<CString> p_floorColumns, const CWindowAndCount& p_winAndCount)
{
	CString str, str1;
	const AttrWindow * pWinAtt = &(p_winAndCount.winAtt);

	//设计编号
	p_table->setTextString(p_dataStartRow, 2, pWinAtt->GetInstanceCode());

	//洞口尺寸
	CString W, H;
	W.Format(L"%d", (int)(pWinAtt->GetW()));
	H.Format(L"%d", (int)(pWinAtt->GetH()));
	p_table->setTextString(p_dataStartRow, 3, W + L"*" + H);

	//if (0 == pWinAtt->GetFloorInfo().GetFloorCount())
	//{
	//	AfxMessageBox(_T("门窗未设置楼层信息"));
	//	return;
	//}

	//楼层列
	vector<int> nAllFloorCount; //用于合计
	nAllFloorCount.clear();

	for (int i = 0; i < p_floorColumnCount; i++)
	{
		// 将p_floorColumns按"-"拆分，确定是否有楼层区间
		int nPos = p_floorColumns[i].Find(_T('-'));
		if (nPos >= 0) 
		{
			CString str1 = p_floorColumns[i].Left(nPos);
			CString str2 = p_floorColumns[i].Mid(nPos + 1);
			int nStart = _ttoi(str1);
			int nEnd = _ttoi(str2);
			if (nStart > nEnd || nStart == 0)
			{
				AfxMessageBox(_T("门窗楼层信息设置有误"));
				return;
			}

			int nFloorCount = nEnd - nStart + 1;

			int nCount = pWinAtt->GetFloorInfo().GetFloorCountByFloor(p_floorColumns[i]);
			if (nCount==0)
			{
				continue;
			}

			int numWindowDoor = pWinAtt->GetFloorInfo().GetFloorCountByFloorIndex(nStart);
			str.Format(L"%d*%d", numWindowDoor, nFloorCount);
			p_table->setTextString(p_dataStartRow, 4 + i, str);
			nAllFloorCount.push_back(numWindowDoor * nFloorCount);
		}
		else
		{
			int numWindowDoor = pWinAtt->GetFloorInfo().GetFloorCountByFloorIndex(_ttoi(p_floorColumns[i]));
			nAllFloorCount.push_back(numWindowDoor);
			str.Format(L"%d", numWindowDoor);
			p_table->setTextString(p_dataStartRow, 4 + i, str);
		}
	}
	
	//合计
	int allFloorCount = 0;
	for (int i = 0; i < nAllFloorCount.size(); i++)
	{
		allFloorCount += nAllFloorCount[i];
	}
	
	str.Format(L"%d", allFloorCount);
	p_table->setTextString(p_dataStartRow, 4 + p_floorColumnCount, str);

	//备注
}

void AddXDataForWinTable(AcDbTable *p_table, AcGePoint3d p_pnt, vAcDbObjectId p_winIds)
{
	vAcDbHandle vHandles;
	JHCOM_GetAcDbHandles(p_winIds, vHandles);

	CString strAppName = L"xData";
	acdbRegApp(strAppName);

	struct resbuf* pRb = acutBuildList(AcDb::kDxfRegAppName, strAppName, RTNONE);
	struct resbuf* pRbNext = pRb;

	ACHAR handleBuffer[20];
	for (int i = 0; i < vHandles.size(); i++)
	{
		vHandles[i].getIntoAsciiBuffer(handleBuffer);
		struct resbuf* pRbTemp = acutBuildList(AcDb::kDxfXdHandle, handleBuffer, RTNONE);
		pRbNext->rbnext = pRbTemp;
		pRbNext = pRbTemp;
	}
	
	Acad::ErrorStatus es = p_table->setXData(pRb);
	acutRelRb(pRb);
}

//地面门窗表
void CMD_SunacFloorWindowsTable()
{
	CDocLock lock;

	CCommandHighlight::GetInstance()->SunacNoHighlight();

	//第一步：选择需要统计的门窗
	//eViewDir viewDir = E_VIEW_TOP;
	//bool bSuc1 = SelectViewDir(viewDir);
	//if (bSuc1 == false)
	//	return;

	const vector<CSunacObjInCad> wins = CSunacSelect::SelectSunacObjs(S_WINDOW, E_VIEW_TOP);
	if (wins.size() == 0)
		return;

	//第二步  选择门窗表插入点
	AcGePoint3d pnt;
	bool bSuc = TY_GetPoint(pnt, L"请选择门窗表插入点");
	if (bSuc == false)
		return;

	//第三步：读取门窗数据并且分类汇总
	CWindowCountArray winCountArray;
	bSuc = winCountArray.InitByWindowIds(wins);
	if (bSuc == false)
	{
		AfxMessageBox(_T("统计失败，请给平面图门窗设置楼层信息"));
		return;
	}
	else
	{
		for (int i = 0; i < winCountArray.GetCount(); i++)
		{
			if (0 == winCountArray.GetWindow(i).winAtt.GetFloorInfo().GetFloorCount())
			{
				AfxMessageBox(_T("请设置门窗楼层"));
				return;
			}
		}
	}

	//第四步 判断平面图楼层信息是否一致
	//for (int i = 0; i < winCountArray.GetCount() - 1; i++)
	//{
	//	if (winCountArray.GetWindow(i).winAtt.GetFloorInfo().GetAllFloor() != winCountArray.GetWindow(i+1).winAtt.GetFloorInfo().GetAllFloor())
	//	{
	//		AfxMessageBox(_T("楼层信息设置错误，请重新设置"));
	//		return;
	//	}
	//}

	//第五步拆分楼层信息，确定门窗表列数和标题栏
	std::vector<CString> floorColumns;
	for (int i = 0; i < winCountArray.GetCount(); i++)
	{
		std::vector<CString> floorsTemp = YT_SplitCString(winCountArray.GetWindow(i).winAtt.GetFloorInfo().GetFloors(), L',');
		for (UINT j = 0; j < floorsTemp.size(); j++)
		{
			bool bFind = false;
			for (UINT n = 0; n < floorColumns.size(); n++)
			{
				if (floorColumns[n].CompareNoCase(floorsTemp[j]) == 0)
				{
					bFind = true;
					break;
				}
			}

			if (bFind == false)
			{
				floorColumns.push_back(floorsTemp[j]);
			}
		}
	}

	sort(floorColumns.begin(), floorColumns.end(), CFloorInfo::FloorLessCmp);

	const int floorColumnCount = (int)floorColumns.size();

	//第六步 开始输出数据
	AcDbTable *table = new AcDbTable();

	//t通用设置
	table->setPosition(pnt);
	table->setAlignment(AcDb::kMiddleCenter);

	//1.设置行数列数, 说明：2 是标题栏行数
	int numWindowDoor = (int)winCountArray.GetCount();
	int allRowNum = 2 + numWindowDoor;
	table->setNumRows(allRowNum);
	table->setNumColumns(7 + floorColumnCount);
	table->setColumnWidth(1000);
	table->setRowHeight(300);

	//1.1 特殊列宽度设置
	table->setColumnWidth(0, 700);	//类型
	table->setColumnWidth(1, 1200); //类型明细
	table->setRowHeight(1, 500);

	//----2.设置字体高度----//
	table->setTextHeight(0, 0, 100);
	//其他所有的设置100高
	for (int i = 1; i < allRowNum; i++)
	{
		for (int j = 0; j < 7 + floorColumnCount; j++)
		{
			table->setTextHeight(i, j, 100);
			table->setAlignment(i, j, AcDb::kMiddleCenter);
		}
	}
	
	//------3.将数据写入表格-----//
	// 3.1 合并Title 并将列名写入表格
	table->mergeCells(0, 0, 0, 11);
	table->mergeCells(1, 1, 0, 1);

	Acad::ErrorStatus es;
	es = table->setTextString(0, 0, L"门窗表");//0,0起头
	table->setTextString(1, 0, L"类型");
	table->setTextString(1, 2, L"设计编号");
	table->setTextString(1, 3, L"洞口尺寸(mm)");
	for (int i = 0; i < floorColumnCount; i++)
	{
		table->setTextString(1, 4 + i, floorColumns[i] + L"F");
	}
	table->setTextString(1, 4 + floorColumnCount, L"合计");
	table->setTextString(1, 5 + floorColumnCount, L"图集名称");
	table->setTextString(1, 6 + floorColumnCount, L"备注");

	//开始行号，结束行号
	int dataStartRow = 2;
	int dataEndRow = 1;

	//3.2 确定门、窗的数量以便合并表格，将"门"写入表格
	int numDoor = winCountArray.GetDoorsCount();
	if (numDoor > 0)
	{
		dataEndRow = dataStartRow + numDoor - 1;
		table->mergeCells(dataStartRow, dataEndRow, 0, 0);
		table->setTextString(dataStartRow, 0, L"门");
		//图集名称
		table->mergeCells(dataStartRow, dataEndRow, 5 + floorColumnCount, 5 + floorColumnCount);
		table->setTextString(dataStartRow, 5 + floorColumnCount, L"成品门");
	}

	int numWindow =  winCountArray.GetWindowsCount();
	if (numWindow > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numWindow - 1;
		table->mergeCells(dataStartRow, dataEndRow, 0, 0);
		table->setTextString(dataStartRow, 0, L"窗");
		//图集名称
		table->mergeCells(dataStartRow, dataEndRow, 5 + floorColumnCount, 5 + floorColumnCount);
		table->setTextString(dataStartRow, 5 + floorColumnCount, L"详门窗详图");
	}

	//3.3 对选择的门窗进行分类
	CSplitWindowDoorArray splitWindowDoorArray;
	splitWindowDoorArray.ClearSplitWindowDoor();
	for (int i = 0; i < numWindowDoor; i++)
	{
		const CWindowAndCount& winAndCount = winCountArray.GetWindow(i);
		splitWindowDoorArray.SplitWindowDoor(winAndCount);
	}

	//3.4 按照各个类型的门窗将数据写入表格
	//将开始行号设为2，结束行号设为1
	dataStartRow = 2;
	dataEndRow = 1;

	//3.4.1 外开门
	int numDoorWM = splitWindowDoorArray.GetDoorWMCount();
	if (numDoorWM > 0)
	{
		dataEndRow = dataStartRow + numDoorWM - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"铝型材外开门");
		for (int i = 0; i < numDoorWM; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetDoorWM(i);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, floorColumns, winAndCount);
		}
	}

	//3.4.2 门连窗
	int numDoorWLC = splitWindowDoorArray.GetDoorWLCCount();
	if (numDoorWLC > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numDoorWLC - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"铝型材门连窗");
		for (int i = 0; i < numDoorWLC; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetDoorWLC(i);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, floorColumns, winAndCount);
		}
	}

	//3.4.3 推拉门
	int numDoorTLM = splitWindowDoorArray.GetDoorTLMCount();
	if (numDoorTLM > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numDoorTLM - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"铝型材推拉门");
		for (int i = 0; i < numDoorTLM; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetDoorTLM(i);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, floorColumns, winAndCount);
		}
	}

	//3.4.4 提升推拉门
	int numDoorTSTLM = splitWindowDoorArray.GetDoorTSTLMCount();
	if (numDoorTSTLM > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numDoorTSTLM - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"铝型材提升推拉门");
		for (int i = 0; i < numDoorTSTLM; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetDoorTSTLM(i);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, floorColumns, winAndCount);
		}
	}

	//3.4.5 内开窗
	int numWindowNC = splitWindowDoorArray.GetWindowNCCount();
	if (numWindowNC > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numWindowNC - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"铝型材内开窗");
		for (int i = 0; i < numWindowNC; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetWindowNC(i);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, floorColumns, winAndCount);
		}
	}

	//3.4.6 内开内倒窗
	int numWindowNDC = splitWindowDoorArray.GetWindowNDCCount();
	if (numWindowNDC > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numWindowNDC - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"铝型材内开内倒窗");
		for (int i = 0; i < numWindowNDC; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetWindowNDC(i);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, floorColumns, winAndCount);
		}
	}

	//3.4.7 推拉窗
	int numWindowTC = splitWindowDoorArray.GetWindowTCCount();
	if (numWindowTC > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numWindowTC - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"铝型材推拉窗");
		for (int i = 0; i < numWindowTC; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetWindowTC(i);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, floorColumns, winAndCount);
		}
	}

	//3.4.8 外开窗
	int numWindowWC = splitWindowDoorArray.GetWindowWCCount();
	if (numWindowWC > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numWindowWC - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"铝型材外开窗");
		for (int i = 0; i < numWindowWC; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetWindowWC(i);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, floorColumns, winAndCount);
		}
	}


	//对选择的门窗高亮
	vAcDbObjectId winIds;
	for (UINT i = 0; i < wins.size(); i++)
	{
		winIds.push_back(wins[i].m_winId);
	}
	CCommandHighlight::GetInstance()->SunacHighlight(winIds);

	AddXDataForWinTable(table, pnt, winIds);

	AcDbObjectId tableId = JHCOM_PostToModelSpace(table);
	
	return;
}