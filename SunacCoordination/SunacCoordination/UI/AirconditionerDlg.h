#pragma once


// CAirconditionerDlg �Ի���

class CAirconditionerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAirconditionerDlg)

public:
	CAirconditionerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAirconditionerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_AIRCONDITIONER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};