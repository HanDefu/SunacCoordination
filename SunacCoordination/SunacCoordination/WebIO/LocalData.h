#pragma  once

#include <vector>
#include "../Object/AttrWindow.h"
#include "../Object/AttrAirCon.h"

using namespace std;

class CLocalData
{
	
public:
	static CLocalData *GetInstance();

private:
	CLocalData();
	~CLocalData();

//门窗
public:
	void LoadWindowFromExcel(CString p_file); //从表格中把数据传到m_windows中

	bool GetWindowById(CString p_sId,AttrWindow& value);  //通过原型编号从m_windows中获取窗户
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value);//通过文件名从m_windows中获取窗户

	
	//width,//宽度值，注意高度值不作为搜索条件 
	//openType, //开启类型
	//openNum,//开启扇数量
	//gongNengQu,//功能区
	//tongFengLiang//通风量
	std::vector<AttrWindow >  GetWindows(double width, CString openType, int openNum, CString gongNengQu, double tongFengLiang);

	//从excel内读出一条尺寸的6个CString转换成dimdata
	RCDimData ConvertStringToDimData
	(
		CString code,
		CString  valueType,
		CString value,
		CString defaultValue,
		CString state
	);

	vector<AttrWindow> GetAllWindows(); //获取所有窗户

	vector<AttrWindow> GetAllDoors();  //获取所有门

protected:
	vector<AttrWindow> m_windows; //所有门窗
	vector<AttrWindow> m_wins;   //所有窗
	vector<AttrWindow> m_doors;  //所有门

//空调
public:
	void LoadAirConFromExcel(CString p_file); //从表格中把数据传到m_aircon中

	bool GetAirConById(CString p_sId,AttrAirCon& value);  //通过原型编号从m_aircon中获取空调
	bool GetAirConByFileName(CString p_sFileName, AttrAirCon&value);//通过文件名从m_aircon中获取空调

	vector<AttrAirCon> GetAllAirCon(); //获取所有空调

protected:
	vector<AttrAirCon> m_aircon; //所有空调
};

