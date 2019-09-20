#pragma once
#include "../Sunac_def.h"
#include "../Object/AttrAirCon.h"
#include "../Object/AttrDoor.h"
#include "../Object/AttrKitchen.h"
#include "../Object/AttrRailing.h"
#include "../Object/AttrToilet.h"
#include "../Object/AttrWindow.h"


class WebIO
{
	WebIO();
public:
	~WebIO();

	static WebIO *GetInstance()
	{
		static WebIO instance;
		return &instance;
	}

public:
	//-----------------ԭ����������--------------//
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrWindow *>  GetWindows
	(
		double width,//����ֵ��ע��߶�ֵ����Ϊ�������� 
		CString openType, //��������
		int openNum,//����������
		CString gongNengQu,//������
		double tongFengLiang//ͨ����
	);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrWindow *>  GetAllWindows();

	//�Ŵ���ȡ�ߴ�ֵ�� yuanXingBianHaoԭ�ͱ�ţ� dimStr�ַ�����(��"H2")��fileName�ļ���,����״̬�����ļ�������ߴ����Ͷ�ȡ�ߴ�ȡֵ
	SRCDimData Window_GetDim(CString yuanXingBianHao, CString dimStr, CString fileName);



	//////////////////////////////////////////////////////////////////////////
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrDoor *> GetDoors
	(
		double minWid, //����ֵ��ע��߶�ֵ����Ϊ�������� 
		CString doorType //������
	);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrDoor *> GetAllDoors();
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrKitchen *> GetKitchens
	(
		double kaiJian,//����
		double jinShen,//����
		CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
		CString type,//��������
		bool hasPaiQiDao//�Ƿ���������
	);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrKitchen *> GetAllKitchens();
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrToilet *> GetToilets
	(
		double kaiJian,//����
		double jinShen,//����
		CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
		CString type,//����������
		bool hasPaiQiDao//�Ƿ���������
	);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrToilet *> GetAllToilets();
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrAirCon *> GetAirCons(double piShu,
												CString weiZhi,//����ˮ��λ��
												bool hasYuShuiGuan,
												CString yuShuiGuanWeizhi);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrAirCon *> GetAllAirCons();
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrRailing *> GetRailings(eRailingType type);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrRailing *> GetAllRailings();

	//------����---------//
	//����ԭ�ͱ�ţ����ص��̶���ַ
	//ok����0 ���򷵻�-1
	static int DownLoadFile(CString id, CString filePathName);
public:
	vCString m_quyus;//����
	vCString  m_gongNengQus;//������
	vCString  m_windowDoorPoss;//�Ŵ�λ�ù�ϵ--����������
	vCString m_windowOpenTypes;//�ⴰ��������
	vCString m_windowOpenAmount;//����������
	vCString m_windowWallDis;//��������ǽ����
	vCString m_rate;//��ֵ�����ڼ���ͨ����
	vCString m_doorTypes;//������
	vCString m_kitchenTypes;//��������
	vCString m_kitchenShuiPenTypes;//����ˮ������
	vCString m_kitchenBingXiangTypes;//������������
	vCString m_kitchenZaoTaiWidths;//������̨����
	vCString m_toiletTypes;//����������
	vCString m_toiletTaiPenWidths;//����������
	vCString m_toiletMaTongTypes;//����������
	vCString m_toiletGuanXiWidths;//��������ϴ������---�Ƿ�֧���ֶ����룿��
	vCString m_airPiShus;//�յ����е�ƥ��
	vCString m_airLengNingShuiGuanPos;//�յ�����ˮ��λ��

protected:
	//---------------ͨ�ò��ֵ�-----------------------//
	vCString GetQuyus();//����
	vCString GetGongNengQus();//������
	vCString GetWindowDoorPoss();//�Ŵ�λ�ù�ϵ

	vCString Window_GetOpenTypes();//��������
	vCString Window_GetOpenAmount();//����������
	vCString Window_GetRate();//��ֵ�����ڼ���ͨ����
	vCString Window_GetWallDis();//��������ǽ����

	vCString Door_GetTypes();//������

	vCString Kitchen_GetTypes();//��������
	vCString Kitchen_GetShuiPenTypes();//ˮ������
	vCString Kitchen_GetBingXiangTypes();//��������
	vCString Kitchen_GetZaoTaiWidths();//��̨����

	vCString Toilet_GetTypes();//����������
	vCString Toilet_GetTaiPenWidths();//����������
	vCString Toilet_GetMaTongTypes();//����������
	vCString Toilet_GetGuanXiWidths();//��ϴ������---�Ƿ�֧���ֶ����룿��

	vCString Air_GetPiShus();//���е�ƥ��
	vCString Air_GetLengNingShuiGuanPos();//����ˮ��λ��
}; 

#define WEBINST (WebIO::GetInstance())
