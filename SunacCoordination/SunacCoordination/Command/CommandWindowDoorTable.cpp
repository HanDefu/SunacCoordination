﻿//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2013 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//
// AsdkAcUiSample.cpp : Defines the initialization routines for the DLL.
//
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include <rxmfcapi.h>
#include <dbgroup.h>
#include <geassign.h>
#include "accmd.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCWindow.h"
#include "../Common/ComFun_ACad.h"
#include "dbtable.h"

//门窗表
void CMD_SUNACWINDOWTable()
{
	//第二：选择钢筋对象
	vAcDbObjectId m_vids;//当前选择的ids
	acutPrintf(L"请选择需要统计的门窗");
	ads_name sset;
	acedSSGet(NULL,NULL,NULL,NULL,sset);
	long length = 0;
	acedSSLength(sset, &length);
	for (int i = 0; i < length; i++)
	{
		ads_name ent;
		acedSSName(sset, i, ent);
		AcDbObjectId objId = 0;
		acdbGetObjectId(objId, ent);
		if (objId != 0 && TY_IsWindow(objId))
		{
			m_vids.push_back(objId);
		}
	}
	acedSSFree(sset);

	CString info,str;
	info.Format(L"共选择了%d个外门窗\n",m_vids.size());
	acutPrintf(L"请选择需要统计的门窗\n");

	if (m_vids.size() == 0)
		return;

	//第二步选择门窗表插入点--开始绘图
	AcGePoint3d pnt = TY_GetPoint(L"请选择门窗表插入点");

	//第三步：读取门窗数据并且分类汇总
	vRCWindow allWindowsTypes;
	for (int i = 0; i < m_vids.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = m_vids[i];
		oneWindow.InitParametersFromDynamicBlock();
		oneWindow.GetAttribute();
		int index = vFind(oneWindow,allWindowsTypes);
		if (index == -1)
		{
			oneWindow.m_sameTypeIds.push_back(oneWindow.m_id);
			allWindowsTypes.push_back(oneWindow);
		}
		else
		{
			allWindowsTypes[index].m_sameTypeIds.push_back(oneWindow.m_id);
		}
		
	}

	int numWindow = allWindowsTypes.size();
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
	table->setAlignment(AcDb::CellAlignment::kMiddleCenter);


	//1.设置行数列数
	                //说明：3 是标题栏， +1是合计
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
			table->setAlignment(i,j,AcDb::CellAlignment::kMiddleCenter);
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
	table->setAlignment(2,7,AcDb::CellAlignment::kMiddleCenter);
	table->setTextString(2,8,L"高（mm)");
	table->setAlignment(2,8,AcDb::CellAlignment::kMiddleCenter);
	table->setTextString(1,9,L"樘数");
	table->setTextString(1,10,L"单樘门窗展开面积（m2)");
	table->setTextString(1,11,L"标准化窗框展开面积小计(m2)");
	table->setTextString(1,12,L"开启方式");
	table->setTextString(1,13,L"通风开启面积(m2)");
	table->setTextString(1,14,L"通风开启面积(%)");
	table->setTextString(1,15,L"应用部位");
	table->setTextString(1,16,L"备注");

	//--------外窗数据写出------//
	if (numWindow >= 1)
	{
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
			//序号
			str.Format(L"%d",i+1);
			table->setTextString(dataStartRow+i,1,str);

			//门窗编号
			str.Format(L"%s%dX%d",allWindowsTypes[i].GetAttribute()->m_id, 
				                  (int)(allWindowsTypes[i].GetW()),
								  (int)(allWindowsTypes[i].GetH()));
			table->setTextString(dataStartRow+i,2,str);
			table->setContentColor(dataStartRow+i,2,0,redColor);

			//型材种类
			str = L"铝合金平开窗";
			table->setTextString(dataStartRow+i,4,str);
			table->setContentColor(dataStartRow+i,4,0,yellowColor);

			//玻璃构造
			str = L"6Low-E+12Ar+6(高透）";
			table->setTextString(dataStartRow+i,5,str);
			table->setContentColor(dataStartRow+i,5,0,yellowColor);


			//传热系数设计值
			str = L"2.4";
			table->setTextString(dataStartRow+i,6,str);
			table->setContentColor(dataStartRow+i,6,0,yellowColor);

			//洞口宽
			str.Format(L"%d", (int)(allWindowsTypes[i].GetW()));
			table->setTextString(dataStartRow+i,7,str);
			table->setContentColor(dataStartRow+i,7,0,redColor);

			//洞口高
			str.Format(L"%d", (int)(allWindowsTypes[i].GetH()));
			table->setTextString(dataStartRow+i,8,str);
			table->setContentColor(dataStartRow+i,8,0,redColor);

			//樘数
			str.Format(L"%d", allWindowsTypes[i].m_sameTypeIds.size());
			table->setTextString(dataStartRow+i,9,str);
			table->setContentColor(dataStartRow+i,9,0,redColor);

			//面积1
			double area = allWindowsTypes[i].GetH() * allWindowsTypes[i].GetW()/1000000;
			str.Format(L"%.3f", area);
			table->setTextString(dataStartRow+i,10,str);
			table->setContentColor(dataStartRow+i,10,0,yellowColor);
			allArea1 += area * allWindowsTypes[i].m_sameTypeIds.size();

			//面积2
			str.Format(L"%.3f", area);
			table->setTextString(dataStartRow+i,11,str);
			table->setContentColor(dataStartRow+i,11,0,yellowColor);

			//开启方式
			str = L"内平开";
			table->setTextString(dataStartRow+i,12,str);
			table->setContentColor(dataStartRow+i,12,0,redColor);
		
			//通风开启面积
			// W1*(H-H2)/1000000;
			double area2 = allWindowsTypes[i].GetW1() * 
				(allWindowsTypes[i].GetH()-allWindowsTypes[i].GetH2())/1000000;
			str.Format(L"%.3f", area2);
			table->setTextString(dataStartRow+i,13,str);
			table->setContentColor(dataStartRow+i,13,0,yellowColor);

			allArea2 += area2 * allWindowsTypes[i].m_sameTypeIds.size();

			//通风开启面积 百分比
			//area2/area
			str.Format(L"%.3f", 100*area2/area);
			table->setTextString(dataStartRow+i,14,str);
			table->setContentColor(dataStartRow+i,14,0,yellowColor);

			//使用位置
			str = L"阳台";
			table->setTextString(dataStartRow+i,15,str);
			table->setContentColor(dataStartRow+i,15,0,redColor);
		}

		//-----窗最后一个合计---//
		for (int kk = 2; kk <= 9; kk++ )
		{
			table->setTextString(dataStartRow+numWindow,kk,L"-");
		}
		//总面积1
		str.Format(L"%.3f", allArea1);
		table->setTextString(dataStartRow+numWindow,10,str);
		table->setContentColor(dataStartRow+numWindow,10,0,yellowColor);
		
		//总面积2
		table->setTextString(dataStartRow+numWindow,11,str);
		table->setContentColor(dataStartRow+numWindow,11,0,yellowColor);

		table->setTextString(dataStartRow+numWindow,12,L"-");

		//合计通风面积
		str.Format(L"%.3f", allArea2);
		table->setTextString(dataStartRow+numWindow,13,str);
		table->setContentColor(dataStartRow+numWindow,13,0,yellowColor);

		//合计通风面积%
		str.Format(L"%.3f", allArea2/allArea1 * 100);
		table->setTextString(dataStartRow+numWindow,14,str);
		table->setContentColor(dataStartRow+numWindow,14,0,yellowColor);

		table->setTextString(dataStartRow+numWindow,15,L"-");

	}

	AcDbObjectId tableId = JHCOM_PostToModelSpace(table);

	return;
}