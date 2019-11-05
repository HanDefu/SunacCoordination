#pragma  once

#include <vector>
#include "..\Tool\Excel\Excel.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "..\Tool\SQLite\sqlite3.h"

using namespace std;

class CWindowLocalDataFromDB
{
	CWindowLocalDataFromDB();
public:
	~CWindowLocalDataFromDB();
	static CWindowLocalDataFromDB* Instance();

//门窗
public:
	bool GetWindowById(CString p_sId, AttrWindow& value)const;  //通过原型编号从m_windows中获取窗户
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value)const;//通过文件名从m_windows中获取窗户
	static int OutputWindowData(void *NotUsed, int nCol, char **value, char **ColName);//数据库查询函数

	//注意高度值不作为搜索条件 
	//width宽度值，openType开启类型, openNum开启扇数量  gongNengQu功能区, tongFengLiang通风量
	std::vector<AttrWindow >  GetWindows(double width, double height, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetDoors(double width, double height, CString openType, int openNum, CString gongNengQu)const;

	vector<AttrWindow> GetAllWindows()const; //获取所有窗户
	vector<AttrWindow> GetAllDoors()const;  //获取所有门

	vector<AttrWindow> GetAllWindowDoors()const{ return m_allWindowsData; }

protected:
	void LoadDataFromDataBase(); //从数据库中把数据传到m_windows中
	static CWindowsDimData GetWindowDimData(CString code, char **ColName, int nStart);

protected:
	vector<AttrWindow> m_allWindowsData; //所有门窗
};

