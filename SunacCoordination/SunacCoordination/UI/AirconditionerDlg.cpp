//m_name
//m_name
//m_name
//m_power
//m_power
//m_prototypeCode
//m_prototypeCode
//m_name
// UI\AirconditionerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AirconditionerDlg.h"
#include "../WebIO/WebIO.h"
#include "../Object/AirCondition/AttrAirCon.h"
#include "../Object/AirCondition/RCAirCondition.h"
#include "../Common/ComFun_Sunac.h"
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_ACad.h"


// CAirconditionerDlg 对话框

CAirconditionerDlg* g_airconditionerDlg = NULL;

BOOL CloseAirconditionerDlg()
{
	if (g_airconditionerDlg == NULL)
		return TRUE;
	BOOL ret = g_airconditionerDlg->DestroyWindow();
	if (ret)
		g_airconditionerDlg = NULL;
	return ret;
}


IMPLEMENT_DYNAMIC(CAirconditionerDlg, CAcUiDialog)

CAirconditionerDlg::CAirconditionerDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CAirconditionerDlg::IDD, pParent)
	, m_rSize(0)
{
	m_flag = false;
	m_pCurEdit = NULL;
}

CAirconditionerDlg::~CAirconditionerDlg()
{
}

LRESULT CAirconditionerDlg::onAcadKeepFocus(WPARAM, LPARAM)
{
	//return FALSE;
	return TRUE;
}

void CAirconditionerDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PNUM, m_pNum);
	DDX_Control(pDX, IDC_COMBO_LNTUBEPOS, m_lNTubePos);
	DDX_Control(pDX, IDC_CHECK_HASRAINTUBE, m_hasRainTube);
	DDX_Control(pDX, IDC_COMBO_RAINTUBEPOS, m_rainTubePos);
	//DDX_Control(pDX, IDC_STATIC_AC, m_preAirCon);
	DDX_Control(pDX, IDC_BUTTON_INSERTAC, m_btnInbsert);
	DDX_Control(pDX, IDC_CHECK_UPDOWNIMAGE, m_upDownImage);
	DDX_Control(pDX, IDC_CHECK_LEFTRIGHTIMAGE, m_leftRightImage);
	DDX_Control(pDX, IDC_ROOM, m_room);
	DDX_Control(pDX, IDC_ROOMSIZE, m_roomSize);
	DDX_Text(pDX, IDC_ROOMSIZE, m_rSize);
}


BEGIN_MESSAGE_MAP(CAirconditionerDlg, CAcUiDialog)
//	ON_CBN_SELCHANGE(IDC_COMBO_PNUM, &CAirconditionerDlg::OnCbnSelchangeComboPnum)
	ON_BN_CLICKED(IDC_CHECK_HASRAINTUBE, &CAirconditionerDlg::OnBnClickedCheckHasraintube)
	ON_CBN_SELCHANGE(IDC_COMBO_PNUM, &CAirconditionerDlg::OnCbnSelchangeComboPnum)
	ON_CBN_SELCHANGE(IDC_COMBO_LNTUBEPOS, &CAirconditionerDlg::OnCbnSelchangeComboLntubepos)
	ON_CBN_SELCHANGE(IDC_COMBO_RAINTUBEPOS, &CAirconditionerDlg::OnCbnSelchangeComboRaintubepos)
	ON_BN_CLICKED(IDC_BUTTON_INSERTAC, &CAirconditionerDlg::OnBnClickedButtonInsertac)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CAirconditionerDlg::OnBnClickedButtonCalculate)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE2, &CAirconditionerDlg::OnBnClickedButtonCalculate2)
END_MESSAGE_MAP()


// CAirconditionerDlg 消息处理程序


BOOL CAirconditionerDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	TYUI_Disable(m_rainTubePos); //默认灰掉雨水管位置下拉框

	LoadDefaultValue(); //设置下拉框默认值
	
	m_room.ShowWindow(FALSE);	//将静态文本控件和编辑框控件隐藏
	m_roomSize.ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CALCULATE2)->ShowWindow(FALSE);

	m_preAirCon.SubclassDlgItem(IDC_STATIC_AC, this);		// 控件和资源的关联
	m_preAirCon.Init(theArxDLL.ModuleResourceInstance(), true);
	m_preAirCon.SetMouseWheel(false);

	UpdatePreview();

	return TRUE;
}

void CAirconditionerDlg::OnOK()
{
	CAcUiDialog::OnOK();
	DestroyWindow();
}

void CAirconditionerDlg::OnCancel()
{
	CAcUiDialog::OnCancel();
	DestroyWindow();
}

void CAirconditionerDlg::PostNcDestroy()
{
	CAcUiDialog::PostNcDestroy();
	delete this;
	g_airconditionerDlg = NULL;
}

BOOL CAirconditionerDlg::PreTranslateMessage(MSG *pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
		return TRUE;
	return CAcUiDialog::PreTranslateMessage(pMsg);
}

//设置空调对话框中控件的默认值
void CAirconditionerDlg::LoadDefaultValue()
{
	const vCString& pNum = WebIO::GetInstance()->GetConfigDict()->Air_GetPiShus();
	const vCString& lNTubePos = WebIO::GetInstance()->GetConfigDict()->Air_GetLengNingShuiGuanPos();
	const vCString& rainTubePos = WebIO::GetInstance()->GetConfigDict()->Air_GetYuShuiGuanPos();

	TYUI_InitComboBox(m_pNum, pNum, pNum.empty() ? _T("") : pNum[0]);
	TYUI_InitComboBox(m_lNTubePos, lNTubePos, lNTubePos.empty()? _T("") : lNTubePos[0]);
	TYUI_InitComboBox(m_rainTubePos, rainTubePos, rainTubePos.empty() ? _T("") : rainTubePos[0]);
	//UpdateData(FALSE);
}

void CAirconditionerDlg::OnBnClickedCheckHasraintube()
{
	BOOL state = m_hasRainTube.GetCheck(); 

	m_rainTubePos.EnableWindow(state);

	UpdatePreview();
}

void CAirconditionerDlg::UpdatePreview() //当空调对话框中的控件的值发生变化时，图形也要根据控件的值来筛选、变化
{
	//获取控件ComboBox中选的值
	double pNum = _ttof(TYUI_GetComboBoxText(m_pNum));
	CString lNTubePos = TYUI_GetComboBoxText(m_lNTubePos);
	CString rainTubePos = TYUI_GetComboBoxText(m_rainTubePos);
	int hasRainTube = m_hasRainTube.GetCheck();
	CString strHasTube = hasRainTube > 0 ? L"是" : L"否";

	//调用GetAirCon()函数来筛选符合条件的空调
	m_allAirCons = WebIO::GetInstance()->GetAirCons(pNum, lNTubePos, strHasTube, rainTubePos);

	//当未查找到符合条件的空调时，对话框右侧图形为空，并且插入按钮变灰
	if (m_allAirCons.empty())
	{
		acutPrintf(_T("未找到符合条件的记录\n"));
		m_preAirCon.clearAll();
		m_btnInbsert.EnableWindow(FALSE);
	}
	else
	{
		//图形文件路径
		m_fileName = TY_GetPrototypeFilePath() + m_allAirCons[0].m_file.fileName;
		//m_fileName = TY_GetPrototypeFilePath() + m_allAirCons[0].m_prototypeCode + L".dwg";
		m_preAirCon.SetDwgFile(m_fileName);
		m_btnInbsert.EnableWindow(TRUE);
	}
}

void CAirconditionerDlg::OnCbnSelchangeComboPnum()
{
	UpdatePreview();
}

void CAirconditionerDlg::OnCbnSelchangeComboLntubepos()
{
	UpdatePreview();
}

void CAirconditionerDlg::OnCbnSelchangeComboRaintubepos()
{
	UpdatePreview();
}


void CAirconditionerDlg::OnBnClickedButtonInsertac()
{	
	UpdatePreview();

	ShowWindow(SW_HIDE);

	AcGeVector3d offsetXY;
	AcGePoint3d pnt;

	//获取插入点
	if (m_pCurEdit == NULL)
	{
		bool bSuc = TY_GetPoint(pnt);
		acedPostCommandPrompt();
		if (bSuc == false)
		{
			ShowWindow(SW_SHOW);
			return;
		}
	}
	else
	{
		AcDbExtents ext;
		m_pCurEdit->getGeomExtents(ext);
		pnt = ext.minPoint();
		JHCOM_DeleteCadObject(m_pCurEdit->objectId());
	}

	RCAirCondition blockAirCon;
	//将块插入图形空间
	blockAirCon.Insert(m_fileName, pnt, 0, L"Sunac_Aircondition", 256);

	//上下镜像
	if (m_upDownImage.GetCheck())
	{
		TYCOM_Mirror(blockAirCon.m_id, pnt, AcGeVector3d(1,0,0));
		offsetXY = AcGeVector3d(0, m_allAirCons[0].m_airD, 0);
		TYCOM_Move(blockAirCon.m_id, offsetXY);
	}
	//左右镜像
	if (m_leftRightImage.GetCheck())
	{
		TYCOM_Mirror(blockAirCon.m_id, pnt, AcGeVector3d(0,1,0));
		offsetXY = AcGeVector3d(m_allAirCons[0].m_airW, 0, 0);
		TYCOM_Move(blockAirCon.m_id, offsetXY);
	}
	//上下左右镜像
	if (m_upDownImage.GetCheck() && m_leftRightImage.GetCheck())
	{
		TYCOM_Mirror(blockAirCon.m_id, pnt, AcGeVector3d(0,0,0));
	}

	//把UI的数据记录在图框的扩展字典中
	AttrAirCon * pAirCon = new AttrAirCon(m_allAirCons[0]);
	pAirCon->m_bLeftRightMirror = (m_leftRightImage.GetCheck() == TRUE);
	pAirCon->m_bUpDownMirror = (m_upDownImage.GetCheck() == TRUE);
	blockAirCon.AddAttribute(pAirCon);

	pAirCon->close();
	  
	ShowWindow(TRUE);
}

//点击计算按钮，显示房间面积对话框，并通过房间面积计算出空调匹数
void CAirconditionerDlg::OnBnClickedButtonCalculate()
{
	if (m_flag == FALSE)
	{
		//将静态文本和编辑框显示
		m_room.ShowWindow(TRUE);
		m_roomSize.ShowWindow(TRUE);

		GetDlgItem(IDC_BUTTON_CALCULATE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALCULATE2)->ShowWindow(TRUE);

		m_flag = true;
	}
}

void CAirconditionerDlg::OnBnClickedButtonCalculate2()
{
	UpdateData(TRUE);
	const vCString& pNum = WebIO::GetInstance()->GetConfigDict()->Air_GetPiShus();	//	获取空调所有匹数

	//空调匹数计算方法
	if (m_rSize > 0 && m_rSize <= 18)
	{
		TYUI_InitComboBox(m_pNum, pNum, pNum.empty() ? _T("") : pNum[0]);
	}
	else if (m_rSize >= 12 && m_rSize <= 24)
	{
		TYUI_InitComboBox(m_pNum, pNum, pNum.empty() ? _T("") : pNum[1]);
	}
	else if (m_rSize >= 25 && m_rSize <= 34)
	{
		TYUI_InitComboBox(m_pNum, pNum, pNum.empty() ? _T("") : pNum[2]);
	}
	else if (m_rSize >= 35)
	{
		TYUI_InitComboBox(m_pNum, pNum, pNum.empty() ? _T("") : pNum[3]);
	}
	else
	{
		AfxMessageBox(TEXT("房间面积应大于0m²"));
	}
	UpdateData(FALSE);
}

void CAirconditionerDlg::SetEditMode(AcDbBlockReference* pBlock)
{
	m_pCurEdit = pBlock;
	if (m_pCurEdit == NULL)
		return;

	AcDbObject* pAtt = NULL;
	TY_GetAttributeData(pBlock->objectId(), pAtt);
	AttrAirCon *pAirCon = dynamic_cast<AttrAirCon *>(pAtt);
	if (pAirCon == NULL)
		return;

	CString sPower;
	sPower.Format(L"%.1lf", pAirCon->m_power);
	m_pNum.SelectString(-1, sPower);
	m_lNTubePos.SelectString(-1, pAirCon->m_pipePos);
	m_hasRainTube.SetCheck(pAirCon->m_hasRainPipe);
	m_rainTubePos.EnableWindow(pAirCon->m_hasRainPipe);
	m_rainTubePos.SelectString(-1, pAirCon->m_rainPipePos);
	m_upDownImage.SetCheck(pAirCon->m_bUpDownMirror);
	m_leftRightImage.SetCheck(pAirCon->m_bLeftRightMirror);

	TYUI_SetText(*GetDlgItem(IDC_BUTTON_INSERTAC), L"确定");
}
