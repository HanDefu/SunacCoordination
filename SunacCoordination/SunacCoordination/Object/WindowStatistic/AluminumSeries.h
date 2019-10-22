#pragma once

enum E_WindowDoorType
{
	E_WindowDoor_NC = 0,	//内开窗
	E_WindowDoor_WC,		//外开窗
	E_WindowDoor_TC,		//推拉窗

	E_WindowDoor_WM = 0x10,	//外开门
	E_WindowDoor_TLM,		//推拉门
	E_WindowDoor_TSTLM,		//提升推拉门
};

E_WindowDoorType ToWindowDoorType(CString type);
CString WindowTypeToCString(E_WindowDoorType type);



enum E_AluminumType
{
	E_断桥隔热铝型材,
	E_非断桥隔热铝型材,
	E_阳极氧化铝型材,
};

E_AluminumType ToE_AluminumType(CString type);
CString AluminumTypeToCSting(E_AluminumType type);

//型材数据
struct CAluminumData
{
	CString sCode; //型材代号
	E_WindowDoorType windowDoorType;   //门窗类型
	CString sName;  //型材名称
	CString sSerial; //型材系列
	double weightPerMeter; //米重
	double wastageRate; //损耗率
	E_AluminumType aluminumType; //型材类型

	CAluminumData()
	{
		weightPerMeter = 0;
		windowDoorType = E_WindowDoor_NC;
		wastageRate = 0.125;
		aluminumType = E_断桥隔热铝型材;
	}
}; 

//型材系列 //YMY
class CAluminumSeries
{
	CAluminumSeries();
public:
	static CAluminumSeries* Instance();
	virtual ~CAluminumSeries();

	void InitAluminum();

	//通过门窗类型、型材系列，型材名称获取型材数据,获取失败返回false
	bool GetAluminumDataBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sName, CAluminumData& p_dataOut);

	//通过型材编号获取当前型材编号所属的系列
	bool GetAluminumSerialByCode(CString p_code, CString& p_serialOut);

	//获得门窗类型下的型材系列选项
	vector<CString> GetAluminumSerialsByWindowType(E_WindowDoorType p_winType);


	//使用表AluminumSeries


protected:
};
