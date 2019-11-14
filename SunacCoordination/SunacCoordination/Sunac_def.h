#pragma once
#include <vector>
using namespace std;

#define  SUNAC_DEBUG
#define  SUNAC_ATTRIBUTE_ENTITY L"SUNAC_ATTRIBUTE_ENTITY"


#define  FILE_VERSION ((Adesk::Int32)3)
//版本2，门窗增加深化设计材质属性保存
//20191113 版本3，门窗增加窗下墙高度属性


#define  WORK_LOCAL//是否本地工作模式
#define	 CHECK_USE//是否检测权限

//模块类型
typedef enum eRCType
{
    WINDOW,//外窗
	DOOR,//门
	KITCHEN,//厨房
	Bathroom,//卫生间
	AIRCON,//空调
	RAILING,//栏杆
	TYPENUM
}eRCType;


typedef enum eViewDir
{
	E_VIEW_FRONT,//立面图
	E_VIEW_TOP,	//平面图
	E_VIEW_LEFT,//侧视图
	E_VIEW_EXTEND,//展开图

	E_VIEW_ALL = 0xff,
}eViewDir;

enum E_DIRECTION
{
	E_DIR_BOTTOM = 0,
	E_DIR_RIGHT,
	E_DIR_TOP,
	E_DIR_LEFT,
	E_DIR_UNKNOWN
};

typedef enum eRailingType
{
	E_RAILING_TIEYI,//铁艺
	E_RAILING_BOLI,//玻璃
	E_RAILING_ALL = 0x10,//全部
}eRailingType;

inline eRailingType ToERailingType(CString type)
{
	if (type == "铁艺" || type == "铁艺栏杆")
	{
		return E_RAILING_TIEYI;
	} 
	else if (type == "玻璃" || type == "玻璃栏杆")
	{
		return E_RAILING_BOLI;
	}
	else
	{
		ASSERT(FALSE);
		return E_RAILING_TIEYI;
	}
}

enum E_Prototype_Type
{
	E_DYNAMIC_PROTOTYPE,	//动态原型
	E_STATIC_PROTOTYPE,		//静态原型
};

typedef enum eWindowDoorPos//门窗位置关系
{
    DUIKAI,//对开
	CHUIZHIKAI//垂直开
} eWindowDoorPos;

inline eWindowDoorPos ToEWindowDoorPos(CString type)
{
	if (type == "对开")
	{
		return DUIKAI;
	} 
	else if (type == "垂直开")
	{
		return CHUIZHIKAI;
	}
	else
	{
		ASSERT(FALSE);
		return DUIKAI;
	}
}

struct CDwgFileInfo
{
	CString fileName;	// 文件名,不含路径
	CString previewFile; //预览图,不含路径
	int id; //在服务器中的id，用于下载文件
	int PreviewID;//缩略图的ID，用于下载缩略图

	CDwgFileInfo()
	{
		id = 0;
		PreviewID = 0;
	}
};

class CFloorInfo
{
public:
	CFloorInfo();

	bool SetFloors(CString p_sFloors);
	CString GetFloors()const { return m_sFloors; }

	int GetFloorCount()const{ return (int)(m_nAllFloors.size()); }
	vector<int> GetAllFloor()const{ return m_nAllFloors; }

	bool SetFloorHeight(double p_height);
	double GetFloorHeight()const { return m_floorHeight; }

protected:
	CString m_sFloors; //楼层范围，逗号分隔，多楼层用-分隔，示例： 2-5,7
	double m_floorHeight; //层高

	vector<int> m_nAllFloors;
};