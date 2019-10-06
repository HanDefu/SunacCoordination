#pragma once

#define  SUNAC_DEBUG
#define  SUNAC_ATTRIBUTE_ENTITY L"SUNAC_ATTRIBUTE_ENTITY"
#define  SUNAC_COMMON_STR_LEN (256)
#define  FILE_VERSION ((Adesk::Int32)1)

#define  WORK_LOCAL//是否本地工作模式
#define CHECK_USE//是否检测权限

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

//门窗取值参数类型
typedef enum eWindowDimType
{
	SINGLE,//固定值
	MULTI,//值系列
	SCOPE,//范围
	UNLIMIT,//不限
	CALC,//公式
	NOVALUE //无
}eWindowDimType;

typedef enum eViewDir
{
	E_VIEW_FRONT,//立面图
	E_VIEW_TOP,	//平面图
	E_VIEW_LEFT,//侧视图
	E_VIEW_EXTEND,//展开图
}eViewDir;

typedef enum eRailingType
{
	E_RAILING_TIEYI,//铁艺
	E_RAILING_BOLI,//玻璃
}eRailingType;

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


typedef enum eLengNingShuiGuanPos//冷凝水管位置
{
	AIRCON_CEBIAN,//侧边
	AIRCON_HOUBIAN//后边
}eLingNingShuiGuanPos;

typedef enum eWindowCalcType//门窗算量类型
{
	CALC_XINGCAI,//型材
	CALC_BOLI,//玻璃
	CALC_WUJIN,//五金
	CALC_FUCAI//辅材
}eWindowCalcType;

typedef struct SCalcData//门窗算量数据
{
    eWindowCalcType calcType;
	CString formula;
}SCalcData;

typedef std::vector<eWindowCalcType> vSCalcData;

typedef struct RCDimData
{
	CString sCodeName; //代号 W W1 W2 W3 H H1 H2 H3
	eWindowDimType type;//值类型
	vdouble values;  //根据值类型，可存储单个值、系列、取值最小值、最大值
	double minValue;
	double maxValue;
	CString sFomula;	//公式
	double defaultValue;
	CString prompt;  //说明
}SRCDimData;
typedef std::vector<SRCDimData> vSRCDimData;
