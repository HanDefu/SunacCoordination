#pragma  once

#include <vector>
#include "../Object/AttrWindow.h"


using namespace std;

class CWindowLocalData
{
	CWindowLocalData();
public:
	~CWindowLocalData();

	static CWindowLocalData *GetInstance();
	
	void LoadFromExcel(CString p_file);

	const AttrWindow& GetWindowById(CString p_sId);
	const AttrWindow& GetWindowByFileName(CString p_sFileName);

	//width,//宽度值，注意高度值不作为搜索条件 
	//openType, //开启类型
	//openNum,//开启扇数量
	//gongNengQu,//功能区
	//tongFengLiang//通风量
	std::vector<AttrWindow *>  GetWindows(double width, CString openType, int openNum, CString gongNengQu, double tongFengLiang	);

protected:
	vector<AttrWindow> m_windows;

};

