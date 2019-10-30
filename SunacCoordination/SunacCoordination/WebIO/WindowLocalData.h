#pragma  once

#include <vector>
#include "..\Tool\Excel\Excel.h"
#include "../Object/WindowDoor/AttrWindow.h"

using namespace std;

class CWindowLocalData
{
	CWindowLocalData();
public:
	static CWindowLocalData* Instance();
	~CWindowLocalData();

//门窗
public:
	bool GetWindowById(CString p_sId, AttrWindow& value)const;  //通过原型编号从m_windows中获取窗户
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value)const;//通过文件名从m_windows中获取窗户

	//注意高度值不作为搜索条件 
	//width宽度值，openType开启类型, openNum开启扇数量  gongNengQu功能区, tongFengLiang通风量
	std::vector<AttrWindow >  GetWindows(double width, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetDoors(double width, CString openType, int openNum, CString gongNengQu)const;

	vector<AttrWindow> GetAllWindows()const; //获取所有窗户
	vector<AttrWindow> GetAllDoors()const;  //获取所有门

	vector<AttrWindow> GetAllWindowDoors()const{ return m_windows; }

protected:
	void LoadDataFromExcel(CString p_file); //从表格中把数据传到m_windows中

	//从excel内读出一条尺寸的6个CString转换成dimdata
	CWindowsDimData ReadDimData(Excel::CExcelUtil &xls, CString code, int p_row, int p_colum);

protected:
	vector<AttrWindow> m_windows; //所有门窗
};

