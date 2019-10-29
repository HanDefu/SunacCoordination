#pragma  once

#include <vector>
#include "..\Tool\Excel\Excel.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "..\Tool\SQLite\sqlite3.h"

using namespace std;

class CWindowLocalDataFromDB
{
public:
	CWindowLocalDataFromDB();
	~CWindowLocalDataFromDB();
	static CWindowLocalDataFromDB* Instance();

//门窗
public:
	bool GetWindowById(CString p_sId, AttrWindow& value)const;  //通过原型编号从m_windows中获取窗户
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value)const;//通过文件名从m_windows中获取窗户
	static int OutputWindowData(void *NotUsed, int nCol, char **value, char **ColName);//数据库查询函数

	//width,//宽度值，注意高度值不作为搜索条件 
	//openType, //开启类型
	//openNum,//开启扇数量
	//gongNengQu,//功能区
	//tongFengLiang//通风量
	//读取门和窗
	std::vector<AttrWindow >  GetWindows(double width, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetDoors(double width, CString openType, int openNum, CString gongNengQu)const;

	vector<AttrWindow> GetAllWindows()const; //获取所有窗户
	vector<AttrWindow> GetAllDoors()const;  //获取所有门

protected:



protected:
	void LoadDataFromDataBase(); //从数据库中把数据传到m_windows中

	static CWindowsDimData GetWindowDimData(CString code, char **ColName, int nStart);

protected:
	vector<AttrWindow> m_windows; //所有门窗
	vector<AttrWindow> m_wins;   //所有窗
	vector<AttrWindow> m_doors;  //所有门
	sqlite3 *m_pDB;
};

