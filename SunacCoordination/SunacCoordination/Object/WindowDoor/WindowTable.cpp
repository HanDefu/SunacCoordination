#include "StdAfx.h"
#include <rxmfcapi.h>
#include <dbgroup.h>
#include <geassign.h>
#include <algorithm>
#include <afxdisp.h>
#include <accmd.h>
#include <dbtable.h>

#include "../../src/DocumentData.h"
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_String.h"
#include "../../Common/ComFun_Convert.h"
#include "../../Tool/DocLock.h"
#include "RCWindow.h"
#include "WindowSelect.h"
#include "WindowTableClassify.h"
#include "WindowTable.h"


//�Ŵ���old
void CWindTableGen::SunacWindowsTable_AllView()
{
	CString str;

	GetHightLightTool()->NoHighlight();

	//��һ����ѡ����Ҫͳ�Ƶ��Ŵ�
	eViewDir viewDir = E_VIEW_FRONT;
	bool bSuc1 = SelectViewDir(viewDir);
	if (bSuc1 == false)
		return;

	const vector<CSunacObjInCad> wins = CSunacSelect::SelectSunacObjs(S_WINDOW, viewDir);
	if (wins.size() == 0)
		return;

	//�ڶ���  ѡ���Ŵ��������
	AcGePoint3d pnt;
	bool bSuc = TY_GetPoint(pnt, L"��ѡ���Ŵ��������");
	if (bSuc == false)
		return;

	//����������ȡ�Ŵ����ݲ��ҷ������
	CWindowCountArray winCountArray;
	bSuc = winCountArray.InitByWindowIds(wins);
	if (bSuc == false)
	{
		AfxMessageBox(_T("ͳ��ʧ�ܣ����ƽ��ͼ�Ŵ�����¥����Ϣ"));
		return;
	}

	//���Ĳ� ��ʼ�������
	int numWindow = (int)winCountArray.GetCount();
	Acad::ErrorStatus es;
	AcDbTable *table = new AcDbTable();
	int dataStartRow = 3;
	AcCmColor redColor, yellowColor, greenColor;
	double allArea1 = 0;//���������
	double allArea2 = 0;//ͨ�粿��
	redColor.setColorIndex(1);
	yellowColor.setColorIndex(2);
	greenColor.setColorIndex(3);
	//table->setTextHeight(100);//�����ձ����ָ߶ȣ�ò�Ʋ������ã�

	//tͨ������
	table->setPosition(pnt);
	table->setAlignment(AcDb::kMiddleCenter);


	//1.������������, ˵����3 �Ǳ������� +1�Ǻϼ�
	int allRowNum = 3 + (numWindow + 1);//+(doorNum+1)
	table->setNumRows(allRowNum);
	table->setNumColumns(17);
	table->setColumnWidth(1000);
	table->setRowHeight(500);

	//1.1 �����п�������
	table->setColumnWidth(0, 500);
	table->setColumnWidth(1, 500);
	table->setColumnWidth(5, 1700);
	table->setColumnWidth(16, 2000);
	table->setRowHeight(0, 1000);

	//2. �ϲ���Ԫ��
	table->mergeCells(1, 1, 7, 8);

	//2.1 ��һ�кϲ�
	for (int i = 0; i < 7; i++)
	{
		table->mergeCells(1, 2, i, i);
	}

	for (int i = 9; i < 17; i++)
	{
		table->mergeCells(1, 2, i, i);
	}

	//----3.��������߶�----//
	table->setTextHeight(0, 0, 300);
	//�������е�����100��
	for (int i = 1; i < allRowNum; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			table->setTextHeight(i, j, 100);
			table->setAlignment(i, j, AcDb::kMiddleCenter);
		}
	}

	//------4. дTitle ����������߶�-----//
	es = table->setTextString(0, 0, L"�Ŵ���");//0,0��ͷ

											//-------5. д������------//
	table->setTextString(1, 0, L"����");
	table->setTextString(1, 1, L"���");
	table->setTextString(1, 2, L"�Ŵ����");
	table->setTextString(1, 3, L"ͼֽ�������");
	table->setTextString(1, 4, L"�Ͳ�����");
	table->setTextString(1, 5, L"��������");
	table->setTextString(1, 6, L"����ϵ�����ֵK([w/(m?xk)]");
	table->setTextString(1, 7, L"�Ŵ����ڳߴ�");
	table->setTextString(2, 7, L"����mm)");
	table->setAlignment(2, 7, AcDb::kMiddleCenter);
	table->setTextString(2, 8, L"�ߣ�mm)");
	table->setAlignment(2, 8, AcDb::kMiddleCenter);
	table->setTextString(1, 9, L"����");
	table->setTextString(1, 10, L"�����Ŵ�չ�������m2)");
	table->setTextString(1, 11, L"��׼������չ�����С��(m2)");
	table->setTextString(1, 12, L"������ʽ");
	table->setTextString(1, 13, L"ͨ�翪�����(m2)");
	table->setTextString(1, 14, L"ͨ�翪�����(%)");
	table->setTextString(1, 15, L"Ӧ�ò�λ");
	table->setTextString(1, 16, L"��ע");

	//--------�ⴰ����д��------//

	//�ϲ�����һ����
	table->mergeCells(dataStartRow, dataStartRow + numWindow - 1, 0, 0);
	//�ϲ��ϼƵ���һ��
	table->mergeCells(dataStartRow + numWindow, dataStartRow + numWindow, 0, 1);

	table->setTextString(dataStartRow, 0, L"�ⴰ");
	table->setTextString(dataStartRow + numWindow, 0, L"�ϼ�");
	//table->setAlignment(dataStartRow,0,AcDb::CellAlignment::kMiddleCenter);

	//-------6.���д���ⴰ������------//
	for (int i = 0; i < numWindow; i++)
	{
		const CWindowAndCount& winAndCount = winCountArray.GetWindow(i);
		const AttrWindow * pWinAtt = &(winAndCount.winAtt);
		int W = (int)(pWinAtt->GetW());
		int H = (int)(pWinAtt->GetH());

		//���
		str.Format(L"%d", i + 1);
		table->setTextString(dataStartRow + i, 1, str);

		//�Ŵ����
		table->setTextString(dataStartRow + i, 2, pWinAtt->GetInstanceCode());
		table->setContentColor(dataStartRow + i, 2, 0, redColor);

		//�Ͳ�����
		str = L"���Ͻ�";
		table->setTextString(dataStartRow + i, 4, str);
		table->setContentColor(dataStartRow + i, 4, 0, yellowColor);

		//��������
		table->setTextString(dataStartRow + i, 5, pWinAtt->m_material.sGlassSerial);
		table->setContentColor(dataStartRow + i, 5, 0, yellowColor);


		//����ϵ�����ֵ
		str.Format(_T("%.2f"), pWinAtt->m_material.heatCoeff);
		table->setTextString(dataStartRow + i, 6, str);
		table->setContentColor(dataStartRow + i, 6, 0, yellowColor);

		//���ڿ�
		str.Format(L"%d", W);
		table->setTextString(dataStartRow + i, 7, str);
		table->setContentColor(dataStartRow + i, 7, 0, redColor);

		//���ڸ�
		str.Format(L"%d", H);
		table->setTextString(dataStartRow + i, 8, str);
		table->setContentColor(dataStartRow + i, 8, 0, redColor);

		//����
		str.Format(L"%d", winAndCount.nCount);
		table->setTextString(dataStartRow + i, 9, str);
		table->setContentColor(dataStartRow + i, 9, 0, redColor);

		//���1
		double area = pWinAtt->GetWindowArea();
		str.Format(L"%.2f", area);
		table->setTextString(dataStartRow + i, 10, str);
		table->setContentColor(dataStartRow + i, 10, 0, yellowColor);
		allArea1 += area * winAndCount.nCount;

		//���2
		str.Format(L"%.2f", area);
		table->setTextString(dataStartRow + i, 11, str);
		table->setContentColor(dataStartRow + i, 11, 0, yellowColor);

		//������ʽ
		str = pWinAtt->m_openType;
		table->setTextString(dataStartRow + i, 12, str);
		table->setContentColor(dataStartRow + i, 12, 0, redColor);

		//ͨ�翪�����			
		double area2 = pWinAtt->GetTongFengQty();
		//double area2 = allWindowsTypes[i].GetW1() *(allWindowsTypes[i].GetH()-allWindowsTypes[i].GetH2())/1000000;// W1*(H-H2)/1000000;
		str.Format(L"%.2f", area2);
		table->setTextString(dataStartRow + i, 13, str);
		table->setContentColor(dataStartRow + i, 13, 0, yellowColor);

		allArea2 += area2 * winAndCount.nCount;

		//ͨ�翪����� �ٷֱ�
		//area2/area
		str.Format(L"%.2f", 100 * area2 / area);
		table->setTextString(dataStartRow + i, 14, str);
		table->setContentColor(dataStartRow + i, 14, 0, yellowColor);

		//ʹ��λ��
		//str = L"��̨";
		//table->setTextString(dataStartRow+i,15,str);
		//table->setContentColor(dataStartRow+i,15,0,redColor);
	}

	//-----�����һ���ϼ�---//
	for (int kk = 2; kk <= 9; kk++)
	{
		table->setTextString(dataStartRow + numWindow, kk, L"-");
	}
	//�����1
	str.Format(L"%.2f", allArea1);
	table->setTextString(dataStartRow + numWindow, 10, str);
	table->setContentColor(dataStartRow + numWindow, 10, 0, yellowColor);

	//�����2
	table->setTextString(dataStartRow + numWindow, 11, str);
	table->setContentColor(dataStartRow + numWindow, 11, 0, yellowColor);

	table->setTextString(dataStartRow + numWindow, 12, L"-");

	//�ϼ�ͨ�����
	str.Format(L"%.2f", allArea2);
	table->setTextString(dataStartRow + numWindow, 13, str);
	table->setContentColor(dataStartRow + numWindow, 13, 0, yellowColor);

	//�ϼ�ͨ�����%
	str.Format(L"%.2f", allArea2 / allArea1 * 100);
	table->setTextString(dataStartRow + numWindow, 14, str);
	table->setContentColor(dataStartRow + numWindow, 14, 0, yellowColor);

	table->setTextString(dataStartRow + numWindow, 15, L"-");


	AcDbObjectId tableId = JHCOM_PostToModelSpace(table);


	//��ѡ����Ŵ�����
	vAcDbObjectId winIds;
	for (UINT i = 0; i < wins.size(); i++)
	{
		winIds.push_back(wins[i].m_winId);
	}
	GetHightLightTool()->Highlight(winIds);

	return;
}

//p_dataStartRowΪ��ʼ�к�, p_floorColumnCountΪ¥���е�������p_floorColumnsΪ����¥����Ϣ(�磺1,2-5,7,10-16)��p_WinFloorColumnsΪ�����Ŵ�¥����Ϣ
void CWindTableGen::WriteDataToTable(AcDbTable *p_table, int p_dataStartRow, int p_floorColumnCount, vector<CString> p_WinFloorColumns, vector<CString> p_floorColumns, const CWindowAndCount& p_winAndCount)
{
	CString str, str1;
	const AttrWindow * pWinAtt = &(p_winAndCount.winAtt);

	//��Ʊ��
	p_table->setTextString(p_dataStartRow, 2, pWinAtt->GetInstanceCode());

	//���ڳߴ�
	CString W, H;
	W.Format(L"%d", (int)(pWinAtt->GetW()));
	H.Format(L"%d", (int)(pWinAtt->GetH()));
	p_table->setTextString(p_dataStartRow, 3, W + L"*" + H);

	//¥����
	vector<int> nAllFloorCount; //���ںϼ�
	nAllFloorCount.clear();

	for (int j = 0; j < p_WinFloorColumns.size(); j++)
	{
		for (int i = 0; i < p_floorColumns.size(); i++)
		{
			if (p_floorColumns[i] == p_WinFloorColumns[j])
			{
				// ��p_floorColumns��"-"��֣�ȷ���Ƿ���¥������
				int nPos = p_floorColumns[i].Find(_T('-'));
				if (nPos >= 0)
				{
					CString str1 = p_floorColumns[i].Left(nPos);
					CString str2 = p_floorColumns[i].Mid(nPos + 1);
					int nStart = _ttoi(str1);
					int nEnd = _ttoi(str2);
					if (nStart > nEnd || nStart == 0)
					{
						AfxMessageBox(_T("�Ŵ�¥����Ϣ��������"));
						return;
					}

					int nFloorCount = nEnd - nStart + 1;

					int nCount = pWinAtt->GetFloorInfo().GetFloorCountByFloor(p_floorColumns[i]);
					if (nCount == 0)
					{
						continue;
					}

					int numWindowDoor = pWinAtt->GetFloorInfo().GetFloorCountByFloorIndex(nEnd);
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
		}
	}

	//�ϼ�
	int allFloorCount = 0;
	for (int i = 0; i < nAllFloorCount.size(); i++)
	{
		allFloorCount += nAllFloorCount[i];
	}

	str.Format(L"%d", allFloorCount);
	p_table->setTextString(p_dataStartRow, 4 + p_floorColumnCount, str);

	//��ע
}

void CWindTableGen::AddXDataForWinTable(AcDbTable *p_table, vAcDbObjectId p_winIds)
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
		struct resbuf* pRbTemp = acutBuildList(AcDb::kDxfXdAsciiString, handleBuffer, RTNONE);
		pRbNext->rbnext = pRbTemp;
		pRbNext = pRbTemp;
	}

	Acad::ErrorStatus es = p_table->setXData(pRb);
	acutRelRb(pRb);
}

void CWindTableGen::GetWinFloorColumns(CWindowAndCount windowAndCount, vector<CString>& floorColumns)
{
	std::vector<CString> floorsTemp = YT_SplitCString(windowAndCount.winAtt.GetFloorInfo().GetFloors(), L',');
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

	sort(floorColumns.begin(), floorColumns.end(), CFloorInfo::FloorLessCmp);
}

//�����Ŵ���
void CWindTableGen::SunacWindowsTable_TopView()
{
	CDocLock lock;

	GetHightLightTool()->NoHighlight();

	//������ͼ������Ϊ0�㣬����������ͼ���޸Ļص�ǰͼ��
	CString oldLayerName;
	MD2010_GetCurrentLayer(oldLayerName);
	MD2010_SetCurrentLayer(L"0");

	//��һ����ѡ����Ҫͳ�Ƶ��Ŵ�
	const vector<CSunacObjInCad> wins = CSunacSelect::SelectSunacObjs(S_WINDOW, E_VIEW_TOP);
	if (wins.size() == 0)
		return;

	//�ڶ���  ѡ���Ŵ��������
	AcGePoint3d pnt;
	bool bSuc = TY_GetPoint(pnt, L"��ѡ���Ŵ��������");
	if (bSuc == false)
		return;

	//����������ȡ�Ŵ����ݲ��ҷ������
	CWindowCountArray winCountArray;
	bSuc = winCountArray.InitByWindowIds(wins);
	if (bSuc == false)
	{
		AfxMessageBox(_T("ͳ��ʧ�ܣ����ƽ��ͼ�Ŵ�����¥����Ϣ"));
		return;
	}
	else
	{
		for (int i = 0; i < winCountArray.GetCount(); i++)
		{
			if (0 == winCountArray.GetWindow(i).winAtt.GetFloorInfo().GetFloorCount())
			{
				AfxMessageBox(_T("�������Ŵ�¥��"));
				return;
			}
		}
	}

	//���Ĳ� �ж�ƽ��ͼ¥����Ϣ�Ƿ�һ��
	//for (int i = 0; i < winCountArray.GetCount() - 1; i++)
	//{
	//	if (winCountArray.GetWindow(i).winAtt.GetFloorInfo().GetAllFloor() != winCountArray.GetWindow(i+1).winAtt.GetFloorInfo().GetAllFloor())
	//	{
	//		AfxMessageBox(_T("¥����Ϣ���ô�������������"));
	//		return;
	//	}
	//}

	//���岽���¥����Ϣ��ȷ���Ŵ��������ͱ�����
	std::vector<CString> floorColumns;
	for (int i = 0; i < winCountArray.GetCount(); i++)
	{
		CWindowAndCount windowAndCount = winCountArray.GetWindow(i);
		GetWinFloorColumns(windowAndCount, floorColumns);
	}

	sort(floorColumns.begin(), floorColumns.end(), CFloorInfo::FloorLessCmp);

	const int floorColumnCount = (int)floorColumns.size();

	//������ ��ʼ�������
	AcDbTable *table = new AcDbTable();

	//tͨ������
	table->setPosition(pnt);
	table->setAlignment(AcDb::kMiddleCenter);
	table->setColorIndex(256);

	//1.������������, ˵����2 �Ǳ���������
	int numWindowDoor = (int)winCountArray.GetCount();
	int allRowNum = 2 + numWindowDoor;
	table->setNumRows(allRowNum);
	table->setNumColumns(7 + floorColumnCount);
	table->setColumnWidth(1000);
	table->setRowHeight(300);

	//1.1 �����п�������
	table->setColumnWidth(0, 700);	//����
	table->setColumnWidth(1, 1200); //������ϸ
	table->setRowHeight(1, 500);

	//----2.��������߶�----//
	table->setTextHeight(0, 0, 100);
	//�������е�����100��
	for (int i = 1; i < allRowNum; i++)
	{
		for (int j = 0; j < 7 + floorColumnCount; j++)
		{
			table->setTextHeight(i, j, 100);
			table->setAlignment(i, j, AcDb::kMiddleCenter);
		}
	}

	//------3.������д�����-----//
	// 3.1 �ϲ�Title ��������д�����
	table->mergeCells(0, 0, 0, 11);
	table->mergeCells(1, 1, 0, 1);

	Acad::ErrorStatus es;
	es = table->setTextString(0, 0, L"�Ŵ���");//0,0��ͷ
	table->setTextString(1, 0, L"����");
	table->setTextString(1, 2, L"��Ʊ��");
	table->setTextString(1, 3, L"���ڳߴ�(mm)");
	for (int i = 0; i < floorColumnCount; i++)
	{
		table->setTextString(1, 4 + i, floorColumns[i] + L"F");
	}
	table->setTextString(1, 4 + floorColumnCount, L"�ϼ�");
	table->setTextString(1, 5 + floorColumnCount, L"ͼ������");
	table->setTextString(1, 6 + floorColumnCount, L"��ע");

	//��ʼ�кţ������к�
	int dataStartRow = 2;
	int dataEndRow = 1;

	//3.2 ȷ���š����������Ա�ϲ����񣬽�"��"д�����
	int numDoor = winCountArray.GetDoorsCount();
	if (numDoor > 0)
	{
		dataEndRow = dataStartRow + numDoor - 1;
		table->mergeCells(dataStartRow, dataEndRow, 0, 0);
		table->setTextString(dataStartRow, 0, L"��");
		//ͼ������
		table->mergeCells(dataStartRow, dataEndRow, 5 + floorColumnCount, 5 + floorColumnCount);
		table->setTextString(dataStartRow, 5 + floorColumnCount, L"��Ʒ��");
	}

	int numWindow = winCountArray.GetWindowsCount();
	if (numWindow > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numWindow - 1;
		table->mergeCells(dataStartRow, dataEndRow, 0, 0);
		table->setTextString(dataStartRow, 0, L"��");
		//ͼ������
		table->mergeCells(dataStartRow, dataEndRow, 5 + floorColumnCount, 5 + floorColumnCount);
		table->setTextString(dataStartRow, 5 + floorColumnCount, L"���Ŵ���ͼ");
	}

	//3.3 ��ѡ����Ŵ����з���
	CSplitWindowDoorArray splitWindowDoorArray;
	splitWindowDoorArray.ClearSplitWindowDoor();
	for (int i = 0; i < numWindowDoor; i++)
	{
		const CWindowAndCount& winAndCount = winCountArray.GetWindow(i);
		splitWindowDoorArray.SplitWindowDoor(winAndCount);
	}

	//3.4 ���ո������͵��Ŵ�������д�����
	//����ʼ�к���Ϊ2�������к���Ϊ1
	dataStartRow = 2;
	dataEndRow = 1;

	//3.4.1 �⿪��
	int numDoorWM = splitWindowDoorArray.GetDoorWMCount();
	if (numDoorWM > 0)
	{
		dataEndRow = dataStartRow + numDoorWM - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"���Ͳ��⿪��");
		for (int i = 0; i < numDoorWM; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetDoorWM(i);
			vector<CString> winFloorColumns;
			GetWinFloorColumns(winAndCount, winFloorColumns);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, winFloorColumns, floorColumns, winAndCount);
		}
	}

	//3.4.2 ������
	int numDoorWLC = splitWindowDoorArray.GetDoorWLCCount();
	if (numDoorWLC > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numDoorWLC - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"���Ͳ�������");
		for (int i = 0; i < numDoorWLC; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetDoorWLC(i);
			vector<CString> winFloorColumns;
			GetWinFloorColumns(winAndCount, winFloorColumns);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, winFloorColumns, floorColumns, winAndCount);
		}
	}

	//3.4.3 ������
	int numDoorTLM = splitWindowDoorArray.GetDoorTLMCount();
	if (numDoorTLM > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numDoorTLM - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"���Ͳ�������");
		for (int i = 0; i < numDoorTLM; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetDoorTLM(i);
			vector<CString> winFloorColumns;
			GetWinFloorColumns(winAndCount, winFloorColumns);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, winFloorColumns, floorColumns, winAndCount);
		}
	}

	//3.4.4 ����������
	int numDoorTSTLM = splitWindowDoorArray.GetDoorTSTLMCount();
	if (numDoorTSTLM > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numDoorTSTLM - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"���Ͳ�����������");
		for (int i = 0; i < numDoorTSTLM; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetDoorTSTLM(i);
			vector<CString> winFloorColumns;
			GetWinFloorColumns(winAndCount, winFloorColumns);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, winFloorColumns, floorColumns, winAndCount);
		}
	}

	//3.4.5 �ڿ���
	int numWindowNC = splitWindowDoorArray.GetWindowNCCount();
	if (numWindowNC > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numWindowNC - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"���Ͳ��ڿ���");
		for (int i = 0; i < numWindowNC; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetWindowNC(i);
			vector<CString> winFloorColumns;
			GetWinFloorColumns(winAndCount, winFloorColumns);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, winFloorColumns, floorColumns, winAndCount);
		}
	}

	//3.4.6 �ڿ��ڵ���
	int numWindowNDC = splitWindowDoorArray.GetWindowNDCCount();
	if (numWindowNDC > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numWindowNDC - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"���Ͳ��ڿ��ڵ���");
		for (int i = 0; i < numWindowNDC; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetWindowNDC(i);
			vector<CString> winFloorColumns;
			GetWinFloorColumns(winAndCount, winFloorColumns);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, winFloorColumns, floorColumns, winAndCount);
		}
	}

	//3.4.7 ������
	int numWindowTC = splitWindowDoorArray.GetWindowTCCount();
	if (numWindowTC > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numWindowTC - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"���Ͳ�������");
		for (int i = 0; i < numWindowTC; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetWindowTC(i);
			vector<CString> winFloorColumns;
			GetWinFloorColumns(winAndCount, winFloorColumns);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, winFloorColumns, floorColumns, winAndCount);
		}
	}

	//3.4.8 �⿪��
	int numWindowWC = splitWindowDoorArray.GetWindowWCCount();
	if (numWindowWC > 0)
	{
		dataStartRow = dataEndRow + 1;
		dataEndRow = dataStartRow + numWindowWC - 1;
		table->mergeCells(dataStartRow, dataEndRow, 1, 1);
		table->setTextString(dataStartRow, 1, L"���Ͳ��⿪��");
		for (int i = 0; i < numWindowWC; i++)
		{
			const CWindowAndCount& winAndCount = splitWindowDoorArray.GetWindowWC(i);
			vector<CString> winFloorColumns;
			GetWinFloorColumns(winAndCount, winFloorColumns);
			WriteDataToTable(table, dataStartRow + i, floorColumnCount, winFloorColumns, floorColumns, winAndCount);
		}
	}


	//��ѡ����Ŵ�����
	vAcDbObjectId winIds;
	for (UINT i = 0; i < wins.size(); i++)
	{
		winIds.push_back(wins[i].m_winId);
	}
	GetHightLightTool()->Highlight(winIds);

	AddXDataForWinTable(table, winIds);

	AcDbObjectId tableId = JHCOM_PostToModelSpace(table);

	MD2010_SetCurrentLayer(oldLayerName);

	return;
}