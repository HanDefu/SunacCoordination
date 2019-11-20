#pragma once


//门窗型材分类
enum E_WindowAluminumType  
{
	E_WindowAluminum_NC = 0,	//内开窗，包括内开窗，内开内倒窗
	E_WindowAluminum_WC,		//外开窗，包括外开窗、外开上悬窗
	E_WindowAluminum_TC,		//推拉窗

	E_WindowAluminum_WM = 0x10,	//外开门，包括外开门、门连窗
	E_WindowAluminum_TLM,		//推拉门
	E_WindowAluminum_TSTLM,		//提升推拉门
};

E_WindowAluminumType ToWindowDoorAluminumType(CString type);
CString WindowDoorAluminumTypeToCSting(E_WindowAluminumType type);


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
	E_WindowAluminumType windowDoorType;   //门窗类型
	CString sName;  //型材名称
	CString sSerial; //型材系列
	double weightPerMeter; //米重
	double wastageRate; //损耗率
	E_AluminumType aluminumType; //型材类型

	CAluminumData()
	{
		weightPerMeter = 0;
		windowDoorType = E_WindowAluminum_NC;
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
	bool GetAluminumDataBySeriesAndName(E_WindowAluminumType p_winType, CString p_serials, CString sName, CAluminumData& p_dataOut);

	//通过型材编号获取当前型材编号所属的系列
	bool GetAluminumSerialByCode(CString p_code, CString& p_serialOut);

	//获得门窗类型下的型材系列选项
	vector<CString> GetAluminumSerialsByWindowType(E_WindowAluminumType p_winType);

	//获得原型的型材系列选项
	vector<CString> GetAluminumSerialsByPrototype(CString p_sProtoTypeCode);
	
protected:
};
