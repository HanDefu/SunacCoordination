m_airConPrototypeFile
m_airConPrototypeFile
m_airConPrototypeFile
m_airConHorseNumber
m_airConHorseNumber
m_airConPrototypeId
m_airConPrototypeId
m_airConPrototypeFile
// UI\AirconditionerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AirconditionerDlg.h"
#include "afxdialogex.h"
#include "../WebIO/WebIO.h"
#include "../GlobalSetting.h"
#include "../WebIO/LocalData.h"
#include "../Object/AttrAirCon.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCAirCondition.h"
#include "../GlobalSetting.h"
#include "../Common/ComFun_Str.h"


// CAirconditionerDlg 对话框

IMPLEMENT_DYNAMIC(CAirconditionerDlg, CAcUiDialog)

CAirconditionerDlg::CAirconditionerDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CAirconditionerDlg::IDD, pParent)
{

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
}


BEGIN_MESSAGE_MAP(CAirconditionerDlg, CAcUiDialog)
//	ON_CBN_SELCHANGE(IDC_COMBO_PNUM, &CAirconditionerDlg::OnCbnSelchangeComboPnum)
	ON_BN_CLICKED(IDC_CHECK_HASRAINTUBE, &CAirconditionerDlg::OnBnClickedCheckHasraintube)
	ON_CBN_SELCHANGE(IDC_COMBO_PNUM, &CAirconditionerDlg::OnCbnSelchangeComboPnum)
	ON_CBN_SELCHANGE(IDC_COMBO_LNTUBEPOS, &CAirconditionerDlg::OnCbnSelchangeComboLntubepos)
	ON_CBN_SELCHANGE(IDC_COMBO_RAINTUBEPOS, &CAirconditionerDlg::OnCbnSelchangeComboRaintubepos)
	ON_BN_CLICKED(IDC_BUTTON_INSERTAC, &CAirconditionerDlg::OnBnClickedButtonInsertac)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()


// CAirconditionerDlg 消息处理程序


BOOL CAirconditionerDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	//测试用
	//m_preAC.SubclassDlgItem(IDC_STATIC_AC, this);
	//m_preAC.SetLayoutMode(PREVIEW_LAYOUT_HORIZONTAL);
	//m_preAC.SetPreview(_T("D:\\Projects\\Sunac\\Support\\Sunac2019\\LocalMode\\Window_N_7_0.dwg"));
	//m_preAC.SetText(_T("预览图"));
	//m_preAC.Invalidate();

	TYUI_Disable(m_rainTubePos); //默认灰掉雨水管位置下拉框

	LoadDefaultValue(); //设置下拉框默认值

	m_preAirCon.SubclassDlgItem(IDC_STATIC_AC, this);		// 控件和资源的关联
	m_preAirCon.Init(theArxDLL.ModuleResourceInstance(), true);

	UpdatePreview();
   // Air_GetPiShus();
	return TRUE;
}

//设置空调对话框中控件的默认值
void CAirconditionerDlg::LoadDefaultValue()
{
	const vCString& pNum = WebIO::GetConfigDict()->Air_GetPiShus();
	const vCString& lNTubePos = WebIO::GetConfigDict()->Air_GetLengNingShuiGuanPos();
	const vCString& rainTubePos = WebIO::GetConfigDict()->Air_GetYuShuiGuanPos();

	TYUI_InitComboBox(m_pNum, pNum, pNum.empty() ? _T("") : pNum[0]);
	TYUI_InitComboBox(m_lNTubePos, lNTubePos, lNTubePos.empty()? _T("") : lNTubePos[0]);
	TYUI_InitComboBox(m_rainTubePos, rainTubePos, rainTubePos.empty() ? _T("") : rainTubePos[0]);

	UpdateData(FALSE);
}

void CAirconditionerDlg::OnBnClickedCheckHasraintube()
{
	// TODO: 在此添加控件通知处理程序代码

	int state =((CButton *)GetDlgItem(IDC_CHECK_HASRAINTUBE))->GetCheck(); 

	/*将过滤复选框的状态赋值给state
	当state == BST_UNCHECKED 时表示该复选框没有被选中；
	当state == BST_CHECKED 时表示该复选框被选中；
	当state == BST_INDETERMINATE 时表示不确定*/


	if (state == BST_CHECKED )  //如果过滤复选框处于选中状态，那么将通过生产日期对任务进行筛选
	{
		TYUI_Enable(m_rainTubePos);
	}
	else
	{
		TYUI_Disable(m_rainTubePos);
	}

	UpdatePreview();
}

void CAirconditionerDlg::UpdatePreview() //当空调对话框中的控件的值发生变化时，图形也要根据控件的值来筛选、变化
{
	UpdateData(FALSE);

	//获取控件ComboBox中选的值
	CString pNum = TYUI_GetComboBoxText(m_pNum);
	CString lNTubePos = TYUI_GetComboBoxText(m_lNTubePos);
	CString rainTubePos = TYUI_GetComboBoxText(m_rainTubePos);
	int hasRainTube = m_hasRainTube.GetCheck();
	CString strHasTube = hasRainTube > 0 ? L"有" : L"无";

	//调用GetAirCon()函数来筛选符合条件的空调
	m_allAirCons = CLocalData::GetInstance()->GetAirCon(pNum, lNTubePos, strHasTube, rainTubePos);

	//当未查找到符合条件的空调时，对话框右侧图形为空，并且插入按钮变灰
	if (m_allAirCons.empty())
	{
		acutPrintf(_T("未找到符合条件的记录\n"));
		m_preAirCon.SetDatabase(NULL);
		m_btnInbsert.EnableWindow(FALSE);
		return;
	}

	Acad::ErrorStatus es=acDocManager->lockDocument(curDoc());
	AcDbDatabase *pDatabase = new AcDbDatabase();
	//图形文件路径
	m_filePathName = MD2010_GetAppPath() + L"\\support\\Sunac2019\\LocalMode\\" + m_allAirCons[0].m_airConPrototypeFile;
	es = pDatabase->readDwgFile(m_filePathName);
	//DrawSolid(zhu, pDatabase,false);
	m_preAirCon.SetDatabase(pDatabase);
	acDocManager->unlockDocument(curDoc());
	m_btnInbsert.EnableWindow(TRUE);
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
	ShowWindow(FALSE);
	
	//获取插入点
	AcGePoint3d pnt = TY_GetPoint();

	RCAirCondition blockAirCon;
	//将块插入图形空间
	blockAirCon.Insert(m_filePathName, pnt, 0, L"0", 256);

	//上下镜像
	if (m_upDownImage.GetCheck())
		TYCOM_MirrorOneObject(blockAirCon.m_id, pnt, AcGeVector3d(1,0,0));
	//左右镜像
	if (m_leftRightImage.GetCheck())
		TYCOM_MirrorOneObject(blockAirCon.m_id, pnt, AcGeVector3d(0,1,0));
	//上下左右镜像
	if (m_upDownImage.GetCheck() && m_leftRightImage.GetCheck())
	{
		TYCOM_MirrorRotate(blockAirCon.m_id, pnt, 3.1415926);
	}

	//把UI的数据记录在图框的扩展字典中
	AttrAirCon * pAirCon = new AttrAirCon(m_allAirCons[0]);
	blockAirCon.AddAttribute(pAirCon);

	pAirCon->close();
	  
	ShowWindow(TRUE);
}

