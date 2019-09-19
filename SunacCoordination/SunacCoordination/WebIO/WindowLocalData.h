#pragma  once

#include <vector>
#include "../Object/AttrWindow.h"


using namespace std;

class CWindowLocalData
{
	
public:
	static CWindowLocalData *GetInstance();

private:
	CWindowLocalData();
	~CWindowLocalData();

	
public:
	void LoadFromExcel(CString p_file); //从表格中把数据传到m_windows中

	bool GetWindowById(CString p_sId,AttrWindow& value);  //通过原型编号从m_windows中获取窗户
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value);//通过文件名从m_windows中获取窗户

	
	//width,//宽度值，注意高度值不作为搜索条件 
	//openType, //开启类型
	//openNum,//开启扇数量
	//gongNengQu,//功能区
	//tongFengLiang//通风量
	//读取门和窗
	std::vector<AttrWindow >  GetWindows(double width, CString openType, int openNum, CString gongNengQu);

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

};

